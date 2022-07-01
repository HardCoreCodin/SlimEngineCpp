#pragma once

#include "./line.h"
#include "../viewport/viewport.h"


void drawEdge(Edge edge, const Viewport &viewport, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) {
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