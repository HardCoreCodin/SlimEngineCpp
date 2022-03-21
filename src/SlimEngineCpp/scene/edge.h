#pragma once

#include "../viewport/viewport.h"

struct Edge {
    vec3 from, to;

    bool cullAndClip(const Viewport &viewport, Edge &clipped_edge) const {
        f32 distance = viewport.frustum.near_clipping_plane_distance;

        vec3 A{from};
        vec3 B{to};

        u8 out = (A.z < distance) | ((B.z < distance) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, (distance - A.z) / (B.z - A.z));
            else         B = B.lerpTo(A, (distance - B.z) / (A.z - B.z));
        }

        distance = viewport.frustum.far_clipping_plane_distance;
        out = (A.z > distance) | ((B.z > distance) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, (A.z - distance) / (A.z - B.z));
            else         B = B.lerpTo(A, (B.z - distance) / (B.z - A.z));
        }

        // Left plane (facing to the right):
        vec3 N{viewport.camera->focal_length, 0, viewport.dimensions.width_over_height};
        f32 NdotA = N | A;
        f32 NdotB = N | B;

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        // Right plane (facing to the left):
        N.x = -N.x;
        NdotA = N | A;
        NdotB = N | B;

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        // Bottom plane (facing up):
        N = {0, viewport.camera->focal_length, 1};
        NdotA = N | A;
        NdotB = N | B;

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        // Top plane (facing down):
        N.y = -N.y;
        NdotA = N | A;
        NdotB = N | B;

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        clipped_edge.from = A;
        clipped_edge.to   = B;

        return true;
    }

    bool project(const Viewport &viewport, Edge &projected_edge) const {
        if (!cullAndClip(viewport, projected_edge))
            return false;

        vec4 A4{from, 1.0f};
        vec4 B4{to  , 1.0f};

        A4 = viewport.frustum.projection_matrix * A4;
        B4 = viewport.frustum.projection_matrix * B4;

        vec3 A{A4.x, A4.y, A4.z};
        vec3 B{B4.x, B4.y, B4.z};

        // Project:
        A /= A4.w;
        B /= B4.w;

        // NDC->screen:
        A.x += 1;
        B.x += 1;
        A.y += 1;
        B.y += 1;
        A.x *= viewport.dimensions.h_width;
        B.x *= viewport.dimensions.h_width;
        A.y *= viewport.dimensions.h_height;
        B.y *= viewport.dimensions.h_height;

        // Flip Y:
        A.y = viewport.dimensions.f_height - A.y;
        B.y = viewport.dimensions.f_height - B.y;

        projected_edge.from = A;
        projected_edge.to   = B;

        return true;
    }

    void draw(const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1) const {
        static Edge projected_edge;
        if (project(viewport, projected_edge))
            viewport.canvas.drawLine(projected_edge.from.x,
                                     projected_edge.from.y,
                                     projected_edge.from.z,
                                     projected_edge.to.x,
                                     projected_edge.to.y,
                                     projected_edge.to.z,
                                     color, opacity, line_width);
    }
};