#pragma once

#include "../math/vec3.h"

using Ray = RayOf<vec3>;

INLINE BoxSide getBoxSide(const vec3 &octant, u8 axis) {
    switch (axis) {
        case 0 : return octant.x > 0 ? Right : Left;
        case 3 : return octant.x > 0 ? Left : Right;
        case 1 : return octant.y > 0 ? Top : Bottom;
        case 4 : return octant.y > 0 ? Bottom : Top;
        case 2 : return octant.z > 0 ? Front : Back;
        default: return octant.z > 0 ? Back : Front;
    }
}

INLINE BoxSide rayHitsCube(Ray &ray) {
    vec3 octant, RD_rcp = 1.0f / ray.direction;
    octant.x = signbit(ray.direction.x) ? 1.0f : -1.0f;
    octant.y = signbit(ray.direction.y) ? 1.0f : -1.0f;
    octant.z = signbit(ray.direction.z) ? 1.0f : -1.0f;

    f32 t[6];
    t[0] = (+octant.x - ray.origin.x) * RD_rcp.x;
    t[1] = (+octant.y - ray.origin.y) * RD_rcp.y;
    t[2] = (+octant.z - ray.origin.z) * RD_rcp.z;
    t[3] = (-octant.x - ray.origin.x) * RD_rcp.x;
    t[4] = (-octant.y - ray.origin.y) * RD_rcp.y;
    t[5] = (-octant.z - ray.origin.z) * RD_rcp.z;

    u8 max_axis = t[3] < t[4] ? 3 : 4; if (t[5] < t[max_axis]) max_axis = 5;
    f32 max_t = t[max_axis];
    if (max_t < 0) // Further-away hit is behind the ray - intersection can not occur.
        return NoSide;

    u8 min_axis = t[0] > t[1] ? 0 : 1; if (t[2] > t[min_axis]) min_axis = 2;
    f32 min_t = t[min_axis];
    if (max_t < (min_t > 0 ? min_t : 0))
        return NoSide;

    ray.hit.from_behind = min_t < 0; // Further-away hit is in front of the ray, closer one is behind it.
    if (ray.hit.from_behind) {
        min_t = max_t;
        min_axis = max_axis;
    }

    BoxSide side = getBoxSide(octant, min_axis);
    ray.hit.position = ray.direction.scaleAdd(min_t, ray.origin);
    ray.hit.normal = 0.0f;
    switch (side) {
        case Left:   ray.hit.normal.x = ray.hit.from_behind ? +1.0f : -1.0f; break;
        case Right:  ray.hit.normal.x = ray.hit.from_behind ? -1.0f : +1.0f; break;
        case Bottom: ray.hit.normal.y = ray.hit.from_behind ? +1.0f : -1.0f; break;
        case Top:    ray.hit.normal.y = ray.hit.from_behind ? -1.0f : +1.0f; break;
        case Back:   ray.hit.normal.z = ray.hit.from_behind ? +1.0f : -1.0f; break;
        case Front:  ray.hit.normal.z = ray.hit.from_behind ? -1.0f : +1.0f; break;
        default: return NoSide;
    }

    return side;
}

INLINE bool rayHitsPlane(Ray &ray, const vec3 &P, const vec3 &N) {
    f32 NdotRd = N | ray.direction;
    if (NdotRd == 0) // The ray is parallel to the plane
        return false;

    f32 NdotRoP = N | (P - ray.origin);
    if (NdotRoP == 0) return false; // The ray originated within the plane

    bool ray_is_facing_the_plane = NdotRd < 0;
    ray.hit.from_behind = NdotRoP > 0;
    if (ray.hit.from_behind == ray_is_facing_the_plane) // The ray can't hit the plane
        return false;

    ray.hit.distance = NdotRoP / NdotRd;
    ray.hit.position = ray.origin + ray.direction*ray.hit.distance;
    ray.hit.normal = N;

    return true;
}
