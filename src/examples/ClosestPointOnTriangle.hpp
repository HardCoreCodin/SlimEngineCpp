#pragma once

#include "../slim.h"

constexpr float ONE_OVER_SQRT2 = 0.70710678118f;

enum TrianglePointOn {
    TrianglePointOn_None = 0,

    TrianglePointOn_Face,
    TrianglePointOn_Edge,
    TrianglePointOn_Vertex,

    TrianglePointOn_Error
};

struct ClosestPointOnTriangle {
    vec3 search_origin, closest_point;
    vec2 uv;
    f32 squared_distance = 0;
    u32 triangle_index = (u32)-1;
    TrianglePointOn on{TrianglePointOn_None};

    ClosestPointOnTriangle() = default;

    INLINE_XPU ClosestPointOnTriangle(const vec3 &search_origin, f32 max_distance) :
            search_origin{search_origin},
            squared_distance{max_distance * max_distance},
            triangle_index{(u32)-1} {}

    INLINE_XPU TrianglePointOn find(const Triangle &triangle, u32 index = (u32)-1) {
        vec3 tangent_space_point = triangle.local_to_tangent * (search_origin - triangle.position);
        f32 u, v;
        f32 x = tangent_space_point.x;
        f32 y = tangent_space_point.y;

        TrianglePointOn found_on;

        if (x < 0) { // Point is outside on the left, find the closest point on the left edge:
            // If there point is below the triangle, choose the bottom-left corner
            // If there point is above the triangle, choose the top-left corner
            // Otherwise it is within the height bounds of the triangle - find where on V
            v = clampedValue(y);
            u = 0;
            found_on = y < 0 || 1 < y ? TrianglePointOn_Vertex : TrianglePointOn_Edge;
        } else if (y < 0) { // Point is below the triangle, find the closest point on the bottom edge:
            // If the point is below the triangle on the right - choose the bottom-right corner
            // Otherwise it is within the width bounds of the triangle - find where on U
            u = clampedValue(x);
            v = 0;
            found_on = 1 < x ? TrianglePointOn_Vertex : TrianglePointOn_Edge;
        } else if ((x + y) <= 1.0f) { // Point it over the triangle (within it's projection):
            u = x;
            v = y;
            found_on = TrianglePointOn_Face;
        } else if ((y - 1) >= x) { // Point is above the triangle's diagonal-edge projection - choose top corner:
            u = 0;
            v = 1;
            found_on = TrianglePointOn_Vertex;
        } else if ((x - 1) >= y) { // Point is right of the triangle's diagonal-edge projection, choose right corner:
            u = 1;
            v = 0;
            found_on = TrianglePointOn_Vertex;
        } else { // Point is above the triangle and to the right (above it's diagonal-edge):
            // The closest-point to the triangle is the query-point projected orthogonally onto the diagonal edge:
            // Let Q be the query point              : Q = [x, y]
            // Let Q` be the closest point           : Q` = [u, v]
            // Let T be the top-left vertex          : T = [0, 1]
            // Let R be the bottom-right vertex      : R = [1, 0]
            // Let D be a vector from T to R         : D = R - T  = [1, 0] - [0, 1] = [1, -1]
            // Let P be a vector from T to Q         : P = Q - T  = [x, y] - [0, 1] = [x, y - 1]
            // Let D^ be the norm of D               : D^ = norm(D) = [c, -c]  where: c = 1 / sqrt(2)
            // Let P` be the projection of P onto D^ : P` = proj(P, D) = D * t where: t = dot(P, D^)
            // Q` = T + P` = T + D * t = [0, 1] + [1, -1] * t = [u, v]
            // u = 0 + 1 * t = t
            // v = 1 + -1 * t = 1 - t = 1 - u
            // u = t = dot([x, y - 1], [c, -c]]) = x * c + (y - 1) * (-c) = x * c + (1 - y) * c = c * (x - y + 1)
            // u = 1 / sqrt(2) * (x - y + 1)
            u = ONE_OVER_SQRT2 * (x - y + 1);
            v = 1 - u;
            found_on = TrianglePointOn_Edge;
        }

        vec3 current_point{triangle.position + (triangle.U * u) + (triangle.V * v)};
        f32 current_distance_squared = (current_point - search_origin).squaredLength();
        if (current_distance_squared < squared_distance) {
            squared_distance = current_distance_squared;
            on = found_on;
            closest_point = current_point;
            uv.u = u;
            uv.v = v;
            triangle_index = index;
        } else
            found_on = TrianglePointOn_None;

        return found_on;
    }

    INLINE_XPU void find(const Triangle *triangles, u32 start, u32 end) {
        for (u32 i = start; i < end; i++) find(triangles[i], i);
    }
};