#pragma once

#include "./scene.h"
#include "../core/ray.h"
#include "../core/transform.h"
#include "../viewport/viewport.h"

struct Selection {
    Transform xform;
    quat object_rotation;
    vec3 transformation_plane_origin,
         transformation_plane_normal,
         transformation_plane_center,
         object_scale,
         world_offset,
         *world_position{nullptr};
    Geometry *geometry{nullptr};
    f32 object_distance = 0;
    u32 geo_id = 0;
    GeometryType geo_type = GeometryType_None;
    BoxSide box_side = NoSide;
    bool changed = false;
    bool left_mouse_button_was_pressed = false;

    void manipulate(const Viewport &viewport, const Scene &scene) {
        static Ray ray, local_ray;

        const Dimensions &dimensions = viewport.dimensions;
        Camera &camera = *viewport.camera;
        f32 x = (f32)(mouse::pos_x - viewport.bounds.left);
        f32 y = (f32)(mouse::pos_y - viewport.bounds.top);

        ray.origin = camera.position;
        ray.direction = camera.getRayDirectionAt(x, y, dimensions.f_width, dimensions.f_height);
        ray.hit.distance_squared = INFINITY;

        if (mouse::left_button.is_pressed && !left_mouse_button_was_pressed) {
            // This is the first frame after the left mouse button went down:
            // Cast a ray onto the scene to find the closest object behind the hovered pixel:
            if (scene.castRay(ray)) {
                // Detect if object scene->selection has changed:
                changed = (
                    geo_type != ray.hit.geo_type ||
                    geo_id != ray.hit.geo_id
                );

                // Track the object that is now selected:
                geo_type = ray.hit.geo_type;
                geo_id   = ray.hit.geo_id;
                geometry = nullptr;

                // Capture a pointer to the selected object's position for later use in transformations:
                geometry = scene.geometries + geo_id;
                world_position = &geometry->transform.position;
                transformation_plane_origin = ray.hit.position;

                world_offset = ray.hit.position - *world_position;

                // Track how far away the hit position is from the camera along the depth axis:
                object_distance = (camera.rotation.transposed() * (ray.hit.position - ray.origin)).z;
            } else {
                if (geo_type) changed = true;
                geo_type = GeometryType_None;
            }
        }
        left_mouse_button_was_pressed = mouse::left_button.is_pressed;
        if (geo_type) {
            if (controls::is_pressed::alt) {
                bool any_mouse_button_is_pressed = (
                        mouse::left_button.is_pressed ||
                        mouse::middle_button.is_pressed ||
                        mouse::right_button.is_pressed);
                if (geometry && !any_mouse_button_is_pressed) {
                    // Cast a ray onto the bounding box of the currently selected object:
                    xform = geometry->transform;
                    if (geometry->type == GeometryType_Mesh)
                        xform.scale *= scene.meshes[geometry->id].aabb.max;

                    xform.internPosAndDir(ray.origin, ray.direction, local_ray.origin, local_ray.direction);

                    box_side = local_ray.hitsCube();
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
                            if (ray.hitsPlane(transformation_plane_origin, transformation_plane_normal)) {
                                xform = geometry->transform;
                                if (geometry->type == GeometryType_Mesh)
                                    xform.scale *= scene.meshes[geometry->id].aabb.max;

                                if (mouse::left_button.is_pressed) {
                                    *world_position = ray.hit.position - world_offset;
                                } else if (mouse::middle_button.is_pressed) {
                                    ray.hit.position = xform.internPos(ray.hit.position);
                                    ray.hit.position /= xform.internPos(transformation_plane_origin);
                                    geometry->transform.scale = object_scale * ray.hit.position;
                                } else if (mouse::right_button.is_pressed) {
                                    vec3 v1{ ray.hit.position - transformation_plane_center };
                                    vec3 v2{ transformation_plane_origin - transformation_plane_center };
                                    quat rotation = quat{v2.cross(v1), (v1.dot(v2)) + sqrtf(v1.squaredLength() * v2.squaredLength())};
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
                    x = (x + 0.5f) / dimensions.h_width  - 1;
                    y = (y + 0.5f) / dimensions.h_height - 1;

                    // NDC -> View:
                    x *= object_distance / (camera.focal_length * dimensions.height_over_width);
                    y *= object_distance / camera.focal_length;

                    // View -> World (Back-track by the world offset from the hit position back to the selected-object's center):
                    *world_position = camera.rotation * vec3{x, -y, object_distance} + camera.position - world_offset;
                }
            }
        }
    }
};