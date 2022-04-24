#pragma once

#include "../draw/edge.h"
#include "../core/transform.h"
#include "../viewport/viewport.h"

#define CURVE_STEPS 360

void draw(const Curve &curve, const Transform &transform, const Viewport &viewport,
          const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1, u32 step_count = CURVE_STEPS) {
    const Camera &cam = *viewport.camera;

    f32 one_over_step_count = 1.0f / (f32)step_count;
    f32 rotation_step = one_over_step_count * TAU;
    f32 rotation_step_times_rev_count = rotation_step * (f32)curve.revolution_count;

    if (curve.type == CurveType::Helix)
        rotation_step = rotation_step_times_rev_count;

    vec3 center_to_orbit;
    center_to_orbit.x = 1;
    center_to_orbit.y = center_to_orbit.z = 0;

    vec3 orbit_to_curve;
    orbit_to_curve.x = curve.thickness;
    orbit_to_curve.y = orbit_to_curve.z = 0;

    mat3 rotation;
    rotation.X.x = rotation.Z.z = cosf(rotation_step);
    rotation.X.z = sinf(rotation_step);
    rotation.Z.x = -rotation.X.z;
    rotation.X.y = rotation.Z.y = rotation.Y.x = rotation.Y.z =  0;
    rotation.Y.y = 1;

    mat3 orbit_to_curve_rotation;
    if (curve.type == CurveType::Coil) {
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

        switch (curve.type) {
            case CurveType::Helix:
                current_position = center_to_orbit;
                current_position.y -= 1;
                break;
            case CurveType::Coil:
                orbit_to_curve  = orbit_to_curve_rotation * orbit_to_curve;
                current_position = accumulated_orbit_rotation * orbit_to_curve;
                current_position += center_to_orbit;
                break;
            default:
                break;
        }

        current_position = cam.internPos(transform.externPos(current_position));

        if (i) {
            edge.from = previous_position;
            edge.to   = current_position;
            draw(edge, viewport, color, opacity, line_width);
        }

        switch (curve.type) {
            case CurveType::Helix:
                center_to_orbit.y += 2 * one_over_step_count;
                break;
            case CurveType::Coil:
                accumulated_orbit_rotation *= rotation;
                break;
            default:
                break;
        }

        previous_position = current_position;
    }
}