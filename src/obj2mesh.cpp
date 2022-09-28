#ifdef COMPILER_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <stdio.h>
#include <string.h>
#include <unordered_set>

#include "./slim/platforms/win32_base.h"
#include "./slim/scene/bvh_builder.h"
#include "./slim/serialization/mesh.h"

// Or using the single-header file:
// #include "../slim.h"

enum VertexAttributes {
    VertexAttributes_None,
    VertexAttributes_Positions,
    VertexAttributes_PositionsAndUVs,
    VertexAttributes_PositionsUVsAndNormals
};

int obj2mesh(char* obj_file_path, char* mesh_file_path, bool invert_winding_order = false, f32 scale = 1, float rotY = 0) {
    const u8 v1_id = 0;
    const u8 v2_id = invert_winding_order ? 2 : 1;
    const u8 v3_id = invert_winding_order ? 1 : 2;

    int vertex_indices[3];
    int normal_indices[3];
    int uvs_indices[3];

    u32 f, t;

    std::unordered_set<u64> edge_hashes;

    Mesh mesh;
    mesh.triangle_count = 0;
    mesh.normals_count = 0;
    mesh.vertex_count = 0;
    mesh.edge_count = 0;
    mesh.uvs_count = 0;
    mesh.vertex_normals          = nullptr;
    mesh.vertex_normal_indices   = nullptr;
    mesh.vertex_uvs              = nullptr;
    mesh.vertex_uvs_indices      = nullptr;

    FILE* obj_file;

    obj_file = fopen(obj_file_path, "r");
    char line[1024];

    enum VertexAttributes vertex_attributes = VertexAttributes_None;
    while (fgets(line, 1024, obj_file)) {
        if (strncmp(line, (char*)"vn ", 2) == 0) mesh.normals_count++;
        if (strncmp(line, (char*)"vt ", 2) == 0) mesh.uvs_count++;
        if (strncmp(line, (char*)"v ", 2) == 0) mesh.vertex_count++;
        if (strncmp(line, (char*)"f ", 2) == 0) {
            mesh.triangle_count++;
            if (vertex_attributes == VertexAttributes_None) {
                int forward_slash_count = 0;
                char *character = line;
                while (*character) {
                    if ((*character) == '/') forward_slash_count++;
                    character++;
                }
                switch (forward_slash_count) {
                    case 0: vertex_attributes = VertexAttributes_Positions; break;
                    case 3: vertex_attributes = VertexAttributes_PositionsAndUVs; break;
                    case 6: vertex_attributes = VertexAttributes_PositionsUVsAndNormals; break;
                    default: break;
                }
            }

            switch (vertex_attributes) {
                case VertexAttributes_Positions:
                    sscanf(
                            line, (char*)"f %d %d %d",
                            &vertex_indices[v1_id],
                            &vertex_indices[v2_id],
                            &vertex_indices[v3_id]
                    );
                    break;
                case VertexAttributes_PositionsAndUVs:
                    sscanf(
                            line, (char*)"f %d/%d %d/%d %d/%d",
                            &vertex_indices[v1_id], &uvs_indices[v1_id],
                            &vertex_indices[v2_id], &uvs_indices[v2_id],
                            &vertex_indices[v3_id], &uvs_indices[v3_id]
                    );
                    break;
                case VertexAttributes_PositionsUVsAndNormals:
                    sscanf(
                            line, (char*)"f %d/%d/%d %d/%d/%d %d/%d/%d",
                            &vertex_indices[v1_id], &uvs_indices[v1_id], &normal_indices[v1_id],
                            &vertex_indices[v2_id], &uvs_indices[v2_id], &normal_indices[v2_id],
                            &vertex_indices[v3_id], &uvs_indices[v3_id], &normal_indices[v3_id]
                    );
                    break;
                default:
                    return 1;
            }

            for (u8 i = 0; i < 3; i++) vertex_indices[i]--;

            u64 edge_hash1, edge_hash2;
            for (u8 from = 0, to = 1; from < 3; from++, to = (to + 1) % 3) {
                f = vertex_indices[from];
                t = vertex_indices[to];
                edge_hash1 = (u64)f + ((u64)t << 32);
                edge_hash2 = (u64)t + ((u64)f << 32);
                if (edge_hashes.find(edge_hash1) == edge_hashes.end() &&
                    edge_hashes.find(edge_hash2) == edge_hashes.end()) {
                    edge_hashes.insert(edge_hash1);
                    mesh.edge_count++;
                }
            }
        }
    }
    fclose(obj_file);

    mesh.bvh.node_count = mesh.triangle_count * 2;
    mesh.bvh.height = (u8)mesh.triangle_count;

    u64 memory_capacity = getSizeInBytes(mesh);
    memory_capacity += BVHBuilder::getSizeInBytes(mesh.triangle_count * 2);
    memory::MonotonicAllocator memory_allocator{memory_capacity};
    allocateMemory(mesh, &memory_allocator);
    BVHBuilder builder{&mesh, 1, &memory_allocator};

    vec3 *vertex_position = mesh.vertex_positions;
    vec3 *vertex_normal = mesh.vertex_normals;
    vec2 *vertex_uvs = mesh.vertex_uvs;
    TriangleVertexIndices *vertex_position_indices = mesh.vertex_position_indices;
    TriangleVertexIndices *vertex_normal_indices = mesh.vertex_normal_indices;
    TriangleVertexIndices *vertex_uvs_indices = mesh.vertex_uvs_indices;

    obj_file = fopen(obj_file_path, (char*)"r");
    while (fgets(line, 1024, obj_file)) {
        // Vertex information
        if (strncmp(line, (char*)"v ", 2) == 0) {
            sscanf(line, (char*)"v %f %f %f", &vertex_position->x, &vertex_position->y, &vertex_position->z);
            vertex_position++;
        } else if (strncmp(line, (char*)"vn ", 2) == 0) {
            sscanf(line, (char*)"vn %f %f %f", &vertex_normal->x, &vertex_normal->y, &vertex_normal->z);
            vertex_normal++;
        } else if (strncmp(line, (char*)"vt ", 2) == 0) {
            sscanf(line, (char*)"vt %f %f", &vertex_uvs->x, &vertex_uvs->y);
            vertex_uvs++;
        } else if (strncmp(line, (char*)"f ", 2) == 0) {
            switch (vertex_attributes) {
                case VertexAttributes_Positions:
                    sscanf(
                            line, (char*)"f %d %d %d",
                            &vertex_indices[v1_id],
                            &vertex_indices[v2_id],
                            &vertex_indices[v3_id]
                    );
                    break;
                case VertexAttributes_PositionsAndUVs:
                    sscanf(
                            line, (char*)"f %d/%d %d/%d %d/%d",
                            &vertex_indices[v1_id], &uvs_indices[v1_id],
                            &vertex_indices[v2_id], &uvs_indices[v2_id],
                            &vertex_indices[v3_id], &uvs_indices[v3_id]
                    );
                    vertex_uvs_indices->ids[0] = uvs_indices[0] - 1;
                    vertex_uvs_indices->ids[1] = uvs_indices[1] - 1;
                    vertex_uvs_indices->ids[2] = uvs_indices[2] - 1;
                    vertex_uvs_indices++;
                    break;
                case VertexAttributes_PositionsUVsAndNormals:
                    sscanf(
                            line, (char*)"f %d/%d/%d %d/%d/%d %d/%d/%d",
                            &vertex_indices[v1_id], &uvs_indices[v1_id], &normal_indices[v1_id],
                            &vertex_indices[v2_id], &uvs_indices[v2_id], &normal_indices[v2_id],
                            &vertex_indices[v3_id], &uvs_indices[v3_id], &normal_indices[v3_id]
                    );
                    vertex_uvs_indices->ids[0] = uvs_indices[0] - 1;
                    vertex_uvs_indices->ids[1] = uvs_indices[1] - 1;
                    vertex_uvs_indices->ids[2] = uvs_indices[2] - 1;
                    vertex_normal_indices->ids[0] = normal_indices[0] - 1;
                    vertex_normal_indices->ids[1] = normal_indices[1] - 1;
                    vertex_normal_indices->ids[2] = normal_indices[2] - 1;
                    vertex_normal_indices++;
                    vertex_uvs_indices++;
                    break;
                default:
                    return 1;
            }
            vertex_position_indices->ids[0] = vertex_indices[0] - 1;
            vertex_position_indices->ids[1] = vertex_indices[1] - 1;
            vertex_position_indices->ids[2] = vertex_indices[2] - 1;
            vertex_position_indices++;
        }
    }
    fclose(obj_file);

    f = 0;
    for (u64 edge_hash : edge_hashes) mesh.edge_vertex_indices[f++] = {(u32)edge_hash, (u32)(edge_hash >> 32)};

    mat3 rot;
    if (rotY) {
        rot = mat3::RotationAroundY(rotY *  DEG_TO_RAD);
        for (u32 i = 0; i < mesh.normals_count; i++) {
            vec3 &normal = mesh.vertex_normals[i];
            normal = rot * normal;
        }
    }

    mesh.aabb.min = INFINITY;
    mesh.aabb.max = -INFINITY;
    for (u32 i = 0; i < mesh.vertex_count; i++) {
        vec3 &position = mesh.vertex_positions[i];
        position *= scale;
        if (rotY) position = rot * position;
        mesh.aabb.min = minimum(mesh.aabb.min, position);
        mesh.aabb.max = maximum(mesh.aabb.max, position);
    }

    vec3 centroid = (mesh.aabb.min + mesh.aabb.max) / 2.0f;
    if (centroid.nonZero()) {
        mesh.aabb.min -= centroid;
        mesh.aabb.max -= centroid;
        for (u32 i = 0; i < mesh.vertex_count; i++)
            mesh.vertex_positions[i] -= centroid;
    }

    builder.buildMesh(mesh);
    save(mesh, mesh_file_path);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 2 && !strcmp(argv[1], (char*)"--help")) {
        printf((char*)("Exactly 2 file paths need to be provided: "
                       "An '.obj' file (input) then a '.mesh' file (output), "
                       "an optional flag '-invert_winding_order' for inverting winding order"
                       "an optional flag 'scale:<float>' for scaling the mesh,"
                       "an optional flag 'rotY:<float> for rotating the mesh around Y"
                       ));
        return 0;
    } else if (argc == 3 || // 2 arguments
               argc == 4 || // 3 arguments
               argc == 5 || // 4 arguments
               argc == 6    // 5 arguments
            ) {
        char *obj_file_path = argv[1];
        char *mesh_file_path = argv[2];
        if (argc == 3) return obj2mesh(obj_file_path, mesh_file_path);

        bool invert_winding_order = false;
        float scale{1}, rotY{0};
        for (u32 i = 3; i < (u32)argc; i++) {
            char *arg = argv[i];
            if (strcmp(arg, (char *) "-invert_winding_order") == 0)
                invert_winding_order = true;
            else {
                char *scale_arg_prefix = (char *) "scale:";
                bool is_scale_arg = true;
                for (u32 c = 0; c < 6; c++)
                    if (arg[c] != scale_arg_prefix[c]) {
                        is_scale_arg = false;
                        break;
                    }
                if (is_scale_arg)
                    scale = (f32)atof(arg + 6);
                else {
                    char *rotY_arg_prefix = (char *) "rotY:";
                    bool is_rotY_arg = true;
                    for (u32 c = 0; c < 5; c++)
                        if (arg[c] != rotY_arg_prefix[c]) {
                            is_rotY_arg = false;
                            break;
                        }
                    if (is_rotY_arg) rotY = (f32)atof(arg + 5);
                }
            }
        }
        return obj2mesh(obj_file_path, mesh_file_path, invert_winding_order, scale, rotY);
    }

    printf((char*)("Exactly 2 file paths need to be provided: "
                   "An '.obj' file (input) then a '.mesh' file (output), "
                   "and an optional flag '-invert_winding_order' for inverting winding order"));
    return 1;
}


//suzanne.obj suzanne.mesh -invert_winding_order scale:2 rotY:90