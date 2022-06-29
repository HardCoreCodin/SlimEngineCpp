#pragma once

#if defined(SLIM_ENABLE_VIEWPORT_BOX_DRAWING) & !defined(SLIM_ENABLE_VIEWPORT_EDGE_DRAWING)
#define SLIM_ENABLE_VIEWPORT_EDGE_DRAWING
#endif

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
        viewport.drawEdge(edge, color, opacity, line_width);
    else {
        BoxEdgeSides &box_edges = view_space_box.edges.sides;
        if (sides & BoxSide_Front | sides & BoxSide_Top   ) viewport.drawEdge(box_edges.front_top,    color, opacity, line_width);
        if (sides & BoxSide_Front | sides & BoxSide_Bottom) viewport.drawEdge(box_edges.front_bottom, color, opacity, line_width);
        if (sides & BoxSide_Front | sides & BoxSide_Left  ) viewport.drawEdge(box_edges.front_left,   color, opacity, line_width);
        if (sides & BoxSide_Front | sides & BoxSide_Right ) viewport.drawEdge(box_edges.front_right,  color, opacity, line_width);
        if (sides & BoxSide_Back  | sides & BoxSide_Top   ) viewport.drawEdge(box_edges.back_top,     color, opacity, line_width);
        if (sides & BoxSide_Back  | sides & BoxSide_Bottom) viewport.drawEdge(box_edges.back_bottom,  color, opacity, line_width);
        if (sides & BoxSide_Back  | sides & BoxSide_Left  ) viewport.drawEdge(box_edges.back_left,    color, opacity, line_width);
        if (sides & BoxSide_Back  | sides & BoxSide_Right ) viewport.drawEdge(box_edges.back_right,   color, opacity, line_width);
        if (sides & BoxSide_Left  | sides & BoxSide_Top   ) viewport.drawEdge(box_edges.left_top,     color, opacity, line_width);
        if (sides & BoxSide_Left  | sides & BoxSide_Bottom) viewport.drawEdge(box_edges.left_bottom,  color, opacity, line_width);
        if (sides & BoxSide_Right | sides & BoxSide_Top   ) viewport.drawEdge(box_edges.right_top,    color, opacity, line_width);
        if (sides & BoxSide_Right | sides & BoxSide_Bottom) viewport.drawEdge(box_edges.right_bottom, color, opacity, line_width);
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