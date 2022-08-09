#pragma once

#include "../slim/scene/mesh.h"
#include "../slim/viewport/viewport.h"

INLINE TrianglePointOn findClosestPointOnTriangle(const Triangle &triangle,
                                                  const vec3 &point,
                                                  f32 &closest_distance_so_far_squared,
                                                  vec3 &closest_point_so_far,
                                                  vec2 &closest_uv) {
    vec3 closest_point;
    vec3 tanget_space_point = triangle.local_to_tangent * (point - triangle.position);
    f32 u = tanget_space_point.x;
    f32 v = tanget_space_point.y;

    TrianglePointOn found_on;

    if (u < 0) { // Point is outside on the left, find the closest point on the left edge:
        // If there point is below the triangle, choose the bottom-left corner
        // If there point is above the triangle, choose the top-left corner
        // Otherwise it is within the height bounds of the triangle - find where on V
        found_on = v < 0 || 1 < v ? TrianglePointOn_Vertex : TrianglePointOn_Edge;
        v = clampedValue(v);
        u = 0;
    } else if (v < 0) { // Point is below the triangle, find the closest point on the bottom edge:
        // If the point is below the triangle on the right - choose the bottom-right corner
        // Otherwise it is within the width bounds of the triangle - find where on U
        found_on = 1 < u ? TrianglePointOn_Vertex : TrianglePointOn_Edge;
        u = clampedValue(u);
        v = 0;
    } else if ((u + v) <= 1) { // Point it over the triangle (within it's projection):
        found_on = TrianglePointOn_Face;
    } else if ((v - 1) >= u) { // Point is above the triangle's diagonal-edge projection - choose top corner:
        found_on = TrianglePointOn_Vertex;
        u = 0;
        v = 1;
    } else if ((u - 1) >= v) { // Point is right of the triangle's diagonal-edge projection, choose right corner:
        found_on = TrianglePointOn_Vertex;
        u = 1;
        v = 0;
    } else { // Point is above the triangle and to the right, above it's diagonal-edge projection:
        found_on = TrianglePointOn_Edge;
        // Project the 2D vector from the top corner to the point onto a down-right diagonal normalized vector;
        // t = ([u, v] - [0, 1]) dot [0.707, -0.707]
        // t = [u - 0, v - 1] dot [0.707, -0.707]
        // t = u * 0.707 + (v - 1) * -0.707
        // t = u * 0.707 + -(v - 1) * 0.707
        // t = u * 0.707 + (1 - v) * 0.707
        // t = 0.707 * (u + 1 - v)
        // Then, starting from the top-left corner [0, 1] go towards the bottom right corner [1, 0] by t:
        // [u`, v`] = [0, 1] + t[1, -1];
        // u` = 0 + t * +1
        // v` = 1 + t * -1
        // u` = t
        // v` = 1 - t
        u = 0.70710678118f * (u + 1 - v);
        v = 1 - u;
    }

    closest_point = triangle.position + (triangle.U * u) + (triangle.V * v);
    f32 distance_squared = (closest_point - point).squaredLength();
    if (distance_squared < closest_distance_so_far_squared) {
        closest_distance_so_far_squared = distance_squared;
        closest_point_so_far = closest_point;
        closest_uv.u = u;
        closest_uv.v = v;
    } else
        found_on = TrianglePointOn_None;

    return found_on;
}


struct MeshQuery : RTreeQuery {
    vec2 closest_uv;
    u32 closest_triangle_index;
    Mesh *mesh = nullptr;

    MeshQuery(Mesh *mesh) : RTreeQuery{(u8)mesh->rtree.height, mesh->triangle_count}, mesh{mesh} {}

    TrianglePointOn findClosestPointOnMesh(const vec3 &point, f32 max_distance, vec3 &closest_point) {
        RTree &rtree = mesh->rtree;
        RTreeNode *nodes = rtree.nodes;
        TrianglePointOn point_on = TrianglePointOn_None;

        // Broad phase using an RTree query:
        // =================================
        // Narrow down the search to just the triangles within the given max_distance radius from the point.
        // The RTree query returns an array of leaf-nodes that overlap the search range, containing triangle ids.
        if (collectOverlappedNodes(point, max_distance, nodes)) {
            f32 closest_distance_squared = max_distance * max_distance;
            vec2 uv;
            TrianglePointOn on;
            for (u32 result_index = 0; result_index < result_count; result_index++) {
                RTreeQuery::Result &result = results[result_index];
                for (u32 triangle_index = result.start; triangle_index < result.end; triangle_index++) {
                    on = findClosestPointOnTriangle(mesh->triangles[triangle_index], point,
                                                    closest_distance_squared,
                                                    closest_point,
                                                    uv);
                    if (on) {
                        point_on = on;
                        closest_triangle_index = triangle_index;
                        closest_uv = uv;
                    }
                }
            }
        }

        return point_on;
    }
};