#pragma once

#include "./ClosestPointOnTriangle.hpp"

#define BROAD_PHASE_INCLUDED 1

struct ClosestPointOnMesh {
    // The query does a broad-phase to narrows the search to only triangles that are close enough to matter.
    // Leaf-node triangles get tested immediately when visited and tracked on the node flags (no aux. array needed).
    // In adaptive mode the search radius is also reduced dynamically as closer triangles are found.

    Mesh *mesh = nullptr;
    Transform *mesh_transform = nullptr;
    Transform *search_origin_transform = nullptr;
    u32 *stack = nullptr;
    u32 max_stack_size = 0;
    u32 max_result_count = 0;
    ClosestPointOnTriangle *results = nullptr;

    INLINE_XPU TrianglePointOn find(vec3 search_origin, f32 max_distance, ClosestPointOnTriangle &closest_point_on_triangle, bool adaptive = true) const {
#ifndef NDEBUG
        if (max_stack_size < mesh->rtree.height) {
        closest_point_on_triangle.on = TrianglePointOn_Error;
        return TrianglePointOn_Error;
    }
#endif
        if (search_origin_transform) search_origin = search_origin_transform->externPos(search_origin);
        closest_point_on_triangle = ClosestPointOnTriangle{search_origin, max_distance};
        if (mesh_transform)
            closest_point_on_triangle.search_origin = mesh_transform->internPos(closest_point_on_triangle.search_origin);

        RTreeNode *nodes = mesh->rtree.nodes;
        RTreeNode &root = nodes[0];

        if (!root.aabb.overlapSphere(closest_point_on_triangle.search_origin, max_distance))
            return TrianglePointOn_None;

        u32 start, end, node_id;
        if (unlikely(root.isLeaf())) {
            start = root.first_index;
            end = start + root.leaf_count;
            closest_point_on_triangle.find(mesh->triangles, start, end);
            if (closest_point_on_triangle.on) {
                closest_point_on_triangle.search_origin = search_origin;
                if (mesh_transform)
                    closest_point_on_triangle.closest_point = mesh_transform->externPos(closest_point_on_triangle.closest_point);
            }

            return closest_point_on_triangle.on;
        }

        f32 squared_distance_before;
        f32 radius = max_distance;
        i32 stack_size = 0;
        stack[stack_size++] = root.first_index;
        stack[stack_size++] = root.first_index + 1;

        while (stack_size > 0) {
            node_id = stack[--stack_size];
            RTreeNode &node = nodes[node_id];
            if (!node.aabb.overlapSphere(closest_point_on_triangle.search_origin, radius))
                continue;

            if (node.isLeaf()) {
                node.flags = BROAD_PHASE_INCLUDED;
                start = node.first_index;
                end = start + node.leaf_count;
                squared_distance_before = closest_point_on_triangle.squared_distance;
                closest_point_on_triangle.find(mesh->triangles, start, end);

                // Since triangles are tested eagerly the search radius may be safely narrowed down on the fly:
                if (adaptive && closest_point_on_triangle.squared_distance < squared_distance_before)
                    radius = sqrtf(closest_point_on_triangle.squared_distance);
            } else {
                stack[stack_size++] = node.first_index;
                stack[stack_size++] = node.first_index + 1;
            }
        }

        if (closest_point_on_triangle.on) {
            closest_point_on_triangle.search_origin = search_origin;
            if (mesh_transform)
                closest_point_on_triangle.closest_point = mesh_transform->externPos(closest_point_on_triangle.closest_point);
        }

        return closest_point_on_triangle.on;
    }

    void find(const vec3 *search_origins, u32 search_origins_count, f32 max_distance, bool adaptive = true) const {
        for (u32 i = 0; i < search_origins_count; i++)
            find(search_origins[i], max_distance, results[i], adaptive);
    }

    void allocate(u32 result_count = 0, memory::MonotonicAllocator *allocator = nullptr) {
        if (result_count && results)
            result_count = 0;

        if (stack && !result_count)
            return;

        u32 capacity = 0;
        if (!stack) {
            max_stack_size = mesh->rtree.height;
            capacity += max_stack_size * sizeof(u32);
        }
        if (result_count) {
            max_result_count = result_count;
            capacity += max_result_count * sizeof(ClosestPointOnTriangle);
        }

        memory::MonotonicAllocator tmp;
        if (!allocator || allocator->capacity < capacity) {
            allocator = &tmp;
            tmp = memory::MonotonicAllocator{capacity};
        }
        if (!stack) stack = (u32*)allocator->allocate(max_stack_size * sizeof(u32));
        if (result_count) results = (ClosestPointOnTriangle*)allocator->allocate(result_count * sizeof(ClosestPointOnTriangle));
    }
};