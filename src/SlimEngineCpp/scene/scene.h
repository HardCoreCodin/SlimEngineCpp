#pragma once

#include "./mesh.h"
#include "./grid.h"
#include "./box.h"
#include "./camera.h"
//#include "./texture.h"

struct SceneCounts {
    u32 cameras{1};
    u32 geometries{0};
    u32 grids{0};
    u32 boxes{0};
    u32 curves{0};
    u32 meshes{0};
};

struct Scene {
    SceneCounts counts;
    String file_path;
//    String *mesh_files{nullptr};
//    String *texture_files{nullptr};
//    AmbientLight ambient_light;
//    Light *lights{nullptr};
//    Material *materials{nullptr};
//    Texture *textures{nullptr};

    Geometry *geometries{nullptr};
    Curve *curves{nullptr};
    Grid *grids{nullptr};
    Box *boxes{nullptr};
    Camera *cameras{nullptr};
    Mesh *meshes{nullptr};
//    AABB *mesh_aabbs{nullptr};
    u64 last_io_ticks{0};
    bool last_io_is_save{false};

    Scene(SceneCounts counts,
          char *file_path = nullptr,
          Camera *cameras = nullptr,
          Geometry *geometries = nullptr,
          Grid *grids = nullptr,
          Box *boxes = nullptr,
          Curve *curves = nullptr,
          Mesh *meshes = nullptr,
          String *mesh_files = nullptr,
          memory::MonotonicAllocator *memory_allocator = nullptr
    ) : counts{counts},
        file_path{file_path},
        cameras{cameras},
        geometries{geometries},
        grids{grids},
        boxes{boxes},
        curves{curves},
        meshes{meshes}
    {
        if (meshes && mesh_files && counts.meshes) {
            meshes = new(meshes) Mesh[counts.meshes];
            memory::MonotonicAllocator temp_allocator;
            if (!memory_allocator) {
                u32 capacity = Mesh::getTotalMemoryForMeshes(mesh_files, 2);
                temp_allocator = memory::MonotonicAllocator{capacity};
                memory_allocator = &temp_allocator;
            }
            for (u32 i = 0; i < counts.meshes; i++) meshes[i].load(mesh_files[i].char_ptr, memory_allocator);
        }
//        if (counts.lights) {
//            Light *light = lights = (Light*)memory::allocate(sizeof(Light) * counts.lights);
//            for (u32 i = 0; i < counts.lights; i++, light++) {
//                light = new(light) Light;
//                for (u8 c = 0; c < 3; c++) {
//                    light->position_or_direction.components[c] = 0;
//                    light->color.components[c]                 = 1;
//                    light->attenuation.components[c]           = 1;
//                }
//                light->intensity = 1;
//                light->is_directional = false;
//            }
//        }
//
//        if (counts.materials)   {
//            Material *material = materials = (Material*)memory::allocate(sizeof(Material) * counts.materials);
//            for (u32 i = 0; i < counts.materials; i++, material++) new(material) Material;
//        }
//
//        if (counts.textures && counts.texture_files) {
//            Texture *texture = textures = (Texture*)memory::allocate(sizeof(Texture) * counts.textures);
//            if (textures) for (u32 i = 0; i < counts.textures; i++, texture++) {
//                texture = new(texture) Texture;
//                texture->load(counts.texture_files[i].char_ptr);
//            }
//        }
    }

    INLINE bool castRay(Ray &ray) const {
        static Ray local_ray;
        static Transform xform;

        bool found{false};
        bool current_found{false};

        Geometry *geo = geometries;

        for (u32 i = 0; i < counts.geometries; i++, geo++) {
            xform = geo->transform;
            if (geo->type == GeometryType_Mesh)
                xform.scale *= meshes[geo->id].aabb.max;

            xform.internPosAndDir(ray.origin, ray.direction, local_ray.origin, local_ray.direction);

            current_found = local_ray.hitCube();
            if (current_found) {
                local_ray.hit.position         = xform.externPos(local_ray.hit.position);
                local_ray.hit.distance_squared = (local_ray.hit.position - ray.origin).squaredLength();
                if (local_ray.hit.distance_squared < ray.hit.distance_squared) {
                    ray.hit = local_ray.hit;
                    ray.hit.geo_type = geo->type;
                    ray.hit.geo_id = i;
                    found = true;
                }
            }
        }

        if (found) {
            ray.hit.distance = sqrtf(ray.hit.distance_squared);
            ray.hit.normal = geometries[ray.hit.geo_id].transform.externDir(ray.hit.normal).normalized();
        }

        return found;
    }

    void load(char* scene_file_path = nullptr) {
        if (scene_file_path)
            file_path = scene_file_path;
        else
            scene_file_path = file_path.char_ptr;

        void *file_handle = os::openFileForReading(scene_file_path);

        os::readFromFile(&counts, sizeof(SceneCounts), file_handle);

        if (counts.cameras) {
            Camera *camera = cameras;
            for (u32 i = 0; i < counts.cameras; i++, camera++) {
                os::readFromFile(&camera->focal_length, sizeof(f32), file_handle);
                os::readFromFile(&camera->zoom_amount, sizeof(f32), file_handle);
                os::readFromFile(&camera->dolly_amount, sizeof(f32), file_handle);
                os::readFromFile(&camera->target_distance, sizeof(f32), file_handle);
                os::readFromFile(&camera->current_velocity, sizeof(vec3), file_handle);
                os::readFromFile(&camera->position, sizeof(vec3), file_handle);
                os::readFromFile(&camera->rotation, sizeof(Orientation<mat3>), file_handle);
            }
        }

        if (counts.geometries)
            for (u32 i = 0; i < counts.geometries; i++)
                os::readFromFile(geometries + i, sizeof(Geometry), file_handle);

        if (counts.grids)
            for (u32 i = 0; i < counts.grids; i++)
                os::readFromFile(grids + i, sizeof(Grid), file_handle);

        if (counts.boxes)
            for (u32 i = 0; i < counts.boxes; i++)
                os::readFromFile(boxes + i, sizeof(Box), file_handle);

        if (counts.curves)
            for (u32 i = 0; i < counts.curves; i++)
                os::readFromFile(curves + i, sizeof(Curve), file_handle);

        if (counts.meshes) {
            Mesh *mesh = meshes;
            for (u32 i = 0; i < counts.meshes; i++, mesh++) {
                mesh->readHeader(file_handle);
                mesh->readContent(file_handle);
            }
        }

        os::closeFile(file_handle);
    }

    void save(char* scene_file_path = nullptr) {
        if (scene_file_path)
            file_path = scene_file_path;
        else
            scene_file_path = file_path.char_ptr;

        void *file_handle = os::openFileForWriting(scene_file_path);

        os::writeToFile(&counts, sizeof(SceneCounts), file_handle);

        if (counts.cameras) {
            Camera *camera = cameras;
            for (u32 i = 0; i < counts.cameras; i++, camera++) {
                os::writeToFile(&camera->focal_length, sizeof(f32), file_handle);
                os::writeToFile(&camera->zoom_amount, sizeof(f32), file_handle);
                os::writeToFile(&camera->dolly_amount, sizeof(f32), file_handle);
                os::writeToFile(&camera->target_distance, sizeof(f32), file_handle);
                os::writeToFile(&camera->current_velocity, sizeof(vec3), file_handle);
                os::writeToFile(&camera->position, sizeof(vec3), file_handle);
                os::writeToFile(&camera->rotation, sizeof(Orientation<mat3>), file_handle);
            }
        }

        if (counts.geometries)
            for (u32 i = 0; i < counts.geometries; i++)
                os::writeToFile(geometries + i, sizeof(Geometry), file_handle);

        if (counts.grids)
            for (u32 i = 0; i < counts.grids; i++)
                os::writeToFile(grids + i, sizeof(Grid), file_handle);

        if (counts.boxes)
            for (u32 i = 0; i < counts.boxes; i++)
                os::writeToFile(boxes + i, sizeof(Box), file_handle);

        if (counts.curves)
            for (u32 i = 0; i < counts.curves; i++)
                os::writeToFile(curves + i, sizeof(Curve), file_handle);

        if (counts.meshes) {
            Mesh *mesh = meshes;
            for (u32 i = 0; i < counts.meshes; i++, mesh++) {
                mesh->writeHeader(file_handle);
                mesh->writeContent(file_handle);
            }
        }

        os::closeFile(file_handle);
    }
};