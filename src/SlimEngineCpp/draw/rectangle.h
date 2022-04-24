#pragma once

#include "./line.h"
#include "../core/rectangle.h"
#include "../viewport/viewport.h"

template<class VectorType = vec2i, typename ValueType = i32>
void draw(const RectOf<VectorType, ValueType> &rect, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f) {
    if (rect.bottom_right.x < 0 || rect.top_left.x >= viewport.dimensions.width ||
        rect.bottom_right.y < 0 || rect.top_left.y >= viewport.dimensions.height)
        return;

    drawHLine(rect.top_left.x, rect.bottom_right.x, rect.top_left.y, viewport, color, opacity);
    drawHLine(rect.top_left.x, rect.bottom_right.x, rect.bottom_right.y, viewport, color, opacity);
    drawVLine(rect.top_left.y, rect.bottom_right.y, rect.top_left.x, viewport, color, opacity);
    drawVLine(rect.top_left.y, rect.bottom_right.y, rect.bottom_right.x, viewport, color, opacity);
}

template<class VectorType = vec2i, typename ValueType = i32>
void fill(const RectOf<VectorType, ValueType> &rect, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f) {
    if (rect.bottom_right.x < 0 || rect.top_left.x >= viewport.dimensions.width ||
        rect.bottom_right.y < 0 || rect.top_left.y >= viewport.dimensions.height)
        return;

    i32 min_x, min_y, max_x, max_y;
    subRange(rect.top_left.x, rect.bottom_right.x, viewport.dimensions.width, 0, &min_x, &max_x);
    subRange(rect.top_left.y, rect.bottom_right.y, viewport.dimensions.height, 0, &min_y, &max_y);
    for (i32 y = min_y; y <= max_y; y++) drawHLine(min_x, max_x, y, viewport, color, opacity);
}