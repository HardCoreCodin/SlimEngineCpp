#pragma once

#include "../slim/scene/mesh.h"
#include "../slim/viewport/viewport.h"

constexpr float ONE_OVER_SQRT2 = 0.70710678118f;

struct ClosestPointOnTriangle {
    vec3 search_origin, closest_point;
    vec2 uv;
    f32 squared_distance = 0;
    u32 triangle_index = (u32)-1;
    TrianglePointOn on{TrianglePointOn_None};

    ClosestPointOnTriangle() = default;

    ClosestPointOnTriangle(const vec3 &search_origin, f32 max_distance) :
            search_origin{search_origin},
            squared_distance{max_distance * max_distance},
            triangle_index{(u32)-1} {}

    INLINE TrianglePointOn find(const Triangle &triangle, u32 index = (u32)-1) {
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

    INLINE void find(const Triangle *triangles, u32 start, u32 end) {
        for (u32 i = start; i < end; i++) find(triangles[i], i);
    }
};

#define BROAD_PHASE_INCLUDED 1

struct ClosestPointOnMesh {
    // The query does a broad-phase to narrows the search to only triangles that are close enough to matter.
    // There are 3 operation modes catering to different use-cases:
    // 1. Deferred: IDs of Leaf-node and triangles first get collected into an array and only then triangles get tested.
    // 2. Immediate: Leaf-node triangles get tested when visited and tracked on the node flags (no array is needed).
    // 3. Adaptive: Same as Immediate but also the search radius is reduced dynamically as closer triangles are found.

    enum Mode { Deferred, Immediate, Adaptive };
    struct BroadPhaseResult { u32 start, end, node_index; };

    Mesh *mesh = nullptr;
    u32 *stack = nullptr;
    BroadPhaseResult *broad_phase_results = nullptr;
    u32 max_stack_size = 0;
    u32 max_broad_phase_result_count = 0;
    u32 broad_phase_result_count = 0;
    Mode mode = Adaptive;

    TrianglePointOn find(const vec3 &search_origin, f32 max_distance, ClosestPointOnTriangle &closest_point_on_triangle) {
        closest_point_on_triangle = ClosestPointOnTriangle{search_origin, max_distance};

#ifndef NDEBUG
        if (max_stack_size < getMaxStackSize(*mesh)) {
            closest_point_on_triangle.on = TrianglePointOn_Error;
            return TrianglePointOn_Error;
        }
        if (mode == Deferred) {
            broad_phase_result_count = 0;
            if (!broad_phase_results || max_broad_phase_result_count < getMaxBroadPhaseResultCount(*mesh)) {
                closest_point_on_triangle.on = TrianglePointOn_Error;
                return TrianglePointOn_Error;
            }
        }
#endif

        RTreeNode *nodes = mesh->rtree.nodes;
        RTreeNode &root = nodes[0];

        if (!root.aabb.overlapSphere(search_origin, max_distance))
            return TrianglePointOn_None;

        u32 start, end, node_id;
        if (unlikely(root.isLeaf())) {
            start = root.first_index;
            end = start + root.leaf_count;
            if (mode == Deferred) {
                broad_phase_results[0] = {start, end, 0};
                broad_phase_result_count = 1;
            }
            closest_point_on_triangle.find(mesh->triangles, start, end);
            return closest_point_on_triangle.on;
        }

        f32 radius = max_distance;
        i32 stack_size = 0;
        stack[stack_size++] = root.first_index;
        stack[stack_size++] = root.first_index + 1;

        while (stack_size > 0) {
            node_id = stack[--stack_size];
            RTreeNode &node = nodes[node_id];
            if (!node.aabb.overlapSphere(search_origin, radius))
                continue;

            if (node.isLeaf()) {
                start = node.first_index;
                end = start + node.leaf_count;

                switch (mode) {
                    case Deferred:
                        broad_phase_results[broad_phase_result_count++] = {start, end, node_id};
                        break;
                    case Immediate:
                        node.flags = BROAD_PHASE_INCLUDED;
                        closest_point_on_triangle.find(mesh->triangles, start, end);
                        break;
                    default:
                        node.flags = BROAD_PHASE_INCLUDED;

                        // When testing triangles eagerly the search radius may be safely narrowed down on the fly:
                        f32 squared_distance_before = closest_point_on_triangle.squared_distance;
                        closest_point_on_triangle.find(mesh->triangles, start, end);
                        if (closest_point_on_triangle.squared_distance < squared_distance_before)
                            radius = sqrtf(closest_point_on_triangle.squared_distance);
                }
            } else {
                stack[stack_size++] = node.first_index;
                stack[stack_size++] = node.first_index + 1;
            }
        }

        if (broad_phase_results)
            for (u32 result_index = 0; result_index < broad_phase_result_count; result_index++) {
                start = broad_phase_results[result_index].start;
                end = broad_phase_results[result_index].end;
                closest_point_on_triangle.find(mesh->triangles, start, end);
            }

        return closest_point_on_triangle.on;
    }

    void allocate(bool broad_phase = true, memory::MonotonicAllocator *allocator = nullptr) {
        if (broad_phase && broad_phase_results)
            broad_phase = false;

        if (stack && !broad_phase)
            return;

        u32 capacity = 0;
        if (!stack) {
            max_stack_size = getMaxStackSize(*mesh);
            capacity += max_stack_size * sizeof(u32);
        }
        if (broad_phase) {
            max_broad_phase_result_count = getMaxBroadPhaseResultCount(*mesh);
            capacity += max_broad_phase_result_count * sizeof(BroadPhaseResult);
        }

        memory::MonotonicAllocator tmp;
        if (!allocator || allocator->capacity < capacity) {
            allocator = &tmp;
            tmp = memory::MonotonicAllocator{capacity};
        }
        if (!stack) stack = (u32*)allocator->allocate(max_stack_size * sizeof(u32));
        if (broad_phase) broad_phase_results = (BroadPhaseResult*)allocator->allocate(max_broad_phase_result_count * sizeof(BroadPhaseResult));
    }

    static u32 getMaxBroadPhaseResultCount(const Mesh &mesh) { return mesh.triangle_count; }
    static u32 getMaxStackSize(const Mesh &mesh) { return mesh.rtree.height; }
};