#pragma once

#include "../core/base.h"


#define SLIM_VEC2

struct vec2i {
    i32 x, y;

    INLINE_XPU vec2i() noexcept : vec2i{0} {}
    INLINE_XPU vec2i(i32 x, i32 y) noexcept : x(x), y(y) {}
    INLINE_XPU vec2i(vec2i &other) noexcept : vec2i{other.x, other.y} {}
    INLINE_XPU explicit vec2i(i32 value) noexcept : vec2i{value, value} {}

    INLINE_XPU bool operator == (const vec2i &other) const {
        return (other.x == x) &&
               (other.y == y);
    }

    INLINE_XPU vec2i & operator = (f32 value) {
        x = y = (i32)value;
        return *this;
    }

    INLINE_XPU vec2i & operator = (i32 value) {
        x = y = value;
        return *this;
    }

    INLINE_XPU bool operator ! () const {
        return nonZero();
    }

    INLINE_XPU vec2i operator - () const {
        return {
                -x,
                -y
        };
    }

    INLINE_XPU vec2i operator - (const vec2i &rhs) const {
        return {
                x - rhs.x,
                y - rhs.y
        };
    }

    INLINE_XPU vec2i operator + (const vec2i &rhs) const {
        return {
                x + rhs.x,
                y + rhs.y
        };
    }

    INLINE_XPU vec2i operator * (const vec2i &rhs) const {
        return {
                x * rhs.x,
                y * rhs.y
        };
    }

    INLINE_XPU vec2i operator / (const vec2i &rhs) const {
        return {
                x / rhs.x,
                y / rhs.y
        };
    }

    INLINE_XPU vec2i& operator -= (const vec2i &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    INLINE_XPU vec2i& operator += (const vec2i &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    INLINE_XPU vec2i& operator *= (const vec2i &rhs) {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    INLINE_XPU vec2i& operator /= (const vec2i &rhs) {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    INLINE_XPU vec2i operator - (i32 rhs) const {
        return {
                x - rhs,
                y - rhs
        };
    }

    INLINE_XPU vec2i operator + (i32 rhs) const {
        return {
                x + rhs,
                y + rhs
        };
    }

    INLINE_XPU vec2i operator * (i32 rhs) const {
        return {
                x * rhs,
                y * rhs
        };
    }

    INLINE_XPU vec2i operator / (i32 rhs) const {
        return {
                x / rhs,
                y / rhs
        };
    }

    INLINE_XPU vec2i& operator -= (i32 rhs) {
        x -= rhs;
        y -= rhs;
        return *this;
    }

    INLINE_XPU vec2i& operator += (i32 rhs) {
        x += rhs;
        y += rhs;
        return *this;
    }

    INLINE_XPU vec2i& operator *= (i32 rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    INLINE_XPU vec2i& operator /= (i32 rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    INLINE_XPU vec2i operator - (f32 rhs) const {
        return {
                (i32)((f32)x - rhs),
                (i32)((f32)y - rhs)
        };
    }

    INLINE_XPU vec2i operator + (f32 rhs) const {
        return {
                (i32)((f32)x + rhs),
                (i32)((f32)y + rhs)
        };
    }

    INLINE_XPU vec2i operator * (f32 rhs) const {
        return {
                (i32)((f32)x * rhs),
                (i32)((f32)y * rhs)
        };
    }

    INLINE_XPU vec2i operator / (f32 rhs) const {
        return {
                (i32)((f32)x / rhs),
                (i32)((f32)y / rhs)
        };
    }

    INLINE_XPU vec2i& operator -= (f32 rhs) {
        x -= (i32)rhs;
        y -= (i32)rhs;
        return *this;
    }

    INLINE_XPU vec2i& operator += (f32 rhs) {
        x += (i32)rhs;
        y += (i32)rhs;
        return *this;
    }

    INLINE_XPU vec2i& operator *= (f32 rhs) {
        x *= (i32)rhs;
        y *= (i32)rhs;
        return *this;
    }

    INLINE_XPU vec2i& operator /= (f32 rhs) {
        x /= (i32)rhs;
        y /= (i32)rhs;
        return *this;
    }

    INLINE_XPU bool nonZero() const {
        return x != 0 ||
               y != 0;
    }

    INLINE_XPU i32 minimum() const {
        return x < y ? x : y;
    }

    INLINE_XPU i32 maximum() const {
        return x > y ? x : y;
    }

    INLINE_XPU vec2i clamped() const {
        return {
                clampedValue(x),
                clampedValue(y)
        };
    }

    INLINE_XPU vec2i clamped(const vec2i &upper) const {
        return {
                clampedValue(x, upper.x),
                clampedValue(y, upper.y)
        };
    }

    INLINE_XPU vec2i clamped(const f32 min_value, const f32 max_value) const {
        return {
                (i32)(clampedValue((f32)x, min_value, max_value)),
                (i32)(clampedValue((f32)y, min_value, max_value))
        };
    }

    INLINE_XPU vec2i clamped(const i32 min_value, const i32 max_value) const {
        return {
                clampedValue(x, min_value, max_value),
                clampedValue(y, min_value, max_value)
        };
    }

    INLINE_XPU vec2i approachTo(const vec2i &trg, f32 diff) const {
        return {
                (i32)(approach((f32)x, (f32)trg.x, diff)),
                (i32)(approach((f32)y, (f32)trg.y, diff))
        };
    }

    INLINE_XPU vec2i scaleAdd(f32 factor, const vec2i &to_be_added) const {
        return {
                (i32)fast_mul_add((f32)x, factor, (f32)to_be_added.x),
                (i32)fast_mul_add((f32)y, factor, (f32)to_be_added.y)
        };
    }
};


struct vec2 {
    union {
        struct {f32 components[2]; };
        struct {f32 x, y; };
        struct {f32 u, v; };
    };

    static vec2 X, Y;

    INLINE_XPU vec2() : vec2{0} {}
    INLINE_XPU vec2(f32 x, f32 y) noexcept : x(x), y(y) {}
    INLINE_XPU vec2(i32 x, i32 y) noexcept : x((f32)x), y((f32)y) {}
    INLINE_XPU vec2(const vec2 &other) noexcept : vec2{other.x, other.y} {}
    INLINE_XPU explicit vec2(f32 value) noexcept : vec2{value, value} {}
    INLINE_XPU explicit vec2(vec2i &other) noexcept : vec2{(f32)other.x, (f32)other.y} {}
    INLINE_XPU explicit vec2(const vec2i &other) noexcept : vec2{(f32)other.x, (f32)other.y} {}

    INLINE_XPU bool operator == (const vec2 &other) const {
        return (other.x == x) &&
               (other.y == y);
    }

    INLINE_XPU vec2 & operator = (f32 value) {
        x = y = value;
        return *this;
    }

    INLINE_XPU vec2 & operator = (i32 value) {
        x = y = (f32)value;
        return *this;
    }

    INLINE_XPU bool operator ! () const {
        return nonZero();
    }

    INLINE_XPU vec2 operator - () const {
        return {
                -x,
                -y
        };
    }

    INLINE_XPU vec2 operator - (const vec2 &rhs) const {
        return {
                x - rhs.x,
                y - rhs.y
        };
    }

    INLINE_XPU vec2 operator + (const vec2 &rhs) const {
        return {
                x + rhs.x,
                y + rhs.y
        };
    }

    INLINE_XPU vec2 operator * (const vec2 &rhs) const {
        return {
                x * rhs.x,
                y * rhs.y
        };
    }

    INLINE_XPU vec2 operator / (const vec2 &rhs) const {
        return {
                x / rhs.x,
                y / rhs.y
        };
    }

    INLINE_XPU vec2& operator -= (const vec2 &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    INLINE_XPU vec2& operator += (const vec2 &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    INLINE_XPU vec2& operator *= (const vec2 &rhs) {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    INLINE_XPU vec2& operator /= (const vec2 &rhs) {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    INLINE_XPU vec2 operator - (f32 rhs) const {
        return {
                x - rhs,
                y - rhs
        };
    }

    INLINE_XPU vec2 operator + (f32 rhs) const {
        return {
                x + rhs,
                y + rhs
        };
    }

    INLINE_XPU vec2 operator * (f32 rhs) const {
        return {
                x * rhs,
                y * rhs
        };
    }

    INLINE_XPU vec2 operator / (f32 rhs) const {
        f32 factor = 1.0f / rhs;
        return {
                x * factor,
                y * factor
        };
    }

    INLINE_XPU vec2& operator -= (f32 rhs) {
        x -= rhs;
        y -= rhs;
        return *this;
    }

    INLINE_XPU vec2& operator += (f32 rhs) {
        x += rhs;
        y += rhs;
        return *this;
    }

    INLINE_XPU vec2& operator *= (f32 rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    INLINE_XPU vec2& operator /= (f32 rhs) {
        f32 factor = 1.0f / rhs;
        x *= factor;
        y *= factor;
        return *this;
    }

    INLINE_XPU vec2 operator - (i32 rhs) const {
        return {
                x - (f32)rhs,
                y - (f32)rhs
        };
    }

    INLINE_XPU vec2 operator + (i32 rhs) const {
        return {
                x + (f32)rhs,
                y + (f32)rhs
        };
    }

    INLINE_XPU vec2 operator * (i32 rhs) const {
        return {
                x * (f32)rhs,
                y * (f32)rhs
        };
    }

    INLINE_XPU vec2 operator / (i32 rhs) const {
        f32 factor = 1.0f / (f32)rhs;
        return {
                x * factor,
                y * factor
        };
    }

    INLINE_XPU vec2& operator -= (i32 rhs) {
        x -= (f32)rhs;
        y -= (f32)rhs;
        return *this;
    }

    INLINE_XPU vec2& operator += (i32 rhs) {
        x += (f32)rhs;
        y += (f32)rhs;
        return *this;
    }

    INLINE_XPU vec2& operator *= (i32 rhs) {
        x *= (f32)rhs;
        y *= (f32)rhs;
        return *this;
    }

    INLINE_XPU vec2& operator /= (i32 rhs) {
        f32 factor = 1.0f / (f32)rhs;
        x *= factor;
        y *= factor;
        return *this;
    }

    INLINE_XPU vec2 operator - (const vec2i &rhs) const {
        return {
                x - (f32)rhs.x,
                y - (f32)rhs.y
        };
    }

    INLINE_XPU vec2 operator + (const vec2i &rhs) const {
        return {
                x + (f32)rhs.x,
                y + (f32)rhs.y
        };
    }

    INLINE_XPU vec2 operator * (const vec2i &rhs) const {
        return {
                x * (f32)rhs.x,
                y * (f32)rhs.y
        };
    }

    INLINE_XPU vec2 operator / (const vec2i &rhs) const {
        return {
                x / (f32)rhs.x,
                y / (f32)rhs.y
        };
    }

    INLINE_XPU vec2& operator -= (const vec2i &rhs) {
        x -= (f32)rhs.x;
        y -= (f32)rhs.y;
        return *this;
    }

    INLINE_XPU vec2& operator += (const vec2i &rhs) {
        x += (f32)rhs.x;
        y += (f32)rhs.y;
        return *this;
    }

    INLINE_XPU vec2& operator *= (const vec2i &rhs) {
        x *= (f32)rhs.x;
        y *= (f32)rhs.y;
        return *this;
    }

    INLINE_XPU vec2& operator /= (const vec2i &rhs) {
        x /= (f32)rhs.x;
        y /= (f32)rhs.y;
        return *this;
    }

    INLINE_XPU bool nonZero() const {
        return x != 0.0f ||
               y != 0.0f;
    }

    INLINE_XPU f32 minimum() const {
        return x < y ? x : y;
    }

    INLINE_XPU f32 maximum() const {
        return x > y ? x : y;
    }

    INLINE_XPU vec2 perp() const {
        return {
                -y,
                x
        };
    }

    INLINE_XPU f32 dot(const vec2 &rhs) const {
        return (x * rhs.x) + (y * rhs.y);
    }

    INLINE_XPU f32 cross(const vec2 &rhs) const {
        return (x * rhs.y) - (y * rhs.x);
    }

    INLINE_XPU f32 squaredLength() const {
        return x*x + y*y;
    }

    INLINE_XPU f32 length() const {
        return sqrtf(squaredLength());
    }

    INLINE_XPU vec2 normalized() const {
        return *this / length();
    }

    INLINE_XPU vec2 reflectedAround(const vec2 &N) const {
        return N.scaleAdd(-2 * dot(N), *this);
    }

    INLINE_XPU vec2 clamped() const {
        return {
                clampedValue(x),
                clampedValue(y)
        };
    }

    INLINE_XPU vec2 clamped(const vec2 &upper) const {
        return {
                clampedValue(x, upper.x),
                clampedValue(y, upper.y)
        };
    }

    INLINE_XPU vec2 clamped(const vec2 &lower, const vec2 &upper) const {
        return {
                clampedValue(x, lower.x, upper.x),
                clampedValue(y, lower.y, upper.y)
        };
    }

    INLINE_XPU vec2 clamped(const f32 min_value, const f32 max_value) const {
        return {
                clampedValue(x, min_value, max_value),
                clampedValue(y, min_value, max_value)
        };
    }

    INLINE_XPU vec2 approachTo(const vec2 &trg, f32 diff) const {
        return {
                approach(x, trg.x, diff),
                approach(y, trg.y, diff)
        };
    }

    INLINE_XPU vec2 lerpTo(const vec2 &to, f32 by) const {
        return (to - *this).scaleAdd(by, *this);
    }

    INLINE_XPU vec2 scaleAdd(f32 factor, const vec2 &to_be_added) const {
        return {
                fast_mul_add(x, factor, to_be_added.x),
                fast_mul_add(y, factor, to_be_added.y)
        };
    }

    INLINE_XPU vec2 mulAdd(const vec2 &factors, const vec2 &to_be_added) const {
        return {
                fast_mul_add(x, factors.x, to_be_added.x),
                fast_mul_add(y, factors.y, to_be_added.y)
        };
    }
};

vec2 vec2::X{1.0f, 0.0f};
vec2 vec2::Y{0.0f, 1.0f};

INLINE_XPU vec2 minimum(const vec2 &a, const vec2 &b) {
    return {
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y
    };
}

INLINE_XPU vec2 maximum(const vec2 &a, const vec2 &b) {
    return {
            a.x > b.x ? a.x : b.x,
            a.y > b.y ? a.y : b.y
    };
}

INLINE_XPU vec2i minimum(const vec2i &a, const vec2i &b) {
    return {
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y
    };
}

INLINE_XPU vec2i maximum(const vec2i &a, const vec2i &b) {
    return {
            a.x > b.x ? a.x : b.x,
            a.y > b.y ? a.y : b.y
    };
}

INLINE_XPU vec2i operator - (const vec2i &lhs, const vec2 &rhs) {
    return {
            lhs.x - (i32)rhs.x,
            lhs.y - (i32)rhs.y
    };
}

INLINE_XPU vec2i operator + (const vec2i &lhs, const vec2 &rhs) {
    return {
            lhs.x + (i32)rhs.x,
            lhs.y + (i32)rhs.y
    };
}

INLINE vec2i operator * (const vec2i &lhs, const vec2 &rhs) {
    return {
            lhs.x * (i32)rhs.x,
            lhs.y * (i32)rhs.y
    };
}

INLINE_XPU vec2i operator / (const vec2i &lhs, const vec2 &rhs) {
    return {
            lhs.x / (i32)rhs.x,
            lhs.y / (i32)rhs.y
    };
}

INLINE_XPU vec2i operator - (i32 lhs, const vec2 &rhs) {
    return {
            lhs - (i32)rhs.x,
            lhs - (i32)rhs.y
    };
}

INLINE vec2i operator + (i32 lhs, const vec2 &rhs) {
    return {
            lhs + (i32)rhs.x,
            lhs + (i32)rhs.y
    };
}

INLINE vec2i operator * (i32 lhs, const vec2 &rhs) {
    return {
            lhs * (i32)rhs.x,
            lhs * (i32)rhs.y
    };
}

INLINE_XPU vec2i operator / (i32 lhs, const vec2 &rhs) {
    return {
            lhs / (i32)rhs.x,
            lhs / (i32)rhs.y
    };
}

INLINE_XPU vec2i operator - (i32 lhs, const vec2i &rhs) {
    return {
            lhs - rhs.x,
            lhs - rhs.y
    };
}

INLINE_XPU vec2i operator + (i32 lhs, const vec2i &rhs) {
    return {
            lhs + rhs.x,
            lhs + rhs.y
    };
}

INLINE_XPU vec2i operator * (i32 lhs, const vec2i &rhs) {
    return {
            lhs * rhs.x,
            lhs * rhs.y
    };
}

INLINE_XPU vec2i operator / (i32 lhs, const vec2i &rhs) {
    return {
            lhs / rhs.x,
            lhs / rhs.y
    };
}

INLINE_XPU vec2 operator - (f32 lhs, const vec2 &rhs) {
    return {
            lhs - rhs.x,
            lhs - rhs.y
    };
}

INLINE_XPU vec2 operator + (f32 lhs, const vec2 &rhs) {
    return {
            lhs + rhs.x,
            lhs + rhs.y
    };
}

INLINE_XPU vec2 operator / (f32 lhs, const vec2 &rhs) {
    return {
            lhs / rhs.x,
            lhs / rhs.y
    };
}

INLINE_XPU vec2 operator * (f32 lhs, const vec2 &rhs) {
    return {
            lhs * rhs.x,
            lhs * rhs.y
    };
}

INLINE_XPU vec2 lerp(const vec2 &from, const vec2 &to, f32 by) {
    return (to - from).scaleAdd(by, from);
}