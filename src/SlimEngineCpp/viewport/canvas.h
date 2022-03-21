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

    PixelQuad(const Pixel &top_left, const Pixel & top_right, const Pixel & bottom_left, const Pixel & bottom_right) :
            TL{top_left},
            TR{top_right},
            BL{bottom_left},
            BR{bottom_right}
            {}
};

struct Canvas {
    vec2i &offset;
    Dimensions &dimensions;
    PixelQuad *pixels{nullptr};
    Pixel background{Black, 0, INFINITY};
    bool antialias{false};

    Canvas(Dimensions &dimensions, vec2i &offset) : offset{offset}, dimensions{dimensions}, pixels{(PixelQuad*)memory::allocate(FRAME_BUFFER_MEMORY_SIZE)} {}

    INLINE PixelQuad* row(u32 y) const { return pixels + y * (u32)dimensions.width; }
    INLINE PixelQuad* operator[](u32 y) const { return row(y); }

    INLINE void setPixel(i32 x, i32 y, const Pixel &pixel) const { setPixel(x, y, pixel.color, pixel.opacity, pixel.depth); }
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

    void fill(const Pixel &pixel) const { fill(pixel.color, pixel.opacity, pixel.depth); }
    void fill(const vec3 &color, f32 opacity, f64 depth) const {
        PixelQuad fill_pixel;
        Pixel fill_sub_pixel;
        fill_sub_pixel.color = color;
        fill_sub_pixel.opacity = opacity;
        fill_sub_pixel.depth = depth;
        fill_pixel.TL = fill_pixel.TR = fill_pixel.BL = fill_pixel.BR = fill_sub_pixel;
        for (i32 y = offset.y; y < (offset.y + dimensions.height); y++)
            for (i32 x = offset.x; x < (offset.x + dimensions.width); x++)
                pixels[dimensions.stride * y + x] = fill_pixel;
    }

    void clear() const {
        fill(background.color,
             background.opacity,
             background.depth);
    }

    void drawHLine(i32 x_start, i32 x_end, i32 y, const vec3 &color, f32 opacity = 1.0f) const {
        x_start += offset.x;
        x_end   += offset.x;
        y       += offset.y;

        if (!inRange(y, dimensions.height + offset.y, offset.y)) return;

        i32 first, last, step = 1;
        subRange(x_start, x_end, dimensions.width + offset.x, offset.x, &first, &last);

        Pixel pixel{color * color, opacity, 0};
        if (antialias) {
            y <<= 1;
            first <<= 1;
            last  <<= 1;
            step = 2;
            for (i32 x = first; x <= last; x += step) {
                setPixel(x+0, y+0, pixel);
                setPixel(x+1, y+0, pixel);
                setPixel(x+0, y+1, pixel);
                setPixel(x+1, y+1, pixel);
            }
        } else
            for (i32 x = first; x <= last; x += step)
                setPixel(x, y, pixel);
    }

    void drawVLine(i32 y_start, i32 y_end, i32 x, const vec3 &color, f32 opacity = 1.0f) const {
        y_start += offset.y;
        y_end   += offset.y;
        x       += offset.x;

        if (!inRange(x, dimensions.width + offset.x, offset.x)) return;
        i32 first, last, step = 1;

        subRange(y_start, y_end, dimensions.height + offset.y, offset.y, &first, &last);
        Pixel pixel{color * color, opacity, 0};
        if (antialias) {
            x <<= 1;
            first <<= 1;
            last  <<= 1;
            step = 2;
            for (i32 y = first; y <= last; y += step) {
                setPixel(x+0, y+0, pixel);
                setPixel(x+1, y+0, pixel);
                setPixel(x+0, y+1, pixel);
                setPixel(x+1, y+1, pixel);
            }
        } else
            for (i32 y = first; y <= last; y += step)
                setPixel(x, y, pixel);
    }

    void drawLine(f32 x1, f32 y1, f64 z1,
                  f32 x2, f32 y2, f64 z2,
                  vec3 color, f32 opacity, u8 line_width) const {
        if (x1 < 0 &&
            y1 < 0 &&
            x2 < 0 &&
            y2 < 0)
            return;

        i32 x_left = offset.x;
        i32 y_top  = offset.y;
        x1 += (f32)x_left;
        x2 += (f32)x_left;
        y1 += (f32)y_top;
        y2 += (f32)y_top;

        i32 w = dimensions.width + x_left;
        i32 h = dimensions.height + y_top;

        color *= color;
        i32 x, y;
        if (antialias) {
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
                if (inRange(y, h, y_top)) setPixel(x, y, color, oneMinusFractionOf(first.y) * gap * opacity, z1);

                for (u8 i = 0; i < line_width; i++) {
                    y++;
                    if (inRange(y, h, y_top)) setPixel(x, y, color, opacity, z1);
                }

                y++;
                if (inRange(y, h, y_top)) setPixel(x, y, color, fractionOf(first.y) * gap * opacity, z1);
            }

            x = end.x;
            y = end.y;
            gap = fractionOf(x2 + 0.5f);

            if (inRange(x, w, x_left)) {
                if (inRange(y, h, y_top)) setPixel(x, y, color, oneMinusFractionOf(last.y) * gap * opacity, z2);

                for (u8 i = 0; i < line_width; i++) {
                    y++;
                    if (inRange(y, h, y_top)) setPixel(x, y, color, opacity, z2);
                }

                y++;
                if (inRange(y, h, y_top)) setPixel(x, y, color, fractionOf(last.y) * gap * opacity, z2);
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
                    if (inRange(y, h, y_top)) setPixel(x, y, color, oneMinusFractionOf(gap) * opacity, z);

                    for (u8 i = 0; i < line_width; i++) {
                        y++;
                        if (inRange(y, h, y_top)) setPixel(x, y, color, opacity, z);
                    }

                    y++;
                    if (inRange(y, h, y_top)) setPixel(x, y, color, fractionOf(gap) * opacity, z);
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
                if (inRange(x, w, x_left)) setPixel(x, y, color, oneMinusFractionOf(first.x) * gap * opacity, z1);

                for (u8 i = 0; i < line_width; i++) {
                    x++;
                    if (inRange(x, w, x_left)) setPixel(x, y, color, opacity, z1);
                }

                x++;
                if (inRange(x, w, x_left)) setPixel(x, y, color, fractionOf(first.x) * gap * opacity, z1);
            }

            x = end.x;
            y = end.y;
            gap = fractionOf(y2 + 0.5f);

            if (inRange(y, h, y_top)) {
                if (inRange(x, w, x_left)) setPixel(x, y, color, oneMinusFractionOf(last.x) * gap * opacity, z2);

                for (u8 i = 0; i < line_width; i++) {
                    x++;
                    if (inRange(x, w, x_left)) setPixel(x, y, color, opacity, z2);
                }

                x++;
                if (inRange(x, w, x_left)) setPixel(x, y, color, fractionOf(last.x) * gap * opacity, z2);
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

                    if (inRange(x, w, x_left)) setPixel(x, y, color, oneMinusFractionOf(gap) * opacity, z);

                    for (u8 i = 0; i < line_width; i++) {
                        x++;
                        if (inRange(x, w, x_left)) setPixel(x, y, color, opacity, z);
                    }

                    x++;
                    if (inRange(x, w, x_left)) setPixel(x, y, color, fractionOf(gap) * opacity, z);
                }

                gap += grad;
                if (has_depth) z_curr += z_step;
            }
        }
    }

    void drawText(char *str, i32 x, i32 y, const vec3 &color, f32 opacity) const {
        if (x < 0 || x > dimensions.width  - FONT_WIDTH ||
            y < 0 || y > dimensions.height - FONT_HEIGHT)
            return;

        Pixel pixel{color, opacity, 0};
        pixel.color.r *= pixel.color.r;
        pixel.color.g *= pixel.color.g;
        pixel.color.b *= pixel.color.b;

        u16 current_x = (u16)x;
        u16 current_y = (u16)y;
        u16 pixel_x, sub_pixel_x;
        u16 pixel_y, sub_pixel_y;
        u16 t_offset;
        u8* byte;
        char character = *str;
        while (character) {
            if (character == '\n') {
                if (current_y + FONT_HEIGHT > dimensions.height)
                    break;

                current_x = (u16)x;
                current_y += LINE_HEIGHT;
            } else if (character == '\t') {
                t_offset = FONT_WIDTH * (4 - ((current_x / FONT_WIDTH) & 3));
                current_x += t_offset;
            } else if (character >= FIRST_CHARACTER_CODE &&
                       character <= LAST_CHARACTER_CODE) {
                byte = char_addr[character - FIRST_CHARACTER_CODE];
                for (int i = 1; i < 4; i++) {
                    pixel_x = current_x;
                    pixel_y = current_y + i * FONT_HEIGHT / 3;
                    for (int w = 0; w < FONT_WIDTH ; w++) {
                        for (int h = 0; h < FONT_HEIGHT/3; h++) {
                            /* skip background bits */
                            if (*byte & (0x80  >> h)) {
                                if (antialias) {
                                    sub_pixel_x = pixel_x << 1;
                                    sub_pixel_y = pixel_y << 1;
                                    setPixel(sub_pixel_x + 0, sub_pixel_y + 0, pixel);
                                    setPixel(sub_pixel_x + 1, sub_pixel_y + 0, pixel);
                                    setPixel(sub_pixel_x + 0, sub_pixel_y + 1, pixel);
                                    setPixel(sub_pixel_x + 1, sub_pixel_y + 1, pixel);
                                } else
                                    setPixel(pixel_x, pixel_y, pixel);
                            }

                            pixel_y--;
                        }
                        byte++;
                        pixel_y += FONT_HEIGHT / 3;
                        pixel_x++;
                    }
                }
                current_x += FONT_WIDTH;
                if (current_x + FONT_WIDTH > dimensions.width)
                    return;
            }
            character = *++str;
        }
    }

    void drawNumber(i32 number, i32 x, i32 y, const vec3 &color, f32 opacity) const {
        static NumberString number_string;
        number_string = number;
        drawText(number_string.string.char_ptr, x - (i32)number_string.string.length * FONT_WIDTH, y, color, opacity);
    }
};




//    void fillTriangle(RGBA color, vec2 v1, vec2 v2, vec2 v3) const {
//        // Cull this triangle against the edges of the viewport:
//        vec2 pixel_min = min(v1, min(v2, v3));
//        if (pixel_min.x >= dimensions.f_width ||
//            pixel_min.y >= dimensions.f_height)
//            return;
//
//        vec2 pixel_max = max(v1, max(v2, v3));
//        if (pixel_max.x < 0 ||
//            pixel_max.y < 0)
//            return;
//
//        // Clip the bounds of the triangle to the viewport:
//        pixel_min = pixel_min.clamped();
//        pixel_max = pixel_max.clamped(vec2(dimensions.f_width - 1, dimensions.f_height - 1));
//
//        // Compute area components:
//        f32 ABy = v2.y - v1.y;
//        f32 ABx = v2.x - v1.x;
//
//        f32 ACy = v3.y - v1.y;
//        f32 ACx = v3.x - v1.x;
//
//        f32 ABC = ACx*ABy - ACy*ABx;
//
//        // Cull faces facing backwards:
//        if (ABC < 0) {
//            vec2 tmp = v3;
//            v3 = v2;
//            v2 = tmp;
//
//            ABy = v2.y - v1.y;
//            ABx = v2.x - v1.x;
//
//            ACy = v3.y - v1.y;
//            ACx = v3.x - v1.x;
//            ABC = ACx*ABy - ACy*ABx;
//        } else if (ABC == 0)
//            return;
//
//        // Floor bounds coordinates down to their integral component:
//        u32 first_x = (u32)pixel_min.x;
//        u32 first_y = (u32)pixel_min.y;
//        u32 last_x  = (u32)pixel_max.x;
//        u32 last_y  = (u32)pixel_max.y;
//
//        pixel_min.x = (f32)first_x;
//        pixel_min.y = (f32)first_y;
//        pixel_max.x = (f32)last_x;
//        pixel_max.y = (f32)last_y;
//
//        // Drawing: Top-down
//        // Origin: Top-left
//
//        // Compute weight constants:
//        f32 one_over_ABC = 1.0f / ABC;
//
//        f32 Cdx =  ABy * one_over_ABC;
//        f32 Bdx = -ACy * one_over_ABC;
//
//        f32 Cdy = -ABx * one_over_ABC;
//        f32 Bdy =  ACx * one_over_ABC;
//
//        // Compute initial areal coordinates for the first pixel center:
//        pixel_min = vec2{pixel_min.x + 0.5f, pixel_min.y + 0.5f};
//        f32 C_start = Cdx*pixel_min.x + Cdy*pixel_min.y + (v1.y*v2.x - v1.x*v2.y) * one_over_ABC;
//        f32 B_start = Bdx*pixel_min.x + Bdy*pixel_min.y + (v3.y*v1.x - v3.x*v1.y) * one_over_ABC;
//
//        f32 A, B, C;
//
//        // Scan the bounds:
//        for (u32 y = first_y; y <= last_y; y++, C_start += Cdy, B_start += Bdy) {
//            B = B_start;
//            C = C_start;
//
//            for (u32 x = first_x; x <= last_x; x++, B += Bdx, C += Cdx) {
//                if (Bdx < 0 && B < 0 ||
//                    Cdx < 0 && C < 0)
//                    break;
//
//                A = 1 - B - C;
//
//                // Skip the pixel if it's outside:
//                if (fminf(A, fminf(B, C)) < 0)
//                    continue;
//
//                pixels[dimensions.width * y + x].color = color;
//            }
//        }
//    }
//
//    void fillTriangle(RGBA color, f32 *X, f32 *Y) const {
//        vec2 v1{*X++, *Y++};
//        vec2 v2{*X++, *Y++};
//        vec2 v3{*X, *Y};
//        return fillTriangle(color, v1, v2, v3);
//    }
//
//    void drawCircle(RGBA color, i32 center_x, i32 center_y, i32 radius) const {
//        if (radius <= 1) {
//            if (inRange(0, center_x, dimensions.width - 1) &&
//                inRange(0, center_y, dimensions.height - 1))
//                pixels[dimensions.width * center_y + center_x].color = color;
//
//            return;
//        }
//
//        i32 width = (i32)dimensions.width;
//        i32 size  = (i32)dimensions.width_times_height;
//
//        i32 x = radius, y = 0, y2 = 0;
//        i32 r2 = radius * radius;
//        i32 x2 = r2;
//
//        i32 Sx1 = center_x - radius;
//        i32 Ex1 = center_x + radius;
//        i32 Sy1 = center_y * width;
//        i32 Ey1 = Sy1;
//
//        i32 Sx2 = center_x;
//        i32 Ex2 = center_x;
//        i32 Sy2 = (center_y - radius) * width;
//        i32 Ey2 = (center_y + radius) * width;
//
//        while (y <= x) {
//            if (Sy1 >= 0 && Sy1 < size) {
//                if (Sx1 >= 0 && Sx1 < width) pixels[Sy1 + Sx1].color = color;
//                if (Ex1 >= 0 && Ex1 < width) pixels[Sy1 + Ex1].color = color;
//            }
//            if (Ey1 >= 0 && Ey1 < size) {
//                if (Sx1 >= 0 && Sx1 < width) pixels[Ey1 + Sx1].color = color;
//                if (Ex1 >= 0 && Ex1 < width) pixels[Ey1 + Ex1].color = color;
//            }
//
//            if (Sy2 >= 0 && Sy2 < size) {
//                if (Sx2 >= 0 && Sx2 < width) pixels[Sy2 + Sx2].color = color;
//                if (Ex2 >= 0 && Ex2 < width) pixels[Sy2 + Ex2].color = color;
//            }
//            if (Ey2 >= 0 && Ey2 < size) {
//                if (Sx2 >= 0 && Sx2 < width) pixels[Ey2 + Sx2].color = color;
//                if (Ex2 >= 0 && Ex2 < width) pixels[Ey2 + Ex2].color = color;
//            }
//
//            if ((x2 + y2) > r2) {
//                x -= 1;
//                x2 = x * x;
//
//                Sx1 += 1;
//                Ex1 -= 1;
//
//                Sy2 += width;
//                Ey2 -= width;
//            }
//
//            y += 1;
//            y2 = y * y;
//
//            Sy1 -= width;
//            Ey1 += width;
//
//            Sx2 -= 1;
//            Ex2 += 1;
//        }
//    }
//    INLINE void drawCircle(RGBA color, vec2i center, i32 radius) const {
//        return drawCircle(color, center.x, center.y, radius);
//    }
//    void fillCircle(RGBA color, i32 center_x, i32 center_y, i32 radius) const {
//        if (radius <= 1) {
//            if (inRange(0, center_x, dimensions.width - 1) &&
//                inRange(0, center_y, dimensions.height - 1))
//                pixels[dimensions.width * center_y + center_x].color = color;
//
//            return;
//        }
//
//        i32 width = (i32)dimensions.width;
//        i32 size  = (i32)dimensions.width_times_height;
//
//        i32 x = radius, y = 0, y2 = 0;
//        i32 r2 = radius * radius;
//        i32 x2 = r2;
//
//        i32 Sx1 = center_x - radius;
//        i32 Ex1 = center_x + radius;
//        i32 Sy1 = center_y * width;
//        i32 Ey1 = Sy1;
//
//        i32 Sx2 = center_x;
//        i32 Ex2 = center_x;
//        i32 Sy2 = (center_y - radius) * width;
//        i32 Ey2 = (center_y + radius) * width;
//
//        i32 i, start, end;
//
//        while (y <= x) {
//            start = Sx1 > 0 ? Sx1 : 0;
//            end   = Ex1 < (width - 1) ? Ex1 : (width - 1);
//            if (Sy1 >= 0 && Sy1 < size) for (i = start; i <= end; i++) pixels[Sy1 + i].color = color;
//            if (Ey1 >= 0 && Ey1 < size) for (i = start; i <= end; i++) pixels[Ey1 + i].color = color;
//
//            start = Sx2 > 0 ? Sx2 : 0;
//            end   = Ex2 < (width - 1) ? Ex2 : (width - 1);
//            if (Sy2 >= 0 && Sy2 < size) for (i = start; i <= end; i++) pixels[Sy2 + i].color = color;
//            if (Ey2 >= 0 && Ey2 < size) for (i = start; i <= end; i++) pixels[Ey2 + i].color = color;
//
//            if ((x2 + y2) > r2) {
//                x -= 1;
//                x2 = x * x;
//
//                Sx1 += 1;
//                Ex1 -= 1;
//
//                Sy2 += width;
//                Ey2 -= width;
//            }
//
//            y += 1;
//            y2 = y * y;
//
//            Sy1 -= width;
//            Ey1 += width;
//
//            Sx2 -= 1;
//            Ex2 += 1;
//        }
//    }
//
//    INLINE void fillCircle(RGBA color, vec2i center, i32 radius) const {
//        return fillCircle(color, center.x, center.y, radius);
//    }