#pragma once

#include "../math/vec3.h"

struct Frustum {
    enum class ProjectionType {
        Orthographic = 0,
        PerspectiveGL,
        PerspectiveDX
    };

    struct Projection {
        vec3 scale;
        f32 shear;
        ProjectionType type;

        Projection(f32 focal_length, f32 height_over_width, f32 n, f32 f,
                   ProjectionType projection_type = ProjectionType::PerspectiveGL) :
                scale{0}, shear{0}, type{projection_type} {
            update(focal_length, height_over_width, n, f);
        }
        Projection(const Projection &other) : scale{other.scale}, shear{other.shear} {}


        void update(f32 focal_length, f32 height_over_width, f32 n, f32 f) {
            scale.x = focal_length * height_over_width;
            scale.y = focal_length;
            if (type != ProjectionType::Orthographic) {
                scale.z = shear = 1.0f / (f - n);
                if (type == ProjectionType::PerspectiveGL) {
                    scale.z *= f + n;
                    shear *= f * n * -2;
                } else {
                    scale.z *= f;
                    shear *= f * -n;
                }
            }
        }

        vec3 project(const vec3 &position) const {
            vec3 projected_position{
                    position.x * scale.x,
                    position.y * scale.y,
                    position.z * scale.z + shear
            };
            return projected_position / position.z;
        }
    };
    Projection projection{
            CAMERA_DEFAULT__FOCAL_LENGTH,
            (f32)DEFAULT_HEIGHT / (f32)DEFAULT_WIDTH,
            VIEWPORT_DEFAULT__NEAR_CLIPPING_PLANE_DISTANCE,
            VIEWPORT_DEFAULT__FAR_CLIPPING_PLANE_DISTANCE
    };

    f32 near_clipping_plane_distance{VIEWPORT_DEFAULT__NEAR_CLIPPING_PLANE_DISTANCE};
    f32 far_clipping_plane_distance{ VIEWPORT_DEFAULT__FAR_CLIPPING_PLANE_DISTANCE};
    bool flip_z{false}, cull_back_faces{true};

    void updateProjection(f32 focal_length, f32 height_over_width) {
        projection.update(focal_length,
                          height_over_width,
                          near_clipping_plane_distance,
                          far_clipping_plane_distance);
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
        f32 NdotA = N.dot(A);
        f32 NdotB = N.dot(B);

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        // Right plane (facing to the left):
        N.x = -N.x;
        NdotA = N.dot(A);
        NdotB = N.dot(B);

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        // Bottom plane (facing up):
        N = {0, focal_length, 1};
        NdotA = N.dot(A);
        NdotB = N.dot(B);

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        // Top plane (facing down):
        N.y = -N.y;
        NdotA = N.dot(A);
        NdotB = N.dot(B);

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

    INLINE void projectPoint(vec3 &point, const Dimensions &dimensions) const {
        point.x = ((projection.scale.x * point.x / point.z) + 1) * dimensions.h_width;
        point.y = ((projection.scale.y * point.y / point.z) + 1) * dimensions.h_height;
        point.y = dimensions.f_height - point.y;
    }

    void projectEdge(Edge &edge, const Dimensions &dimensions) const {
        projectPoint(edge.from, dimensions);
        projectPoint(edge.to, dimensions);
    }
};
