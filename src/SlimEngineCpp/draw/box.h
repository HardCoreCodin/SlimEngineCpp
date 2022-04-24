#pragma once

#include "./edge.h"
#include "../core/transform.h"
#include "../scene/box.h"
#include "../viewport/viewport.h"

void draw(const Box &box, const Transform &transform, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1, u8 sides = BOX__ALL_SIDES) {
    static Box view_space_box;

    // Transform vertices positions from local-space to world-space and then to view-space:
    for (u8 i = 0; i < BOX__VERTEX_COUNT; i++)
        view_space_box.vertices.buffer[i] = viewport.camera->internPos(transform.externPos(box.vertices.buffer[i]));

    // Distribute transformed vertices positions to edges:
    view_space_box.edges.setFrom(view_space_box.vertices);

    if (sides == BOX__ALL_SIDES) for (const auto &edge : view_space_box.edges.buffer) draw(edge, viewport, color, opacity, line_width);
    else {
        BoxEdgeSides &box_edges = view_space_box.edges.sides;
        if (sides & Front | sides & Top   ) draw(box_edges.front_top,    viewport, color, opacity, line_width);
        if (sides & Front | sides & Bottom) draw(box_edges.front_bottom, viewport, color, opacity, line_width);
        if (sides & Front | sides & Left  ) draw(box_edges.front_left,   viewport, color, opacity, line_width);
        if (sides & Front | sides & Right ) draw(box_edges.front_right,  viewport, color, opacity, line_width);
        if (sides & Back  | sides & Top   ) draw(box_edges.back_top,     viewport, color, opacity, line_width);
        if (sides & Back  | sides & Bottom) draw(box_edges.back_bottom,  viewport, color, opacity, line_width);
        if (sides & Back  | sides & Left  ) draw(box_edges.back_left,    viewport, color, opacity, line_width);
        if (sides & Back  | sides & Right ) draw(box_edges.back_right,   viewport, color, opacity, line_width);
        if (sides & Left  | sides & Top   ) draw(box_edges.left_top,     viewport, color, opacity, line_width);
        if (sides & Left  | sides & Bottom) draw(box_edges.left_bottom,  viewport, color, opacity, line_width);
        if (sides & Right | sides & Top   ) draw(box_edges.right_top,    viewport, color, opacity, line_width);
        if (sides & Right | sides & Bottom) draw(box_edges.right_bottom, viewport, color, opacity, line_width);
    }
}