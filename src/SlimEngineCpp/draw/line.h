#pragma once

#include "../viewport/viewport.h"

void drawHLine(i32 x_start, i32 x_end, i32 y, const Viewport &viewport, const vec3 &color, f32 opacity = 1.0f) {
    x_start += viewport.position.x;
    x_end   += viewport.position.x;
    y       += viewport.position.y;

    if (!inRange(y, viewport.dimensions.height + viewport.position.y, viewport.position.y)) return;

    i32 first, last, step = 1;
    subRange(x_start, x_end, viewport.dimensions.width + viewport.position.x, viewport.position.x, &first, &last);

    Pixel pixel{color * color, opacity, 0};
    if (viewport.canvas.antialias) {
        y <<= 1;
        first <<= 1;
        last  <<= 1;
        step = 2;
        for (i32 x = first; x <= last; x += step) {
            viewport.canvas.setPixel(x+0, y+0, pixel);
            viewport.canvas.setPixel(x+1, y+0, pixel);
            viewport.canvas.setPixel(x+0, y+1, pixel);
            viewport.canvas.setPixel(x+1, y+1, pixel);
        }
    } else
        for (i32 x = first; x <= last; x += step)
            viewport.canvas.setPixel(x, y, pixel);
}

void drawVLine(i32 y_start, i32 y_end, i32 x, const Viewport &viewport, const vec3 &color, f32 opacity = 1.0f) {
    y_start += viewport.position.y;
    y_end   += viewport.position.y;
    x       += viewport.position.x;

    if (!inRange(x, viewport.dimensions.width + viewport.position.x, viewport.position.x)) return;
    i32 first, last, step = 1;

    subRange(y_start, y_end, viewport.dimensions.height + viewport.position.y, viewport.position.y, &first, &last);
    Pixel pixel{color * color, opacity, 0};
    if (viewport.canvas.antialias) {
        x <<= 1;
        first <<= 1;
        last  <<= 1;
        step = 2;
        for (i32 y = first; y <= last; y += step) {
            viewport.canvas.setPixel(x+0, y+0, pixel);
            viewport.canvas.setPixel(x+1, y+0, pixel);
            viewport.canvas.setPixel(x+0, y+1, pixel);
            viewport.canvas.setPixel(x+1, y+1, pixel);
        }
    } else
        for (i32 y = first; y <= last; y += step)
            viewport.canvas.setPixel(x, y, pixel);
}

void drawLine(f32 x1, f32 y1, f64 z1,
              f32 x2, f32 y2, f64 z2,
              const Viewport &viewport,
              vec3 color, f32 opacity, u8 line_width) {
    if (x1 < 0 &&
        y1 < 0 &&
        x2 < 0 &&
        y2 < 0)
        return;

    i32 x_left = viewport.position.x;
    i32 y_top  = viewport.position.y;
    x1 += (f32)x_left;
    x2 += (f32)x_left;
    y1 += (f32)y_top;
    y2 += (f32)y_top;

    i32 w = viewport.dimensions.width + x_left;
    i32 h = viewport.dimensions.height + y_top;

    color *= color;
    i32 x, y;
    if (viewport.canvas.antialias) {
        x1 += x1;
        x2 += x2;
        y1 += y1;
        y2 += y2;
        w <<= 1;
        h <<= 1;
        line_width <<= 1;
        line_width++;
    }
    f64 tmp, z_range, range_remap;
    f32 dx = x2 - x1;
    f32 dy = y2 - y1;
    f32 gap, grad, first_offset, last_offset;
    f64 z, z_curr, z_step;
    vec3 first, last;
    vec2i start, end;
    bool has_depth = z1 != 0.0 || z2 != 0.0;
    if (fabsf(dx) > fabsf(dy)) { // Shallow:
        if (x2 < x1) { // Left to right:
            tmp = x2; x2 = x1; x1 = (f32)tmp;
            tmp = y2; y2 = y1; y1 = (f32)tmp;
            tmp = z2; z2 = z1; z1 = tmp;
        }

        grad = dy / dx;

        first.x = roundf(x1);
        last.x  = roundf(x2);
        first_offset = first.x - x1;
        last_offset  = last.x  - x2;

        first.y = y1 + grad * first_offset;
        last.y  = y2 + grad * last_offset;

        start.x = (i32)first.x;
        start.y = (i32)first.y;
        end.x   = (i32)last.x;
        end.y   = (i32)last.y;

        x = start.x;
        y = start.y;
        gap = oneMinusFractionOf(x1 + 0.5f);

        if (inRange(x, w, x_left)) {
            if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, oneMinusFractionOf(first.y) * gap * opacity, z1);

            for (u8 i = 0; i < line_width; i++) {
                y++;
                if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, opacity, z1);
            }

            y++;
            if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, fractionOf(first.y) * gap * opacity, z1);
        }

        x = end.x;
        y = end.y;
        gap = fractionOf(x2 + 0.5f);

        if (inRange(x, w, x_left)) {
            if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, oneMinusFractionOf(last.y) * gap * opacity, z2);

            for (u8 i = 0; i < line_width; i++) {
                y++;
                if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, opacity, z2);
            }

            y++;
            if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, fractionOf(last.y) * gap * opacity, z2);
        }

        if (has_depth) { // Compute one-over-depth start and step
            z1 = 1.0 / z1;
            z2 = 1.0 / z2;
            z_range = z2 - z1;
            range_remap = z_range / (f64)(x2 - x1);
            z1 += range_remap * (f64)(first_offset + 1);
            z2 += range_remap * (f64)(last_offset  - 1);
            z_range = z2 - z1;
            z_step = z_range / (f64)(last.x - first.x - 1);
            z_curr = z1;
        } else z = 0;

        gap = first.y + grad;
        for (x = start.x + 1; x < end.x; x++) {
            if (inRange(x, w, x_left)) {
                if (has_depth) z = 1.0 / z_curr;
                y = (i32) gap;
                if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, oneMinusFractionOf(gap) * opacity, z);

                for (u8 i = 0; i < line_width; i++) {
                    y++;
                    if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, opacity, z);
                }

                y++;
                if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, fractionOf(gap) * opacity, z);
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

        first.y = roundf(y1);
        last.y  = roundf(y2);

        first_offset = y1 - first.y;
        last_offset  = last.y  - y2;

        first.x = x1 + grad * first_offset;
        last.x  = x2 + grad * last_offset;

        start.y = (i32)first.y;
        start.x = (i32)first.x;

        end.y = (i32)last.y;
        end.x = (i32)last.x;

        x = start.x;
        y = start.y;
        gap = oneMinusFractionOf(y1 + 0.5f);

        if (inRange(y, h, y_top)) {
            if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, oneMinusFractionOf(first.x) * gap * opacity, z1);

            for (u8 i = 0; i < line_width; i++) {
                x++;
                if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, opacity, z1);
            }

            x++;
            if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, fractionOf(first.x) * gap * opacity, z1);
        }

        x = end.x;
        y = end.y;
        gap = fractionOf(y2 + 0.5f);

        if (inRange(y, h, y_top)) {
            if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, oneMinusFractionOf(last.x) * gap * opacity, z2);

            for (u8 i = 0; i < line_width; i++) {
                x++;
                if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, opacity, z2);
            }

            x++;
            if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, fractionOf(last.x) * gap * opacity, z2);
        }

        if (has_depth) { // Compute one-over-depth start and step
            z1 = 1.0 / z1;
            z2 = 1.0 / z2;
            z_range = z2 - z1;
            range_remap = z_range / (f64)(y2 - y1);
            z1 += range_remap * (f64)(first_offset + 1);
            z2 += range_remap * (f64)(last_offset  - 1);
            z_range = z2 - z1;
            z_step = z_range / (f64)(last.y - first.y - 1);
            z_curr = z1;
        } else z = 0;

        gap = first.x + grad;
        for (y = start.y + 1; y < end.y; y++) {
            if (inRange(y, h, y_top)) {
                if (has_depth) z = 1.0 / z_curr;
                x = (i32)gap;

                if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, oneMinusFractionOf(gap) * opacity, z);

                for (u8 i = 0; i < line_width; i++) {
                    x++;
                    if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, opacity, z);
                }

                x++;
                if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, fractionOf(gap) * opacity, z);
            }

            gap += grad;
            if (has_depth) z_curr += z_step;
        }
    }
}
