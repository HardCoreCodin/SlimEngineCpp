#include "./ClosestPointOnMesh.hpp"

void runQueryOnCPU(ClosestPointOnMesh &query, Geometry *source_geo, Scene &scene, f32 max_distance, bool adaptive) {
    query.search_origin_transform = &source_geo->transform;
    Mesh &source_mesh = scene.meshes[source_geo->id];
    query.find(source_mesh.vertex_positions, source_mesh.vertex_count, max_distance, adaptive);
}