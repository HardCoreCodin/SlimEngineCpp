#pragma once

#include "./rectangle.h"

INLINE bool rightwardRayHitsBound(RayOf<vec2> &ray, const float x_bound) {
    f32 distance = x_bound - ray.origin.x;
    if (distance < 0 || // Rect is behind the ray
        distance > ray.direction.x) // Rect is too far in front of the ray
        return false;

    ray.hit.distance = distance;
    ray.hit.position = ray[distance / ray.direction.x];
    ray.hit.normal = {-1.0f, 0.0f};
    ray.hit.from_behind = false;

    return true;
}

INLINE bool leftwardRayHitsBound(RayOf<vec2> &ray, const float x_bound) {
    f32 distance = ray.origin.x - x_bound;
    if (distance < 0 || // Rect is behind the ray
        distance > -ray.direction.x) // Rect is too far in front of the ray
        return false;

    ray.hit.position = ray[distance / -ray.direction.x];
    ray.hit.normal = {1.0f, 0.0f};
    ray.hit.distance = distance;
    ray.hit.from_behind = false;

    return true;
}

INLINE bool horizontalRayHitsRect(RayOf<vec2> &ray, const Rect &rect) {
    if (ray.direction.x == 0) return false;
    return signbit(ray.direction.x) ?
           leftwardRayHitsBound(ray, rect.left) :
           rightwardRayHitsBound(ray, rect.right);
}

INLINE bool upwardRayHitsBound(RayOf<vec2> &ray, const float y_bound) {
    f32 distance = y_bound - ray.origin.y;
    if (distance < 0 || // Rect is behind the ray
        distance > ray.direction.y) // Rect is too far in front of the ray
        return false;

    ray.hit.distance = distance;
    ray.hit.position = ray[distance / ray.direction.y];
    ray.hit.normal = {0.0f, -1.0f};
    ray.hit.from_behind = false;

    return true;
}

INLINE bool downwardRayHitsBound(RayOf<vec2> &ray, const float y_bound) {
    f32 distance = ray.origin.y - y_bound;
    if (distance < 0 || // Rect is behind the ray
        distance > -ray.direction.y) // Rect is too far in front of the ray
        return false;

    ray.hit.position = ray[distance / -ray.direction.y];
    ray.hit.normal = {0.0f, 1.0f};
    ray.hit.distance = distance;
    ray.hit.from_behind = false;

    return true;
}

INLINE bool verticalRayHitsRect(RayOf<vec2> &ray, const Rect &rect) {
    if (ray.direction.y == 0) return false;
    return signbit(ray.direction.y) ?
           downwardRayHitsBound(ray, rect.bottom) :
           upwardRayHitsBound(ray, rect.top);
}

INLINE bool rayHitRect(RayOf<vec2> &ray, const Rect &rect) {
    if (ray.direction.x == 0) return verticalRayHitsRect(ray, rect);
    if (ray.direction.y == 0) return horizontalRayHitsRect(ray, rect);
    bool aiming_left{signbit(ray.direction.x)};
    bool aiming_down{signbit(ray.direction.y)};
    vec2 RD_rcp = 1.0f / ray.direction;
    vec2 t_near{aiming_left ? rect.right : rect.left, aiming_down ? rect.top : rect.bottom};
    vec2 t_far{ aiming_left ? rect.left : rect.right, aiming_down ? rect.bottom : rect.top};
    t_near -= ray.origin;
    t_near *= RD_rcp;
    t_far -= ray.origin;
    t_far *= RD_rcp;
    if (t_near.x > t_far.y || t_near.y > t_far.x) return false; // Early rejection

    f32 near_t = t_near.x > t_near.y ? t_near.x : t_near.y;
    f32 far_t  = t_far.x  < t_far.y  ? t_far.x  : t_far.y; if (far_t < 0) return false; // Reject if ray direction is pointing away from object

    // Contact point of collision from parametric line equation
    ray.hit.distance = near_t;
    ray.hit.position = ray[near_t];
    ray.hit.normal = 0.0f;
    if (     t_near.x > t_near.y) ray.hit.normal.x = aiming_left ? 1.0f : -1.0f;
    else if (t_near.x < t_near.y) ray.hit.normal.y = aiming_down ? 1.0f : -1.0f;

    return true;
}


//INLINE BoxSide rayHitsRect(RayOf<vec2> &ray, Rect &rect) {
//    vec2 octant, RD_rcp = 1.0f / ray.direction;
//    octant.x = signbit(ray.direction.x) ? 1.0f : -1.0f;
//    octant.y = signbit(ray.direction.y) ? 1.0f : -1.0f;
//
//    f32 t[4];
//    t[0] = (+octant.x - ray.origin.x) * RD_rcp.x;
//    t[1] = (+octant.y - ray.origin.y) * RD_rcp.y;
//    t[2] = (-octant.x - ray.origin.x) * RD_rcp.x;
//    t[3] = (-octant.y - ray.origin.y) * RD_rcp.y;
//
//    u8 max_axis = t[2] < t[3] ? 2 : 3;
//    f32 max_t = t[max_axis];
//    if (max_t < 0) // Further-away hit is behind the ray - intersection can not occur.
//        return NoSide;
//
//    u8 min_axis = t[0] > t[1] ? 0 : 1;
//    f32 min_t = t[min_axis];
//    if (max_t < (min_t > 0 ? min_t : 0))
//        return NoSide;
//
//    ray.hit.from_behind = min_t < 0; // Further-away hit is in front of the ray, closer one is behind it.
//    if (ray.hit.from_behind) {
//        min_t = max_t;
//        min_axis = max_axis;
//    }
//
//    BoxSide side = getRectSide(octant, min_axis);
//    ray.hit.position_x = ray.direction.scaleAdd(min_t, ray.origin);
//    ray.hit.normal = 0.0f;
//    switch (side) {
//        case Left:   ray.hit.normal.x = ray.hit.from_behind ? +1.0f : -1.0f; break;
//        case Right:  ray.hit.normal.x = ray.hit.from_behind ? -1.0f : +1.0f; break;
//        case Bottom: ray.hit.normal.y = ray.hit.from_behind ? +1.0f : -1.0f; break;
//        case Top:    ray.hit.normal.y = ray.hit.from_behind ? -1.0f : +1.0f; break;
//        default: return NoSide;
//    }
//
//    return side;
//}
