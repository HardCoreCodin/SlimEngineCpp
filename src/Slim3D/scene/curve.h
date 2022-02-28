#pragma once

#include "../scene/edge.h"

#define CURVE_STEPS 3600

struct Curve {
    f32 thickness{0.1f};
    u32 revolution_count{1};

    void draw(const Viewport &viewport, const Geometry &geometry, const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1, u32 step_count = CURVE_STEPS) const {
        const Transform &xform = geometry.transform;
        const Camera &cam = *viewport.camera;

        f32 one_over_step_count = 1.0f / (f32)step_count;
        f32 rotation_step = one_over_step_count * TAU;
        f32 rotation_step_times_rev_count = rotation_step * (f32)revolution_count;

        if (geometry.type == GeometryType_Helix)
            rotation_step = rotation_step_times_rev_count;

        vec3 center_to_orbit;
        center_to_orbit.x = 1;
        center_to_orbit.y = center_to_orbit.z = 0;

        vec3 orbit_to_curve;
        orbit_to_curve.x = thickness;
        orbit_to_curve.y = orbit_to_curve.z = 0;

        mat3 rotation;
        rotation.X.x = rotation.Z.z = cosf(rotation_step);
        rotation.X.z = sinf(rotation_step);
        rotation.Z.x = -rotation.X.z;
        rotation.X.y = rotation.Z.y = rotation.Y.x = rotation.Y.z =  0;
        rotation.Y.y = 1;

        mat3 orbit_to_curve_rotation;
        if (geometry.type == GeometryType_Coil) {
            orbit_to_curve_rotation.X.x = orbit_to_curve_rotation.Y.y = cosf(rotation_step_times_rev_count);
            orbit_to_curve_rotation.X.y = sinf(rotation_step_times_rev_count);
            orbit_to_curve_rotation.Y.x = -orbit_to_curve_rotation.X.y;
            orbit_to_curve_rotation.X.z = orbit_to_curve_rotation.Y.z = orbit_to_curve_rotation.Z.x = orbit_to_curve_rotation.Z.y =  0;
            orbit_to_curve_rotation.Z.z = 1;
        }

        // Transform vertices positions of edges from view-space to screen-space (w/ culling and clipping):
        mat3 accumulated_orbit_rotation = rotation;
        vec3 current_position, previous_position;
        Edge edge;

        for (u32 i = 0; i < step_count; i++) {
            center_to_orbit = rotation * center_to_orbit;

            switch (geometry.type) {
                case GeometryType_Helix:
                    current_position = center_to_orbit;
                    current_position.y -= 1;
                    break;
                case GeometryType_Coil:
                    orbit_to_curve  = orbit_to_curve_rotation * orbit_to_curve;
                    current_position = accumulated_orbit_rotation * orbit_to_curve;
                    current_position += center_to_orbit;
                    break;
                default:
                    break;
            }

            current_position = cam.internPos(xform.externPos(current_position));

            if (i) {
                edge.from = previous_position;
                edge.to   = current_position;
                edge.draw(viewport, color, opacity, line_width);
            }

            switch (geometry.type) {
                case GeometryType_Helix:
                    center_to_orbit.y += 2 * one_over_step_count;
                    break;
                case GeometryType_Coil:
                    accumulated_orbit_rotation *= rotation;
                    break;
                default:
                    break;
            }

            previous_position = current_position;
        }
    }
};