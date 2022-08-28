#pragma once

#include "./vec4.h"

struct mat4 {
    vec4 X, Y, Z, W;

    static mat4 Identity;

    INLINE_XPU mat4() noexcept :
            X{1, 0, 0, 0},
            Y{0, 1, 0, 0},
            Z{0, 0, 1, 0},
            W{0, 0, 0, 1} {}
    INLINE_XPU mat4(vec4 X, vec4 Y, vec4 Z, vec4 W) noexcept : X{X}, Y{Y}, Z{Z}, W{W} {}
    INLINE_XPU mat4(f32 Xx, f32 Xy, f32 Xz, f32 Xw,
         f32 Yx, f32 Yy, f32 Yz, f32 Yw,
         f32 Zx, f32 Zy, f32 Zz, f32 Zw,
         f32 Wx, f32 Wy, f32 Wz, f32 Ww) noexcept :
            X{Xx, Xy, Xz, Xw},
            Y{Yx, Yy, Yz, Yw},
            Z{Zx, Zy, Zz, Zw},
            W{Wx, Wy, Wz, Ww} {}
    INLINE_XPU mat4(const mat4 &other) noexcept : mat4{other.X, other.Y, other.Z, other.W} {}

    INLINE_XPU void setRotationAroundX(f32 angle) {
        Z.z = Y.y = cos(angle);
        Y.z = Z.y = sin(angle);
        Y.z = -Y.z;
        X.z = X.y = Y.x = Z.x = 0;
        X.x = 1;
        W = vec4{0, 0, 0, 1};
        X.w = Y.w = Z.w = 0;
    };

    INLINE_XPU void setRotationAroundY(f32 angle) {
        X.x = Z.z = cos(angle);
        Z.x = X.z = sin(angle);
        Z.x = -Z.x;
        Y.x = Y.z = X.y = Z.y = 0;
        Y.y = 1;
        W = vec4{0, 0, 0, 1};
        X.w = Y.w = Z.w = 0;
    }

    INLINE_XPU void setRotationAroundZ(f32 angle) {
        X.x = Y.y = cos(angle);
        Y.x = X.y = sin(angle);
        X.y = -X.y;
        X.z = Y.z = Z.x = Z.y = 0;
        Z.z = 1;
        W = vec4{0, 0, 0, 1};
        X.w = Y.w = Z.w = 0;
    }

    INLINE_XPU void rotateAroundX(f32 angle) {
        f32 c = cos(angle);
        f32 s = sin(angle);
        mat4 lhs = *this;
        X.y = c*lhs.X.y + s*lhs.X.z; // Row 1 | Column 1
        Y.y = c*lhs.Y.y + s*lhs.Y.z; // Row 2 | Column 1
        Z.y = c*lhs.Z.y + s*lhs.Z.z; // Row 3 | Column 1

        X.z = c*lhs.X.z - s*lhs.X.y; // Row 1 | Column 2
        Y.z = c*lhs.Y.z - s*lhs.Y.y; // Row 2 | Column 2
        Z.z = c*lhs.Z.z - s*lhs.Z.y; // Row 3 | Column 2
    }

    INLINE_XPU void rotateAroundY(f32 angle) {
        f32 c = cos(angle);
        f32 s = sin(angle);
        mat4 lhs{*this};

        X.x = c*lhs.X.x - s*lhs.X.z; // Row 1 | Column 1
        Y.x = c*lhs.Y.x - s*lhs.Y.z; // Row 2 | Column 1
        Z.x = c*lhs.Z.x - s*lhs.Z.z; // Row 3 | Column 1

        X.z = c*lhs.X.z + s*lhs.X.x; // Row 1 | Column 2
        Y.z = c*lhs.Y.z + s*lhs.Y.x; // Row 2 | Column 2
        Z.z = c*lhs.Z.z + s*lhs.Z.x; // Row 3 | Column 2
    }

    INLINE_XPU void rotateAroundZ(f32 angle) {
        f32 c = cos(angle);
        f32 s = sin(angle);
        mat4 lhs{*this};

        X.x = c*lhs.X.x + s*lhs.X.y; // Row 1 | Column 1
        Y.x = c*lhs.Y.x + s*lhs.Y.y; // Row 2 | Column 1
        Z.x = c*lhs.Z.x + s*lhs.Z.y; // Row 3 | Column 1

        X.y = c*lhs.X.y - s*lhs.X.x; // Row 1 | Column 2
        Y.y = c*lhs.Y.y - s*lhs.Y.x; // Row 2 | Column 2
        Z.y = c*lhs.Z.y - s*lhs.Z.x; // Row 3 | Column 2
    }

    INLINE_XPU mat4 rotatedAroundXby(f32 angle) const {
        mat4 out{*this};
        out.rotateAroundX(angle);
        return out;
    }

    INLINE_XPU mat4 rotatedAroundYby(f32 angle) const {
        mat4 out{*this};
        out.rotateAroundY(angle);
        return out;
    }

    INLINE_XPU mat4 rotatedAroundZby(f32 angle) const {
        mat4 out{*this};
        out.rotateAroundZ(angle);
        return out;
    }

    INLINE_XPU f32 det() const {
        return (
                X.x * (+Y.y*Z.z*W.w - Y.y*Z.w*W.z - Z.y*Y.z*W.w + Z.y*Y.w*W.z + W.y*Y.z*Z.w - W.y*Y.w*Z.z)
                + X.y * (-Y.x*Z.z*W.w + Y.x*Z.w*W.z + Z.x*Y.z*W.w - Z.x*Y.w*W.z - W.x*Y.z*Z.w + W.x*Y.w*Z.z)
                + X.z * (+Y.x*Z.y*W.w - Y.x*Z.w*W.y - Z.x*Y.y*W.w + Z.x*Y.w*W.y + W.x*Y.y*Z.w - W.x*Y.w*Z.y)
                + X.w * (-Y.x*Z.y*W.z + Y.x*Z.z*W.y + Z.x*Y.y*W.z - Z.x*Y.z*W.y - W.x*Y.y*Z.z + W.x*Y.z*Z.y)
        );
    }

    INLINE_XPU bool has_inverse() const {
        return det() != 0;
    }

    INLINE_XPU mat4 transposed() const {
        return {
                X.x, Y.x, Z.x, W.x,
                X.y, Y.y, Z.y, W.y,
                X.z, Y.z, Z.z, W.z,
                X.w, Y.w, Z.w, W.w
        };
    }

    INLINE_XPU mat4 inverted() const {
        mat4 out;
        out.X.x = +Y.y*Z.z*W.w - Y.y*Z.w*W.z - Z.y*Y.z*W.w + Z.y*Y.w*W.z + W.y*Y.z*Z.w - W.y*Y.w*Z.z;
        out.X.y = -X.y*Z.z*W.w + X.y*Z.w*W.z + Z.y*X.z*W.w - Z.y*X.w*W.z - W.y*X.z*Z.w + W.y*X.w*Z.z;
        out.X.z = +X.y*Y.z*W.w - X.y*Y.w*W.z - Y.y*X.z*W.w + Y.y*X.w*W.z + W.y*X.z*Y.w - W.y*X.w*Y.z;
        out.X.w = -X.y*Y.z*Z.w + X.y*Y.w*Z.z + Y.y*X.z*Z.w - Y.y*X.w*Z.z - Z.y*X.z*Y.w + Z.y*X.w*Y.z;

        out.Y.x = -Y.x*Z.z*W.w + Y.x*Z.w*W.z + Z.x*Y.z*W.w - Z.x*Y.w*W.z - W.x*Y.z*Z.w + W.x*Y.w*Z.z;
        out.Y.y = +X.x*Z.z*W.w - X.x*Z.w*W.z - Z.x*X.z*W.w + Z.x*X.w*W.z + W.x*X.z*Z.w - W.x*X.w*Z.z;
        out.Y.z = -X.x*Y.z*W.w + X.x*Y.w*W.z + Y.x*X.z*W.w - Y.x*X.w*W.z - W.x*X.z*Y.w + W.x*X.w*Y.z;
        out.Y.w = +X.x*Y.z*Z.w - X.x*Y.w*Z.z - Y.x*X.z*Z.w + Y.x*X.w*Z.z + Z.x*X.z*Y.w - Z.x*X.w*Y.z;

        out.Z.x = +Y.x*Z.y*W.w - Y.x*Z.w*W.y - Z.x*Y.y*W.w + Z.x*Y.w*W.y + W.x*Y.y*Z.w - W.x*Y.w*Z.y;
        out.Z.y = -X.x*Z.y*W.w + X.x*Z.w*W.y + Z.x*X.y*W.w - Z.x*X.w*W.y - W.x*X.y*Z.w + W.x*X.w*Z.y;
        out.Z.z = +X.x*Y.y*W.w - X.x*Y.w*W.y - Y.x*X.y*W.w + Y.x*X.w*W.y + W.x*X.y*Y.w - W.x*X.w*Y.y;
        out.Z.w = -X.x*Y.y*Z.w + X.x*Y.w*Z.y + Y.x*X.y*Z.w - Y.x*X.w*Z.y - Z.x*X.y*Y.w + Z.x*X.w*Y.y;

        out.W.x = -Y.x*Z.y*W.z + Y.x*Z.z*W.y + Z.x*Y.y*W.z - Z.x*Y.z*W.y - W.x*Y.y*Z.z + W.x*Y.z*Z.y;
        out.W.y = +X.x*Z.y*W.z - X.x*Z.z*W.y - Z.x*X.y*W.z + Z.x*X.z*W.y + W.x*X.y*Z.z - W.x*X.z*Z.y;
        out.W.z = -X.x*Y.y*W.z + X.x*Y.z*W.y + Y.x*X.y*W.z - Y.x*X.z*W.y - W.x*X.y*Y.z + W.x*X.z*Y.y;
        out.W.w = +X.x*Y.y*Z.z - X.x*Y.z*Z.y - Y.x*X.y*Z.z + Y.x*X.z*Z.y + Z.x*X.y*Y.z - Z.x*X.z*Y.y;

        f32 determinant = X.x*out.X.x + X.y*out.Y.x + X.z*out.Z.x + X.w*out.W.x;
        return determinant ? out / determinant : mat4{};
    }

    INLINE_XPU mat4 operator ! () const {
        return inverted();
    }

    INLINE_XPU mat4 operator ~ () const {
        return transposed();
    }

    INLINE_XPU mat4 operator + (f32 rhs) const {
        return {
                X.x + rhs, X.y + rhs, X.z + rhs, X.w + rhs,
                Y.x + rhs, Y.y + rhs, Y.z + rhs, Y.w + rhs,
                Z.x + rhs, Z.y + rhs, Z.z + rhs, Z.w + rhs,
                W.x + rhs, W.y + rhs, W.z + rhs, W.w + rhs
        };
    }

    INLINE_XPU mat4 operator - (f32 rhs) const {
        return {
                X.x - rhs, X.y - rhs, X.z - rhs, X.w - rhs,
                Y.x - rhs, Y.y - rhs, Y.z - rhs, Y.w - rhs,
                Z.x - rhs, Z.y - rhs, Z.z - rhs, Z.w - rhs,
                W.x - rhs, W.y - rhs, W.z - rhs, W.w - rhs
        };
    }

    INLINE_XPU mat4 operator * (f32 rhs) const {
        return {
                X.x * rhs, X.y * rhs, X.z * rhs, X.w * rhs,
                Y.x * rhs, Y.y * rhs, Y.z * rhs, Y.w * rhs,
                Z.x * rhs, Z.y * rhs, Z.z * rhs, Z.w * rhs,
                W.x * rhs, W.y * rhs, W.z * rhs, W.w * rhs
        };
    }

    INLINE_XPU mat4 operator / (f32 rhs) const {
        f32 factor = 1.0f / rhs;
        return {
                X.x * factor, X.y * factor, X.z * factor, X.w * factor,
                Y.x * factor, Y.y * factor, Y.z * factor, Y.w * factor,
                Z.x * factor, Z.y * factor, Z.z * factor, Z.w * factor,
                W.x * factor, W.y * factor, W.z * factor, W.w * factor
        };
    }

    INLINE_XPU mat4 operator + (const mat4 &rhs) const {
        return {
                X.x + rhs.X.x, X.y + rhs.X.y, X.z + rhs.X.z, X.w + rhs.X.w,
                Y.x + rhs.Y.x, Y.y + rhs.Y.y, Y.z + rhs.Y.z, Y.w + rhs.Y.w,
                Z.x + rhs.Z.x, Z.y + rhs.Z.y, Z.z + rhs.Z.z, Z.w + rhs.Z.w,
                W.x + rhs.W.x, W.y + rhs.W.y, W.z + rhs.W.z, W.w + rhs.W.w
        };
    }
    INLINE_XPU mat4 operator - (const mat4 &rhs) const {
        return {
                X.x - rhs.X.x, X.y - rhs.X.y, X.z - rhs.X.z, X.w - rhs.X.w,
                Y.x - rhs.Y.x, Y.y - rhs.Y.y, Y.z - rhs.Y.z, Y.w - rhs.Y.w,
                Z.x - rhs.Z.x, Z.y - rhs.Z.y, Z.z - rhs.Z.z, Z.w - rhs.Z.w,
                W.x - rhs.W.x, W.y - rhs.W.y, W.z - rhs.W.z, W.w - rhs.W.w
        };
    }

    INLINE_XPU mat4 operator * (const mat4 &rhs) const {
        return {
                X.x*rhs.X.x + X.y*rhs.Y.x + X.z*rhs.Z.x + X.w*rhs.W.x, // Row 1 | Column 1
                X.x*rhs.X.y + X.y*rhs.Y.y + X.z*rhs.Z.y + X.w*rhs.W.y, // Row 1 | Column 2
                X.x*rhs.X.z + X.y*rhs.Y.z + X.z*rhs.Z.z + X.w*rhs.W.z, // Row 1 | Column 3
                X.x*rhs.X.w + X.y*rhs.Y.w + X.z*rhs.Z.w + X.w*rhs.W.w, // Row 1 | Column 4

                Y.x*rhs.X.x + Y.y*rhs.Y.x + Y.z*rhs.Z.x + Y.w*rhs.W.x, // Row 2 | Column 1
                Y.x*rhs.X.y + Y.y*rhs.Y.y + Y.z*rhs.Z.y + Y.w*rhs.W.y, // Row 2 | Column 2
                Y.x*rhs.X.z + Y.y*rhs.Y.z + Y.z*rhs.Z.z + Y.w*rhs.W.z, // Row 2 | Column 3
                Y.x*rhs.X.w + Y.y*rhs.Y.w + Y.z*rhs.Z.w + Y.w*rhs.W.w, // Row 2 | Column 4

                Z.x*rhs.X.x + Z.y*rhs.Y.x + Z.z*rhs.Z.x + Z.w*rhs.W.x, // Row 3 | Column 1
                Z.x*rhs.X.y + Z.y*rhs.Y.y + Z.z*rhs.Z.y + Z.w*rhs.W.y, // Row 3 | Column 2
                Z.x*rhs.X.z + Z.y*rhs.Y.z + Z.z*rhs.Z.z + Z.w*rhs.W.z, // Row 3 | Column 3
                Z.x*rhs.X.w + Z.y*rhs.Y.w + Z.z*rhs.Z.w + Z.w*rhs.W.w, // Row 3 | Column 4

                W.x*rhs.X.x + W.y*rhs.Y.x + W.z*rhs.Z.x + W.w*rhs.W.x, // Row 3 | Column 1
                W.x*rhs.X.y + W.y*rhs.Y.y + W.z*rhs.Z.y + W.w*rhs.W.y, // Row 3 | Column 2
                W.x*rhs.X.z + W.y*rhs.Y.z + W.z*rhs.Z.z + W.w*rhs.W.z, // Row 3 | Column 3
                W.x*rhs.X.w + W.y*rhs.Y.w + W.z*rhs.Z.w + W.w*rhs.W.w  // Row 3 | Column 4
        };
    }

    INLINE_XPU vec4 operator * (const vec4 &rhs) const {
        return {
                X.x*rhs.x + Y.x*rhs.y + Z.x*rhs.z + W.x*rhs.w,
                X.y*rhs.x + Y.y*rhs.y + Z.y*rhs.z + W.y*rhs.w,
                X.z*rhs.x + Y.z*rhs.y + Z.z*rhs.z + W.z*rhs.w,
                X.w*rhs.x + Y.w*rhs.y + Z.w*rhs.z + W.w*rhs.w
        };
    }

    INLINE_XPU void operator += (const mat4 &rhs) {
        X.x += rhs.X.x; Y.x += rhs.Y.x; Z.x += rhs.Z.x; W.x += rhs.W.x;
        X.y += rhs.X.y; Y.y += rhs.Y.y; Z.y += rhs.Z.y; W.y += rhs.W.y;
        X.z += rhs.X.z; Y.z += rhs.Y.z; Z.z += rhs.Z.z; W.z += rhs.W.z;
        X.w += rhs.X.w; Y.w += rhs.Y.w; Z.w += rhs.Z.w; W.w += rhs.W.w;
    }

    INLINE_XPU void operator -= (const mat4 &rhs) {
        X.x -= rhs.X.x; Y.x -= rhs.Y.x; Z.x -= rhs.Z.x;
        X.y -= rhs.X.y; Y.y -= rhs.Y.y; Z.y -= rhs.Z.y;
        X.z -= rhs.X.z; Y.z -= rhs.Y.z; Z.z -= rhs.Z.z;
    }

    INLINE_XPU void operator *= (const mat4 &rhs) {
        mat4 lhs{*this};
        X.x = lhs.X.x*rhs.X.x + lhs.X.y*rhs.Y.x + lhs.X.z*rhs.Z.x + lhs.X.w*rhs.W.x; // Row 1 | Column 1
        X.y = lhs.X.x*rhs.X.y + lhs.X.y*rhs.Y.y + lhs.X.z*rhs.Z.y + lhs.X.w*rhs.W.y; // Row 1 | Column 2
        X.z = lhs.X.x*rhs.X.z + lhs.X.y*rhs.Y.z + lhs.X.z*rhs.Z.z + lhs.X.w*rhs.W.z; // Row 1 | Column 3
        X.w = lhs.X.x*rhs.X.w + lhs.X.y*rhs.Y.w + lhs.X.z*rhs.Z.w + lhs.X.w*rhs.W.w; // Row 1 | Column 4

        Y.x = lhs.Y.x*rhs.X.x + lhs.Y.y*rhs.Y.x + lhs.Y.z*rhs.Z.x + lhs.Y.w*rhs.W.x; // Row 2 | Column 1
        Y.y = lhs.Y.x*rhs.X.y + lhs.Y.y*rhs.Y.y + lhs.Y.z*rhs.Z.y + lhs.Y.w*rhs.W.y; // Row 2 | Column 2
        Y.z = lhs.Y.x*rhs.X.z + lhs.Y.y*rhs.Y.z + lhs.Y.z*rhs.Z.z + lhs.Y.w*rhs.W.z; // Row 2 | Column 3
        Y.w = lhs.Y.x*rhs.X.w + lhs.Y.y*rhs.Y.w + lhs.Y.z*rhs.Z.w + lhs.Y.w*rhs.W.w; // Row 2 | Column 4

        Z.x = lhs.Z.x*rhs.X.x + lhs.Z.y*rhs.Y.x + lhs.Z.z*rhs.Z.x + lhs.Z.w*rhs.W.x; // Row 3 | Column 1
        Z.y = lhs.Z.x*rhs.X.y + lhs.Z.y*rhs.Y.y + lhs.Z.z*rhs.Z.y + lhs.Z.w*rhs.W.y; // Row 3 | Column 2
        Z.z = lhs.Z.x*rhs.X.z + lhs.Z.y*rhs.Y.z + lhs.Z.z*rhs.Z.z + lhs.Z.w*rhs.W.z; // Row 3 | Column 3
        Z.w = lhs.Z.x*rhs.X.w + lhs.Z.y*rhs.Y.w + lhs.Z.z*rhs.Z.w + lhs.Z.w*rhs.W.w; // Row 3 | Column 4

        W.x = lhs.W.x*rhs.X.x + lhs.W.y*rhs.Y.x + lhs.W.z*rhs.Z.x + lhs.W.w*rhs.W.x; // Row 4 | Column 1
        W.y = lhs.W.x*rhs.X.y + lhs.W.y*rhs.Y.y + lhs.W.z*rhs.Z.y + lhs.W.w*rhs.W.y; // Row 4 | Column 2
        W.z = lhs.W.x*rhs.X.z + lhs.W.y*rhs.Y.z + lhs.W.z*rhs.Z.z + lhs.W.w*rhs.W.z; // Row 4 | Column 3
        W.w = lhs.W.x*rhs.X.w + lhs.W.y*rhs.Y.w + lhs.W.z*rhs.Z.w + lhs.W.w*rhs.W.w; // Row 4 | Column 4
    }

    INLINE_XPU void operator += (f32 rhs) {
        X.x += rhs; Y.x += rhs; Z.x += rhs; W.x += rhs;
        X.y += rhs; Y.y += rhs; Z.y += rhs; W.y += rhs;
        X.z += rhs; Y.z += rhs; Z.z += rhs; W.z += rhs;
        X.w += rhs; Y.w += rhs; Z.w += rhs; W.w += rhs;
    }

    INLINE_XPU void operator -= (f32 rhs) {
        X.x -= rhs; Y.x -= rhs; Z.x -= rhs; W.x -= rhs;
        X.y -= rhs; Y.y -= rhs; Z.y -= rhs; W.y -= rhs;
        X.z -= rhs; Y.z -= rhs; Z.z -= rhs; W.z -= rhs;
        X.w -= rhs; Y.w -= rhs; Z.w -= rhs; W.w -= rhs;
    }

    INLINE_XPU void operator *= (f32 rhs) {
        X.x *= rhs; Y.x *= rhs; Z.x *= rhs; W.x *= rhs;
        X.y *= rhs; Y.y *= rhs; Z.y *= rhs; W.y *= rhs;
        X.z *= rhs; Y.z *= rhs; Z.z *= rhs; W.z *= rhs;
        X.w *= rhs; Y.w *= rhs; Z.w *= rhs; W.w *= rhs;
    }

    INLINE_XPU void operator /= (f32 rhs) {
        f32 factor = 1.0f / rhs;
        X.x *= factor; Y.x *= factor; Z.x *= factor; W.x *= factor;
        X.y *= factor; Y.y *= factor; Z.y *= factor; W.y *= factor;
        X.z *= factor; Y.z *= factor; Z.z *= factor; W.z *= factor;
        X.w *= factor; Y.w *= factor; Z.w *= factor; W.w *= factor;
    }
};
mat4 mat4::Identity = {};

INLINE_XPU mat4 operator * (f32 lhs, const mat4 &rhs) {
    return rhs * lhs;
}

INLINE_XPU mat4 operator + (f32 lhs, const mat4 &rhs) {
    return rhs + lhs;
}