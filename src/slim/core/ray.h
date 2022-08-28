#pragma once

#include "../math/vec3.h"

enum RayIsFacing {
    RayIsFacing_Left = 1,
    RayIsFacing_Down = 2,
    RayIsFacing_Back = 4
};

struct RayHit {
    vec3 position, normal;
    f32 distance, distance_squared;
    u32 geo_id;
    enum GeometryType geo_type = GeometryType_None;
    bool from_behind = false;
};

struct Ray {
    vec3 origin, direction;
    RayHit hit;

    INLINE_XPU vec3 at(f32 t) const { return direction.scaleAdd(t, origin);; }
    INLINE_XPU vec3 operator [](f32 t) const { return at(t); }

    INLINE_XPU BoxSide hitsCube() {
        u8 ray_is_facing = 0;

        if (signbit(direction.x)) ray_is_facing |= RayIsFacing_Left;
        if (signbit(direction.y)) ray_is_facing |= RayIsFacing_Down;
        if (signbit(direction.z)) ray_is_facing |= RayIsFacing_Back;

        f32 x = ray_is_facing & RayIsFacing_Left ? 1.0f : -1.0f;
        f32 y = ray_is_facing & RayIsFacing_Down ? 1.0f : -1.0f;
        f32 z = ray_is_facing & RayIsFacing_Back ? 1.0f : -1.0f;

        vec3 RD_rcp = 1.0f / direction;
        vec3 near{(x - origin.x) * RD_rcp.x,
                  (y - origin.y) * RD_rcp.y,
                  (z - origin.z) * RD_rcp.z};
        vec3 far{(-x - origin.x) * RD_rcp.x,
                 (-y - origin.y) * RD_rcp.y,
                 (-z - origin.z) * RD_rcp.z};

        Axis far_hit_axis;
        f32 far_hit_t = far.minimum(&far_hit_axis);
        if (far_hit_t < 0) // Further-away hit is behind the ray - intersection can not occur.
            return BoxSide_None;

        Axis near_hit_axis;
        f32 near_hit_t = near.maximum(&near_hit_axis);
        if (far_hit_t < (near_hit_t > 0 ? near_hit_t : 0))
            return BoxSide_None;

        BoxSide side;
        f32 t = near_hit_t;
        hit.from_behind = t < 0; // Near hit is behind the ray, far hit is in front of it: hit is from behind
        if (hit.from_behind) {
            t = far_hit_t;
            switch (far_hit_axis) {
                case Axis_X : side = ray_is_facing & RayIsFacing_Left ? BoxSide_Left : BoxSide_Right; break;
                case Axis_Y : side = ray_is_facing & RayIsFacing_Down ? BoxSide_Bottom : BoxSide_Top; break;
                case Axis_Z : side = ray_is_facing & RayIsFacing_Back ? BoxSide_Back : BoxSide_Front; break;
            }
        } else {
            switch (near_hit_axis) {
                case Axis_X: side = ray_is_facing & RayIsFacing_Left ? BoxSide_Right : BoxSide_Left; break;
                case Axis_Y: side = ray_is_facing & RayIsFacing_Down ? BoxSide_Top : BoxSide_Bottom; break;
                case Axis_Z: side = ray_is_facing & RayIsFacing_Back ? BoxSide_Front : BoxSide_Back; break;
            }
        }

        hit.position = at(t);
        hit.normal = 0.0f;
        switch (side) {
            case BoxSide_Left  : hit.normal.x = hit.from_behind ?  1.0f : -1.0f; break;
            case BoxSide_Right : hit.normal.x = hit.from_behind ? -1.0f :  1.0f; break;
            case BoxSide_Bottom: hit.normal.y = hit.from_behind ?  1.0f : -1.0f; break;
            case BoxSide_Top   : hit.normal.y = hit.from_behind ? -1.0f :  1.0f; break;
            case BoxSide_Back  : hit.normal.z = hit.from_behind ?  1.0f : -1.0f; break;
            case BoxSide_Front : hit.normal.z = hit.from_behind ? -1.0f :  1.0f; break;
            default: return BoxSide_None;
        }

        return side;
    }

    INLINE_XPU bool hitsPlane(const vec3 &P, const vec3 &N) {
        f32 NdotRd = N.dot(direction);
        if (NdotRd == 0) // The ray is parallel to the plane
            return false;

        f32 NdotRoP = N.dot(P - origin);
        if (NdotRoP == 0) return false; // The ray originated within the plane

        bool ray_is_facing_the_plane = NdotRd < 0;
        hit.from_behind = NdotRoP > 0;
        if (hit.from_behind == ray_is_facing_the_plane) // The ray can't hit the plane
            return false;

        hit.distance = NdotRoP / NdotRd;
        hit.position = at(hit.distance);
        hit.normal = N;

        return true;
    }
};