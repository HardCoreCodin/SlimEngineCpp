#pragma once

#include "../core/types.h"

struct Frustum {
    mat4 projection_matrix{};
    f32 near_clipping_plane_distance{VIEWPORT_DEFAULT__NEAR_CLIPPING_PLANE_DISTANCE};
    f32 far_clipping_plane_distance{ VIEWPORT_DEFAULT__FAR_CLIPPING_PLANE_DISTANCE};
    bool use_cube_NDC{false}, flip_z{false}, cull_back_faces{true};

    Frustum() {
        updateProjectionMatrix(CAMERA_DEFAULT__FOCAL_LENGTH, (f32)DEFAULT_HEIGHT / (f32)DEFAULT_WIDTH);
    }

    void updateProjectionMatrix(f32 focal_length, f32 height_over_width) {
        const f32 n = near_clipping_plane_distance;
        const f32 f = far_clipping_plane_distance;
        const f32 d = 1.0f / (f - n);
        projection_matrix.X = { focal_length * height_over_width, 0, 0, 0};
        projection_matrix.Y = {0, focal_length, 0, 0};
        projection_matrix.Z = {0, 0, (use_cube_NDC ? (f + n) : f) * d, 1.0f};
        projection_matrix.W = {0, 0, (use_cube_NDC ? (-2 * f * n) : (-n * f)) * d, 0};
    }

    bool cullAndClipEdge(Edge &edge, f32 focal_length, f32 aspect_ratio) const {
        f32 distance = near_clipping_plane_distance;

        vec3 A{edge.from};
        vec3 B{edge.to};

        u8 out = (A.z < distance) | ((B.z < distance) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, (distance - A.z) / (B.z - A.z));
            else         B = B.lerpTo(A, (distance - B.z) / (A.z - B.z));
        }

        distance = far_clipping_plane_distance;
        out = (A.z > distance) | ((B.z > distance) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, (A.z - distance) / (A.z - B.z));
            else         B = B.lerpTo(A, (B.z - distance) / (B.z - A.z));
        }

        // Left plane (facing to the right):
        vec3 N{focal_length, 0, aspect_ratio};
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
        N = {0, focal_length, 1};
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

        edge.from = A;
        edge.to   = B;

        return true;
    }

    void projectEdge(Edge &edge, const Dimensions &dimensions) const {
        vec4 A4{edge.from, 1.0f};
        vec4 B4{edge.to  , 1.0f};

        A4 = projection_matrix * A4;
        B4 = projection_matrix * B4;

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
        A.x *= dimensions.h_width;
        B.x *= dimensions.h_width;
        A.y *= dimensions.h_height;
        B.y *= dimensions.h_height;

        // Flip Y:
        A.y = dimensions.f_height - A.y;
        B.y = dimensions.f_height - B.y;

        edge.from = A;
        edge.to   = B;
    }
};
