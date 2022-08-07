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

struct RTreeQueryResult {
    u32 *stack, *node_ids, node_count;
};

struct RTree {
    u32 height, node_count, leaf_ids_count, *leaf_ids;
    RTreeNode *nodes;
    RTreeQueryResult query_result;

    bool collectOverlappedNodes(const vec3 &point, f32 max_distance) {
        query_result.node_count = 0;
        RTreeNode &root = nodes[0];
        if (!root.aabb.overlapSphere(point, max_distance))
            return false;

        if (unlikely(root.isLeaf())) {
            query_result.node_count = 1;
            query_result.node_ids[0] = 0;
            return true;
        }

        u32 node_id;
        i32 stack_size = 0;
        u32 *stack = query_result.stack;
        stack[stack_size++] = root.first_child_id;
        stack[stack_size++] = root.first_child_id + 1;

        while (stack_size >= 0) {
            node_id = stack[--stack_size];
            RTreeNode &node = nodes[node_id];
            if (!node.aabb.overlapSphere(point, max_distance))
                continue;

            if (node.isLeaf()) {
                query_result.node_ids[query_result.node_count++] = node_id;
            } else {
                stack[stack_size++] = node.first_child_id;
                stack[stack_size++] = node.first_child_id + 1;
            }
        }

        return true;


//        RTreeNode *left_node = nodes + root.first_child_id;
//        RTreeNode *right_node, *tmp_node;
//        u32 stack_size = 0;
//
//        while (true) {
//            right_node = left_node + 1;
//
//            hit_left  = hitAABB(&left_node->aabb,  ray, closest_hit->distance, &left_distance);
//            hit_right = hitAABB(&right_node->aabb, ray, closest_hit->distance, &right_distance);
//
//            if (hit_left) {
//                if (unlikely(left_node->child_count)) {
//                    if (hitTriangles(ray, hit, closest_hit, mesh->triangles + left_node->first_child_id, left_node->child_count, any_hit)) {
//                        closest_hit->object_id += left_node->first_child_id;
//                        found = true;
//                        if (any_hit)
//                            break;
//                    }
//
//                    left_node = nullptr;
//                }
//            } else
//                left_node = nullptr;
//
//            if (hit_right) {
//                if (unlikely(right_node->child_count)) {
//                    if (hitTriangles(ray, hit, closest_hit, mesh->triangles + right_node->first_child_id, right_node->child_count, any_hit)) {
//                        closest_hit->object_id += right_node->first_child_id;
//                        found = true;
//                        if (any_hit)
//                            break;
//                    }
//
//                    right_node = nullptr;
//                }
//            } else
//                right_node = nullptr;
//
//            if (left_node) {
//                if (right_node) {
//                    if (left_distance > right_distance) {
//                        tmp_node = left_node;
//                        left_node = right_node;
//                        right_node = tmp_node;
//                    }
//                    query.stack[stack_size++] = right_node->first_child_id;
//                }
//                left_node = nodes + left_node->first_child_id;
//            } else if (right_node) {
//                left_node = nodes + right_node->first_child_id;
//            } else {
//                if (stack_size == 0) break;
//                left_node = nodes + query.stack[--stack_size];
//            }
//        }
//
//        return found;
    }
};