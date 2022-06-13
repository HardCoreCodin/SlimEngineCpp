#pragma once

#include "../core/canvas.h"
#include "../core/string.h"
#include "./text.h"

void draw(i32 number, i32 x, i32 y, const Canvas &canvas, const Color &color, f32 opacity, RectI *viewport_bounds = nullptr) {
    static NumberString number_string;
    number_string = number;
    draw(number_string.string.char_ptr, x - (i32)number_string.string.length * FONT_WIDTH, y, canvas, color, opacity, viewport_bounds);
}