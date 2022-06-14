#pragma once

#include "../scene/scene.h"

void load(Scene &scene, char* scene_file_path = nullptr) {
    if (scene_file_path)
        scene.file_path = scene_file_path;
    else
        scene_file_path = scene.file_path.char_ptr;

    void *file_handle = os::openFileForReading(scene_file_path);

    os::readFromFile(&scene.counts, sizeof(SceneCounts), file_handle);

    if (scene.counts.cameras) {
        Camera *camera = scene.cameras;
        for (u32 i = 0; i < scene.counts.cameras; i++, camera++) {
            os::readFromFile(&camera->focal_length, sizeof(f32), file_handle);
            os::readFromFile(&camera->zoom_amount, sizeof(f32), file_handle);
            os::readFromFile(&camera->dolly_amount, sizeof(f32), file_handle);
            os::readFromFile(&camera->target_distance, sizeof(f32), file_handle);
            os::readFromFile(&camera->current_velocity, sizeof(vec3), file_handle);
            os::readFromFile(&camera->position, sizeof(vec3), file_handle);
            os::readFromFile(&camera->rotation, sizeof(Orientation<mat3>), file_handle);
        }
    }

    if (scene.counts.geometries)
        for (u32 i = 0; i < scene.counts.geometries; i++)
            os::readFromFile(scene.geometries + i, sizeof(Geometry), file_handle);

    if (scene.counts.grids)
        for (u32 i = 0; i < scene.counts.grids; i++)
            os::readFromFile(scene.grids + i, sizeof(Grid), file_handle);

    if (scene.counts.boxes)
        for (u32 i = 0; i < scene.counts.boxes; i++)
            os::readFromFile(scene.boxes + i, sizeof(Box), file_handle);

    if (scene.counts.curves)
        for (u32 i = 0; i < scene.counts.curves; i++)
            os::readFromFile(scene.curves + i, sizeof(Curve), file_handle);

    if (scene.counts.meshes) {
        Mesh *mesh = scene.meshes;
        for (u32 i = 0; i < scene.counts.meshes; i++, mesh++) {
            readHeader(*mesh, file_handle);
            readContent(*mesh, file_handle);
        }
    }

    os::closeFile(file_handle);
}

void save(Scene &scene, char* scene_file_path = nullptr) {
    if (scene_file_path)
        scene.file_path = scene_file_path;
    else
        scene_file_path = scene.file_path.char_ptr;

    void *file_handle = os::openFileForWriting(scene_file_path);

    os::writeToFile(&scene.counts, sizeof(SceneCounts), file_handle);

    if (scene.counts.cameras) {
        Camera *camera = scene.cameras;
        for (u32 i = 0; i < scene.counts.cameras; i++, camera++) {
            os::writeToFile(&camera->focal_length, sizeof(f32), file_handle);
            os::writeToFile(&camera->zoom_amount, sizeof(f32), file_handle);
            os::writeToFile(&camera->dolly_amount, sizeof(f32), file_handle);
            os::writeToFile(&camera->target_distance, sizeof(f32), file_handle);
            os::writeToFile(&camera->current_velocity, sizeof(vec3), file_handle);
            os::writeToFile(&camera->position, sizeof(vec3), file_handle);
            os::writeToFile(&camera->rotation, sizeof(Orientation<mat3>), file_handle);
        }
    }

    if (scene.counts.geometries)
        for (u32 i = 0; i < scene.counts.geometries; i++)
            os::writeToFile(scene.geometries + i, sizeof(Geometry), file_handle);

    if (scene.counts.grids)
        for (u32 i = 0; i < scene.counts.grids; i++)
            os::writeToFile(scene.grids + i, sizeof(Grid), file_handle);

    if (scene.counts.boxes)
        for (u32 i = 0; i < scene.counts.boxes; i++)
            os::writeToFile(scene.boxes + i, sizeof(Box), file_handle);

    if (scene.counts.curves)
        for (u32 i = 0; i < scene.counts.curves; i++)
            os::writeToFile(scene.curves + i, sizeof(Curve), file_handle);

    if (scene.counts.meshes) {
        Mesh *mesh = scene.meshes;
        for (u32 i = 0; i < scene.counts.meshes; i++, mesh++) {
            writeHeader(*mesh, file_handle);
            writeContent(*mesh, file_handle);
        }
    }

    os::closeFile(file_handle);
}