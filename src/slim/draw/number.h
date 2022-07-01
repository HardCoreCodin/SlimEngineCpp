#pragma once

#include "../core/string.h"
#include "./text.h"

void _drawNumber(i32 number, i32 x, i32 y, const Canvas &canvas, const Color &color, f32 opacity, const RectI *viewport_bounds) {
    static NumberString number_string;
    number_string = number;
    _drawText(number_string.string.char_ptr, x - (i32)number_string.string.length * FONT_WIDTH, y, canvas, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawNumber(i32 number, i32 x, i32 y, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawNumber(number, x, y, *this, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
INLINE void Canvas::drawNumber(i32 number, vec2i position, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawNumber(number, position.x, position.y, *this, color, opacity, viewport_bounds);
}
INLINE void Canvas::drawNumber(i32 number, vec2 position, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawNumber(number, (i32)position.x, (i32)position.y, *this, color, opacity, viewport_bounds);
}
#endif


INLINE void drawNumber(i32 number, i32 x, i32 y, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawNumber(number, x, y, canvas, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
INLINE void drawNumber(i32 number, vec2i position, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawNumber(number, position.x, position.y, canvas, color, opacity, viewport_bounds);
}
INLINE void drawNumber(i32 number, vec2 position, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawNumber(number, (i32)position.x, (i32)position.y, canvas, color, opacity, viewport_bounds);
}
#endif