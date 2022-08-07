#pragma once

#include "../math/vec3.h"

struct RTreeNode {
    AABB aabb;
    u32 first_child_id{0};
    u16 child_count{0}, depth{0};

    INLINE bool isLeaf() const {
        return child_count != 0;
    }
};

struct RTreeQuery {
    u32 node_count = 0;
    u32 *node_ids = nullptr;
    u32 *stack = nullptr;

    bool collectOverlappedNodes(const vec3 &point, f32 max_distance, RTreeNode *nodes) {
        node_count = 0;
        RTreeNode &root = nodes[0];
        if (!root.aabb.overlapSphere(point, max_distance))
            return false;

        if (unlikely(root.isLeaf())) {
            node_count = 1;
            node_ids[0] = 0;
            return true;
        }

        u32 node_id;
        i32 stack_size = 0;
        stack[stack_size++] = root.first_child_id;
        stack[stack_size++] = root.first_child_id + 1;

        while (stack_size >= 0) {
            node_id = stack[--stack_size];
            RTreeNode &node = nodes[node_id];
            if (!node.aabb.overlapSphere(point, max_distance))
                continue;

            if (node.isLeaf()) {
                node_ids[node_count++] = node_id;
            } else {
                stack[stack_size++] = node.first_child_id;
                stack[stack_size++] = node.first_child_id + 1;
            }
        }

        return true;
    }
};

struct RTree {
    u32 height, node_count, leaf_ids_count, *leaf_ids;
    RTreeNode *nodes;
};