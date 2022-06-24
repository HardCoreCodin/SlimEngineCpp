#pragma once

#include "./edge.h"
#include "../core/transform.h"
#include "../scene/grid.h"
#include "../viewport/viewport.h"

void _drawGrid(const Grid &grid, const Transform &transform, const Viewport &viewport, const Color &color, f32 opacity, u8 line_width) {
    static Grid view_space_grid;

    // Transform vertices positions from local-space to world-space and then to view-space:
    for (u8 segment = 0; segment < grid.u_segments; segment++) {
        view_space_grid.vertices.u.from[segment] = (viewport.camera->internPos(transform.externPos(grid.vertices.u.from[segment])));
        view_space_grid.vertices.u.to[segment] = (viewport.camera->internPos(transform.externPos(grid.vertices.u.to[segment])));
    }
    for (u8 segment = 0; segment < grid.v_segments; segment++) {
        view_space_grid.vertices.v.from[segment] = (viewport.camera->internPos(transform.externPos(grid.vertices.v.from[segment])));
        view_space_grid.vertices.v.to[segment] = (viewport.camera->internPos(transform.externPos(grid.vertices.v.to[segment])));
    }

    // Distribute transformed vertices positions to edges:
    view_space_grid.edges.update(view_space_grid.vertices, grid.u_segments, grid.v_segments);

    for (u8 u = 0; u < grid.u_segments; u++) drawEdge(view_space_grid.edges.u.edges[u], viewport, color, opacity, line_width);
    for (u8 v = 0; v < grid.v_segments; v++) drawEdge(view_space_grid.edges.v.edges[v], viewport, color, opacity, line_width);
}

#ifdef SLIM_ENABLE_VIEWPORT_GRID_DRAWING
INLINE void Viewport::drawGrid(const Grid &grid, const Transform &transform, const Color &color, f32 opacity, u8 line_width) const {
    _drawGrid(grid, transform, *this, color, opacity, line_width);
}
#endif

INLINE void drawGrid(const Grid &grid, const Transform &transform, const Viewport &viewport, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) {
    _drawGrid(grid, transform, viewport, color, opacity, line_width);
}