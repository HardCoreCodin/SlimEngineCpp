#pragma once

#include "./mesh.h"
#include "./grid.h"
#include "./curve.h"
#include "./box.h"
#include "./texture.h"


struct Selection {
    vec3 transformation_plane_origin,
            transformation_plane_normal,
            transformation_plane_center,
            object_scale,
            world_offset,
            *world_position;

    Geometry *geometry{nullptr};

    enum BoxSide box_side{NoSide};

    quat object_rotation;
    f32 object_distance{0};
    u32 geo_id;
    enum GeometryType geo_type;

    bool changed{false};

    static Transform xform;

    void draw(const Viewport &viewport, AABB *mesh_aabbs) {
        static Box box;

        if (controls::is_pressed::alt && !mouse::is_captured && geo_type && geometry) {
            xform = geometry->transform;
            if (geometry->type == GeometryType_Mesh)
                xform.scale *= mesh_aabbs[geometry->id].max;

            box.draw(viewport, xform, Color(Yellow), 0.5f, 0);
            if (box_side) {
                vec3 color = Color(White);
                switch (box_side) {
                    case Left:  case Right:  color = Color(Red);   break;
                    case Top:   case Bottom: color = Color(Green); break;
                    case Front: case Back:   color = Color(Blue);  break;
                    case NoSide: break;
                }

                box.draw(viewport, xform, color, 0.5f, 1, box_side);
            }
        }
    }

    void manipulate(const Viewport &viewport, Geometry *geometries, AABB *mesh_aabbs) {
        static Ray ray, local_ray;

        const Dimensions &dimensions = viewport.dimensions;
        Camera &camera = *viewport.camera;
        vec3 &cam_pos = camera.position;
        quat &cam_rot = camera.rotation;
        quat inv_rot = camera.rotation.conjugate();

        vec2i mouse_pos{mouse::pos.x - viewport.position.x,
                        mouse::pos.y - viewport.position.y};

        if (mouse::left_button.is_pressed) {
            if (!mouse::left_button.is_handled) { // This is the first frame after the left mouse button went down:
                mouse::left_button.is_handled = true;

                // Cast a ray onto the scene to find the closest object behind the hovered pixel:
                ray = viewport.getRayAt(mouse_pos);

                ray.hit.distance_squared = INFINITY;
                if (ray.hitScene(geometries, mesh_aabbs)) {
                    // Detect if object scene->selection has changed:
                    changed = (
                            geo_type  != ray.hit.geo_type ||
                            geo_id != ray.hit.geo_id
                    );

                    // Track the object that is now selected:
                    geo_type = ray.hit.geo_type;
                    geo_id   = ray.hit.geo_id;
                    geometry   = null;

                    // Capture a pointer to the selected object's position for later use in transformations:
                    geometry = geometries + geo_id;
                    world_position = &geometry->transform.position;
                    transformation_plane_origin = ray.hit.position;

                    world_offset = ray.hit.position - *world_position;

                    // Track how far away the hit position is from the camera along the depth axis:
                    object_distance = (inv_rot * (ray.hit.position - ray.origin)).z;
                } else {
                    if (geo_type)
                        changed = true;
                    geo_type = GeometryType_None;
                }
            }
        }
        if (geo_type) {
            if (controls::is_pressed::alt) {
                bool any_mouse_button_is_pressed = (
                        mouse::left_button.is_pressed ||
                        mouse::middle_button.is_pressed ||
                        mouse::right_button.is_pressed);
                if (geometry && !any_mouse_button_is_pressed) {
                    // Cast a ray onto the bounding box of the currently selected object:
                    ray = viewport.getRayAt(mouse_pos);

                    xform = geometry->transform;
                    if (geometry->type == GeometryType_Mesh)
                        xform.scale *= mesh_aabbs[geometry->id].max;

                    xform.internPosAndDir(ray.origin, ray.direction, local_ray.origin, local_ray.direction);

                    box_side = local_ray.hitCube();
                    if (box_side) {
                        transformation_plane_center = xform.externPos(local_ray.hit.normal);
                        transformation_plane_origin = xform.externPos(local_ray.hit.position);
                        transformation_plane_normal = xform.externDir(local_ray.hit.normal);
                        transformation_plane_normal = transformation_plane_normal.normalized();
                        world_offset = transformation_plane_origin - *world_position;
                        object_scale    = geometry->transform.scale;
                        object_rotation = geometry->transform.rotation;
                    }
                }

                if (box_side) {
                    if (geometry) {
                        if (any_mouse_button_is_pressed) {
                            ray = viewport.getRayAt(mouse_pos);
                            if (ray.hitPlane(transformation_plane_origin, transformation_plane_normal)) {
                                xform = geometry->transform;
                                if (geometry->type == GeometryType_Mesh)
                                    xform.scale *= mesh_aabbs[geometry->id].max;

                                if (mouse::left_button.is_pressed) {
                                    *world_position = ray.hit.position - world_offset;
                                } else if (mouse::middle_button.is_pressed) {
                                    ray.hit.position = xform.internPos(ray.hit.position);
                                    ray.hit.position /= xform.internPos(transformation_plane_origin);
                                    geometry->transform.scale = object_scale * ray.hit.position;
                                } else if (mouse::right_button.is_pressed) {
                                    vec3 v1{ ray.hit.position - transformation_plane_center };
                                    vec3 v2{ transformation_plane_origin - transformation_plane_center };
                                    quat rotation = quat{v2 ^ v1, v1 | v2 + sqrtf(v1.squaredLength() * v2.squaredLength())};
                                    geometry->transform.rotation = (rotation.normalized() * object_rotation).normalized();
                                }
                            }
                        }
                    }
                }
            } else {
                box_side = NoSide;
                if (mouse::left_button.is_pressed && mouse::moved) {
                    // Back-project the new mouse position onto a quad at a distance of the selected-object away from the camera

                    // Screen -> NDC:
                    f32 x = ((f32)mouse_pos.x + 0.5f) / dimensions.h_width  - 1;
                    f32 y = ((f32)mouse_pos.y + 0.5f) / dimensions.h_height - 1;

                    // NDC -> View:
                    x *= object_distance / (camera.focal_length * dimensions.height_over_width);
                    y *= object_distance / camera.focal_length;

                    // View -> World (Back-track by the world offset from the hit position back to the selected-object's center):
                    *world_position = cam_rot * vec3{x, -y, object_distance} + cam_pos - world_offset;
                }
            }
        }
    }
};
Transform Selection::xform;

struct Scene {
//    AmbientLight ambient_light;
    Selection selection;
//    Light *lights{nullptr};
//    Material *materials{nullptr};
//    Texture *textures{nullptr};
    Camera *cameras{nullptr};
    Mesh *meshes{nullptr};
    AABB *mesh_aabbs{nullptr};
    Geometry *geometries{nullptr};
    Curve *curves{nullptr};
    Grid *grids{nullptr};
    Box *boxes{nullptr};
    u64 last_io_ticks{0};
    bool last_io_is_save{false};

    Scene() {
        if (settings::scene::meshes && settings::scene::mesh_files) {
            mesh_aabbs = (AABB*)memory::allocate(sizeof(AABB) * settings::scene::meshes);
            Mesh *mesh = meshes = (Mesh*)memory::allocate(sizeof(Mesh) * settings::scene::meshes);
            if (meshes) for (u32 i = 0; i < settings::scene::meshes; i++, mesh++) {
                mesh = new(mesh) Mesh();
                mesh->load(settings::scene::mesh_files[i].char_ptr);
                mesh_aabbs[i] = mesh->aabb;
            }
        }

        if (settings::scene::cameras) {
            Camera *camera = cameras = (Camera*)memory::allocate(sizeof(Camera) * settings::scene::cameras);
            if (cameras) for (u32 i = 0; i < settings::scene::cameras; i++, camera++) new (camera)Camera;
        }

        if (settings::scene::geometries) {
            Geometry *geometry = geometries = (Geometry*)memory::allocate(sizeof(Geometry) * settings::scene::geometries);
            if (geometries) for (u32 i = 0; i < settings::scene::geometries; i++, geometry++) {
                geometry = new (geometry) Geometry;
                geometry->id = i;
            }
        }

        if (settings::scene::curves) {
            Curve *curve = curves = (Curve*)memory::allocate(sizeof(Curve) * settings::scene::curves);
            if (curves) for (u32 i = 0; i < settings::scene::curves; i++, curve++) new(curve) Curve;
        }

        if (settings::scene::boxes) {
            Box *box = boxes = (Box*)memory::allocate(sizeof(Box) * settings::scene::boxes);
            if (boxes) for (u32 i = 0; i < settings::scene::boxes; i++, box++) new(box) Box;
        }

        if (settings::scene::grids) {
            Grid *grid = grids = (Grid*)memory::allocate(sizeof(Grid) * settings::scene::grids);
            if (grids) for (u32 i = 0; i < settings::scene::grids; i++, grid++) new(grid) Grid{3, 3};
        }

//        if (settings::scene::lights) {
//            Light *light = lights = (Light*)memory::allocate(sizeof(Light) * settings::scene::lights);
//            for (u32 i = 0; i < settings::scene::lights; i++, light++) {
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
//        if (settings::scene::materials)   {
//            Material *material = materials = (Material*)memory::allocate(sizeof(Material) * settings::scene::materials);
//            for (u32 i = 0; i < settings::scene::materials; i++, material++) new(material) Material;
//        }
//
//        if (settings::scene::textures && settings::scene::texture_files) {
//            Texture *texture = textures = (Texture*)memory::allocate(sizeof(Texture) * settings::scene::textures);
//            if (textures) for (u32 i = 0; i < settings::scene::textures; i++, texture++) {
//                texture = new(texture) Texture;
//                texture->load(settings::scene::texture_files[i].char_ptr);
//            }
//        }
    }

    void load(char* file_path = nullptr) {
        if (file_path)
            settings::scene::file_path = file_path;
        else
            file_path = settings::scene::file_path.char_ptr;

        void *file_handle = os::openFileForReading(file_path);

        settings::scene::readFromFile(file_handle);

        if (settings::scene::cameras) {
            Camera *camera = cameras;
            for (u32 i = 0; i < settings::scene::cameras; i++, camera++) {
                os::readFromFile(&camera->focal_length, sizeof(f32), file_handle);
                os::readFromFile(&camera->zoom_amount, sizeof(f32), file_handle);
                os::readFromFile(&camera->dolly_amount, sizeof(f32), file_handle);
                os::readFromFile(&camera->target_distance, sizeof(f32), file_handle);
                os::readFromFile(&camera->current_velocity, sizeof(vec3), file_handle);
                os::readFromFile(&camera->position, sizeof(vec3), file_handle);
                os::readFromFile(&camera->right, sizeof(vec3), file_handle);
                os::readFromFile(&camera->up, sizeof(vec3), file_handle);
                os::readFromFile(&camera->forward, sizeof(vec3), file_handle);
                os::readFromFile(&camera->rotation, sizeof(quat), file_handle);
            }
        }

        if (settings::scene::geometries)
            for (u32 i = 0; i < settings::scene::geometries; i++)
                os::readFromFile(geometries + i, sizeof(Geometry), file_handle);

        if (settings::scene::grids)
            for (u32 i = 0; i < settings::scene::grids; i++)
                os::readFromFile(grids + i, sizeof(Grid), file_handle);

        if (settings::scene::boxes)
            for (u32 i = 0; i < settings::scene::boxes; i++)
                os::readFromFile(boxes + i, sizeof(Box), file_handle);

        if (settings::scene::curves)
            for (u32 i = 0; i < settings::scene::curves; i++)
                os::readFromFile(curves + i, sizeof(Curve), file_handle);

        if (settings::scene::meshes) {
            Mesh *mesh = meshes;
            for (u32 i = 0; i < settings::scene::meshes; i++, mesh++) {
                os::readFromFile(&mesh->aabb,           sizeof(AABB), file_handle);
                os::readFromFile(&mesh->vertex_count,   sizeof(u32),  file_handle);
                os::readFromFile(&mesh->triangle_count, sizeof(u32),  file_handle);
                os::readFromFile(&mesh->edge_count,     sizeof(u32),  file_handle);
                os::readFromFile(&mesh->uvs_count,      sizeof(u32),  file_handle);
                os::readFromFile(&mesh->normals_count,  sizeof(u32),  file_handle);

                os::readFromFile(mesh->vertex_positions,             sizeof(vec3)                  * mesh->vertex_count,   file_handle);
                os::readFromFile(mesh->vertex_position_indices,      sizeof(TriangleVertexIndices) * mesh->triangle_count, file_handle);
                os::readFromFile(mesh->edge_vertex_indices,          sizeof(EdgeVertexIndices)     * mesh->edge_count,     file_handle);
                if (mesh->uvs_count) {
                    os::readFromFile(mesh->vertex_uvs,               sizeof(vec2)                  * mesh->uvs_count,      file_handle);
                    os::readFromFile(mesh->vertex_uvs_indices,       sizeof(TriangleVertexIndices) * mesh->triangle_count, file_handle);
                }
                if (mesh->normals_count) {
                    os::readFromFile(mesh->vertex_normals,                sizeof(vec3)                  * mesh->normals_count,  file_handle);
                    os::readFromFile(mesh->vertex_normal_indices,         sizeof(TriangleVertexIndices) * mesh->triangle_count, file_handle);
                }
            }
        }

        os::closeFile(file_handle);
    }

    void save(char* file_path = nullptr) {
        if (file_path)
            settings::scene::file_path = file_path;
        else
            file_path = settings::scene::file_path.char_ptr;

        void *file_handle = os::openFileForWriting(file_path);

        settings::scene::writeToFile(file_handle);

        if (settings::scene::cameras) {
            Camera *camera = cameras;
            for (u32 i = 0; i < settings::scene::cameras; i++, camera++) {
                os::writeToFile(&camera->focal_length, sizeof(f32), file_handle);
                os::writeToFile(&camera->zoom_amount, sizeof(f32), file_handle);
                os::writeToFile(&camera->dolly_amount, sizeof(f32), file_handle);
                os::writeToFile(&camera->target_distance, sizeof(f32), file_handle);
                os::writeToFile(&camera->current_velocity, sizeof(vec3), file_handle);
                os::writeToFile(&camera->position, sizeof(vec3), file_handle);
                os::writeToFile(&camera->right, sizeof(vec3), file_handle);
                os::writeToFile(&camera->up, sizeof(vec3), file_handle);
                os::writeToFile(&camera->forward, sizeof(vec3), file_handle);
                os::writeToFile(&camera->rotation, sizeof(quat), file_handle);
            }
        }

        if (settings::scene::geometries)
            for (u32 i = 0; i < settings::scene::geometries; i++)
                os::writeToFile(geometries + i, sizeof(Geometry), file_handle);

        if (settings::scene::grids)
            for (u32 i = 0; i < settings::scene::grids; i++)
                os::writeToFile(grids + i, sizeof(Grid), file_handle);

        if (settings::scene::boxes)
            for (u32 i = 0; i < settings::scene::boxes; i++)
                os::writeToFile(boxes + i, sizeof(Box), file_handle);

        if (settings::scene::curves)
            for (u32 i = 0; i < settings::scene::curves; i++)
                os::writeToFile(curves + i, sizeof(Curve), file_handle);

        if (settings::scene::meshes) {
            Mesh *mesh = meshes;
            for (u32 i = 0; i < settings::scene::meshes; i++, mesh++) {
                os::writeToFile(&mesh->aabb,           sizeof(AABB), file_handle);
                os::writeToFile(&mesh->vertex_count,   sizeof(u32),  file_handle);
                os::writeToFile(&mesh->triangle_count, sizeof(u32),  file_handle);
                os::writeToFile(&mesh->edge_count,     sizeof(u32),  file_handle);
                os::writeToFile(&mesh->uvs_count,      sizeof(u32),  file_handle);
                os::writeToFile(&mesh->normals_count,  sizeof(u32),  file_handle);

                os::writeToFile(mesh->vertex_positions,        sizeof(vec3)                  * mesh->vertex_count,   file_handle);
                os::writeToFile(mesh->vertex_position_indices, sizeof(TriangleVertexIndices) * mesh->triangle_count, file_handle);
                os::writeToFile(mesh->edge_vertex_indices,     sizeof(EdgeVertexIndices)     * mesh->edge_count,     file_handle);
                if (mesh->uvs_count) {
                    os::writeToFile(mesh->vertex_uvs,          sizeof(vec2)                  * mesh->uvs_count,      file_handle);
                    os::writeToFile(mesh->vertex_uvs_indices,  sizeof(TriangleVertexIndices) * mesh->triangle_count, file_handle);
                }
                if (mesh->normals_count) {
                    os::writeToFile(mesh->vertex_normals,        sizeof(vec3)                  * mesh->normals_count,  file_handle);
                    os::writeToFile(mesh->vertex_normal_indices, sizeof(TriangleVertexIndices) * mesh->triangle_count, file_handle);
                }
            }
        }

        os::closeFile(file_handle);
    }

    void manipulateSelection(const Viewport &viewport) {
        selection.manipulate(viewport, geometries, mesh_aabbs);
    }
};