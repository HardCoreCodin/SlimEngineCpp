#pragma once

#include "../math/vec3.h"

struct BVHNode {
    AABB aabb;
    u32 first_index = 0;
    u16 leaf_count = 0;
    u8 depth = 0;
    u8 flags = 0;

    INLINE_XPU bool isLeaf() const {
        return leaf_count != 0;
    }
};

struct BVH {
    BVHNode *nodes;
    u32 node_count;
    u8 height;
};