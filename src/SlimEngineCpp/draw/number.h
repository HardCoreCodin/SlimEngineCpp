#pragma once

#include "../core/string.h"
#include "./text.h"

void draw(i32 number, i32 x, i32 y, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, RectI *viewport_bounds = nullptr) {
    static NumberString number_string;
    number_string = number;
    draw(number_string.string.char_ptr, x - (i32)number_string.string.length * FONT_WIDTH, y, canvas, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
void draw(i32 number, vec2i position, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, RectI *viewport_bounds = nullptr) {
    draw(number, position.x, position.y, canvas, color, opacity, viewport_bounds);
}
void draw(i32 number, vec2 position, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, RectI *viewport_bounds = nullptr) {
    draw(number, (i32)position.x, (i32)position.y, canvas, color, opacity, viewport_bounds);
}
#endif