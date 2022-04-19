#pragma once

#include "../math/vec3.h"

struct Pixel {
    f64 depth;
    f32 opacity;
    vec3 color;

    Pixel() : Pixel{vec3{0.0f}} {}
    explicit Pixel(const vec3 &color, f32 opacity = 1.0f, f64 depth = 0.0) : depth(depth), opacity(opacity), color{color} {}
    explicit Pixel(enum ColorID color_id, f32 opacity = 1.0f, f64 depth = 0.0) : Pixel{Color(color_id), opacity, depth} {}
};

union PixelQuad {
    Pixel quad[2][2];
    struct {
        Pixel TL, TR, BL, BR;
    };

    PixelQuad() noexcept : PixelQuad{
            Pixel{vec3{0}},
            Pixel{vec3{0}},
            Pixel{vec3{0}},
            Pixel{vec3{0}}
    } {}

    PixelQuad(const Pixel &top_left, const Pixel &top_right, const Pixel &bottom_left, const Pixel &bottom_right) :
            TL{top_left},
            TR{top_right},
            BL{bottom_left},
            BR{bottom_right}
            {}
};

#define PIXEL_QUAD_SIZE (sizeof(PixelQuad))
#define CANVAS_SIZE (MAX_WINDOW_SIZE * PIXEL_QUAD_SIZE)

struct Canvas {
    Dimensions dimensions;
    PixelQuad *pixels{nullptr};
    Pixel background{Black, 0, INFINITY};
    bool antialias{true};

    explicit Canvas(PixelQuad *pixels) noexcept : pixels{pixels} {}

    void clear() const {
        fill(background.color,
             background.opacity,
             background.depth);
    }
    void fill(const Pixel &pixel) const {
        fill(pixel.color, pixel.opacity, pixel.depth);
    }
    void fill(const vec3 &color, f32 opacity, f64 depth) const {
        PixelQuad fill_pixel;
        Pixel fill_sub_pixel;
        fill_sub_pixel.color = color;
        fill_sub_pixel.opacity = opacity;
        fill_sub_pixel.depth = depth;
        fill_pixel.TL = fill_pixel.TR = fill_pixel.BL = fill_pixel.BR = fill_sub_pixel;
        for (i32 y = 0; y < dimensions.height; y++)
            for (i32 x = 0; x < dimensions.width; x++)
                pixels[dimensions.stride * y + x] = fill_pixel;
    }
    INLINE void setPixel(i32 x, i32 y, const Pixel &pixel) const {
        setPixel(x, y, pixel.color, pixel.opacity, pixel.depth);
    }
    INLINE void setPixel(i32 x, i32 y, const vec3 &color, f32 opacity, f64 depth) const {
        Pixel *pixel;
        PixelQuad *pixel_quad;
        if (antialias) {
            pixel_quad = pixels + (dimensions.stride * (y >> 1)) + (x >> 1);
            pixel = &pixel_quad->quad[y & 1][x & 1];
        } else {
            pixel_quad = pixels + (dimensions.stride * y) + x;
            pixel = &pixel_quad->TL;
        }

        Pixel new_pixel;
        new_pixel.opacity = opacity;
        new_pixel.color = color;
        new_pixel.depth = depth;

        if (!(opacity == 1 && depth == 0)) {
            Pixel background_pixel, foreground_pixel, old_pixel = *pixel;

            if (old_pixel.depth < new_pixel.depth) {
                background_pixel = new_pixel;
                foreground_pixel = old_pixel;
            } else {
                background_pixel = old_pixel;
                foreground_pixel = new_pixel;
            }
            if (foreground_pixel.opacity != 1) {
                f32 one_minus_foreground_opacity = 1.0f - foreground_pixel.opacity;
                opacity = foreground_pixel.opacity + background_pixel.opacity * one_minus_foreground_opacity;
                f32 one_over_opacity = opacity == 0 ? 1.0f : 1.0f / opacity;
                f32 background_factor = background_pixel.opacity * one_over_opacity * one_minus_foreground_opacity;
                f32 foreground_factor = foreground_pixel.opacity * one_over_opacity;

                pixel->color.r = fast_mul_add(foreground_pixel.color.r, foreground_factor, background_pixel.color.r * background_factor);
                pixel->color.g = fast_mul_add(foreground_pixel.color.g, foreground_factor, background_pixel.color.g * background_factor);
                pixel->color.b = fast_mul_add(foreground_pixel.color.b, foreground_factor, background_pixel.color.b * background_factor);
                pixel->opacity = opacity;
                pixel->depth   = foreground_pixel.depth;
            } else *pixel = foreground_pixel;
        } else *pixel = new_pixel;

        if (!antialias) pixel_quad->BR = pixel_quad->BL = pixel_quad->TR = pixel_quad->TL;
    }
    INLINE PixelQuad* row(u32 y) const {
        return pixels + y * (u32)dimensions.width;
    }
    INLINE PixelQuad* operator[](u32 y) const {
        return row(y);
    }
};