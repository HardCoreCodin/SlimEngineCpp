#pragma once

#include "../core/canvas.h"

void drawHLine(i32 x_start, i32 x_end, i32 y, const Canvas &canvas, Color color, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    RangeI x_range{x_start, x_end};
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

    color.toGamma();
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

void drawVLine(i32 y_start, i32 y_end, i32 x, const Canvas &canvas, Color color, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    RangeI x_range{0, canvas.dimensions.width - 1};
    RangeI y_range{y_start, y_end};

    if (viewport_bounds) {
        x += viewport_bounds->left;
        x_range -= viewport_bounds->x_range;

        y_range += viewport_bounds->top;
        y_range -= viewport_bounds->y_range;
    }
    y_range.sub(0, canvas.dimensions.height - 1);
    if (!y_range || !x_range[x])
        return;

    color.toGamma();
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

void drawLine(f32 x1, f32 y1, f32 z1,
              f32 x2, f32 y2, f32 z2,
              const Canvas &canvas,
              Color color = White, f32 opacity = 1.0f, u8 line_width = 1,
              const RectI *viewport_bounds = nullptr) {
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

    color.toGamma();
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
    bool has_depth = z1 != 0.0f || z2 != 0.0f;
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

void drawLine2(f32 x1, f32 y1, f32 z1,
               f32 x2, f32 y2, f32 z2,
               const RectI &viewport_bounds,
               const Canvas &canvas,
               Color color, f32 opacity, u8 line_width) {
    x1 += (f32)viewport_bounds.left;
    x2 += (f32)viewport_bounds.left;
    y1 += (f32)viewport_bounds.top;
    y2 += (f32)viewport_bounds.top;
    if (x1 < 0 &&
        y1 < 0 &&
        x2 < 0 &&
        y2 < 0)
        return;

    f32 half_width = (f32)line_width * 0.5f;

    f32 left{x1}, right{x2}, top{y1}, bottom{y2};
    if (x2 < x1) {
        left = x2;
        right = x1;
    }
    if (y2 < y1) {
        top = y2;
        bottom = y1;
    }

    left -= half_width;
    right += half_width;
    top -= half_width;
    bottom += half_width;

    if ((f32)viewport_bounds.right < left || right < (f32)viewport_bounds.left ||
        (f32)viewport_bounds.bottom < top || bottom < (f32)viewport_bounds.top)
        return;

    if (left < (f32)viewport_bounds.left) left = (f32)viewport_bounds.left;
    if ((f32)viewport_bounds.right < right) right = (f32)viewport_bounds.right;
    if (top < (f32)viewport_bounds.top) top = (f32)viewport_bounds.top;
    if ((f32)viewport_bounds.bottom < bottom) bottom = (f32)viewport_bounds.bottom;

    if (canvas.antialias == SSAA) {
        x1 += x1;
        x2 += x2;
        y1 += y1;
        y2 += y2;

        half_width += half_width;
        left += left;
        right += right;
        top += top;
        bottom += bottom;
    }

    f32 X = x2 - x1;
    f32 Y = y2 - y1;

    f32 vector_length = sqrtf(X*X + Y*Y);
    f32 one_over_distance = 1.0f / vector_length;

    f32 dir_x = X * one_over_distance;
    f32 dir_y = Y * one_over_distance;

    f32 perp_x = -dir_y;
    f32 perp_y = dir_x;

    i32 x_start = (i32)left;
    i32 x_end = (i32)(right + 1) + 1;
    i32 y_start = (i32)top;
    i32 y_end = (i32)(bottom + 1) + 1;

    f32 line_softness = 1.0f;

    f32 max_distance_to_line = half_width + line_softness * 2.0f;
    f32 distance_to_line, dir_dot_x, dir_dot_y, perp_dot_x, perp_dot_y, t, z, pixel_opacity, gap, px, py, fx, fy = (f32)y_start + 0.5f;

    bool has_depth = z1 != 0.0f || z2 != 0.0f;
    if (has_depth) { // Compute one-over-depth start and step
        z1 = 1.0f / z1;
        z2 = 1.0f / z2;
    } else z = 0.0f;

    for (i32 y = y_start; y < y_end; y++) {
        py = fy - y1;
        perp_dot_y = py * perp_y;
        dir_dot_y = py * dir_y;
        for (i32 x = x_start; x < x_end; x++) {
            fx = (f32)x + 0.5f;
            px = fx - x1;
            perp_dot_x = px * perp_x;
            distance_to_line = fabsf(perp_dot_x + perp_dot_y) + line_softness;
            if (max_distance_to_line < distance_to_line)
                continue;

            dir_dot_x = px * dir_x;
            t = dir_dot_x + dir_dot_y;
            if (t < 0 || t > vector_length)
                continue;

            t /= vector_length;
            if (has_depth) {
                z = z1 + t*z2;
                z = 1.0f / z;
            }

            pixel_opacity = opacity;
            gap = distance_to_line - half_width;
            if (0 < gap)
                pixel_opacity *= gap / (line_softness * 2.0f);

            canvas.setPixel(x, y, color, pixel_opacity, z);
        }
        fy += 1.0f;
    }
}