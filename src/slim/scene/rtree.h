#pragma once

#include "../math/vec3.h"

struct RTreeNode {
    AABB aabb;
    u32 first_index = 0;
    u16 leaf_count = 0;
    u8 depth = 0;
    u8 flags = 0;

    INLINE bool isLeaf() const {
        return leaf_count != 0;
    }
};

struct RTreeQuery {
    struct Result { u32 start, end, node_index; };
    Result *results = nullptr;
    u32 *stack = nullptr;
    u32 result_count = 0;

    RTreeQuery(u8 max_stack_size, u32 max_result_count, memory::MonotonicAllocator *memory_allocator = nullptr) {
        memory::MonotonicAllocator temp_allocator;
        if (!memory_allocator) {
            memory_allocator = &temp_allocator;
            temp_allocator = memory::MonotonicAllocator{
                sizeof(Result) * max_result_count +
                sizeof(u32) * max_stack_size,
                Terabytes(3)
                };
        }
        results  = (Result*)memory_allocator->allocate(sizeof(Result) * max_result_count);
        stack    = (u32*   )memory_allocator->allocate(sizeof(u32) * max_stack_size);
    }

    bool collectOverlappedNodes(const vec3 &point, f32 max_distance, RTreeNode *nodes) {
        result_count = 0;
        RTreeNode &root = nodes[0];
        if (!root.aabb.overlapSphere(point, max_distance))
            return false;

        if (unlikely(root.isLeaf())) {
            result_count = 1;
            results[0] = {root.first_index, root.first_index + root.leaf_count, 0};
            return true;
        }

        u32 node_id;
        i32 stack_size = 0;
        stack[stack_size++] = root.first_index;
        stack[stack_size++] = root.first_index + 1;

        while (stack_size >= 0) {
            node_id = stack[--stack_size];
            RTreeNode &node = nodes[node_id];
            if (!node.aabb.overlapSphere(point, max_distance))
                continue;

            if (node.isLeaf()) {
                results[result_count++] = {node.first_index, node.first_index + node.leaf_count, node_id};
            } else {
                stack[stack_size++] = node.first_index;
                stack[stack_size++] = node.first_index + 1;
            }
        }

        return true;
    }
};

struct RTree {
    RTreeNode *nodes;
    u32 node_count;
    u32 height;
};