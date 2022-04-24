#pragma once

#include "./edge.h"
#include "../core/transform.h"
#include "../scene/grid.h"
#include "../viewport/viewport.h"

void draw(const Grid &grid, const Transform &transform, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1) {
    static Grid view_space_grid;

    // Transform vertices positions from local-space to world-space and then to view-space:
    for (u8 side = 0; side < 2; side++) {
        for (u8 axis = 0; axis < 2; axis++) {
            u8 segment_count = axis ? grid.v_segments : grid.u_segments;
            for (u8 segment = 0; segment < segment_count; segment++)
                view_space_grid.vertices.buffer[axis][side][segment] = (viewport.camera->internPos(transform.externPos(grid.vertices.buffer[axis][side][segment])));
        }
    }

    // Distribute transformed vertices positions to edges:
    view_space_grid.edges.update(view_space_grid.vertices, grid.u_segments, grid.v_segments);

    for (u8 u = 0; u < grid.u_segments; u++) draw(view_space_grid.edges.u.edges[u], viewport, color, opacity, line_width);
    for (u8 v = 0; v < grid.v_segments; v++) draw(view_space_grid.edges.v.edges[v], viewport, color, opacity, line_width);
}