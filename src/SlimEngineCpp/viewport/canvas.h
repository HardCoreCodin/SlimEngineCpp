#pragma once

#include "../math/vec3.h"

struct Canvas {
    Dimensions dimensions;
    Pixel *pixels{nullptr};
    f32 *depths{nullptr};

    bool SSAA{true}, MSAA{false};

    Canvas(Pixel *pixels, f32 *depths) noexcept : pixels{pixels}, depths{depths} {}

    void clear(f32 red = 0, f32 green = 0, f32 blue = 0, f32 opacity = 0, f32 depth = INFINITY) const {
        i32 pixels_width  = dimensions.width;
        i32 pixels_height = dimensions.height;
        i32 depths_width  = dimensions.width;
        i32 depths_height = dimensions.height;

        if (SSAA || MSAA) {
            depths_width *= 2;
            depths_height *= 2;
            if (SSAA) {
                pixels_width *= 2;
                pixels_height *= 2;
            }
        }

        i32 pixels_count = pixels_width * pixels_height;
        i32 depths_count = depths_width * depths_height;

        Pixel pixel{red, green, blue, opacity};

        for (i32 i = 0; i < pixels_count; i++) pixels[i] = pixel;
        for (i32 i = 0; i < depths_count; i++) depths[i] = depth;
    }

    INLINE void setPixel(i32 x, i32 y, const Color &color, f32 opacity = 1.0f, f32 depth = 0, f32 z_top = 0, f32 z_bottom = 0, f32 z_right = 0) {
        u32 offset = SSAA ? ((dimensions.stride * (y >> 1) + (x >> 1)) * 4 + (2 * (y & 1)) + (x & 1)) : (dimensions.stride * y + x);
        Pixel pixel{color, opacity};
        Pixel *out_pixel = pixels + offset;
        f32 *out_depth = depths + (MSAA ? offset * 4 : offset);
        if (opacity == 1.0f && depth == 0.0f && z_top == 0.0f && z_bottom == 0.0f && z_right == 0.0f) {
            *out_pixel = pixel;
            out_depth[0] = 0;
            if (MSAA) out_depth[1] = out_depth[2] = out_depth[3] = 0;

            return;
        }

        Pixel *bg, *fg;
        if (MSAA) {
            Pixel accumulated_pixel{};
            for (u8 i = 0; i < 4; i++, out_depth++) {
                if (i) depth = i == 1 ? z_top : (i == 2 ? z_bottom : z_right);
                _sortPixelsByDepth(depth, &pixel, out_depth, out_pixel, &bg, &fg);
                accumulated_pixel += fg->opacity == 1 ? *fg : fg->alphaBlendOver(*bg);
            }
            *out_pixel = accumulated_pixel * 0.25f;
        } else {
            _sortPixelsByDepth(depth, &pixel, out_depth, out_pixel, &bg, &fg);
            *out_pixel = fg->opacity == 1 ? *fg : fg->alphaBlendOver(*bg);
        }
    }

    void renderToContent(u32 *content) const {
        u32 *content_value = content;
        Pixel *pixel = pixels;
        if (SSAA)
            for (u16 y = 0; y < dimensions.height; y++)
                for (u16 x = 0; x < dimensions.width; x++, content_value++, pixel += 4)
                    *content_value = _isTransparentPixelQuad(pixel) ? 0 : _blendPixelQuad(pixel).asContent();
        else
            for (u16 y = 0; y < dimensions.height; y++)
                for (u16 x = 0; x < dimensions.width; x++, content_value++, pixel++)
                    *content_value = pixel->opacity ? pixel->asContent() : 0;
    }

private:
    static INLINE bool _isTransparentPixelQuad(Pixel *pixel_quad) {
        return pixel_quad->opacity == 0.0f && pixel_quad[1].opacity == 0.0f  && pixel_quad[2].opacity == 0.0f  && pixel_quad[3].opacity == 0.0f;
    }

    static INLINE Pixel _blendPixelQuad(Pixel *pixel_quad) {
        Pixel TL{pixel_quad[0]}, TR{pixel_quad[1]}, BL{pixel_quad[2]}, BR{pixel_quad[3]};
        TL.opacity *= 0.25f;
        TR.opacity *= 0.25f;
        BL.opacity *= 0.25f;
        BR.opacity *= 0.25f;
        return {
            {
                (TL.color.r * TL.opacity) + (TR.color.r * TR.opacity) + (BL.color.r * BL.opacity) + (BR.color.r * BR.opacity),
                (TL.color.g * TL.opacity) + (TR.color.g * TR.opacity) + (BL.color.g * BL.opacity) + (BR.color.g * BR.opacity),
                (TL.color.r * TL.opacity) + (TR.color.b * TR.opacity) + (BL.color.b * BL.opacity) + (BR.color.b * BR.opacity)
            },
            TL.opacity + TR.opacity + BL.opacity + BR.opacity
        };
    }

    static INLINE void _sortPixelsByDepth(f32 depth, Pixel *pixel, f32 *out_depth, Pixel *out_pixel, Pixel **background, Pixel **foreground) {
        if (depth == 0.0f || depth < *out_depth) {
            *out_depth = depth;
            *background = out_pixel;
            *foreground = pixel;
        } else {
            *background = pixel;
            *foreground = out_pixel;
        }
    }
};