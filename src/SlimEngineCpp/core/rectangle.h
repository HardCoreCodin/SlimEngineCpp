#pragma once

#include "../math/vec2.h"

template<class VectorType, typename scalar_type>
struct RectOf {
    union {
        struct {
            VectorType top_left;
            VectorType bottom_right;
        };
        struct {
            scalar_type left;
            scalar_type top;
            scalar_type right;
            scalar_type bottom;
        };
    };

    RectOf() : RectOf{0, 0, 0 , 0} {}
    RectOf(const VectorType &top_left, const VectorType &bottom_right) :
            top_left{top_left}, bottom_right{bottom_right} {}
    RectOf(i32 top_left_x, i32 top_left_y, i32 bottom_right_x, i32 bottom_right_y) :
            top_left{top_left_x, top_left_y}, bottom_right{bottom_right_x, bottom_right_y} {}

    INLINE bool contains(const VectorType &pos) const {
        return pos.x >= top_left.x &&
               pos.x <= bottom_right.x &&
               pos.y >= top_left.y &&
               pos.y <= bottom_right.y;
    }

    INLINE bool bounds(const VectorType &pos) const {
        return pos.x > top_left.x &&
               pos.x < bottom_right.x &&
               pos.y > top_left.y &&
               pos.y < bottom_right.y;
    }

    INLINE bool is_zero() const {
        return top_left.x == bottom_right.x &&
               top_left.y == bottom_right.y;
    }

    INLINE VectorType clamped(const VectorType &vec) const {
        return vec.clamped(top_left, bottom_right);
    }

    INLINE bool operator ! () const {
        return is_zero();
    }

    INLINE bool operator [] (const VectorType &pos) const {
        return contains(pos);
    }

    INLINE bool operator () (const VectorType &pos) const {
        return bounds(pos);
    }
};

using Rect = RectOf<vec2, f32>;
using RectI = RectOf<vec2i, i32>;