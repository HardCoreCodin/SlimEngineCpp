#pragma once

#include "./line.h"

void draw(RectI rect, const RectI &viewport_bounds, const Canvas &canvas, Color color = White, f32 opacity = 1.0f) {
    rect.x_range += viewport_bounds.left;
    rect.y_range += viewport_bounds.top;
    if (rect.isOutsideOf(viewport_bounds))
        return;

    const bool draw_top = viewport_bounds.y_range[rect.top];
    const bool draw_bottom = viewport_bounds.y_range[rect.bottom];
    const bool draw_left = viewport_bounds.x_range[rect.left];
    const bool draw_right = viewport_bounds.x_range[rect.right];
    const bool draw_horizontal = draw_left || draw_right;
    const bool draw_vertical = draw_top || draw_bottom;
    if (!(draw_horizontal || draw_vertical))
        return;

    color.toGamma();

    if (draw_horizontal) {
        RangeI x_range{rect.x_range.sub(viewport_bounds.x_range)};

        i32 bottom = rect.bottom;
        i32 top = rect.top;

        if (canvas.antialias == SSAA) {
            bottom <<= 1;
            top <<= 1;
            x_range.first <<= 1;
            x_range.last  <<= 1;
            for (i32 x = x_range.first; x <= x_range.last; x += 2) {
                if (draw_bottom) {
                    canvas.setPixel(x, bottom, color, opacity);
                    canvas.setPixel(x+1, bottom, color, opacity);
                    canvas.setPixel(x, bottom+1, color, opacity);
                    canvas.setPixel(x+1, bottom+1, color, opacity);
                }
                if (draw_top) {
                    canvas.setPixel(x, top, color, opacity);
                    canvas.setPixel(x+1, top, color, opacity);
                    canvas.setPixel(x, top+1, color, opacity);
                    canvas.setPixel(x+1, top+1, color, opacity);
                }
            }
        } else {
            for (i32 x = x_range.first; x <= x_range.last; x += 1) {
                if (draw_bottom) canvas.setPixel(x, bottom, color, opacity);
                if (draw_top) canvas.setPixel(x, top, color, opacity);
            }
        }
    }

    if (draw_vertical) {
        RangeI y_range{rect.y_range.sub(viewport_bounds.y_range)};

        i32 left = rect.left;
        i32 right = rect.right;

        if (canvas.antialias == SSAA) {
            left <<= 1;
            right <<= 1;
            y_range.first <<= 1;
            y_range.last  <<= 1;
            for (i32 y = y_range.first; y <= y_range.last; y += 2) {
                if (draw_left) {
                    canvas.setPixel(left, y, color, opacity);
                    canvas.setPixel(left+1, y, color, opacity);
                    canvas.setPixel(left, y+1, color, opacity);
                    canvas.setPixel(left+1, y+1, color, opacity);
                }
                if (draw_right) {
                    canvas.setPixel(right, y, color, opacity);
                    canvas.setPixel(right+1, y, color, opacity);
                    canvas.setPixel(right, y+1, color, opacity);
                    canvas.setPixel(right+1, y+1, color, opacity);
                }
            }
        } else {
            for (i32 y = y_range.first; y <= y_range.last; y += 1) {
                if (draw_right) canvas.setPixel(left, y, color, opacity);
                if (draw_left) canvas.setPixel(right, y, color, opacity);
            }
        }
    }
}

void fill(RectI rect, const RectI &viewport_bounds, const Canvas &canvas, Color color = White, f32 opacity = 1.0f) {
    rect.x_range += viewport_bounds.left;
    rect.y_range += viewport_bounds.top;
    if (rect.isOutsideOf(viewport_bounds))
        return;

    RangeI x_range{rect.x_range.sub(viewport_bounds.x_range)};
    RangeI y_range{rect.y_range.sub(viewport_bounds.y_range)};

    color.toGamma();

    if (canvas.antialias == SSAA) {
        x_range.first <<= 1;
        x_range.last <<= 1;
        y_range.first <<= 1;
        y_range.last <<= 1;

        for (i32 y = y_range.first; y <= y_range.last; y += 2) {
            for (i32 x = x_range.first; x <= x_range.last; x += 2) {
                canvas.setPixel(x, y, color, opacity);
                canvas.setPixel(x+1, y, color, opacity);
                canvas.setPixel(x, y+1, color, opacity);
                canvas.setPixel(x+1, y+1, color, opacity);
            }
        }
    } else
        for (i32 y = y_range.first; y <= y_range.last; y++)
            for (i32 x = x_range.first; x <= x_range.last; x++)
                canvas.setPixel(x, y, color, opacity);
}