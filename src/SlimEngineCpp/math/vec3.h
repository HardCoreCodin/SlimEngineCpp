#pragma once

#include "../core/base.h"

struct vec3 {
    union {
        struct {f32 components[3]; };
        struct {f32 x, y, z; };
        struct {f32 u, v, w; };
        struct {f32 r, g, b; };
    };

    static vec3 X, Y, Z;

    vec3() noexcept : vec3{0} {}
    vec3(f32 x, f32 y, f32 z) noexcept : x(x), y(y), z(z) {}
    vec3(vec3 &other) noexcept : vec3{other.x, other.y, other.z} {}
    vec3(const vec3 &other) noexcept : vec3{other.x, other.y, other.z} {}
    explicit vec3(enum ColorID color_id) noexcept : vec3{RGBA{color_id}} {}
    explicit vec3(f32 value) noexcept : vec3{value, value, value} {}
    explicit vec3(RGBA rgba) noexcept :
        r{(f32)rgba.R * COLOR_COMPONENT_TO_FLOAT},
        g{(f32)rgba.G * COLOR_COMPONENT_TO_FLOAT},
        b{(f32)rgba.B * COLOR_COMPONENT_TO_FLOAT} {}

    INLINE vec3& operator = (f32 value) {
        x = y = z = value;
        return *this;
    }

    INLINE vec3& operator = (i32 value) {
        x = y = z = (f32)value;
        return *this;
    }

    INLINE RGBA toRGBA(u8 alpha = MAX_COLOR_VALUE) const {
        return {
                (u8)(255.0f * fmaxf(0, fminf(r, 1.0f))),
                (u8)(255.0f * fmaxf(0, fminf(g, 1.0f))),
                (u8)(255.0f * fmaxf(0, fminf(b, 1.0f))),
                alpha
        };
    }

    INLINE bool operator == (const vec3 &other) const {
        return other.x == x &&
               other.y == y &&
               other.z == z;
    }

    INLINE bool operator ! () const {
        return nonZero();
    }

    INLINE f32 operator | (const vec3 &rhs) const {
        return dot(rhs);
    }

    INLINE vec3 operator ^ (const vec3 &rhs) const {
        return cross(rhs);
    }

    INLINE vec3 operator % (const vec3 &rhs) const {
        return reflectAround(rhs);
    }

    INLINE vec3 operator - () const {
        return {
            -x,
            -y,
            -z
        };
    }

    INLINE vec3 operator - (const vec3 &rhs) const {
        return {
            x - rhs.x,
            y - rhs.y,
            z - rhs.z
        };
    }

    INLINE vec3 operator + (const vec3 &rhs) const {
        return {
            x + rhs.x,
            y + rhs.y,
            z + rhs.z
        };
    }

    INLINE vec3 operator * (const vec3 &rhs) const {
        return {
            x * rhs.x,
            y * rhs.y,
            z * rhs.z
        };
    }

    INLINE vec3 operator / (const vec3 &rhs) const {
        return {
            x / rhs.x,
            y / rhs.y,
            z / rhs.z
        };
    }

    INLINE vec3& operator -= (const vec3 &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    INLINE vec3& operator += (const vec3 &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    INLINE vec3& operator *= (const vec3 &rhs) {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }

    INLINE vec3& operator /= (const vec3 &rhs) {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }

    INLINE vec3 operator - (f32 rhs) const {
        return {
            x - rhs,
            y - rhs,
            z - rhs
        };
    }

    INLINE vec3 operator + (f32 rhs) const {
        return {
            x + rhs,
            y + rhs,
            z + rhs
        };
    }

    INLINE vec3 operator * (f32 rhs) const {
        return {
            x * rhs,
            y * rhs,
            z * rhs
        };
    }

    INLINE vec3 operator / (f32 rhs) const {
        return {
            x / rhs,
            y / rhs,
            z / rhs
        };
    }

    INLINE vec3& operator -= (f32 rhs) {
        x -= rhs;
        y -= rhs;
        z -= rhs;
        return *this;
    }

    INLINE vec3& operator += (f32 rhs) {
        x += rhs;
        y += rhs;
        z += rhs;
        return *this;
    }

    INLINE vec3& operator *= (f32 rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    INLINE vec3& operator /= (f32 rhs) {
        f32 factor = 1.0f / rhs;
        x *= factor;
        y *= factor;
        z *= factor;
        return *this;
    }

    INLINE vec3 operator - (i32 rhs) const {
        return {
            x - (f32)rhs,
            y - (f32)rhs,
            z - (f32)rhs
        };
    }

    INLINE vec3 operator + (i32 rhs) const {
        return {
            x + (f32)rhs,
            y + (f32)rhs,
            z + (f32)rhs
        };
    }

    INLINE vec3 operator * (i32 rhs) const {
        return {
            x * (f32)rhs,
            y * (f32)rhs,
            z * (f32)rhs
        };
    }

    INLINE vec3 operator / (i32 rhs) const {
        f32 factor = 1.0f / (f32)rhs;
        return {
            x * factor,
            y * factor,
            z * factor
        };
    }

    INLINE vec3& operator -= (i32 rhs) {
        x -= (f32)rhs;
        y -= (f32)rhs;
        z -= (f32)rhs;
        return *this;
    }

    INLINE vec3& operator += (i32 rhs) {
        x += (f32)rhs;
        y += (f32)rhs;
        z += (f32)rhs;
        return *this;
    }

    INLINE vec3& operator *= (i32 rhs) {
        x *= (f32)rhs;
        y *= (f32)rhs;
        z *= (f32)rhs;
        return *this;
    }

    INLINE vec3& operator /= (i32 rhs) {
        f32 factor = 1.0f / (f32)rhs;
        x *= factor;
        y *= factor;
        z *= factor;
        return *this;
    }

    INLINE bool nonZero() const {
        return x != 0.0f ||
               y != 0.0f ||
               z != 0.0f;
    }

    INLINE vec3 perpZ() const {
        return {
            -y,
            x,
            z
        };
    }

    INLINE f32 min() const {
        return x < y ? (x < z ? x : z) : (y < z ? y : z);
    }

    INLINE f32 max() const {
        return x > y ? (x > z ? x : z) : (y > z ? y : z);
    }

    INLINE f32 dot(const vec3 &rhs) const {
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
    }

    INLINE vec3 cross(const vec3 &rhs) const {
        return {
                (y * rhs.z) - (z * rhs.y),
                (z * rhs.x) - (x * rhs.z),
                (x * rhs.y) - (y * rhs.x)
        };
    }

    INLINE f32 squaredLength() const {
        return x*x + y*y + z*z;
    }

    INLINE f32 length() const {
        return sqrtf(squaredLength());
    }

    INLINE vec3 normalized() const {
        return *this / length();
    }

    INLINE vec3 reflectAround(const vec3 &N) const {
        return N.scaleAdd(-2 * dot(N), *this);
    }

    INLINE vec3 clamped() const {
        return {
                clampedValue(x),
                clampedValue(y),
                clampedValue(z)
        };
    }

    INLINE vec3 clamped(const vec3 &upper) const {
        return {
                clampedValue(x, upper.x),
                clampedValue(y, upper.y),
                clampedValue(z, upper.z)
        };
    }

    INLINE vec3 clamped(const vec3 &lower, const vec3 &upper) const {
        return {
                clampedValue(x, lower.x, upper.x),
                clampedValue(y, lower.y, upper.y),
                clampedValue(z, lower.z, upper.z)
        };
    }

    INLINE vec3 clamped(const f32 min_value, const f32 max_value) const {
        return {
                clampedValue(x, min_value, max_value),
                clampedValue(y, min_value, max_value),
                clampedValue(z, min_value, max_value)
        };
    }

    INLINE vec3 approachTo(const vec3 &trg, f32 diff) const {
        return {
                approach(x, trg.x, diff),
                approach(y, trg.y, diff),
                approach(z, trg.z, diff)
        };
    }

    INLINE vec3 lerpTo(const vec3 &to, f32 by) const {
        return (to - *this).scaleAdd(by, *this);
    }

    INLINE vec3 scaleAdd(f32 factor, const vec3 &to_be_added) const {
        return {
                fast_mul_add(x, factor, to_be_added.x),
                fast_mul_add(y, factor, to_be_added.y),
                fast_mul_add(z, factor, to_be_added.z)
        };
    }

    INLINE vec3 mulAdd(const vec3 &factors, const vec3 &to_be_added) const {
        return {
                fast_mul_add(x, factors.x, to_be_added.x),
                fast_mul_add(y, factors.y, to_be_added.y),
                fast_mul_add(z, factors.z, to_be_added.z)
        };
    }
};

vec3 vec3::X{1, 0, 0};
vec3 vec3::Y{0, 1, 0};
vec3 vec3::Z{0, 0, 1};

INLINE vec3 min(const vec3 &a, const vec3 &b) {
    return {
        a.x < b.x ? a.x : b.x,
        a.y < b.y ? a.y : b.y,
        a.z < b.y ? a.z : b.z
    };
}

INLINE vec3 max(const vec3 &a, const vec3 &b) {
    return {
        a.x > b.x ? a.x : b.x,
        a.y > b.y ? a.y : b.y,
        a.z > b.z ? a.z : b.z
    };
}

INLINE vec3 operator - (f32 lhs, const vec3 &rhs) {
    return {
        lhs - rhs.x,
        lhs - rhs.y,
        lhs - rhs.z
    };
}

INLINE vec3 operator + (f32 lhs, const vec3 &rhs) {
    return {
        lhs + rhs.x,
        lhs + rhs.y,
        lhs + rhs.z
    };
}

INLINE vec3 operator / (f32 lhs, const vec3 &rhs) {
    return {
        lhs / rhs.x,
        lhs / rhs.y,
        lhs / rhs.z
    };
}

INLINE vec3 operator * (f32 lhs, const vec3 &rhs) {
    return {
        lhs * rhs.x,
        lhs * rhs.y,
        lhs * rhs.z
    };
}

INLINE vec3 lerp(const vec3 &from, const vec3 &to, f32 by) {
    return (to - from).scaleAdd(by, from);
}

vec3 Color(enum ColorID color_id) {
    return vec3{color_id};
}

struct Edge {
    vec3 from, to;
};

struct AABB {
    vec3 min{-1}, max{1};

    AABB(f32 min_x, f32 min_y, f32 min_z,
         f32 max_x, f32 max_y, f32 max_z) : AABB{
            vec3{min_x, min_y, min_z},
            vec3{max_x, max_y, max_z}
    } {}
    AABB(f32 min_value, f32 max_value) : AABB{vec3{min_value}, vec3{max_value}} {}
    AABB(const vec3 &min, const vec3 &max) : min{min}, max{max} {}
    AABB() : AABB{0, 0} {}
};