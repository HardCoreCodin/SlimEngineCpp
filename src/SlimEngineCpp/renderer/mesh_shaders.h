#pragma once

#include "../renderer/rasterizer.h"


//u8 shadeMesh(const Mesh &mesh, const Rasterizer &rasterizer) {
//    // Transform the mesh's vertex positions into clip space and world space:
//    vec4 world_space;
//    for (u32 i = 0; i < mesh.vertex_count; i++) {
//        world_space = vec4{mesh.vertex_positions[i], 1.0f};
//        world_space = rasterizer.model_to_world * world_space;
//        rasterizer.clip_space_vertex_positions[i] = rasterizer.world_to_clip * world_space;
//        rasterizer.world_space_vertex_positions[i] = world_space.v3;
//    }
//
//    // Transform the mesh's vertex normals into world space:
//    for (u32 i = 0; i < mesh.normals_count; i++) {
//        world_space = vec4{mesh.vertex_normals[i]};
//        world_space = rasterizer.model_to_world_inverted_transposed * world_space;
//        rasterizer.world_space_vertex_normals[i] = world_space.v3;
//    }
//
//    return INSIDE;
//}
//
