#pragma once

#include "./line.h"

INLINE void _drawTriangle(f32 x1, f32 y1, f32 z1,
                          f32 x2, f32 y2, f32 z2,
                          f32 x3, f32 y3, f32 z3,
                         const Canvas &canvas, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) {
    drawLine(x1, y1, z1, x2, y2, z3, canvas, color, opacity, line_width, viewport_bounds);
    drawLine(x2, y2, z2, x3, y3, z3, canvas, color, opacity, line_width, viewport_bounds);
    drawLine(x3, y3, z3, x1, y1, z3, canvas, color, opacity, line_width, viewport_bounds);
}


void _fillTriangle(f32 x1, f32 y1, f32 z1,
                   f32 x2, f32 y2, f32 z2,
                   f32 x3, f32 y3, f32 z3,
                   const Canvas &canvas, const Color &color, f32 opacity, const RectI *viewport_bounds) {
    // Cull this triangle against the edges of the viewport:
    Rect bounds{0, canvas.dimensions.f_width - 1.0f, 0, canvas.dimensions.f_height - 1.0f};
    Rect rect{
        x1 < x2 ? x1 : x2,
        x1 > x2 ? x1 : x2,
        y1 < y2 ? y1 : y2,
        y1 > y2 ? y1 : y2,
    };
    if (x3 < rect.left) rect.left = x3;
    if (x3 > rect.right) rect.right = x3;
    if (y3 < rect.top) rect.top = y3;
    if (y3 > rect.bottom) rect.bottom = y3;
    if (viewport_bounds) {
        Rect float_bounds{
            (f32)viewport_bounds->left,
            (f32)viewport_bounds->right,
            (f32)viewport_bounds->top,
            (f32)viewport_bounds->bottom,
        };
        x1 += float_bounds.left;
        x2 += float_bounds.left;
        x3 += float_bounds.left;
        y1 += float_bounds.top;
        y2 += float_bounds.top;
        y3 += float_bounds.top;
        rect.x_range += float_bounds.left;
        rect.y_range += float_bounds.top;
        rect -= float_bounds;
        bounds -= float_bounds;
    }
    rect -= bounds;
    if (!rect)
        return;

    if (canvas.antialias == SSAA) {
        x1 *= 2.0f;
        x2 *= 2.0f;
        x3 *= 2.0f;
        y1 *= 2.0f;
        y2 *= 2.0f;
        y3 *= 2.0f;
        rect *= 2.0f;;
    }

    // Compute area components:
    f32 ABx = x2 - x1;
    f32 ABy = y2 - y1;

    f32 ACx = x3 - x1;
    f32 ACy = y3 - y1;

    f32 ABC = ACx*ABy - ACy*ABx;

    // Cull faces facing backwards:
    if (ABC < 0) {
        f32 tmp = x3;
        x3 = x2;
        x2 = tmp;

        tmp = y3;
        y3 = y2;
        y2 = tmp;

        ABx = x2 - x1;
        ABy = y2 - y1;

        ACx = x3 - x1;
        ACy = y3 - y1;
        ABC = ACx*ABy - ACy*ABx;
    } else if (ABC == 0)
        return;

    // Floor bounds coordinates down to their integral component:
    u32 first_x = (u32)rect.left;
    u32 first_y = (u32)rect.top;
    u32 last_x  = (u32)rect.right;
    u32 last_y  = (u32)rect.bottom;

    rect.left   = (f32)first_x;
    rect.top    = (f32)first_y;
    rect.right  = (f32)last_x;
    rect.bottom = (f32)last_y;

    // Drawing: Top-down
    // Origin: Top-left

    // Compute weight constants:
    f32 one_over_ABC = 1.0f / ABC;

    f32 Cdx =  ABy * one_over_ABC;
    f32 Bdx = -ACy * one_over_ABC;

    f32 Cdy = -ABx * one_over_ABC;
    f32 Bdy =  ACx * one_over_ABC;

    // Compute initial areal coordinates for the first pixel center:
    rect.left += 0.5f;
    rect.top += 0.5f;
    f32 C_start = Cdx*rect.left + Cdy*rect.top + (y1*x2 - x1*y2) * one_over_ABC;
    f32 B_start = Bdx*rect.left + Bdy*rect.top + (y3*x1 - x3*y1) * one_over_ABC;

    f32 A, B, C;

    // Scan the bounds:
    bool depth_provided = z1 != 0 || z2 != 0 || z3 != 0;
    f32 depth = 0;
    for (u32 y = first_y; y <= last_y; y++, C_start += Cdy, B_start += Bdy) {
        B = B_start;
        C = C_start;

        for (u32 x = first_x; x <= last_x; x++, B += Bdx, C += Cdx) {
            if (((Bdx < 0) && (B < 0)) ||
                ((Cdx < 0) && (C < 0)))
                break;

            A = 1 - B - C;

            // Skip the pixel if it's outside:
            if (A < 0 || B < 0 || C < 0)
                continue;

            if (depth_provided)
                depth = A*z1 + B*z2 + C*z3;

            canvas.setPixel(x, y, color, opacity, depth);
        }
    }
}


INLINE void Canvas::drawTriangle(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawTriangle(x1, y1, 0, x2, y2, 0, x3, y3, 0, *this, color, opacity, line_width, viewport_bounds);
}

INLINE void Canvas::drawTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawTriangle((f32)x1, (f32)y1, 0, (f32)x2, (f32)y2, 0, (f32)x3, (f32)y3, 0, *this, color, opacity, line_width, viewport_bounds);
}

INLINE void Canvas::drawTriangle(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawTriangle(x1, y1, z1, x2, y2, z2, x3, y3, z3, *this, color, opacity, line_width, viewport_bounds);
}

INLINE void Canvas::fillTriangle(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _fillTriangle(x1, y1, z1, x2, y2, z2, x3, y3, z3, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::fillTriangle(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _fillTriangle(x1, y1, 0, x2, y2, 0, x3, y3, 0, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::fillTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _fillTriangle((f32)x1, (f32)y1, 0, (f32)x2, (f32)y2, 0, (f32)x3, (f32)y3, 0, *this, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
INLINE void Canvas::drawTriangle(vec2 p1, vec2 p2, vec2 p3, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawTriangle(p1.x, p1.y,0,  p2.x, p2.y, 0, p3.x, p3.y, 0, *this, color, opacity, line_width, viewport_bounds);
}

INLINE void Canvas::fillTriangle(vec2 p1, vec2 p2, vec2 p3, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _fillTriangle(p1.x, p1.y,0,  p2.x, p2.y, 0, p3.x, p3.y, 0, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawTriangle(vec2i p1, vec2i p2, vec2i p3, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawTriangle((f32)p1.x, (f32)p1.y, 0, (f32)p2.x, (f32)p2.y, 0, (f32)p3.x, (f32)p3.y, 0, *this, color, opacity, line_width, viewport_bounds);
}

INLINE void Canvas::fillTriangle(vec2i p1, vec2i p2, vec2i p3, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _fillTriangle((f32)p1.x, (f32)p1.y, 0, (f32)p2.x, (f32)p2.y, 0, (f32)p3.x, (f32)p3.y, 0, *this, color, opacity, viewport_bounds);
}
#endif

#ifdef SLIM_VEC3
INLINE void Canvas::drawTriangle(vec3 p1, vec3 p2, vec3 p3, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawTriangle(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z, *this, color, opacity, line_width, viewport_bounds);
}

INLINE void Canvas::fillTriangle(vec3 p1, vec3 p2, vec3 p3, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _fillTriangle(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z, *this, color, opacity, viewport_bounds);
}
#endif


INLINE void drawTriangle(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, const Canvas &canvas,
                         Color color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) {
    _drawTriangle(x1, y1, 0, x2, y2, 0, x3, y3, 0, canvas, color, opacity, line_width, viewport_bounds);
}

INLINE void drawTriangle(i32 x1, i32 y1,
                         i32 x2, i32 y2,
                         i32 x3, i32 y3,
                         const Canvas &canvas,
                         Color color = White, f32 opacity = 0.5f, u8 line_width = 0,
                         const RectI *viewport_bounds = nullptr) {
    _drawTriangle((f32)x1, (f32)y1, 0, (f32)x2, (f32)y2, 0, (f32)x3, (f32)y3, 0, canvas, color, opacity, line_width, viewport_bounds);
}


INLINE void fillTriangle(f32 x1, f32 y1,
                         f32 x2, f32 y2,
                         f32 x3, f32 y3,
                         const Canvas &canvas, Color color = White, f32 opacity = 1.0f,
                         const RectI *viewport_bounds = nullptr) {
    _fillTriangle(x1, y1, 0, x2, y2, 0, x3, y3, 0, canvas, color, opacity, viewport_bounds);
}

INLINE void fillTriangle(i32 x1, i32 y1,
                         i32 x2, i32 y2,
                         i32 x3, i32 y3,
                         const Canvas &canvas, Color color = White, f32 opacity = 1.0f,
                         const RectI *viewport_bounds = nullptr) {
    _fillTriangle((f32)x1, (f32)y1, 0, (f32)x2, (f32)y2, 0, (f32)x3, (f32)y3, 0, canvas, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
void drawTriangle(vec2 p1, vec2 p2, vec2 p3, const Canvas &canvas,
                  Color color = White, f32 opacity = 0.5f, u8 line_width = 0, const RectI *viewport_bounds = nullptr) {
    _drawTriangle(p1.x, p1.y, 0, p2.x, p2.y, 0, p3.x, p3.y, 0, canvas, color, opacity, line_width, viewport_bounds);
}

void fillTriangle(vec2 p1, vec2 p2, vec2 p3, const Canvas &canvas,
                  Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _fillTriangle(p1.x, p1.y, 0, p2.x, p2.y, 0, p3.x, p3.y, 0, canvas, color, opacity, viewport_bounds);
}

void drawTriangle(vec2i p1, vec2i p2, vec2i p3, const Canvas &canvas,
                  Color color = White, f32 opacity = 0.5f, u8 line_width = 0,
                  const RectI *viewport_bounds = nullptr) {
    _drawTriangle((f32)p1.x, (f32)p1.y, 0, (f32)p2.x, (f32)p2.y, 0, (f32)p3.x, (f32)p3.y, 0, canvas, color, opacity, line_width, viewport_bounds);
}

void fillTriangle(vec2i p1, vec2i p2, vec2i p3,
                  const Canvas &canvas,
                  Color color = White, f32 opacity = 1.0f,
                  const RectI *viewport_bounds = nullptr) {
    _fillTriangle((f32)p1.x, (f32)p1.y, 0, (f32)p2.x, (f32)p2.y, 0, (f32)p3.x, (f32)p3.y, 0, canvas, color, opacity, viewport_bounds);
}
#endif

#ifdef SLIM_VEC3
void drawTriangle(vec3 p1, vec3 p2, vec3 p3, const Canvas &canvas,
                  Color color = White, f32 opacity = 0.5f, u8 line_width = 0, const RectI *viewport_bounds = nullptr) {
    _drawTriangle(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z, canvas, color, opacity, line_width, viewport_bounds);
}

void fillTriangle(vec3 p1, vec3 p2, vec3 p3, const Canvas &canvas,
                  Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _fillTriangle(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z, canvas, color, opacity, viewport_bounds);
}
#endif