#pragma once

#include "./vec3.h"

struct mat3 {
    vec3 X, Y, Z;

    static mat3 Identity;

    INLINE_XPU mat3() noexcept : X{1.0f, 0.0f, 0.0f},
                      Y{0.0f, 1.0f, 0.0f},
                      Z{0.0f, 0.0f, 1.0f} {}
    INLINE_XPU mat3(vec3 X, vec3 Y, vec3 Z) noexcept : X{X}, Y{Y}, Z{Z} {}
    INLINE_XPU mat3(f32 Xx, f32 Xy, f32 Xz,
         f32 Yx, f32 Yy, f32 Yz,
         f32 Zx, f32 Zy, f32 Zz) noexcept :
            X{Xx, Xy, Xz},
            Y{Yx, Yy, Yz},
            Z{Zx, Zy, Zz} {}
    INLINE_XPU mat3(const mat3 &other) noexcept : mat3{other.X, other.Y, other.Z} {}

    static INLINE_XPU mat3 RotationAroundX(f32 radians) {
        f32 c = cos(radians);
        f32 s = sin(radians);
        return {
                {1, 0, 0},
                {0, c, -s},
                {0, s, c}
        };
    }

    static INLINE_XPU mat3 RotationAroundY(f32 radians) {
        f32 c = cos(radians);
        f32 s = sin(radians);
        return {
                {c, 0, s},
                {0, 1, 0},
                {-s, 0, c}
        };
    }

    static INLINE_XPU mat3 RotationAroundZ(f32 radians) {
        f32 c = cos(radians);
        f32 s = sin(radians);
        return {
                {c, -s, 0},
                {s, c, 0},
                {0, 0, 1}
        };
    }

    INLINE_XPU void rotateAroundX(f32 radians) {
        f32 c = cos(radians);
        f32 s = sin(radians);
        mat3 lhs = *this;
        X.y = c*lhs.X.y + s*lhs.X.z; // Row 1 | Column 1
        Y.y = c*lhs.Y.y + s*lhs.Y.z; // Row 2 | Column 1
        Z.y = c*lhs.Z.y + s*lhs.Z.z; // Row 3 | Column 1

        X.z = c*lhs.X.z - s*lhs.X.y; // Row 1 | Column 2
        Y.z = c*lhs.Y.z - s*lhs.Y.y; // Row 2 | Column 2
        Z.z = c*lhs.Z.z - s*lhs.Z.y; // Row 3 | Column 2
    }

    INLINE_XPU void rotateAroundY(f32 radians) {
        f32 c = cos(radians);
        f32 s = sin(radians);
        mat3 lhs = *this;
        X.x = c*lhs.X.x - s*lhs.X.z; // Row 1 | Column 1
        Y.x = c*lhs.Y.x - s*lhs.Y.z; // Row 2 | Column 1
        Z.x = c*lhs.Z.x - s*lhs.Z.z; // Row 3 | Column 1

        X.z = c*lhs.X.z + s*lhs.X.x; // Row 1 | Column 2
        Y.z = c*lhs.Y.z + s*lhs.Y.x; // Row 2 | Column 2
        Z.z = c*lhs.Z.z + s*lhs.Z.x; // Row 3 | Column 2
    }

    INLINE_XPU void rotateAroundZ(f32 radians) {
        f32 c = cos(radians);
        f32 s = sin(radians);
        mat3 lhs = *this;
        X.x = c*lhs.X.x + s*lhs.X.y; // Row 1 | Column 1
        Y.x = c*lhs.Y.x + s*lhs.Y.y; // Row 2 | Column 1
        Z.x = c*lhs.Z.x + s*lhs.Z.y; // Row 3 | Column 1

        X.y = c*lhs.X.y - s*lhs.X.x; // Row 1 | Column 2
        Y.y = c*lhs.Y.y - s*lhs.Y.x; // Row 2 | Column 2
        Z.y = c*lhs.Z.y - s*lhs.Z.x; // Row 3 | Column 2
    }

    INLINE_XPU mat3 rotatedAroundXby(f32 radians) const {
        mat3 out{*this};
        out.rotateAroundX(radians);
        return out;
    }

    INLINE_XPU mat3 rotatedAroundYby(f32 radians) const {
        mat3 out{*this};
        out.rotateAroundY(radians);
        return out;
    }

    INLINE_XPU mat3 rotatedAroundZby(f32 radians) const {
        mat3 out{*this};
        out.rotateAroundZ(radians);
        return out;
    }

    INLINE_XPU f32 det() const {
        return (
                + X.x * (Y.y * Z.z - Z.y * Y.z)
                - Y.x * (X.y * Z.z - Z.y * X.z)
                + Z.x * (X.y * Y.z - Y.y * X.z)
        );
    }

    INLINE_XPU bool has_inverse() const {
        return det() != 0;
    }

    INLINE_XPU mat3 transposed() const {
        return {
                X.x, Y.x, Z.x,
                X.y, Y.y, Z.y,
                X.z, Y.z, Z.z
        };
    }

    INLINE_XPU mat3 inverted() const {
        return mat3{
                +(Y.y * Z.z - Z.y * Y.z),
                -(X.y * Z.z - Z.y * X.z),
                +(X.y * Y.z - Y.y * X.z),

                -(Y.x * Z.z - Z.x * Y.z),
                +(X.x * Z.z - Z.x * X.z),
                -(X.x * Y.z - Y.x * X.z),

                +(Y.x * Z.y - Z.x * Y.y),
                -(X.x * Z.y - Z.x * X.y),
                +(X.x * Y.y - Y.x * X.y)
        } / det();
    }

    INLINE_XPU mat3 operator ! () const {
        return inverted();
    }

    INLINE_XPU mat3 operator ~ () const {
        return transposed();
    }

    INLINE_XPU mat3 operator + (f32 rhs) const {
        return {
                X.x + rhs, X.y + rhs, X.z + rhs,
                Y.x + rhs, Y.y + rhs, Y.z + rhs,
                Z.x + rhs, Z.y + rhs, Z.z + rhs
        };
    }

    INLINE_XPU mat3 operator - (f32 rhs) const {
        return {
                X.x - rhs, X.y - rhs, X.z - rhs,
                Y.x - rhs, Y.y - rhs, Y.z - rhs,
                Z.x - rhs, Z.y - rhs, Z.z - rhs
        };
    }

    INLINE_XPU mat3 operator * (f32 rhs) const {
        return {
                X.x * rhs, X.y * rhs, X.z * rhs,
                Y.x * rhs, Y.y * rhs, Y.z * rhs,
                Z.x * rhs, Z.y * rhs, Z.z * rhs
        };
    }

    INLINE_XPU mat3 operator / (f32 rhs) const {
        f32 factor = 1.0f / rhs;
        return mat3{
                X.x, X.y, X.z,
                Y.x, Y.y, Y.z,
                Z.x, Z.y, Z.z
        } * factor;
    }

    INLINE_XPU mat3 operator + (const mat3 &rhs) const {
        return {
                X.x + rhs.X.x, X.y + rhs.X.y, X.z + rhs.X.z,
                Y.x + rhs.Y.x, Y.y + rhs.Y.y, Y.z + rhs.Y.z,
                Z.x + rhs.Z.x, Z.y + rhs.Z.y, Z.z + rhs.Z.z
        };
    }

    INLINE_XPU mat3 operator - (const mat3 &rhs) const {
        return {
                X.x - rhs.X.x, X.y - rhs.X.y, X.z - rhs.X.z,
                Y.x - rhs.Y.x, Y.y - rhs.Y.y, Y.z - rhs.Y.z,
                Z.x - rhs.Z.x, Z.y - rhs.Z.y, Z.z - rhs.Z.z
        };
    }

    INLINE_XPU mat3 operator * (const mat3 &rhs) const {
        return {
                X.x*rhs.X.x + X.y*rhs.Y.x + X.z*rhs.Z.x, // Row 1 | Column 1
                X.x*rhs.X.y + X.y*rhs.Y.y + X.z*rhs.Z.y, // Row 1 | Column 2
                X.x*rhs.X.z + X.y*rhs.Y.z + X.z*rhs.Z.z, // Row 1 | Column 3

                Y.x*rhs.X.x + Y.y*rhs.Y.x + Y.z*rhs.Z.x, // Row 2 | Column 1
                Y.x*rhs.X.y + Y.y*rhs.Y.y + Y.z*rhs.Z.y, // Row 2 | Column 2
                Y.x*rhs.X.z + Y.y*rhs.Y.z + Y.z*rhs.Z.z, // Row 2 | Column 3

                Z.x*rhs.X.x + Z.y*rhs.Y.x + Z.z*rhs.Z.x, // Row 3 | Column 1
                Z.x*rhs.X.y + Z.y*rhs.Y.y + Z.z*rhs.Z.y, // Row 3 | Column 2
                Z.x*rhs.X.z + Z.y*rhs.Y.z + Z.z*rhs.Z.z, // Row 3 | Column 3
        };
    }

    INLINE_XPU vec3 operator * (const vec3 &rhs) const {
        return {
                X.x*rhs.x + Y.x*rhs.y + Z.x*rhs.z,
                X.y*rhs.x + Y.y*rhs.y + Z.y*rhs.z,
                X.z*rhs.x + Y.z*rhs.y + Z.z*rhs.z
        };
    }

    INLINE_XPU void operator += (const mat3 &rhs) {
        X.x += rhs.X.x; Y.x += rhs.Y.x; Z.x += rhs.Z.x;
        X.y += rhs.X.y; Y.y += rhs.Y.y; Z.y += rhs.Z.y;
        X.z += rhs.X.z; Y.z += rhs.Y.z; Z.z += rhs.Z.z;
    }

    INLINE_XPU void operator -= (const mat3 &rhs) {
        X.x -= rhs.X.x; Y.x -= rhs.Y.x; Z.x -= rhs.Z.x;
        X.y -= rhs.X.y; Y.y -= rhs.Y.y; Z.y -= rhs.Z.y;
        X.z -= rhs.X.z; Y.z -= rhs.Y.z; Z.z -= rhs.Z.z;
    }

    INLINE_XPU void operator *= (const mat3 &rhs) {
        mat3 lhs = *this;
        X.x = lhs.X.x*rhs.X.x + lhs.X.y*rhs.Y.x + lhs.X.z*rhs.Z.x; // Row 1 | Column 1
        X.y = lhs.X.x*rhs.X.y + lhs.X.y*rhs.Y.y + lhs.X.z*rhs.Z.y; // Row 1 | Column 2
        X.z = lhs.X.x*rhs.X.z + lhs.X.y*rhs.Y.z + lhs.X.z*rhs.Z.z; // Row 1 | Column 3

        Y.x = lhs.Y.x*rhs.X.x + lhs.Y.y*rhs.Y.x + lhs.Y.z*rhs.Z.x; // Row 2 | Column 1
        Y.y = lhs.Y.x*rhs.X.y + lhs.Y.y*rhs.Y.y + lhs.Y.z*rhs.Z.y; // Row 2 | Column 2
        Y.z = lhs.Y.x*rhs.X.z + lhs.Y.y*rhs.Y.z + lhs.Y.z*rhs.Z.z; // Row 2 | Column 3

        Z.x = lhs.Z.x*rhs.X.x + lhs.Z.y*rhs.Y.x + lhs.Z.z*rhs.Z.x; // Row 3 | Column 1
        Z.y = lhs.Z.x*rhs.X.y + lhs.Z.y*rhs.Y.y + lhs.Z.z*rhs.Z.y; // Row 3 | Column 2
        Z.z = lhs.Z.x*rhs.X.z + lhs.Z.y*rhs.Y.z + lhs.Z.z*rhs.Z.z; // Row 3 | Column 3
    }

    INLINE_XPU void operator += (f32 rhs) {
        X.x += rhs; Y.x += rhs; Z.x += rhs;
        X.y += rhs; Y.y += rhs; Z.y += rhs;
        X.z += rhs; Y.z += rhs; Z.z += rhs;
    }

    INLINE_XPU void operator -= (f32 rhs) {
        X.x -= rhs; Y.x -= rhs; Z.x -= rhs;
        X.y -= rhs; Y.y -= rhs; Z.y -= rhs;
        X.z -= rhs; Y.z -= rhs; Z.z -= rhs;
    }

    INLINE_XPU void operator *= (f32 rhs) {
        X.x *= rhs; Y.x *= rhs; Z.x *= rhs;
        X.y *= rhs; Y.y *= rhs; Z.y *= rhs;
        X.z *= rhs; Y.z *= rhs; Z.z *= rhs;
    }

    INLINE_XPU void operator /= (f32 rhs) {
        f32 factor = 1.0f / rhs;
        X.x *= factor; Y.x *= factor; Z.x *= factor;
        X.y *= factor; Y.y *= factor; Z.y *= factor;
        X.z *= factor; Y.z *= factor; Z.z *= factor;
    }
};
mat3 mat3::Identity = {};

INLINE_XPU mat3 operator * (f32 lhs, const mat3 &rhs) {
    return rhs * lhs;
}

INLINE_XPU mat3 operator + (f32 lhs, const mat3 &rhs) {
    return rhs + lhs;
}

INLINE_XPU mat3 outerVec3(const vec3 &lhs, const vec3 &rhs) {
    return {
            lhs * rhs.x,
            lhs * rhs.y,
            lhs * rhs.z
    };
}

struct OrientationUsing3x3Matrix : Orientation<mat3> {
    vec3 &right{rotation.X};
    vec3 &up{rotation.Y};
    vec3 &forward{rotation.Z};

    INLINE_XPU OrientationUsing3x3Matrix(f32 x_radians = 0.0f, f32 y_radians = 0.0f, f32 z_radians = 0.0f) :
            Orientation<mat3>{x_radians, y_radians, z_radians} {}
};