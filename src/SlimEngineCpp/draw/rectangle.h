#pragma once

#include "./line.h"
#include "../core/rectangle.h"
#include "../viewport/viewport.h"


void draw(const RectI &RectI, const vec2i &offset, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f) {
    if (RectI.right < 0 || RectI.left >= canvas.dimensions.width ||
        RectI.top < 0 || RectI.bottom >= canvas.dimensions.height)
        return;

    drawHLine(RectI.left, RectI.right, RectI.bottom, offset, canvas, color, opacity);
    drawHLine(RectI.left, RectI.right, RectI.top, offset, canvas, color, opacity);
    drawVLine(RectI.bottom, RectI.top, RectI.left, offset, canvas, color, opacity);
    drawVLine(RectI.bottom, RectI.top, RectI.right, offset, canvas, color, opacity);
}

void fill(const RectI &RectI, vec2i &offset, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f) {
    if (RectI.right < 0 || RectI.left >= canvas.dimensions.width ||
        RectI.top < 0 || RectI.bottom >= canvas.dimensions.height)
        return;

    i32 min_x, min_y, max_x, max_y;
    subRange(RectI.left, RectI.right, canvas.dimensions.width,  0, &min_x, &max_x);
    subRange(RectI.bottom, RectI.top, canvas.dimensions.height, 0, &min_y, &max_y);
    for (i32 y = min_y; y <= max_y; y++) drawHLine(min_x, max_x, y, offset, canvas, color, opacity);
}