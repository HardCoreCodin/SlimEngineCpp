#pragma once

#include "./canvas.h"

void drawImage(const Image &image, const Canvas &canvas, const RectI draw_bounds, f32 opacity = 1.0f) {
    if (draw_bounds.right < 0 ||
        draw_bounds.bottom < 0 ||
        draw_bounds.left >= canvas.dimensions.width ||
        draw_bounds.top >= canvas.dimensions.height)
        return;

    i32 draw_width = draw_bounds.right - draw_bounds.left;
    i32 draw_height = draw_bounds.bottom - draw_bounds.top;
    if (draw_width > (i32)image.width) draw_width = (i32)image.width;
    if (draw_height > (i32)image.height) draw_height = (i32)image.height;
    i32 remainder_x = (i32)image.width - draw_width;
    Pixel *pixel = image.pixels;
    i32 Y = draw_bounds.top;
    for (i32 y = 0; y < draw_height; y++, Y++) {
        i32 X = draw_bounds.left;
        for (i32 x = 0; x < draw_width; x++, X++, pixel++)
            canvas.setPixel(X, Y, pixel->color, opacity);

        pixel += remainder_x;
    }
}