#pragma once

#include "./canvas.h"

void drawImage(const PixelImage &image, const Canvas &canvas, RectI bounds, f32 opacity = 1.0f) {
    if (bounds.right < 0 ||
        bounds.bottom < 0 ||
        bounds.left >= canvas.dimensions.width ||
        bounds.top >= canvas.dimensions.height)
        return;

    i32 image_width = (i32)image.width;
    i32 image_height = (i32)image.height;
    i32 width = bounds.right - bounds.left;
    i32 height = bounds.bottom - bounds.top;
    if (width > image_width) {
        width = image_width;
        bounds.right = bounds.left + width;
    }
    if (height > image_height) {
        height = image_height;
        bounds.bottom = bounds.top + height;
    }

    Pixel *pixel = image.content;
    if (image.flags.tile) {
        TiledGridInfo grid{image};
        u32 X, Y = 0;
        for (grid.row = 0; grid.row < grid.rows; grid.row++) {
            X = 0;
            u32 tile_height = grid.row == grid.bottom_row ? grid.bottom_row_tile_height : image.tile_height;
            for (grid.column = 0; grid.column < grid.columns; grid.column++) {
                u32 tile_width = grid.column == grid.right_column ? grid.right_column_tile_stride : image.tile_width;

                for (u32 y = 0; y < tile_height; y++)
                    for (u32 x = 0; x < tile_width; x++, pixel++)
                        canvas.setPixel(X + x, Y + y, pixel->color, image.flags.alpha ? (pixel->opacity * opacity) : opacity);

                X += image.tile_width;
            }
            Y += image.tile_height;
        }
    } else {
        i32 remainder_x = image_width - width;
        for (i32 y = bounds.top; y <= bounds.bottom; y++) {
            for (i32 x = bounds.left; x <= bounds.right; x++, pixel++)
                canvas.setPixel(x, y, pixel->color, image.flags.alpha ? (pixel->opacity * opacity) : opacity);

            pixel += remainder_x;
        }
    }
}

void drawImage(const FloatImage &image, const Canvas &canvas, RectI bounds, f32 opacity = 1.0f) {
    if (bounds.right < 0 ||
        bounds.bottom < 0 ||
        bounds.left >= canvas.dimensions.width ||
        bounds.top >= canvas.dimensions.height)
        return;

    i32 image_width = (i32)image.width;
    i32 image_height = (i32)image.height;
    i32 width = bounds.right - bounds.left;
    i32 height = bounds.bottom - bounds.top;
    if (width > image_width) {
        width = image_width;
        bounds.right = bounds.left + width;
    }
    if (height > image_height) {
        height = image_height;
        bounds.bottom = bounds.top + height;
    }

    bool has_opacity = image.flags.alpha;
    i32 channels_per_pixel = has_opacity ? 4 : 3;
    Color color;
    f32 pixel_opacity;
    f32 *channel = image.content;
    if (image.flags.tile) {
        TiledGridInfo grid{image};
        u32 X, Y = 0;
        for (grid.row = 0; grid.row < grid.rows; grid.row++) {
            X = 0;
            u32 tile_height = grid.row == grid.bottom_row ? grid.bottom_row_tile_height : image.tile_height;
            for (grid.column = 0; grid.column < grid.columns; grid.column++) {
                u32 tile_width = grid.column == grid.right_column ? grid.right_column_tile_stride : image.tile_width;

                for (u32 y = 0; y < tile_height; y++) {
                    for (u32 x = 0; x < tile_width; x++) {
                        color.r = *(channel++);
                        color.g = *(channel++);
                        color.b = *(channel++);
                        if (has_opacity)
                            pixel_opacity = *(channel++);
                        else
                            pixel_opacity = 1.0f;

                        canvas.setPixel(X + x, Y + y, color, pixel_opacity * opacity);
                    }
                }

                X += image.tile_width;
            }
            Y += image.tile_height;
        }
    } else {
        i32 remainder_x = ((i32)image.stride - width) * channels_per_pixel;
        for (i32 y = bounds.top; y <= bounds.bottom; y++) {
            for (i32 x = bounds.left; x <= bounds.right; x++) {
                color.r = *(channel++);
                color.g = *(channel++);
                color.b = *(channel++);
                if (has_opacity)
                    pixel_opacity = *(channel++);
                else
                    pixel_opacity = 1.0f;

                canvas.setPixel(x, y, color, pixel_opacity * opacity);
            }

            channel += remainder_x;
        }
    }
}

void drawImage(const ByteColorImage &image, const Canvas &canvas, RectI bounds, f32 opacity = 1.0f) {
    if (bounds.right < 0 ||
        bounds.bottom < 0 ||
        bounds.left >= canvas.dimensions.width ||
        bounds.top >= canvas.dimensions.height)
        return;

    i32 image_width = (i32)image.width;
    i32 image_height = (i32)image.height;
    i32 width = bounds.right - bounds.left;
    i32 height = bounds.bottom - bounds.top;
    if (width > image_width) {
        width = image_width;
        bounds.right = bounds.left + width;
    }
    if (height > image_height) {
        height = image_height;
        bounds.bottom = bounds.top + height;
    }

    Pixel pixel;;
    ByteColor *byte_color = image.content;
    if (image.flags.tile) {
        TiledGridInfo grid{image};
        u32 X, Y = 0;
        for (grid.row = 0; grid.row < grid.rows; grid.row++) {
            X = 0;
            u32 tile_height = grid.row == grid.bottom_row ? grid.bottom_row_tile_height : image.tile_height;
            for (grid.column = 0; grid.column < grid.columns; grid.column++) {
                u32 tile_width = grid.column == grid.right_column ? grid.right_column_tile_stride : image.tile_width;

                for (u32 y = 0; y < tile_height; y++) {
                    for (u32 x = 0; x < tile_width; x++, byte_color++) {
                        pixel = *byte_color;
                        canvas.setPixel(X + x, Y + y, pixel.color, image.flags.alpha ? (pixel.opacity * opacity) : opacity);
                    }
                }

                X += image.tile_width;
            }
            Y += image.tile_height;
        }
    } else {
        i32 remainder_x = (i32)image.stride - width;
        for (i32 y = bounds.top; y <= bounds.bottom; y++) {
            for (i32 x = bounds.left; x <= bounds.right; x++, byte_color++) {
                pixel = *byte_color;
                canvas.setPixel(x, y, pixel.color, image.flags.alpha ? (pixel.opacity * opacity) : opacity);
            }

            byte_color += remainder_x;
        }
    }
}

void drawImageToWindow(const ByteColorImage &image, RectI bounds, f32 opacity = 1.0f) {
    if (bounds.right < 0 ||
        bounds.bottom < 0 ||
        bounds.left >= window::width ||
        bounds.top >= window::height)
        return;

    i32 image_width = (i32)image.width;
    i32 image_height = (i32)image.height;
    i32 width = bounds.right - bounds.left;
    i32 height = bounds.bottom - bounds.top;
    if (width > image_width) {
        width = image_width;
        bounds.right = bounds.left + width;
    }
    if (height > image_height) {
        height = image_height;
        bounds.bottom = bounds.top + height;
    }

    ByteColor *byte_color = image.content;
    if (image.flags.tile) {
        TiledGridInfo grid{image};
        u32 X, Y = 0;
        for (grid.row = 0; grid.row < grid.rows; grid.row++) {
            X = 0;
            u32 tile_height = grid.row == grid.bottom_row ? grid.bottom_row_tile_height : image.tile_height;
            for (grid.column = 0; grid.column < grid.columns; grid.column++) {
                u32 tile_width = grid.column == grid.right_column ? grid.right_column_tile_stride : image.tile_width;

                for (u32 y = 0; y < tile_height; y++)
                    for (u32 x = 0; x < tile_width; x++, byte_color++)
                        window::content[window::width * (Y + y) + X + x] = byte_color->value;

                X += image.tile_width;
            }
            Y += image.tile_height;
        }
    } else {
        i32 remainder_x = (i32)image.stride - width;
        for (i32 y = bounds.top; y <= bounds.bottom; y++) {
            for (i32 x = bounds.left; x <= bounds.right; x++, byte_color++)
                window::content[window::width * y + x] = byte_color->value;

            byte_color += remainder_x;
        }
    }
}