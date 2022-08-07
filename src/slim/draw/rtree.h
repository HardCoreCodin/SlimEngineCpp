#pragma once

#include "./edge.h"
#include "../scene/mesh.h"
#include "../core/transform.h"
#include "./box.h"

void drawRTree(const RTree &rtree, const Transform &transform, const Viewport &viewport, u16 min_depth = 0, u16 max_depth = 5, f32 opacity = 0.25f, u8 line_width = 1) {
    static Box box;
    static Transform box_transform;

    for (u32 node_id = 0; node_id < rtree.node_count; node_id++) {
        RTreeNode &node = rtree.nodes[node_id];
        if (node.depth < min_depth || node.depth > max_depth)
            continue;

        box_transform = transform;
        box_transform.scale *= (node.aabb.max - node.aabb.min) * 0.5f;
        box_transform.position = transform.externPos((node.aabb.min + node.aabb.max) * 0.5f);

        drawBox(box, box_transform, viewport, node.child_count ? Magenta : (node_id ? Green : Cyan),
                opacity, line_width);
    }
}

void drawRTreeQueryResult(const RTreeQueryResult &query_result, const RTreeNode *nodes, const Transform &transform,
                    const Viewport &viewport, const Color &color, f32 opacity = 0.5f, u8 line_width = 1) {
    static Box box;
    static Transform box_transform;

    for (u32 result_index = 0; result_index < query_result.node_count; result_index++) {
        const RTreeNode &node = nodes[query_result.node_ids[result_index]];

        box_transform = transform;
        box_transform.scale *= (node.aabb.max - node.aabb.min) * 0.5f;
        box_transform.position = transform.externPos((node.aabb.min + node.aabb.max) * 0.5f);

        drawBox(box, box_transform, viewport, color, opacity, line_width);
    }
}