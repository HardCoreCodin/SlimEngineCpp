#pragma once

#include "./canvas.h"

void _drawHLine(RangeI x_range, i32 y, const Canvas &canvas, const Color &color, f32 opacity, const RectI *viewport_bounds) {
    RangeI y_range{0, canvas.dimensions.height - 1};

    if (viewport_bounds) {
        y += viewport_bounds->top;
        y_range -= viewport_bounds->y_range;

        x_range += viewport_bounds->left;
        x_range -= viewport_bounds->x_range;
    }
    x_range.sub(0, canvas.dimensions.width - 1);
    if (!x_range || !y_range[y])
        return;

    if (canvas.antialias == SSAA) {
        y *= 2;
        x_range *= 2;
        for (i32 x = x_range.first; x <= x_range.last; x += 2) {
            canvas.setPixel(x, y, color, opacity);
            canvas.setPixel(x+1, y, color, opacity);
            canvas.setPixel(x, y+1, color, opacity);
            canvas.setPixel(x+1, y+1, color, opacity);
        }
    } else
        for (i32 x = x_range.first; x <= x_range.last; x += 1)
            canvas.setPixel(x, y, color, opacity);
}

void _drawVLine(RangeI y_range, i32 x, const Canvas &canvas, const Color &color, f32 opacity, const RectI *viewport_bounds) {
    RangeI x_range{0, canvas.dimensions.width - 1};

    if (viewport_bounds) {
        x += viewport_bounds->left;
        x_range -= viewport_bounds->x_range;

        y_range += viewport_bounds->top;
        y_range -= viewport_bounds->y_range;
    }
    y_range.sub(0, canvas.dimensions.height - 1);
    if (!y_range || !x_range[x])
        return;

    if (canvas.antialias == SSAA) {
        x *= 2;
        y_range *= 2;
        for (i32 y = y_range.first; y <= y_range.last; y += 2) {
            canvas.setPixel(x, y, color, opacity);
            canvas.setPixel(x+1, y, color, opacity);
            canvas.setPixel(x, y+1, color, opacity);
            canvas.setPixel(x+1, y+1, color, opacity);
        }
    } else
        for (i32 y = y_range.first; y <= y_range.last; y += 1)
            canvas.setPixel(x, y, color, opacity);
}

void _drawLine(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, const Canvas &canvas,
               const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) {
    Range float_x_range{x1 <= x2 ? x1 : x2, x1 <= x2 ? x2 : x1};
    Range float_y_range{y1 <= y2 ? y1 : y2, y1 <= y2 ? y2 : y1};
    if (viewport_bounds) {
        const f32 left = (f32)viewport_bounds->left;
        const f32 right = (f32)viewport_bounds->right;
        const f32 top = (f32)viewport_bounds->top;
        const f32 bottom = (f32)viewport_bounds->bottom;
        x1 += left;
        x2 += left;
        y1 += top;
        y2 += top;
        float_x_range += left;
        float_y_range += top;
        float_x_range.sub(left, right);
        float_y_range.sub(top, bottom);
    }
    float_x_range.sub(0, canvas.dimensions.f_width - 1.0f);
    float_y_range.sub(0, canvas.dimensions.f_height - 1.0f);
    if (!float_x_range || !float_y_range)
        return;

    RangeI x_range{(i32)float_x_range.first, (i32)(ceilf(float_x_range.last))};
    RangeI y_range{(i32)float_y_range.first, (i32)(ceilf(float_y_range.last))};
    if (x_range.last == (i32)canvas.dimensions.width) x_range.last--;
    if (y_range.last == (i32)canvas.dimensions.height) y_range.last--;

    i32 x, y;
    if (canvas.antialias == SSAA) {
        x1 += x1;
        x2 += x2;
        y1 += y1;
        y2 += y2;
        x_range.first <<= 1;
        y_range.first <<= 1;
        x_range.last <<= 1;
        y_range.last <<= 1;
        line_width <<= 1;
        line_width++;
    }
    f32 tmp, z_range, range_remap;
    f32 dx = x2 - x1;
    f32 dy = y2 - y1;
    f32 gap, grad, first_offset, last_offset;
    f32 z, z_curr, z_step;
    f32 first_x, last_x;
    f32 first_y, last_y;
    i32 start_x, end_x;
    i32 start_y, end_y;
    bool has_depth = (canvas.depths != nullptr) && ((z1 != 0.0f) || (z2 != 0.0f));
    if (fabsf(dx) > fabsf(dy)) { // Shallow:
        if (x2 < x1) { // Left to right:
            tmp = x2; x2 = x1; x1 = tmp;
            tmp = y2; y2 = y1; y1 = tmp;
            tmp = z2; z2 = z1; z1 = tmp;
        }

        grad = dy / dx;

        first_x = roundf(x1);
        last_x  = roundf(x2);
        first_offset = first_x - x1;
        last_offset  = last_x  - x2;

        first_y = y1 + grad * first_offset;
        last_y  = y2 + grad * last_offset;

        start_x = (i32)first_x;
        start_y = (i32)first_y;
        end_x   = (i32)last_x;
        end_y   = (i32)last_y;

        x = start_x;
        y = start_y;
        gap = oneMinusFractionOf(x1 + 0.5f);

        if (x_range[x]) {
            if (y_range[y]) canvas.setPixel(x, y, color, oneMinusFractionOf(first_y) * gap * opacity, z1);
            for (u8 i = 0; i < line_width; i++) if (y_range[++y]) canvas.setPixel(x, y, color, opacity, z1);
            if (y_range[++y]) canvas.setPixel(x, y, color, fractionOf(first_y) * gap * opacity, z1);
        }

        x = end_x;
        y = end_y;
        gap = fractionOf(x2 + 0.5f);

        if (x_range[x]) {
            if (y_range[y]) canvas.setPixel(x, y, color, oneMinusFractionOf(last_y) * gap * opacity, z2);
            for (u8 i = 0; i < line_width; i++) if (y_range[++y]) canvas.setPixel(x, y, color, opacity, z2);
            if (y_range[++y]) canvas.setPixel(x, y, color, fractionOf(last_y) * gap * opacity, z2);
        }

        if (has_depth) { // Compute one-over-depth start and step
            z1 = 1.0f / z1;
            z2 = 1.0f / z2;
            z_range = z2 - z1;
            range_remap = z_range / (x2 - x1);
            z1 += range_remap * (first_offset + 1.0f);
            z2 += range_remap * (last_offset  - 1.0f);
            z_curr = z1;
            z_range = z2 - z1;
            z_step = z_range / (last_x - first_x - 1.0f);
        } else z = 0.0f;

        gap = first_y + grad;
        for (x = start_x + 1; x < end_x; x++) {
            if (x_range[x]) {
                y = (i32) gap;

                if (has_depth) z = 1.0f / z_curr;
                if (y_range[y]) canvas.setPixel(x, y, color, oneMinusFractionOf(gap) * opacity, z);
                for (u8 i = 0; i < line_width; i++) if (y_range[++y]) canvas.setPixel(x, y, color, opacity, z);
                if (y_range[++y]) canvas.setPixel(x, y, color, fractionOf(gap) * opacity, z);
            }

            gap += grad;
            if (has_depth) z_curr += z_step;
        }
    } else { // Steep:
        if (y2 < y1) { // Bottom up:
            tmp = x2; x2 = x1; x1 = (f32)tmp;
            tmp = y2; y2 = y1; y1 = (f32)tmp;
            tmp = z2; z2 = z1; z1 = tmp;
        }

        grad = dx / dy;

        first_y = roundf(y1);
        last_y  = roundf(y2);

        first_offset = y1 - first_y;
        last_offset  = last_y  - y2;

        first_x = x1 + grad * first_offset;
        last_x  = x2 + grad * last_offset;

        start_y = (i32)first_y;
        start_x = (i32)first_x;

        end_y = (i32)last_y;
        end_x = (i32)last_x;

        x = start_x;
        y = start_y;
        gap = oneMinusFractionOf(y1 + 0.5f);

        if (y_range[y]) {
            if (x_range[x]) canvas.setPixel(x, y, color, oneMinusFractionOf(first_x) * gap * opacity, z1);
            for (u8 i = 0; i < line_width; i++) if (x_range[++x]) canvas.setPixel(x, y, color, opacity, z1);
            if (x_range[++x]) canvas.setPixel(x, y, color, fractionOf(first_x) * gap * opacity, z1);
        }

        x = end_x;
        y = end_y;
        gap = fractionOf(y2 + 0.5f);

        if (y_range[y]) {
            if (x_range[x]) canvas.setPixel(x, y, color, oneMinusFractionOf(last_x) * gap * opacity, z2);
            for (u8 i = 0; i < line_width; i++) if (x_range[++x]) canvas.setPixel(x, y, color, opacity, z2);
            if (x_range[++x]) canvas.setPixel(x, y, color, fractionOf(last_x) * gap * opacity, z2);
        }

        if (has_depth) { // Compute one-over-depth start and step
            z1 = 1.0f / z1;
            z2 = 1.0f / z2;
            z_range = z2 - z1;
            range_remap = z_range / (y2 - y1);
            z1 += range_remap * (first_offset + 1.0f);
            z2 += range_remap * (last_offset  - 1.0f);
            z_range = z2 - z1;
            z_step = z_range / (last_y - first_y - 1.0f);
            z_curr = z1;
        } else z = 0.0f;

        gap = first_x + grad;
        for (y = start_y + 1; y < end_y; y++) {
            if (y_range[y]) {
                if (has_depth) z = 1.0f / z_curr;
                x = (i32)gap;

                if (x_range[x]) canvas.setPixel(x, y, color, oneMinusFractionOf(gap) * opacity, z);
                for (u8 i = 0; i < line_width; i++) if (x_range[++x]) canvas.setPixel(x, y, color, opacity, z);
                if (x_range[++x]) canvas.setPixel(x, y, color, fractionOf(gap) * opacity, z);
            }

            gap += grad;
            if (has_depth) z_curr += z_step;
        }
    }
}


INLINE void Canvas::drawHLine(RangeI x_range, i32 y, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawHLine(x_range, y, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawHLine(i32 x_start, i32 x_end, i32 y, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawHLine(RangeI{x_start, x_end}, y, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawVLine(RangeI y_range, i32 x, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawVLine(y_range, x, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawVLine(i32 y_start, i32 y_end, i32 x, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawVLine(RangeI{y_start, y_end}, x, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawLine(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawLine(x1, y1, z1, x2, y2, z2, *this, color, opacity, line_width, viewport_bounds);
}
INLINE void Canvas::drawLine(f32 x1, f32 y1, f32 x2, f32 y2, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawLine(x1, y1, 0, x2, y2, 0, *this, color, opacity, line_width, viewport_bounds);
}

#ifdef SLIM_VEC2
INLINE void Canvas::drawLine(vec2 from, vec2 to, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawLine(from.x, from.y, 0, to.x, to.y, 0, *this, color, opacity, line_width, viewport_bounds);
}
INLINE void Canvas::drawLine(vec2i from, vec2i to, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawLine((f32)from.x, (f32)from.y, 0, (f32)to.x, (f32)to.y, 0, *this, color, opacity, line_width, viewport_bounds);
}
#endif


INLINE void drawHLine(RangeI x_range, i32 y, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawHLine(x_range, y, canvas, color, opacity, viewport_bounds);
}

INLINE void drawHLine(i32 x_start, i32 x_end, i32 y, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawHLine(RangeI{x_start, x_end}, y, canvas, color, opacity, viewport_bounds);
}

INLINE void drawVLine(RangeI y_range, i32 x, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawVLine(y_range, x, canvas, color, opacity, viewport_bounds);
}
INLINE void drawVLine(i32 y_start, i32 y_end, i32 x, const Canvas &canvas, const Color &color, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawVLine(RangeI{y_start, y_end}, x, canvas, color, opacity, viewport_bounds);
}

INLINE void drawLine(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) {
    _drawLine(x1, y1, z1, x2, y2, z2, canvas, color, opacity, line_width, viewport_bounds);
}

INLINE void drawLine(f32 x1, f32 y1, f32 x2, f32 y2, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) {
    _drawLine(x1, y1, 0, x2, y2, 0, canvas, color, opacity, line_width, viewport_bounds);
}

#ifdef SLIM_VEC2
void drawLine(vec2 from, vec2 to, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) {
    _drawLine(from.x, from.y, 0, to.x, to.y, 0, canvas, color, opacity, line_width, viewport_bounds);
}
#endif

#ifdef SLIM_VEC3
void drawLine(const vec3 &from, const vec3 &to, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) {
    _drawLine(from.x, from.y, from.z, to.x, to.y, to.z, canvas, color, opacity, line_width, viewport_bounds);
}
#endif