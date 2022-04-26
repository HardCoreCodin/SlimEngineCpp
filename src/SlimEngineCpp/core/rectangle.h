#pragma once

#include "../math/vec2.h"

struct Rect {
    union {
        struct {
            vec2 top_left;
            vec2 bottom_right;
        };
        struct {
            f32 left;
            f32 top;
            f32 right;
            f32 bottom;
        };
    };

    Rect() : Rect{0, 0, 0, 0} {}
    Rect(const Rect &other) : Rect{other.top_left, other.bottom_right} {}
    Rect(const vec2 &top_left, const vec2 &bottom_right) : top_left{top_left}, bottom_right{bottom_right} {}
    Rect(f32 left, f32 right, f32 top, f32 bottom) : left{left}, top{top}, right{right}, bottom{bottom} {}

    INLINE bool contains(const vec2 &pos) const {
        return left <= pos.x && pos.x <= right &&
               bottom <=  pos.y && pos.y <= top;
    }

    INLINE bool bounds(const vec2 &pos) const {
        return left < pos.x && pos.x < right &&
               bottom < pos.y && pos.y < top;
    }

    INLINE bool is_zero() const {
        return left == right &&
               top == bottom;
    }

    INLINE vec2 clamped(const vec2 &vec) const {
        return vec.clamped({left, bottom}, {right, top});
    }

    INLINE bool operator ! () const {
        return is_zero();
    }

    INLINE bool operator [] (const vec2 &pos) const {
        return contains(pos);
    }

    INLINE bool operator () (const vec2 &pos) const {
        return bounds(pos);
    }
};


struct RectI {
    union {
        struct {
            vec2i top_left;
            vec2i bottom_right;
        };
        struct {
            i32 left;
            i32 top;
            i32 right;
            i32 bottom;
        };
    };

    RectI() : RectI{0, 0, 0, 0} {}
    RectI(const RectI &other) : RectI{other.top_left, other.bottom_right} {}
    RectI(const vec2i &top_left, const vec2i &bottom_right) : top_left{top_left}, bottom_right{bottom_right} {}
    RectI(i32 left, i32 right, i32 top, i32 bottom) : left{left}, top{top}, right{right}, bottom{bottom} {}

    INLINE bool contains(const vec2i &pos) const {
        return left <= pos.x && pos.x <= right &&
               bottom <=  pos.y && pos.y <= top;
    }

    INLINE bool bounds(const vec2i &pos) const {
        return left < pos.x && pos.x < right &&
               bottom < pos.y && pos.y < top;
    }

    INLINE bool is_zero() const {
        return left == right &&
               top == bottom;
    }

    INLINE bool operator ! () const {
        return is_zero();
    }

    INLINE bool operator [] (const vec2i &pos) const {
        return contains(pos);
    }

    INLINE bool operator () (const vec2i &pos) const {
        return bounds(pos);
    }
};