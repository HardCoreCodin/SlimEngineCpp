#pragma once

#include "../math/vec3.h"

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

    INLINE vec3 at(f32 t) const { return origin + t*direction; }
    INLINE vec3 operator [](f32 t) const { return at(t); }

    INLINE BoxSide hitsCube() {
        vec3 octant, RD_rcp = 1.0f / direction;
        f32 x = signbit(direction.x) ? 1.0f : -1.0f;
        f32 y = signbit(direction.y) ? 1.0f : -1.0f;
        f32 z = signbit(direction.z) ? 1.0f : -1.0f;

        f32 t[6];
        t[0] = (+x - origin.x) * RD_rcp.x;
        t[1] = (+y - origin.y) * RD_rcp.y;
        t[2] = (+z - origin.z) * RD_rcp.z;
        t[3] = (-x - origin.x) * RD_rcp.x;
        t[4] = (-y - origin.y) * RD_rcp.y;
        t[5] = (-z - origin.z) * RD_rcp.z;

        u8 max_axis = t[3] < t[4] ? 3 : 4; if (t[5] < t[max_axis]) max_axis = 5;
        f32 max_t = t[max_axis];
        if (max_t < 0) // Further-away hit is behind the ray - intersection can not occur.
            return NoSide;

        u8 min_axis = t[0] > t[1] ? 0 : 1; if (t[2] > t[min_axis]) min_axis = 2;
        f32 min_t = t[min_axis];
        if (max_t < (min_t > 0 ? min_t : 0))
            return NoSide;

        hit.from_behind = min_t < 0; // Further-away hit is in front of the ray, closer one is behind it.
        if (hit.from_behind) {
            min_t = max_t;
            min_axis = max_axis;
        }

        BoxSide side = getBoxSide(x, y, z, min_axis);
        hit.position = direction.scaleAdd(min_t, origin);
        hit.normal = 0.0f;
        switch (side) {
            case Left:   hit.normal.x = hit.from_behind ? +1.0f : -1.0f; break;
            case Right:  hit.normal.x = hit.from_behind ? -1.0f : +1.0f; break;
            case Bottom: hit.normal.y = hit.from_behind ? +1.0f : -1.0f; break;
            case Top:    hit.normal.y = hit.from_behind ? -1.0f : +1.0f; break;
            case Back:   hit.normal.z = hit.from_behind ? +1.0f : -1.0f; break;
            case Front:  hit.normal.z = hit.from_behind ? -1.0f : +1.0f; break;
            default: return NoSide;
        }

        return side;
    }

    INLINE bool hitsPlane(const vec3 &P, const vec3 &N) {
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