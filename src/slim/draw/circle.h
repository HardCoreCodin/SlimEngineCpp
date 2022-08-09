#pragma once

#include "canvas.h"

void _drawCircle(bool fill, i32 center_x, i32 center_y, i32 radius, const Canvas &canvas,
                  const Color &color, f32 opacity, const RectI *viewport_bounds) {
    RectI bounds{0, canvas.dimensions.width - 1, 0, canvas.dimensions.height - 1};
    RectI rect{center_x - radius,
               center_x + radius,
               center_y - radius,
               center_y + radius};
    if (viewport_bounds) {
        center_x += viewport_bounds->left;
        center_y += viewport_bounds->top;
        rect.x_range += viewport_bounds->left;
        rect.y_range += viewport_bounds->top;
        rect -= *viewport_bounds;
        bounds -= *viewport_bounds;
    }
    rect -= bounds;
    if (!rect)
        return;

    if (radius <= 1) {
        if (canvas.antialias == SSAA) {
            center_x *= 2;
            center_y *= 2;
            canvas.setPixel(center_x, center_y, color, opacity);
            canvas.setPixel(center_x+1, center_y, color, opacity);
            canvas.setPixel(center_x, center_y+1, color, opacity);
            canvas.setPixel(center_x+1, center_y+1, color, opacity);
        } else
            canvas.setPixel(center_x, center_y, color, opacity);

        return;
    }

    if (canvas.antialias == SSAA) {
        center_x *= 2;
        center_y *= 2;
        radius *= 2;
        bounds *= 2;
    }

    i32 x = radius, y = 0, y2 = 0;
    i32 r2 = radius * radius;
    i32 x2 = r2;

    RangeI x_range1{center_x - radius, center_x + radius};
    RangeI y_range2{center_y - radius, center_y + radius};
    RangeI y_range1{center_y, center_y};
    RangeI x_range2{center_x, center_x};
    RangeI range1, range2;
    i32 i;

    while (y <= x) {
        if (fill) {
            range1 = bounds.x_range - x_range1;
            range2 = bounds.x_range - x_range2;
            if (bounds.y_range[y_range1.first]) for (i = range1.first; i <= range1.last; i++) canvas.setPixel(i, y_range1.first, color, opacity);
            if (bounds.y_range[y_range1.last])  for (i = range1.first; i <= range1.last; i++) canvas.setPixel(i, y_range1.last , color, opacity);
            if (bounds.y_range[y_range2.first]) for (i = range2.first; i <= range2.last; i++) canvas.setPixel(i, y_range2.first, color, opacity);
            if (bounds.y_range[y_range2.last])  for (i = range2.first; i <= range2.last; i++) canvas.setPixel(i, y_range2.last , color, opacity);
        } else {
            if (bounds.y_range[y_range1.first]) {
                if (bounds.x_range[x_range1.first]) canvas.setPixel(x_range1.first, y_range1.first, color, opacity);
                if (bounds.x_range[x_range1.last ]) canvas.setPixel(x_range1.last,  y_range1.first, color, opacity);
            }
            if (bounds.y_range[y_range1.last]) {
                if (bounds.x_range[x_range1.first]) canvas.setPixel(x_range1.first, y_range1.last, color, opacity);
                if (bounds.x_range[x_range1.last ]) canvas.setPixel(x_range1.last,  y_range1.last, color, opacity);
            }

            if (bounds.y_range[y_range2.first]) {
                if (bounds.x_range[x_range2.first]) canvas.setPixel(x_range2.first, y_range2.first, color, opacity);
                if (bounds.x_range[x_range2.last ]) canvas.setPixel(x_range2.last,  y_range2.first, color, opacity);
            }
            if (bounds.y_range[y_range2.last]) {
                if (bounds.x_range[x_range2.first]) canvas.setPixel(x_range2.first, y_range2.last, color, opacity);
                if (bounds.x_range[x_range2.last ]) canvas.setPixel(x_range2.last,  y_range2.last, color, opacity);
            }
        }

        if ((x2 + y2) > r2) {
            x--;
            x2 = x * x;

            x_range1.first++;
            y_range2.first++;
            x_range1.last--;
            y_range2.last--;
        }

        y++;
        y2 = y * y;

        y_range1.first--;
        x_range2.first--;
        y_range1.last++;
        x_range2.last++;
    }
}

INLINE void Canvas::fillCircle(i32 center_x, i32 center_y, i32 radius, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawCircle(true, center_x, center_y, radius, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawCircle(i32 center_x, i32 center_y, i32 radius, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawCircle(false, center_x, center_y, radius, *this, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
INLINE void Canvas::drawCircle(vec2i center, i32 radius, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawCircle(false, center.x, center.y, radius, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::fillCircle(vec2i center, i32 radius, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawCircle(true, center.x, center.y, radius, *this, color, opacity, viewport_bounds);
}
INLINE void Canvas::drawCircle(vec2 center, i32 radius, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawCircle(false, (i32)center.x, (i32)center.y, radius, *this, color, opacity, viewport_bounds);
}
INLINE void Canvas::fillCircle(vec2 center, i32 radius, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawCircle(true, (i32)center.x, (i32)center.y, radius, *this, color, opacity, viewport_bounds);
}
#endif


INLINE void fillCircle(i32 center_x, i32 center_y, i32 radius, const Canvas &canvas,
                       Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawCircle(true, center_x, center_y, radius, canvas, color, opacity, viewport_bounds);
}

INLINE void drawCircle(i32 center_x, i32 center_y, i32 radius, const Canvas &canvas,
                       Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawCircle(false, center_x, center_y, radius, canvas, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
INLINE void drawCircle(vec2i center, i32 radius, const Canvas &canvas,
                       Color color = White, f32 opacity = 1.0f,
                       const RectI *viewport_bounds = nullptr) {
    _drawCircle(false, center.x, center.y, radius, canvas, color, opacity, viewport_bounds);
}

INLINE void fillCircle(vec2i center, i32 radius,
                       const Canvas &canvas,
                       Color color = White, f32 opacity = 1.0f,
                       const RectI *viewport_bounds = nullptr) {
    _drawCircle(true, center.x, center.y, radius, canvas, color, opacity, viewport_bounds);
}
INLINE void drawCircle(vec2 center, i32 radius, const Canvas &canvas,
                       Color color = White, f32 opacity = 1.0f,
                       const RectI *viewport_bounds = nullptr) {
    _drawCircle(false, (i32)center.x, (i32)center.y, radius, canvas, color, opacity, viewport_bounds);
}

INLINE void fillCircle(vec2 center, i32 radius,
                       const Canvas &canvas,
                       Color color = White, f32 opacity = 1.0f,
                       const RectI *viewport_bounds = nullptr) {
    _drawCircle(true, (i32)center.x, (i32)center.y, radius, canvas, color, opacity, viewport_bounds);
}
#endif