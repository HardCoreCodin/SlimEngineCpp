#pragma once

#include "./edge.h"
#include "../core/transform.h"
#include "../scene/box.h"
#include "../viewport/viewport.h"

void _drawBox(const Box &box, const Transform &transform, const Viewport &viewport, const Color &color, f32 opacity, u8 line_width, u8 sides) {
    static Box view_space_box;

    // Transform vertices positions from local-space to world-space and then to view-space:
    for (u8 i = 0; i < BOX__VERTEX_COUNT; i++)
        view_space_box.vertices.buffer[i] = viewport.camera->internPos(transform.externPos(box.vertices.buffer[i]));

    // Distribute transformed vertices positions to edges:
    view_space_box.edges.setFrom(view_space_box.vertices);

    if (sides == BOX__ALL_SIDES) for (const auto &edge : view_space_box.edges.buffer)
            drawEdge(edge, viewport, color, opacity, line_width);
    else {
        BoxEdgeSides &box_edges = view_space_box.edges.sides;
        if (sides & Front | sides & Top   ) drawEdge(box_edges.front_top, viewport, color, opacity, line_width);
        if (sides & Front | sides & Bottom) drawEdge(box_edges.front_bottom, viewport, color, opacity, line_width);
        if (sides & Front | sides & Left  ) drawEdge(box_edges.front_left, viewport, color, opacity, line_width);
        if (sides & Front | sides & Right ) drawEdge(box_edges.front_right, viewport, color, opacity, line_width);
        if (sides & Back  | sides & Top   ) drawEdge(box_edges.back_top, viewport, color, opacity, line_width);
        if (sides & Back  | sides & Bottom) drawEdge(box_edges.back_bottom, viewport, color, opacity, line_width);
        if (sides & Back  | sides & Left  ) drawEdge(box_edges.back_left, viewport, color, opacity, line_width);
        if (sides & Back  | sides & Right ) drawEdge(box_edges.back_right, viewport, color, opacity, line_width);
        if (sides & Left  | sides & Top   ) drawEdge(box_edges.left_top, viewport, color, opacity, line_width);
        if (sides & Left  | sides & Bottom) drawEdge(box_edges.left_bottom, viewport, color, opacity, line_width);
        if (sides & Right | sides & Top   ) drawEdge(box_edges.right_top, viewport, color, opacity, line_width);
        if (sides & Right | sides & Bottom) drawEdge(box_edges.right_bottom, viewport, color, opacity, line_width);
    }
}

#ifdef SLIM_ENABLE_VIEWPORT_BOX_DRAWING
INLINE void Viewport::drawBox(const Box &box, const Transform &transform, const Color &color, f32 opacity, u8 line_width, u8 sides) const {
    _drawBox(box, transform, *this, color, opacity, line_width, sides);
}
#endif

INLINE void drawBox(const Box &box, const Transform &transform, const Viewport &viewport,
                    const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, u8 sides = BOX__ALL_SIDES) {
    _drawBox(box, transform, viewport, color, opacity, line_width, sides);
}