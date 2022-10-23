#pragma once

#include "../core/base.h"

enum AntiAliasing {
    NoAA,
    MSAA,
    SSAA
};

struct Canvas {
    Dimensions dimensions;
    Pixel *pixels{nullptr};
    f32 *depths{nullptr};

    AntiAliasing antialias;

    Canvas(u16 width = MAX_WIDTH, u16 height = MAX_HEIGHT, AntiAliasing antialiasing = NoAA) : antialias{antialiasing} {
        if (memory::canvas_memory_capacity) {
            pixels = (Pixel*)memory::canvas_memory;
            memory::canvas_memory += CANVAS_PIXELS_SIZE;
            memory::canvas_memory_capacity -= CANVAS_PIXELS_SIZE;

            depths = (f32*)memory::canvas_memory;
            memory::canvas_memory += CANVAS_DEPTHS_SIZE;
            memory::canvas_memory_capacity -= CANVAS_DEPTHS_SIZE;

            dimensions.update(MAX_WIDTH, MAX_HEIGHT);
            clear();
            dimensions.update(width, height);
        } else {
            pixels = nullptr;
            depths = nullptr;
        }
    }

    Canvas(Pixel *pixels, f32 *depths) noexcept : pixels{pixels}, depths{depths} {}

    void clear(f32 red = 0, f32 green = 0, f32 blue = 0, f32 opacity = 1.0f, f32 depth = INFINITY) const {
        i32 pixels_width  = dimensions.width;
        i32 pixels_height = dimensions.height;
        i32 depths_width  = dimensions.width;
        i32 depths_height = dimensions.height;

        if (antialias != NoAA) {
            depths_width *= 2;
            depths_height *= 2;
            if (antialias == SSAA) {
                pixels_width *= 2;
                pixels_height *= 2;
            }
        }

        i32 pixels_count = pixels_width * pixels_height;
        i32 depths_count = depths_width * depths_height;

        Pixel pixel{red, green, blue, opacity};

        if (pixels) for (i32 i = 0; i < pixels_count; i++) pixels[i] = pixel;
        if (depths) for (i32 i = 0; i < depths_count; i++) depths[i] = depth;
    }

    void drawFrom(Canvas& source_canvas, const RectI* source_bounds = nullptr, const RectI* target_bounds = nullptr, f32 opacity = 1.0f, bool blend = true, bool include_depths = false) {
        RectI src{
                0, source_canvas.dimensions.width,
                0, source_canvas.dimensions.height
        };
        if (source_bounds)
            src -= *source_bounds;

        RectI trg{
                0, dimensions.width,
                0, dimensions.height
        };
        if (target_bounds)
            trg = *target_bounds;

        if ((antialias == SSAA) && (source_canvas.antialias == SSAA)) {
            src *= 2;
            trg *= 2;
        }

        f32 depth;

        i32 src_y = src.top;
        for (i32 y = trg.top; y < trg.bottom; y++, src_y++) {
            if (y < 0 || y >= dimensions.height)
                continue;

            i32 src_x = src.left;

            for (i32 x = trg.left; x < trg.right; x++, src_x++) {
                if (x < 0 || x >= dimensions.width)
                    continue;

                i32 src_offset = source_canvas.antialias == SSAA ? (
                        (source_canvas.dimensions.stride * (src_y >> 1) + (src_x >> 1)) * 4 + (2 * (src_y & 1)) + (src_x & 1)
                ) : (
                                         source_canvas.dimensions.stride * src_y + src_x
                                 );

                Pixel& pixel{ source_canvas.pixels[src_offset] };
                if ((pixel.opacity == 0.0f) || (
                        (pixel.color.r == 0.0f) &&
                        (pixel.color.g == 0.0f) &&
                        (pixel.color.b == 0.0f)))
                    continue;

                if (include_depths)
                    depth = source_canvas.depths[src_offset];

                if (blend) {
                    setPixel(x, y, pixel.color / pixel.opacity, pixel.opacity * opacity, include_depths ? depth : 0.0f);
                }
                else {
                    i32 trg_offset = antialias == SSAA ? (
                            (dimensions.stride * (y >> 1) + (x >> 1)) * 4 + (2 * (y & 1)) + (x & 1)
                    ) : (
                                             dimensions.stride * y + x
                                     );
                    pixels[trg_offset] = source_canvas.pixels[src_offset];
                    if (include_depths && depth < depths[trg_offset])
                        depths[trg_offset] = depth;
                }
            }
        }
    }

    void drawToWindow() const {
        u32 *content_value = window::content;
        Pixel *pixel = pixels;
        for (u16 y = 0; y < window::height; y++)
            for (u16 x = 0; x < window::width; x++, content_value++) {
                *content_value = getPixelContent(pixel);

                if (antialias == SSAA)
                    pixel += 4;
                else
                    pixel++;
            }
    }

    INLINE void setPixel(i32 x, i32 y, const Color &color, f32 opacity = 1.0f, f32 depth = 0, f32 z_top = 0, f32 z_bottom = 0, f32 z_right = 0) const {
        int w = dimensions.width;
        int h = dimensions.height;
        if (antialias == SSAA) {
            w <<= 1;
            h <<= 1;
        }
        if (x < 0 || y < 0 || x >= w || y >= h)
            return;

        opacity = clampedValue(opacity);
        Pixel pixel{color.clamped(), opacity};
        pixel.color *= pixel.color;
        if (opacity != 1.0f)
            pixel.color *= pixel.opacity;

        u32 offset = antialias == SSAA ? ((dimensions.stride * (y >> 1) + (x >> 1)) * 4 + (2 * (y & 1)) + (x & 1)) : (dimensions.stride * y + x);
        Pixel *out_pixel = pixels + offset;
        f32 *out_depth = depths ? (depths + (antialias == MSAA ? offset * 4 : offset)) : nullptr;
        if (
                (
                        (out_depth == nullptr ||
                         *out_depth == INFINITY) &&
                        (out_pixel->color.r == 0) &&
                        (out_pixel->color.g == 0) &&
                        (out_pixel->color.b == 0)
                ) ||
                (
                        (opacity == 1.0f) &&
                        (depth == 0.0f) &&
                        (z_top == 0.0f) &&
                        (z_bottom == 0.0f) &&
                        (z_right == 0.0f)
                )
                ) {
            *out_pixel = pixel;
            if (depths) {
                out_depth[0] = depth;
                if (antialias == MSAA) out_depth[1] = out_depth[2] = out_depth[3] = 0;
            }

            return;
        }

        Pixel *bg{out_pixel}, *fg{&pixel};
        if (antialias == MSAA) {
            Pixel accumulated_pixel{};
            for (u8 i = 0; i < 4; i++) {
                if (depths) {
                    if (i) depth = i == 1 ? z_top : (i == 2 ? z_bottom : z_right);
                    _sortPixelsByDepth(depth, &pixel, out_depth, out_pixel, &bg, &fg);
                    out_depth++;
                }
                accumulated_pixel += fg->opacity == 1 ? *fg : fg->alphaBlendOver(*bg);
            }
            *out_pixel = accumulated_pixel * 0.25f;
        } else {
            if (depths)
                _sortPixelsByDepth(depth, &pixel, out_depth, out_pixel, &bg, &fg);
            *out_pixel = fg->opacity == 1 ? *fg : fg->alphaBlendOver(*bg);
        }
    }

    INLINE u32 getPixelContent(Pixel *pixel) const {
        return antialias == SSAA ? _isTransparentPixelQuad(pixel) ? 0 : _blendPixelQuad(pixel).asContent() :
               pixel->opacity == 0.0f ? 0 : pixel->asContent();
    }

    INLINE void drawText(char *str, i32 x, i32 y, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#ifdef SLIM_VEC2
    INLINE void drawText(char *str, vec2i position, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawText(char *str, vec2 position, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#endif

    INLINE void drawNumber(i32 number, i32 x, i32 y, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#ifdef SLIM_VEC2
    INLINE void drawNumber(i32 number, vec2i position, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawNumber(i32 number, vec2 position, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#endif

    INLINE void drawHLine(RangeI x_range, i32 y, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawHLine(i32 x_start, i32 x_end, i32 y, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawVLine(RangeI y_range, i32 x, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawVLine(i32 y_start, i32 y_end, i32 x, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawLine(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawLine(f32 x1, f32 y1, f32 x2, f32 y2, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) const;

#ifdef SLIM_VEC2
    INLINE void drawLine(vec2 from, vec2 to, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawLine(vec2i from, vec2i to, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) const;
#endif
#ifdef SLIM_VEC3
    INLINE void drawLine(vec3 from, vec3 to, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) const;
#endif

    INLINE void drawRect(RectI rect, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawRect(Rect rect, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillRect(RectI rect, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillRect(Rect rect, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;

    INLINE void drawTriangle(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, const Color &color = White, f32 opacity = 0.5f, u8 line_width = 0, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillTriangle(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;

    INLINE void drawTriangle(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillTriangle(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;

#ifdef SLIM_VEC2
    INLINE void drawTriangle(vec2 p1, vec2 p2, vec2 p3, const Color &color = White, f32 opacity = 0.5f, u8 line_width = 0, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillTriangle(vec2 p1, vec2 p2, vec2 p3, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawTriangle(vec2i p1, vec2i p2, vec2i p3, const Color &color = White, f32 opacity = 0.5f, u8 line_width = 0, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillTriangle(vec2i p1, vec2i p2, vec2i p3, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#endif

#ifdef SLIM_VEC3
    INLINE void drawTriangle(vec3 p1, vec3 p2, vec3 p3, const Color &color = White, f32 opacity = 0.5f, u8 line_width = 0, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillTriangle(vec3 p1, vec3 p2, vec3 p3, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#endif

    INLINE void fillCircle(i32 center_x, i32 center_y, i32 radius, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawCircle(i32 center_x, i32 center_y, i32 radius, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#ifdef SLIM_VEC2
    INLINE void drawCircle(vec2i center, i32 radius, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillCircle(vec2i center, i32 radius, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawCircle(vec2 center, i32 radius, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillCircle(vec2 center, i32 radius, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#endif

private:
    static INLINE bool _isTransparentPixelQuad(Pixel *pixel_quad) {
        return (
                (pixel_quad[0].opacity == 0.0f) &&
                (pixel_quad[1].opacity == 0.0f) &&
                (pixel_quad[2].opacity == 0.0f) &&
                (pixel_quad[3].opacity == 0.0f)
        );
    }

    INLINE Pixel _blendPixelQuad(Pixel *pixel_quad) const {
        return (pixel_quad[0] + pixel_quad[1] + pixel_quad[2] + pixel_quad[3]) * 0.25f;
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