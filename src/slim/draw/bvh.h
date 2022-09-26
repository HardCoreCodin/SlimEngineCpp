#pragma once

#include "./edge.h"
#include "../scene/mesh.h"
#include "../core/transform.h"
#include "./box.h"

void drawBVH(const BVH &bvh, const Transform &transform, const Viewport &viewport,
               u16 min_depth = 0, u16 max_depth = 5, f32 opacity = 0.25f, u8 line_width = 1) {
    static Box box;
    static Transform box_transform;

    for (u32 node_id = 0; node_id < bvh.node_count; node_id++) {
        BVHNode &node = bvh.nodes[node_id];
        if (node.depth < min_depth || node.depth > max_depth)
            continue;

        box_transform = transform;
        box_transform.scale *= (node.aabb.max - node.aabb.min) * 0.5f;
        box_transform.position = transform.externPos((node.aabb.min + node.aabb.max) * 0.5f);

        drawBox(box, box_transform, viewport, node.leaf_count ? BrightMagenta : (node_id ? BrightGreen : BrightCyan),
                opacity, line_width);
    }
}