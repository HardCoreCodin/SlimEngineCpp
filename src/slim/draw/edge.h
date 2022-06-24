#pragma once

#include "./line.h"
#include "../viewport/viewport.h"


void _drawEdge(Edge edge, const Viewport &viewport, const Color &color, f32 opacity, u8 line_width) {
    if (!viewport.cullAndClipEdge(edge)) return;

    viewport.projectEdge(edge);
    drawLine(edge.from.x,
             edge.from.y,
             edge.from.z,
             edge.to.x,
             edge.to.y,
             edge.to.z,
             viewport.canvas, color, opacity, line_width, &viewport.bounds);
}

#ifdef SLIM_ENABLE_VIEWPORT_EDGE_DRAWING
INLINE void Viewport::drawEdge(Edge edge, const Color &color, f32 opacity, u8 line_width) const {
    _drawEdge(edge, *this, color, opacity, line_width);
}
#endif

INLINE void drawEdge(Edge edge, const Viewport &viewport, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) {
    _drawEdge(edge, viewport, color, opacity, line_width);
}