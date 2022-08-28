#pragma once

#include "../core/base.h"

#define SLIM_VEC3

struct vec3 {
    union {
        struct {f32 components[3]; };
        struct {f32 x, y, z; };
        struct {f32 u, v, w; };
        struct {f32 r, g, b; };
    };

    static vec3 X, Y, Z;

    INLINE_XPU vec3() noexcept : vec3{0} {}
    INLINE_XPU vec3(f32 x, f32 y, f32 z) noexcept : x(x), y(y), z(z) {}
    INLINE_XPU vec3(const vec3 &other) noexcept : vec3{other.x, other.y, other.z} {}
    INLINE_XPU vec3(f32 value) noexcept : vec3{value, value, value} {}
    INLINE_XPU vec3(enum ColorID color_id) noexcept : vec3{Color{color_id}} {}
    INLINE_XPU vec3(const Color &color) noexcept : vec3{color.red, color.green, color.blue} {}

    INLINE_XPU vec3& operator = (f32 value) {
        x = y = z = value;
        return *this;
    }

    INLINE_XPU vec3& operator = (i32 value) {
        x = y = z = (f32)value;
        return *this;
    }

    INLINE_XPU Color toColor() const {
        return {r, g, b};
    }

    INLINE_XPU bool operator == (const vec3 &other) const {
        return (other.x == x) &&
               (other.y == y) &&
               (other.z == z);
    }

    INLINE_XPU bool operator ! () const {
        return nonZero();
    }

    INLINE_XPU vec3 operator - () const {
        return {
                -x,
                -y,
                -z
        };
    }

    INLINE_XPU vec3 operator - (const vec3 &rhs) const {
        return {
                x - rhs.x,
                y - rhs.y,
                z - rhs.z
        };
    }

    INLINE_XPU vec3 operator + (const vec3 &rhs) const {
        return {
                x + rhs.x,
                y + rhs.y,
                z + rhs.z
        };
    }

    INLINE_XPU vec3 operator * (const vec3 &rhs) const {
        return {
                x * rhs.x,
                y * rhs.y,
                z * rhs.z
        };
    }

    INLINE_XPU vec3 operator / (const vec3 &rhs) const {
        return {
                x / rhs.x,
                y / rhs.y,
                z / rhs.z
        };
    }

    INLINE_XPU vec3& operator -= (const vec3 &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    INLINE_XPU vec3& operator += (const vec3 &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    INLINE_XPU vec3& operator *= (const vec3 &rhs) {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }

    INLINE_XPU vec3& operator /= (const vec3 &rhs) {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }

    INLINE_XPU vec3 operator - (f32 rhs) const {
        return {
                x - rhs,
                y - rhs,
                z - rhs
        };
    }

    INLINE_XPU vec3 operator + (f32 rhs) const {
        return {
                x + rhs,
                y + rhs,
                z + rhs
        };
    }

    INLINE_XPU vec3 operator * (f32 rhs) const {
        return {
                x * rhs,
                y * rhs,
                z * rhs
        };
    }

    INLINE_XPU vec3 operator / (f32 rhs) const {
        const f32 factor = 1.0f / rhs;
        return {
                x * factor,
                y * factor,
                z * factor
        };
    }

    INLINE_XPU vec3& operator -= (f32 rhs) {
        x -= rhs;
        y -= rhs;
        z -= rhs;
        return *this;
    }

    INLINE_XPU vec3& operator += (f32 rhs) {
        x += rhs;
        y += rhs;
        z += rhs;
        return *this;
    }

    INLINE_XPU vec3& operator *= (f32 rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    INLINE_XPU vec3& operator /= (f32 rhs) {
        f32 factor = 1.0f / rhs;
        x *= factor;
        y *= factor;
        z *= factor;
        return *this;
    }

    INLINE_XPU vec3 operator - (i32 rhs) const {
        return {
                x - (f32)rhs,
                y - (f32)rhs,
                z - (f32)rhs
        };
    }

    INLINE_XPU vec3 operator + (i32 rhs) const {
        return {
                x + (f32)rhs,
                y + (f32)rhs,
                z + (f32)rhs
        };
    }

    INLINE_XPU vec3 operator * (i32 rhs) const {
        return {
                x * (f32)rhs,
                y * (f32)rhs,
                z * (f32)rhs
        };
    }

    INLINE_XPU vec3 operator / (i32 rhs) const {
        f32 factor = 1.0f / (f32)rhs;
        return {
                x * factor,
                y * factor,
                z * factor
        };
    }

    INLINE_XPU vec3& operator -= (i32 rhs) {
        x -= (f32)rhs;
        y -= (f32)rhs;
        z -= (f32)rhs;
        return *this;
    }

    INLINE_XPU vec3& operator += (i32 rhs) {
        x += (f32)rhs;
        y += (f32)rhs;
        z += (f32)rhs;
        return *this;
    }

    INLINE_XPU vec3& operator *= (i32 rhs) {
        x *= (f32)rhs;
        y *= (f32)rhs;
        z *= (f32)rhs;
        return *this;
    }

    INLINE_XPU vec3& operator /= (i32 rhs) {
        f32 factor = 1.0f / (f32)rhs;
        x *= factor;
        y *= factor;
        z *= factor;
        return *this;
    }

    INLINE_XPU bool nonZero() const {
        return x != 0.0f ||
               y != 0.0f ||
               z != 0.0f;
    }

    INLINE_XPU vec3 perpZ() const {
        return {
                -y,
                x,
                z
        };
    }

    INLINE_XPU f32 minimum(Axis *axis = nullptr) const {
        if (axis) {
            *axis = Axis_X;
            f32 result = x;
            if (y < result) {
                result = y;
                *axis = Axis_Y;
            }
            if (z < result) {
                result = z;
                *axis = Axis_Z;
            }
            return result;
        } else
            return x < y ? (x < z ? x : z) : (y < z ? y : z);
    }

    INLINE_XPU f32 maximum(Axis *axis = nullptr) const {
        if (axis) {
            *axis = Axis_X;
            f32 result = x;
            if (y > result) {
                result = y;
                *axis = Axis_Y;
            }
            if (z > result) {
                result = z;
                *axis = Axis_Z;
            }
            return result;
        } else
            return x > y ? (x > z ? x : z) : (y > z ? y : z);
    }

    INLINE_XPU f32 dot(const vec3 &rhs) const {
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
    }

    INLINE_XPU vec3 cross(const vec3 &rhs) const {
        return {
                (y * rhs.z) - (z * rhs.y),
                (z * rhs.x) - (x * rhs.z),
                (x * rhs.y) - (y * rhs.x)
        };
    }

    INLINE_XPU f32 squaredLength() const {
        return x*x + y*y + z*z;
    }

    INLINE_XPU f32 length() const {
        return sqrtf(squaredLength());
    }

    INLINE_XPU vec3 normalized() const {
        return *this / length();
    }

    INLINE_XPU vec3 reflectedAround(const vec3 &N) const {
        return N.scaleAdd(-2 * dot(N), *this);
    }

    INLINE_XPU vec3 clamped() const {
        return {
                clampedValue(x),
                clampedValue(y),
                clampedValue(z)
        };
    }

    INLINE_XPU vec3 clamped(const vec3 &upper) const {
        return {
                clampedValue(x, upper.x),
                clampedValue(y, upper.y),
                clampedValue(z, upper.z)
        };
    }

    INLINE_XPU vec3 clamped(const vec3 &lower, const vec3 &upper) const {
        return {
                clampedValue(x, lower.x, upper.x),
                clampedValue(y, lower.y, upper.y),
                clampedValue(z, lower.z, upper.z)
        };
    }

    INLINE_XPU vec3 clamped(const f32 min_value, const f32 max_value) const {
        return {
                clampedValue(x, min_value, max_value),
                clampedValue(y, min_value, max_value),
                clampedValue(z, min_value, max_value)
        };
    }

    INLINE_XPU vec3 approachTo(const vec3 &trg, f32 diff) const {
        return {
                approach(x, trg.x, diff),
                approach(y, trg.y, diff),
                approach(z, trg.z, diff)
        };
    }

    INLINE_XPU vec3 lerpTo(const vec3 &to, f32 by) const {
        return (to - *this).scaleAdd(by, *this);
    }

    INLINE_XPU vec3 scaleAdd(f32 factor, const vec3 &to_be_added) const {
        return {
                fast_mul_add(x, factor, to_be_added.x),
                fast_mul_add(y, factor, to_be_added.y),
                fast_mul_add(z, factor, to_be_added.z)
        };
    }

    INLINE_XPU vec3 mulAdd(const vec3 &factors, const vec3 &to_be_added) const {
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

INLINE_XPU vec3 minimum(const vec3 &a, const vec3 &b) {
    return {
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y,
            a.z < b.z ? a.z : b.z
    };
}

INLINE_XPU vec3 maximum(const vec3 &a, const vec3 &b) {
    return {
            a.x > b.x ? a.x : b.x,
            a.y > b.y ? a.y : b.y,
            a.z > b.z ? a.z : b.z
    };
}

INLINE_XPU vec3 operator - (f32 lhs, const vec3 &rhs) {
    return {
            lhs - rhs.x,
            lhs - rhs.y,
            lhs - rhs.z
    };
}

INLINE_XPU vec3 operator + (f32 lhs, const vec3 &rhs) {
    return {
            lhs + rhs.x,
            lhs + rhs.y,
            lhs + rhs.z
    };
}

INLINE_XPU vec3 operator / (f32 lhs, const vec3 &rhs) {
    return {
            lhs / rhs.x,
            lhs / rhs.y,
            lhs / rhs.z
    };
}

INLINE_XPU vec3 operator * (f32 lhs, const vec3 &rhs) {
    return {
            lhs * rhs.x,
            lhs * rhs.y,
            lhs * rhs.z
    };
}

INLINE_XPU vec3 lerp(const vec3 &from, const vec3 &to, f32 by) {
    return (to - from).scaleAdd(by, from);
}

struct Edge {
    vec3 from, to;
};

struct AABB {
    vec3 min, max;

    INLINE_XPU AABB(f32 min_x, f32 min_y, f32 min_z,
         f32 max_x, f32 max_y, f32 max_z) : AABB{
            vec3{min_x, min_y, min_z},
            vec3{max_x, max_y, max_z}
    } {}
    INLINE_XPU AABB(f32 min_value, f32 max_value) : AABB{vec3{min_value}, vec3{max_value}} {}
    INLINE_XPU AABB(const vec3 &min, const vec3 &max) : min{min}, max{max} {}
    INLINE_XPU AABB() : AABB{0, 0} {}

    INLINE_XPU AABB operator + (const AABB &rhs) const {
        return {
                min.x < rhs.min.x ? min.x : rhs.min.x,
                min.y < rhs.min.y ? min.y : rhs.min.y,
                min.z < rhs.min.z ? min.z : rhs.min.z,

                max.x > rhs.max.x ? max.x : rhs.max.x,
                max.y > rhs.max.y ? max.y : rhs.max.y,
                max.z > rhs.max.z ? max.z : rhs.max.z,
        };
    }

    INLINE_XPU const AABB& operator += (const AABB &rhs) {
        min.x = min.x < rhs.min.x ? min.x : rhs.min.x;
        min.y = min.y < rhs.min.y ? min.y : rhs.min.y;
        min.z = min.z < rhs.min.z ? min.z : rhs.min.z;

        max.x = max.x > rhs.max.x ? max.x : rhs.max.x;
        max.y = max.y > rhs.max.y ? max.y : rhs.max.y;
        max.z = max.z > rhs.max.z ? max.z : rhs.max.z;

        return *this;
    }

    INLINE_XPU bool contains(const vec3& point) const {
        return (min.x <= point.x && point.x <= max.x &&
                min.y <= point.y && point.y <= max.y &&
                min.z <= point.z && point.z <= max.z);
    }

    INLINE_XPU bool overlapSphere(const vec3 &center, f32 radius) {
        vec3 min_delta{min - center};
        vec3 max_delta{center - max};
        f32 squared_distance = 0;
        for (u8 i = 0; i < 3; i++) {
            if (min_delta.components[i] > 0) squared_distance += min_delta.components[i] * min_delta.components[i]; else
            if (max_delta.components[i] > 0) squared_distance += max_delta.components[i] * max_delta.components[i];
        }
        return squared_distance <= radius * radius;
    }

    INLINE_XPU f32 area() const {
        vec3 extents = max - min;
        return extents.x*extents.y + extents.y*extents.z + extents.z*extents.x;
    }
};

INLINE Color vec3ToColor(const vec3 &v) {
    return {v.r, v.g, v.b};
}

INLINE Color directionToColor(const vec3 &v) {
    return {
            fast_mul_add(v.r, 0.5f, 0.5f),
            fast_mul_add(v.g, 0.5f, 0.5f),
            fast_mul_add(v.b, 0.5f, 0.5f)
    };
}

INLINE vec3 colorToVec3(const Color &color) {
    return {color.r, color.g, color.b};
}