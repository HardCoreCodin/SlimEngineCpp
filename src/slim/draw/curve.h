#pragma once

#include "../draw/edge.h"
#include "../core/transform.h"

void drawCurve(const Curve &curve, const Transform &transform, const Viewport &viewport,
               const Color &color = White, f32 opacity = 1.0f, u8 line_width = 0, u32 step_count = CURVE_STEPS) {
    const Camera &cam = *viewport.camera;

    f32 rotation_step = 1.0f / (f32)step_count;
    f32 helix_center_to_orbit_y_inc = rotation_step * 2;

    rotation_step *= TAU;
    f32 rotation_step_times_rev_count = rotation_step * (f32)curve.revolution_count;

    if (curve.type == CurveType_Helix)
        rotation_step = rotation_step_times_rev_count;

    vec3 center_to_orbit{1, 0, 0};
    vec3 orbit_to_curve{curve.thickness, 0, 0};
    mat3 rotation{mat3::RotationAroundY(rotation_step)};
    mat3 orbit_to_curve_rotation{mat3::RotationAroundZ(rotation_step_times_rev_count)};

    // Transform vertices positions of edges from view-space to screen-space (w/ culling and clipping):
    mat3 accumulated_orbit_rotation = rotation;
    vec3 local_position, local_previous_position, current_position, previous_position;
    Edge edge;

    for (u32 i = 0; i < step_count; i++) {
        local_position = center_to_orbit = rotation * center_to_orbit;

        switch (curve.type) {
            case CurveType_Helix: local_position.y -= 1; break;
            case CurveType_Coil:
                orbit_to_curve  = orbit_to_curve_rotation * orbit_to_curve;
                local_position += accumulated_orbit_rotation * orbit_to_curve;
                break;
            default: break;
        }

        current_position = cam.internPos(transform.externPos(local_position));

        if (i) {
            edge.from = previous_position;
            edge.to   = current_position;
            drawEdge(edge, viewport, color, opacity, line_width);
            if (curve.type == CurveType_Sphere) {
                edge.from.x = local_previous_position.x;
                edge.from.y = local_previous_position.z;
                edge.from.z = 0;

                edge.to.x = local_position.x;
                edge.to.y = local_position.z;
                edge.to.z = 0;

                edge.from = cam.internPos(transform.externPos(edge.from));
                edge.to   = cam.internPos(transform.externPos(edge.to));
                drawEdge(edge, viewport, color, opacity, line_width);

                edge.from.x = 0;
                edge.from.y = local_previous_position.x;
                edge.from.z = local_previous_position.z;

                edge.to.x = 0;
                edge.to.y = local_position.x;
                edge.to.z = local_position.z;

                edge.from = cam.internPos(transform.externPos(edge.from));
                edge.to   = cam.internPos(transform.externPos(edge.to));
                drawEdge(edge, viewport, color, opacity, line_width);
            }
        }

        switch (curve.type) {
            case CurveType_Helix: center_to_orbit.y += helix_center_to_orbit_y_inc; break;
            case CurveType_Coil:  accumulated_orbit_rotation *= rotation; break;
            default: break;
        }

        previous_position = current_position;
        local_previous_position = local_position;
    }
}