#pragma once

#include "./ClosestPointOnMeshCPU.hpp"

#ifdef __CUDACC__
#include "./ClosestPointOnMeshGPU.hpp"
#define USE_GPU_BY_DEFAULT true

void runQueryOnXPU(ClosestPointOnMesh &query, Geometry *source_geo, Geometry *target_geo, f32 max_distance, bool adaptive, Scene &scene, bool nodes_are_drawing, bool on_gpu = false) {
    if (on_gpu) runQueryOnGPU(query, source_geo, target_geo, max_distance, adaptive, scene, nodes_are_drawing);
    else        runQueryOnCPU(query, source_geo, scene, max_distance, adaptive);
}

#else
#define USE_GPU_BY_DEFAULT false

void allocateDeviceScene(Scene &scene) {}
void uploadScene(Scene &scene) {}
void uploadMeshes(Scene &scene) {}

void runQueryOnXPU(ClosestPointOnMesh &query, Geometry *source_geo, Geometry *target_geo, f32 max_distance, bool adaptive, Scene &scene, bool nodes_are_drawing, bool on_gpu = false) {
    runQueryOnCPU(query, source_geo, scene, max_distance, adaptive);
}
#endif

