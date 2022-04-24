#pragma once

#include "../math/vec3.h"

struct Frustum {
    struct Projection {
        union {
            struct {
                vec3 scale;
                f32 shear;
            };
            struct {
                vec3 projected_position;
                f32 w;
            };
        };

        Projection(f32 x, f32 y, f32 z, f32 w) : scale{x, y, z}, shear{w} {}
        Projection(f32 focal_length, f32 height_over_width, f32 n, f32 f, bool cube_NDC) : scale{0}, shear{0} {
            update(focal_length, height_over_width, n, f, cube_NDC);
        }
        Projection(const Projection &other) : scale{other.scale}, shear{other.shear} {}


        void update(f32 focal_length, f32 height_over_width, f32 n, f32 f, bool cube_NDC) {
            scale.x = focal_length * height_over_width;
            scale.y = focal_length;
            scale.z = shear = 1.0f / (f - n);
            if (cube_NDC) {
                scale.z *= f + n;
                shear *= f * n * -2;
            } else {
                scale.z *= f;
                shear *= f * -n;
            }
        }

        Projection project(const vec3 &position) const {
            return {
                position.x * scale.x,
                position.y * scale.y,
                position.z * scale.z + shear,
                position.z
            };
        }
    };
    Projection projection{
            CAMERA_DEFAULT__FOCAL_LENGTH,
            (f32)DEFAULT_HEIGHT / (f32)DEFAULT_WIDTH,
            VIEWPORT_DEFAULT__NEAR_CLIPPING_PLANE_DISTANCE,
            VIEWPORT_DEFAULT__FAR_CLIPPING_PLANE_DISTANCE,
            false
    };

    f32 near_clipping_plane_distance{VIEWPORT_DEFAULT__NEAR_CLIPPING_PLANE_DISTANCE};
    f32 far_clipping_plane_distance{ VIEWPORT_DEFAULT__FAR_CLIPPING_PLANE_DISTANCE};
    bool use_cube_NDC{false}, flip_z{false}, cull_back_faces{true};

    void updateProjection(f32 focal_length, f32 height_over_width) {
        projection.update(focal_length,
                          height_over_width,
                          near_clipping_plane_distance,
                          far_clipping_plane_distance,
                          use_cube_NDC);
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
        // Project:
        Projection Aproj{projection.project(edge.from)};
        Projection Bproj{projection.project(edge.to)};
        vec3 A{Aproj.projected_position / Aproj.w};
        vec3 B{Bproj.projected_position / Bproj.w};

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
