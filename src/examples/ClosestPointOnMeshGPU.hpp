#include "./ClosestPointOnMesh.hpp"

Geometry   *d_geometries;
Mesh       *d_meshes;
Triangle   *d_triangles;
vec3       *d_vertices;
BVHNode  *d_mesh_bvh_nodes;

u32 *d_mesh_bvh_node_counts,
    *d_mesh_triangle_counts,
    *d_mesh_vertex_counts;

ClosestPointOnTriangle *d_results;

void allocateDeviceScene(Scene &scene) {
    u32 total_triangles = 0;
    u32 total_vertices = 0;
    if (scene.counts.geometries)   gpuErrchk(cudaMalloc(&d_geometries,   sizeof(Geometry)  * scene.counts.geometries))
    if (scene.counts.meshes) {
        for (u32 i = 0; i < scene.counts.meshes; i++) {
            total_triangles += scene.meshes[i].triangle_count;
            total_vertices += scene.meshes[i].vertex_count;
        }

        gpuErrchk(cudaMalloc(&d_meshes,    sizeof(Mesh)     * scene.counts.meshes))
        gpuErrchk(cudaMalloc(&d_triangles, sizeof(Triangle) * total_triangles))
        gpuErrchk(cudaMalloc(&d_vertices,  sizeof(vec3)     * total_vertices))
        gpuErrchk(cudaMalloc(&d_mesh_bvh_node_counts, sizeof(u32) * scene.counts.meshes))
        gpuErrchk(cudaMalloc(&d_mesh_triangle_counts, sizeof(u32) * scene.counts.meshes))
        gpuErrchk(cudaMalloc(&d_mesh_vertex_counts, sizeof(u32) * scene.counts.meshes))
    }

	gpuErrchk(cudaMalloc(&d_results, sizeof(ClosestPointOnTriangle) * scene.max_triangle_count))

    gpuErrchk(cudaMalloc(&d_mesh_bvh_nodes,     sizeof(BVHNode)  * total_triangles * 2))
}

void uploadScene(Scene &scene) {
    uploadN(scene.geometries, d_geometries, scene.counts.geometries)
}

void uploadMeshes(Scene &scene) {
    uploadN(scene.meshes, d_meshes, scene.counts.meshes);
    Mesh *mesh = scene.meshes;
    u32 nodes_offset = 0;
    u32 triangles_offset = 0;
    u32 vertex_offset = 0;
    for (u32 i = 0; i < scene.counts.meshes; i++, mesh++) {
        uploadNto(mesh->bvh.nodes, d_mesh_bvh_nodes, mesh->bvh.node_count, nodes_offset)
        uploadNto(mesh->triangles, d_triangles,      mesh->triangle_count, triangles_offset)
        uploadNto(mesh->vertex_positions, d_vertices,      mesh->vertex_count, vertex_offset)
        nodes_offset        += mesh->bvh.node_count;
        triangles_offset    += mesh->triangle_count;
        vertex_offset       += mesh->vertex_count;
    }

    uploadN(scene.mesh_bvh_node_counts, d_mesh_bvh_node_counts, scene.counts.meshes)
    uploadN(scene.mesh_triangle_counts, d_mesh_triangle_counts, scene.counts.meshes)
    uploadN(scene.mesh_vertex_counts, d_mesh_vertex_counts, scene.counts.meshes)
}


__global__ void d_runQuery(
		u32 source_mesh_id,
		u32 target_mesh_id,
		u32 source_geo_id,
		u32 target_geo_id,

		f32 max_distance,
		bool adaptive,

		u32 vertex_count,
        u32 mesh_count,
		BVHNode  *mesh_bvh_nodes,
        Mesh       *meshes,
        Triangle   *mesh_triangles,
        vec3       *mesh_vertices,
        Geometry   *geometries,

        const u32 *mesh_bvh_node_counts,
        const u32 *mesh_triangle_counts,
        const u32 *mesh_vertex_counts,

		ClosestPointOnTriangle *results
) {
    u32 i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i >= vertex_count)
        return;

    Mesh *mesh = meshes;
    u32 nodes_offset = 0;
    u32 triangles_offset = 0;
    u32 vertex_offset = 0;
    for (u32 m = 0; m < mesh_count; m++, mesh++) {
        mesh->bvh.node_count = mesh_bvh_node_counts[m];
        mesh->triangles        = mesh_triangles + triangles_offset;
        mesh->vertex_positions = mesh_vertices + nodes_offset;
        mesh->bvh.nodes      = mesh_bvh_nodes + vertex_offset;

        nodes_offset        += mesh->bvh.node_count;
        triangles_offset    += mesh->triangle_count;
        vertex_offset       += mesh->vertex_count;
    }
    mesh = meshes + target_mesh_id;

    u32 stack[19];

    ClosestPointOnMesh query;
    query.mesh = mesh;
    query.mesh_transform = &geometries[target_geo_id].transform;
    query.search_origin_transform = &geometries[source_geo_id].transform;
    query.stack = stack;
    query.max_stack_size = 19;
    query.max_result_count = 50000;
    query.results = results;
    query.find(meshes[source_mesh_id].vertex_positions[i], max_distance, results[i], adaptive);
}

void runQueryOnGPU(ClosestPointOnMesh &query, Geometry *source_geo, Geometry *target_geo, f32 max_distance, bool adaptive, Scene &scene, bool nodes_are_drawing) {
    u32 mesh_count = scene.counts.meshes;
	u32 source_mesh_id = source_geo->id;
    u32 target_mesh_id = target_geo->id;
	u32 source_geo_id = (u32)(source_geo - scene.geometries);
    u32 target_geo_id = (u32)(target_geo - scene.geometries);
	u32 vertex_count = scene.meshes[source_mesh_id].vertex_count;


    u32 threads = 512;
    u32 blocks  = vertex_count / threads;
    if (vertex_count < threads) {
        threads = vertex_count;
        blocks = 1;
    } else if (vertex_count % threads)
        blocks++;

    uploadScene(scene);
    if (nodes_are_drawing) {
        Mesh *mesh = scene.meshes;
        u32 nodes_offset = 0;
        for (u32 m = 0; m < mesh_count; m++, mesh++) {
            if (m == target_mesh_id) {
                uploadNto(mesh->bvh.nodes, d_mesh_bvh_nodes, mesh->bvh.node_count, nodes_offset)
            }
            nodes_offset        += mesh->bvh.node_count;
        }
    }

    d_runQuery<<<blocks, threads>>>(
        source_mesh_id,
		target_mesh_id,
		source_geo_id,
		target_geo_id,

		max_distance,
		adaptive,

        vertex_count,
        mesh_count,

        d_mesh_bvh_nodes,
        d_meshes,
        d_triangles,
        d_vertices,
        d_geometries,

        d_mesh_bvh_node_counts,
        d_mesh_triangle_counts,
        d_mesh_vertex_counts,

        d_results);

    checkErrors()
    downloadN(d_results, query.results, vertex_count);

    if (nodes_are_drawing) {
        Mesh *mesh = scene.meshes;
        u32 nodes_offset = 0;
        for (u32 m = 0; m < mesh_count; m++, mesh++) {
            if (m == target_mesh_id) {
                downloadNto(d_mesh_bvh_nodes, mesh->bvh.nodes, mesh->bvh.node_count, nodes_offset)
            }
            nodes_offset        += mesh->bvh.node_count;
        }
    }
}