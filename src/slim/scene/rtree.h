#pragma once

#include "../math/vec3.h"

struct RTreeNode {
    AABB aabb{0, 0};
    u32 first_child_id{0};
    u16 child_count{0}, depth{0};
};

struct RTree {
    u32 height, node_count, leaf_ids_count, *leaf_ids;
    RTreeNode *nodes;
};