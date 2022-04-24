#pragma once

#include "../core/string.h"
#include "./text.h"

void drawNumber(i32 number, i32 x, i32 y, const Viewport &viewport, const vec3 &color, f32 opacity) {
    static NumberString number_string;
    number_string = number;
    drawText(number_string.string.char_ptr, x - (i32)number_string.string.length * FONT_WIDTH, y, viewport, color, opacity);
}