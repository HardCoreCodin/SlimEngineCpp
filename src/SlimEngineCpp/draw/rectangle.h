#pragma once

#include "./line.h"
#include "../viewport/viewport.h"

#define CURVE_STEPS 3600

void draw(const Rect &rect, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f) {
    if (rect.max.x < 0 || rect.min.x >= viewport.dimensions.width ||
        rect.max.y < 0 || rect.min.y >= viewport.dimensions.height)
        return;

    drawHLine(rect.min.x, rect.max.x, rect.min.y, viewport, color, opacity);
    drawHLine(rect.min.x, rect.max.x, rect.max.y, viewport, color, opacity);
    drawVLine(rect.min.y, rect.max.y, rect.min.x, viewport, color, opacity);
    drawVLine(rect.min.y, rect.max.y, rect.max.x, viewport, color, opacity);
}

void fill(const Rect &rect, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f) {
    if (rect.max.x < 0 || rect.min.x >= viewport.dimensions.width ||
        rect.max.y < 0 || rect.min.y >= viewport.dimensions.height)
        return;

    i32 min_x, min_y, max_x, max_y;
    subRange(rect.min.x, rect.max.x, viewport.dimensions.width,  0, &min_x, &max_x);
    subRange(rect.min.y, rect.max.y, viewport.dimensions.height, 0, &min_y, &max_y);
    for (i32 y = min_y; y <= max_y; y++) drawHLine(min_x, max_x, y, viewport, color, opacity);
}