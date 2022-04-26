#pragma once

#include "./line.h"
#include "../core/rectangle.h"
#include "../viewport/viewport.h"


void draw(const RectI &RectI, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f) {
    if (RectI.right < 0 || RectI.left >= viewport.dimensions.width ||
        RectI.top < 0 || RectI.bottom >= viewport.dimensions.height)
        return;

    drawHLine(RectI.left, RectI.right, RectI.bottom, viewport, color, opacity);
    drawHLine(RectI.left, RectI.right, RectI.top, viewport, color, opacity);
    drawVLine(RectI.bottom, RectI.top, RectI.left, viewport, color, opacity);
    drawVLine(RectI.bottom, RectI.top, RectI.right, viewport, color, opacity);
}

void fill(const RectI &RectI, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f) {
    if (RectI.right < 0 || RectI.left >= viewport.dimensions.width ||
        RectI.top < 0 || RectI.bottom >= viewport.dimensions.height)
        return;

    i32 min_x, min_y, max_x, max_y;
    subRange(RectI.left, RectI.right, viewport.dimensions.width,  0, &min_x, &max_x);
    subRange(RectI.bottom, RectI.top, viewport.dimensions.height, 0, &min_y, &max_y);
    for (i32 y = min_y; y <= max_y; y++) drawHLine(min_x, max_x, y, viewport, color, opacity);
}