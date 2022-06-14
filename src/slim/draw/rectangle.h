#pragma once

#include "./line.h"

void draw(RectI rect, const Canvas &canvas, Color color = White, f32 opacity = 1.0f, RectI *viewport_bounds = nullptr) {
    RectI bounds{0, canvas.dimensions.width - 1, 0, canvas.dimensions.height - 1};
    if (viewport_bounds) {
        bounds -= *viewport_bounds;
        rect.x_range += viewport_bounds->left;
        rect.y_range += viewport_bounds->top;
    }
    if (rect.isOutsideOf(bounds))
        return;

    const bool draw_top = bounds.y_range[rect.top];
    const bool draw_bottom = bounds.y_range[rect.bottom];
    const bool draw_left = bounds.x_range[rect.left];
    const bool draw_right = bounds.x_range[rect.right];

    const bool draw_horizontal = draw_left || draw_right;
    const bool draw_vertical = draw_top || draw_bottom;
    if (!(draw_horizontal || draw_vertical))
        return;

    rect = bounds - rect;
    if (!rect)
        return;

    color.toGamma();

    if (canvas.antialias == SSAA) {
        rect *= 2;

        if (draw_horizontal) {
            for (i32 x = rect.left; x <= rect.right; x += 2) {
                if (draw_bottom) {
                    canvas.setPixel(x, rect.bottom, color, opacity);
                    canvas.setPixel(x+1, rect.bottom, color, opacity);
                    canvas.setPixel(x, rect.bottom+1, color, opacity);
                    canvas.setPixel(x+1, rect.bottom+1, color, opacity);
                }
                if (draw_top) {
                    canvas.setPixel(x, rect.top, color, opacity);
                    canvas.setPixel(x+1, rect.top, color, opacity);
                    canvas.setPixel(x, rect.top+1, color, opacity);
                    canvas.setPixel(x+1, rect.top+1, color, opacity);
                }
            }
        }

        if (draw_vertical) {
            for (i32 y = rect.top; y <= rect.bottom; y += 2) {
                if (draw_left) {
                    canvas.setPixel(rect.left, y, color, opacity);
                    canvas.setPixel(rect.left+1, y, color, opacity);
                    canvas.setPixel(rect.left, y+1, color, opacity);
                    canvas.setPixel(rect.left+1, y+1, color, opacity);
                }
                if (draw_right) {
                    canvas.setPixel(rect.right, y, color, opacity);
                    canvas.setPixel(rect.right+1, y, color, opacity);
                    canvas.setPixel(rect.right, y+1, color, opacity);
                    canvas.setPixel(rect.right+1, y+1, color, opacity);
                }
            }
        }
    } else {
        if (draw_horizontal) {
            for (i32 x = rect.left; x <= rect.right; x++) {
                if (draw_bottom) canvas.setPixel(x, rect.bottom, color, opacity);
                if (draw_top) canvas.setPixel(x, rect.top, color, opacity);
            }
        }

        if (draw_vertical) {
            for (i32 y = rect.top; y <= rect.bottom; y++) {
                if (draw_right) canvas.setPixel(rect.right, y, color, opacity);
                if (draw_left) canvas.setPixel(rect.left, y, color, opacity);
            }
        }
    }
}

void fill(RectI rect, const Canvas &canvas, Color color = White, f32 opacity = 1.0f, RectI *viewport_bounds = nullptr) {
    RectI bounds{0, canvas.dimensions.width - 1, 0, canvas.dimensions.height - 1};
    if (viewport_bounds) {
        bounds -= *viewport_bounds;
        rect.x_range += viewport_bounds->left;
        rect.y_range += viewport_bounds->top;
    }
    if (rect.isOutsideOf(bounds))
        return;

    rect = bounds - rect;
    if (!rect)
        return;

    color.toGamma();

    if (canvas.antialias == SSAA) {
        rect *= 2;

        for (i32 y = rect.top; y <= rect.bottom; y += 2) {
            for (i32 x = rect.left; x <= rect.right; x += 2) {
                canvas.setPixel(x, y, color, opacity);
                canvas.setPixel(x+1, y, color, opacity);
                canvas.setPixel(x, y+1, color, opacity);
                canvas.setPixel(x+1, y+1, color, opacity);
            }
        }
    } else
        for (i32 y = rect.top; y <= rect.bottom; y++)
            for (i32 x = rect.left; x <= rect.right; x++)
                canvas.setPixel(x, y, color, opacity);
}