#pragma once

#include "../core/canvas.h"

void drawHLine(i32 x_start, i32 x_end, i32 y, const RectI &viewport_bounds, const Canvas &canvas, Color color, f32 opacity = 1.0f) {
    y += viewport_bounds.top;
    if (!viewport_bounds.y_range[y]) return;

    RangeI x_range{x_start, x_end};
    x_range += viewport_bounds.left;
    x_range = x_range.sub(viewport_bounds.x_range);

    color.toGamma();
    if (canvas.antialias == SSAA) {
        y <<= 1;
        x_range.first <<= 1;
        x_range.last  <<= 1;
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

void drawVLine(i32 y_start, i32 y_end, i32 x, const RectI &viewport_bounds, const Canvas &canvas, Color color, f32 opacity = 1.0f) {
    x += viewport_bounds.left;
    if (!viewport_bounds.x_range[x]) return;

    RangeI y_range{y_start, y_end};
    y_range += viewport_bounds.top;
    y_range = y_range.sub(viewport_bounds.y_range);

    color.toGamma();
    if (canvas.antialias == SSAA) {
        x <<= 1;
        y_range.first <<= 1;
        y_range.last  <<= 1;
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

void drawLine(f32 x1, f32 y1, f32 z1,
              f32 x2, f32 y2, f32 z2,
              const RectI &viewport_bounds,
              const Canvas &canvas,
              Color color, f32 opacity, u8 line_width) {
    if (x1 < 0 &&
        y1 < 0 &&
        x2 < 0 &&
        y2 < 0)
        return;

    RangeI X{viewport_bounds.x_range};
    RangeI Y{viewport_bounds.y_range};
    x1 += (f32)viewport_bounds.left;
    x2 += (f32)viewport_bounds.left;
    y1 += (f32)viewport_bounds.top;
    y2 += (f32)viewport_bounds.top;

    color.toGamma();
    i32 x, y;
    if (canvas.antialias == SSAA) {
        x1 += x1;
        x2 += x2;
        y1 += y1;
        y2 += y2;
        X.first += X.first;
        Y.first += Y.first;
        X.last += X.last;
        Y.last += Y.last;
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
    bool has_depth = z1 != 0.0f || z2 != 0.0f;
    if (fabsf(dx) > fabsf(dy)) { // Shallow:
        if (x2 < x1) { // Left to right:
            tmp = x2; x2 = x1; x1 = (f32)tmp;
            tmp = y2; y2 = y1; y1 = (f32)tmp;
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

        if (X[x]) {
            if (Y[y]) canvas.setPixel(x, y, color, oneMinusFractionOf(first_y) * gap * opacity, z1);
            for (u8 i = 0; i < line_width; i++) if (Y[++y]) canvas.setPixel(x, y, color, opacity, z1);
            if (Y[++y]) canvas.setPixel(x, y, color, fractionOf(first_y) * gap * opacity, z1);
        }

        x = end_x;
        y = end_y;
        gap = fractionOf(x2 + 0.5f);

        if (X[x]) {
            if (Y[y]) canvas.setPixel(x, y, color, oneMinusFractionOf(last_y) * gap * opacity, z2);
            for (u8 i = 0; i < line_width; i++) if (Y[++y]) canvas.setPixel(x, y, color, opacity, z2);
            if (Y[++y]) canvas.setPixel(x, y, color, fractionOf(last_y) * gap * opacity, z2);
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
            if (X[x]) {
                y = (i32) gap;

                if (has_depth) z = 1.0f / z_curr;
                if (Y[y]) canvas.setPixel(x, y, color, oneMinusFractionOf(gap) * opacity, z);
                for (u8 i = 0; i < line_width; i++) if (Y[++y]) canvas.setPixel(x, y, color, opacity, z);
                if (Y[++y]) canvas.setPixel(x, y, color, fractionOf(gap) * opacity, z);
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

        if (Y[y]) {
            if (X[x]) canvas.setPixel(x, y, color, oneMinusFractionOf(first_x) * gap * opacity, z1);
            for (u8 i = 0; i < line_width; i++) if (X[++x]) canvas.setPixel(x, y, color, opacity, z1);
            if (X[++x]) canvas.setPixel(x, y, color, fractionOf(first_x) * gap * opacity, z1);
        }

        x = end_x;
        y = end_y;
        gap = fractionOf(y2 + 0.5f);

        if (Y[y]) {
            if (X[x]) canvas.setPixel(x, y, color, oneMinusFractionOf(last_x) * gap * opacity, z2);
            for (u8 i = 0; i < line_width; i++) if (X[++x]) canvas.setPixel(x, y, color, opacity, z2);
            if (X[++x]) canvas.setPixel(x, y, color, fractionOf(last_x) * gap * opacity, z2);
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
            if (Y[y]) {
                if (has_depth) z = 1.0f / z_curr;
                x = (i32)gap;

                if (X[x]) canvas.setPixel(x, y, color, oneMinusFractionOf(gap) * opacity, z);
                for (u8 i = 0; i < line_width; i++) if (X[++x]) canvas.setPixel(x, y, color, opacity, z);
                if (X[++x]) canvas.setPixel(x, y, color, fractionOf(gap) * opacity, z);
            }

            gap += grad;
            if (has_depth) z_curr += z_step;
        }
    }
}