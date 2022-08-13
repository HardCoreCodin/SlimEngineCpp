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

struct RTree {
    RTreeNode *nodes;
    u32 node_count;
    u32 height;
};