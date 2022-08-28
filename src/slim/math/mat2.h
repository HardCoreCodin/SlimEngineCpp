#pragma once

#include "./vec2.h"

struct mat2 {
    vec2 X, Y;

    static mat2 Identity;

    INLINE_XPU mat2() : mat2{
            vec2{1.0f, 0.0f},
            vec2{0.0f, 1.0f}
    } {}
    INLINE_XPU mat2(vec2i x, vec2i y) noexcept : X{x}, Y{y} {}
    INLINE_XPU mat2(vec2i x, vec2 y) noexcept : X{x}, Y{y} {}
    INLINE_XPU mat2(vec2 x, vec2i y) noexcept : X{x}, Y{y} {}
    INLINE_XPU mat2(vec2 x, vec2 y) noexcept : X{x}, Y{y} {}
    INLINE_XPU mat2(f32 Xx, f32 Xy, f32 Yx, f32 Yy) noexcept : X{Xx, Xy}, Y{Yx, Yy} {}
    INLINE_XPU mat2(i32 Xx, i32 Xy, i32 Yx, i32 Yy) noexcept : X{(f32)Xx, (f32)Xy}, Y{(f32)Yx, (f32)Yy} {}
    INLINE_XPU mat2(const mat2 &other) noexcept : mat2{other.X, other.Y} {}

    INLINE_XPU f32 det() const {
        return X.x*Y.y - X.y*Y.x;
    }

    INLINE_XPU bool has_inverse() const {
        return det() != 0;
    }

    INLINE_XPU mat2 operator ! () const {
        return inverted();
    }

    INLINE_XPU mat2 operator ~ () const {
        return transposed();
    }

    INLINE_XPU mat2 operator + (f32 rhs) const {
        return {
                X.x + rhs, X.y + rhs,
                Y.x + rhs, Y.y + rhs
        };
    }

    INLINE_XPU mat2 operator - (f32 rhs) const {
        return {
                X.x - rhs, X.y - rhs,
                Y.x - rhs, Y.y - rhs
        };
    }

    INLINE_XPU mat2 operator * (f32 rhs) const {
        return {
                X.x * rhs, X.y * rhs,
                Y.x * rhs, Y.y * rhs
        };
    }

    INLINE_XPU mat2 operator / (f32 rhs) const {
        f32 factor = 1.0f / rhs;
        return {
                X.x * factor, X.y * factor,
                Y.x * factor, Y.y * factor
        };
    }

    INLINE_XPU mat2 operator + (const mat2 &rhs) const {
        return {
                X.x + rhs.X.x, X.y + rhs.X.y,
                Y.x + rhs.Y.x, Y.y + rhs.Y.y
        };
    }

    INLINE_XPU mat2 operator - (const mat2 &rhs) const {
        return {
                X.x - rhs.X.x, X.y - rhs.X.y,
                Y.x - rhs.Y.x, Y.y - rhs.Y.y
        };
    }

    INLINE_XPU mat2 operator * (const mat2 &rhs) const {
        return {
                X.x*rhs.X.x + X.y*rhs.Y.x, // Row 1 | Column 1
                X.x*rhs.X.y + X.y*rhs.Y.y, // Row 1 | Column 2
                Y.x*rhs.X.x + Y.y*rhs.Y.x, // Row 2 | Column 1
                Y.x*rhs.X.y + Y.y*rhs.Y.y  // Row 2 | Column 2
        };
    }

    INLINE_XPU vec2 operator * (const vec2 &rhs) const {
        return {
                X.x*rhs.x + Y.x*rhs.y,
                X.y*rhs.x + Y.y*rhs.y
        };
    }

    INLINE_XPU void operator += (const mat2 &rhs) {
        X.x += rhs.X.x;
        X.y += rhs.X.y;
        Y.x += rhs.Y.x;
        Y.y += rhs.Y.y;
    }

    INLINE_XPU void operator -= (const mat2 &rhs) {
        X.x -= rhs.X.x;
        X.y -= rhs.X.y;
        Y.x -= rhs.Y.x;
        Y.y -= rhs.Y.y;
    }

    INLINE_XPU void operator *= (const mat2 &rhs) {
        mat2 lhs{*this};
        X.x = lhs.X.x*rhs.X.x + lhs.X.y*rhs.Y.x; // Row 1 | Column 1
        X.y = lhs.X.x*rhs.X.y + lhs.X.y*rhs.Y.y; // Row 1 | Column 2
        Y.x = lhs.Y.x*rhs.X.x + lhs.Y.y*rhs.Y.x; // Row 2 | Column 1
        Y.y = lhs.Y.x*rhs.X.y + lhs.Y.y*rhs.Y.y; // Row 2 | Column 2
    }

    INLINE_XPU void operator += (f32 rhs) {
        X.x += rhs;
        X.y += rhs;
        Y.x += rhs;
        Y.y += rhs;
    }

    INLINE_XPU void operator -= (f32 rhs) {
        X.x -= rhs;
        X.y -= rhs;
        Y.x -= rhs;
        Y.y -= rhs;
    }

    INLINE_XPU void operator *= (f32 rhs) {
        X.x *= rhs;
        X.y *= rhs;
        Y.x *= rhs;
        Y.y *= rhs;
    }

    INLINE_XPU void operator /= (f32 rhs) {
        f32 factor = 1.0f / rhs;
        X.x *= factor;
        X.y *= factor;
        Y.x *= factor;
        Y.y *= factor;
    }

    INLINE_XPU void setRotation(f32 angle) {
        X.x = Y.y = cos(angle);
        Y.x = X.y = sin(angle);
        X.y = -X.y;
    }

    INLINE_XPU void rotate(f32 angle) {
        f32 c = cos(angle);
        f32 s = sin(angle);
        mat2 lhs{*this};
        X.x = c*lhs.X.x + s*lhs.X.y; // Row 1 | Column 1
        X.y = c*lhs.X.y - s*lhs.X.x; // Row 1 | Column 2
        Y.x = c*lhs.Y.x + s*lhs.Y.y; // Row 2 | Column 1
        Y.y = c*lhs.Y.y - s*lhs.Y.x; // Row 2 | Column 2
    }

    INLINE_XPU mat2 rotated_by(f32 angle) const {
        f32 c = cos(angle);
        f32 s = sin(angle);
        return {
                c*X.x + s*X.y, c*Y.x + s*Y.y,
                c*X.y - s*X.x, c*Y.y - s*Y.x
        };
    }

    INLINE_XPU mat2 transposed() const {
        return {
                X.x, Y.x,
                X.y, Y.y
        };
    }

    INLINE_XPU mat2 inverted() const {
        return mat2{
                Y.y, -X.y,
                -Y.x, X.x
        } / det();
    }
};
mat2 mat2::Identity = {};

INLINE_XPU mat2 operator * (f32 lhs, const mat2 &rhs) {
    return rhs * lhs;
}

INLINE_XPU mat2 operator + (f32 lhs, const mat2 &rhs) {
    return rhs + lhs;
}

INLINE_XPU mat2 outer(const vec2 &lhs, const vec2 &rhs) {
    return {
            lhs * rhs.x,
            lhs * rhs.y
    };
}