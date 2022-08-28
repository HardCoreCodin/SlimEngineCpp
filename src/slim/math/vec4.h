#pragma once

#include "../core/base.h"


struct vec4 {
    union {
        struct {f32 components[4]; };
        struct {f32 x, y, z, w; };
        struct {f32 r, g, b, a; };
    };
    static vec4 X, Y, Z, W;

    INLINE_XPU vec4() noexcept : vec4{0} {}
    INLINE_XPU vec4(f32 x, f32 y, f32 z, f32 w) noexcept : x(x), y(y), z(z), w(w) {}
    INLINE_XPU vec4(const vec4 &other) noexcept : vec4{other.x, other.y, other.z, other.w} {}
    INLINE_XPU explicit vec4(f32 value) noexcept : vec4{value, value, value, value} {}

    INLINE_XPU bool operator == (const vec4 &other) const {
        return (other.x == x) &&
               (other.y == y) &&
               (other.z == z) &&
               (other.w == w);
    }

    INLINE_XPU vec4& operator = (f32 value) {
        x = y = z = value;
        return *this;
    }

    INLINE_XPU vec4& operator = (i32 value) {
        x = y = z = (f32)value;
        return *this;
    }

    INLINE_XPU bool operator ! () const {
        return nonZero();
    }

    INLINE_XPU vec4 operator - () const {
        return {
                -x,
                -y,
                -z,
                -w
        };
    }

    INLINE_XPU vec4 operator - (const vec4 &rhs) const {
        return {
                x - rhs.x,
                y - rhs.y,
                z - rhs.z,
                w - rhs.w
        };
    }

    INLINE_XPU vec4 operator + (const vec4 &rhs) const {
        return {
                x + rhs.x,
                y + rhs.y,
                z + rhs.z,
                w + rhs.w
        };
    }

    INLINE_XPU vec4 operator * (const vec4 &rhs) const {
        return {
                x * rhs.x,
                y * rhs.y,
                z * rhs.z,
                w * rhs.w
        };
    }

    INLINE_XPU vec4 operator / (const vec4 &rhs) const {
        return {
                x / rhs.x,
                y / rhs.y,
                z / rhs.z,
                w / rhs.w
        };
    }

    INLINE_XPU vec4& operator -= (const vec4 &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    INLINE_XPU vec4& operator += (const vec4 &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    INLINE_XPU vec4& operator *= (const vec4 &rhs) {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;
        return *this;
    }

    INLINE_XPU vec4& operator /= (const vec4 &rhs) {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        w /= rhs.w;
        return *this;
    }

    INLINE_XPU vec4 operator - (f32 rhs) const {
        return {
                x - rhs,
                y - rhs,
                z - rhs,
                w - rhs
        };
    }

    INLINE_XPU vec4 operator + (f32 rhs) const {
        return {
                x + rhs,
                y + rhs,
                z + rhs,
                w + rhs
        };
    }

    INLINE_XPU vec4 operator * (f32 rhs) const {
        return {
                x * rhs,
                y * rhs,
                z * rhs,
                w * rhs
        };
    }

    INLINE_XPU vec4 operator / (f32 rhs) const {
        f32 factor = 1.0f / rhs;
        return {
                x * factor,
                y * factor,
                z * factor,
                w * factor
        };
    }

    INLINE_XPU vec4& operator -= (f32 rhs) {
        x -= rhs;
        y -= rhs;
        z -= rhs;
        w -= rhs;
        return *this;
    }

    INLINE_XPU vec4& operator += (f32 rhs) {
        x += rhs;
        y += rhs;
        z += rhs;
        w += rhs;
        return *this;
    }

    INLINE_XPU vec4& operator *= (f32 rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }

    INLINE_XPU vec4& operator /= (f32 rhs) {
        f32 factor = 1.0f / rhs;
        x *= factor;
        y *= factor;
        z *= factor;
        w *= factor;
        return *this;
    }

    INLINE_XPU vec4 operator - (i32 rhs) const {
        return {
                x - (f32)rhs,
                y - (f32)rhs,
                z - (f32)rhs,
                w - (f32)rhs
        };
    }

    INLINE_XPU vec4 operator + (i32 rhs) const {
        return {
                x + (f32)rhs,
                y + (f32)rhs,
                z + (f32)rhs,
                w + (f32)rhs
        };
    }

    INLINE_XPU vec4 operator * (i32 rhs) const {
        return {
                x * (f32)rhs,
                y * (f32)rhs,
                z * (f32)rhs,
                w * (f32)rhs
        };
    }

    INLINE_XPU vec4 operator / (i32 rhs) const {
        f32 factor = 1.0f / (f32)rhs;
        return {
                x * factor,
                y * factor,
                z * factor,
                w * factor
        };
    }

    INLINE_XPU vec4& operator -= (i32 rhs) {
        x -= (f32)rhs;
        y -= (f32)rhs;
        z -= (f32)rhs;
        w -= (f32)rhs;
        return *this;
    }

    INLINE_XPU vec4& operator += (i32 rhs) {
        x += (f32)rhs;
        y += (f32)rhs;
        z += (f32)rhs;
        w += (f32)rhs;
        return *this;
    }

    INLINE_XPU vec4& operator *= (i32 rhs) {
        x *= (f32)rhs;
        y *= (f32)rhs;
        z *= (f32)rhs;
        w *= (f32)rhs;
        return *this;
    }

    INLINE_XPU vec4& operator /= (i32 rhs) {
        f32 factor = 1.0f / (f32)rhs;
        x *= factor;
        y *= factor;
        z *= factor;
        w *= factor;
        return *this;
    }

    INLINE_XPU bool nonZero() const {
        return x != 0.0f ||
               y != 0.0f ||
               z != 0.0f ||
               w != 0.0f;
    }

    INLINE_XPU f32 minimum() const {
        f32 mn = x < y ? x : y;
        mn = mn < z ? mn : z;
        mn = mn < w ? mn : w;
        return mn;
    }

    INLINE_XPU f32 maximum() const {
        f32 mx = x > y ? x : y;
        mx = mx > z ? mx : z;
        mx = mx > w ? mx : w;
        return mx;
    }

    INLINE_XPU f32 dot(const vec4 &rhs) const {
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z) + (z * rhs.w);
    }

    INLINE_XPU f32 squaredLength() const {
        return x*x + y*y + z*z + w*w;
    }

    INLINE_XPU f32 length() const {
        return sqrtf(squaredLength());
    }

    INLINE_XPU vec4 normalized() const {
        return *this / length();
    }

    INLINE_XPU vec4 reflectedAround(const vec4 &N) const {
        return N.scaleAdd(-2 * dot(N), *this);
    }

    INLINE_XPU vec4 clamped() const {
        return {
                clampedValue(x),
                clampedValue(y),
                clampedValue(z),
                clampedValue(w)
        };
    }

    INLINE_XPU vec4 clamped(const vec4 &upper) const {
        return {
                clampedValue(x, upper.x),
                clampedValue(y, upper.y),
                clampedValue(z, upper.z),
                clampedValue(w, upper.w)
        };
    }

    INLINE_XPU vec4 clamped(const vec4 &lower, const vec4 &upper) const {
        return {
                clampedValue(x, lower.x, upper.x),
                clampedValue(y, lower.y, upper.y),
                clampedValue(z, lower.z, upper.z),
                clampedValue(w, lower.w, upper.w)
        };
    }

    INLINE_XPU vec4 clamped(const f32 min_value, const f32 max_value) const {
        return {
                clampedValue(x, min_value, max_value),
                clampedValue(y, min_value, max_value),
                clampedValue(z, min_value, max_value),
                clampedValue(w, min_value, max_value)
        };
    }

    INLINE_XPU vec4 approachTo(const vec4 &trg, f32 diff) const {
        return {
                approach(x, trg.x, diff),
                approach(y, trg.y, diff),
                approach(z, trg.z, diff),
                approach(w, trg.w, diff)
        };
    }

    INLINE_XPU vec4 lerpTo(const vec4 &to, f32 by) const {
        return (to - *this).scaleAdd(by, *this);
    }

    INLINE_XPU vec4 scaleAdd(f32 factor, const vec4 &to_be_added) const {
        return {
                fast_mul_add(x, factor, to_be_added.x),
                fast_mul_add(y, factor, to_be_added.y),
                fast_mul_add(z, factor, to_be_added.z),
                fast_mul_add(w, factor, to_be_added.w)
        };
    }

    INLINE_XPU vec4 mulAdd(const vec4 &factors, const vec4 &to_be_added) const {
        return {
                fast_mul_add(x, factors.x, to_be_added.x),
                fast_mul_add(y, factors.y, to_be_added.y),
                fast_mul_add(z, factors.z, to_be_added.z),
                fast_mul_add(w, factors.w, to_be_added.w)
        };
    }
};

vec4 vec4::X{1, 0, 0, 0};
vec4 vec4::Y{0, 1, 0, 0};
vec4 vec4::Z{0, 0, 1, 0};
vec4 vec4::W{0, 0, 0, 1};

INLINE_XPU vec4 minimum(const vec4 &a, const vec4 &b) {
    return {
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y,
            a.z < b.y ? a.z : b.z,
            a.w < b.w ? a.w : b.w
    };
}

INLINE_XPU vec4 maximum(const vec4 &a, const vec4 &b) {
    return {
            a.x > b.x ? a.x : b.x,
            a.y > b.y ? a.y : b.y,
            a.z > b.z ? a.z : b.z,
            a.w > b.w ? a.w : b.w
    };
}

INLINE_XPU vec4 operator - (f32 lhs, const vec4 &rhs) {
    return {
            lhs - rhs.x,
            lhs - rhs.y,
            lhs - rhs.z,
            lhs - rhs.w
    };
}

INLINE_XPU vec4 operator + (f32 lhs, const vec4 &rhs) {
    return {
            lhs + rhs.x,
            lhs + rhs.y,
            lhs + rhs.z,
            lhs + rhs.w
    };
}

INLINE_XPU vec4 operator / (f32 lhs, const vec4 &rhs) {
    return {
            lhs / rhs.x,
            lhs / rhs.y,
            lhs / rhs.z,
            lhs / rhs.w
    };
}

INLINE_XPU vec4 operator * (f32 lhs, const vec4 &rhs) {
    return {
            lhs * rhs.x,
            lhs * rhs.y,
            lhs * rhs.z,
            lhs * rhs.w
    };
}

INLINE_XPU vec4 lerp(const vec4 &from, const vec4 &to, f32 by) {
    return (to - from).scaleAdd(by, from);
}