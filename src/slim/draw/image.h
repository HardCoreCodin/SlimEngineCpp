#pragma once

#include "./canvas.h"

void drawImage(const Image &image, const Canvas &canvas, const RectI draw_bounds, f32 opacity = 1.0f) {
    if (draw_bounds.right < 0 ||
        draw_bounds.bottom < 0 ||
        draw_bounds.left >= canvas.dimensions.width ||
        draw_bounds.top >= canvas.dimensions.height)
        return;

    u16 draw_width = draw_bounds.right - draw_bounds.left;
    u16 draw_height = draw_bounds.bottom - draw_bounds.top;
    if (draw_width > image.width) draw_width = image.width;
    if (draw_height > image.height) draw_height = image.height;
    u16 remainder_x = image.width - draw_width;
    Pixel *pixel = image.pixels;
    u16 Y = draw_bounds.top;
    for (u16 y = 0; y < draw_height; y++, Y++) {
        u16 X = draw_bounds.left;
        for (u16 x = 0; x < draw_width; x++, X++, pixel++)
            canvas.setPixel(X, Y, pixel->color, opacity);

        pixel += remainder_x;
    }
}