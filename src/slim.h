#pragma once

#define SLIM_DINGLE_HEADER_FILE

#include <cmath>

#if defined(__clang__)
#define COMPILER_CLANG 1
#define COMPILER_CLANG_OR_GCC 1
#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPILER_GCC 1
#define COMPILER_CLANG_OR_GCC 1
#elif defined(_MSC_VER)
#define COMPILER_MSVC 1
#endif

#if (defined(SLIMMER) || !defined(NDEBUG))
#define INLINE
#elif defined(COMPILER_MSVC)
#define INLINE inline __forceinline
#elif defined(COMPILER_CLANG_OR_GCC)
#define INLINE inline __attribute__((always_inline))
#else
#define INLINE inline
#endif

#ifdef COMPILER_CLANG
#define ENABLE_FP_CONTRACT \
        _Pragma("clang diagnostic push") \
        _Pragma("clang diagnostic ignored \"-Wunknown-pragmas\"") \
        _Pragma("STDC FP_CONTRACT ON") \
        _Pragma("clang diagnostic pop")
#else
#define ENABLE_FP_CONTRACT
#endif

#ifdef FP_FAST_FMAF
#define fast_mul_add(a, b, c) fmaf(a, b, c)
#else
ENABLE_FP_CONTRACT
#define fast_mul_add(a, b, c) ((a) * (b) + (c))
#endif

#ifdef __cplusplus
#define null nullptr
#ifndef signbit
#define signbit std::signbit
#endif
#else
#define null 0
    typedef unsigned char      bool;
#endif

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned long int  u32;
typedef unsigned long long u64;
typedef signed   short     i16;
typedef signed   long int  i32;

typedef float  f32;
typedef double f64;

#ifndef SLIM_DISABLE_ALL_CANVAS_DRAWING
#ifndef SLIM_ENABLE_CANVAS_TEXT_DRAWING
#define SLIM_ENABLE_CANVAS_TEXT_DRAWING
#endif

#ifndef SLIM_ENABLE_CANVAS_NUMBER_DRAWING
#define SLIM_ENABLE_CANVAS_NUMBER_DRAWING
#endif

#ifndef SLIM_ENABLE_CANVAS_HUD_DRAWING
#define SLIM_ENABLE_CANVAS_HUD_DRAWING
#endif

#ifndef SLIM_ENABLE_CANVAS_LINE_DRAWING
#define SLIM_ENABLE_CANVAS_LINE_DRAWING
#endif

#ifndef SLIM_ENABLE_CANVAS_RECTANGLE_DRAWING
#define SLIM_ENABLE_CANVAS_RECTANGLE_DRAWING
#endif

#ifndef SLIM_ENABLE_CANVAS_TRIANGLE_DRAWING
#define SLIM_ENABLE_CANVAS_TRIANGLE_DRAWING
#endif

#ifndef SLIM_ENABLE_CANVAS_CIRCLE_DRAWING
#define SLIM_ENABLE_CANVAS_CIRCLE_DRAWING
#endif
#endif

#ifndef SLIM_DISABLE_ALL_VIEWPORT_DRAWING
#ifndef SLIM_ENABLE_VIEWPORT_CURVE_DRAWING
#define SLIM_ENABLE_VIEWPORT_CURVE_DRAWING
#endif

#ifndef SLIM_ENABLE_VIEWPORT_EDGE_DRAWING
#define SLIM_ENABLE_VIEWPORT_EDGE_DRAWING
#endif

#ifndef SLIM_ENABLE_VIEWPORT_BOX_DRAWING
#define SLIM_ENABLE_VIEWPORT_BOX_DRAWING
#endif

#ifndef SLIM_ENABLE_VIEWPORT_MESH_DRAWING
#define SLIM_ENABLE_VIEWPORT_MESH_DRAWING
#endif

#ifndef SLIM_ENABLE_VIEWPORT_CAMERA_DRAWING
#define SLIM_ENABLE_VIEWPORT_CAMERA_DRAWING
#endif

#ifndef SLIM_ENABLE_VIEWPORT_GRID_DRAWING
#define SLIM_ENABLE_VIEWPORT_GRID_DRAWING
#endif
#endif

#ifndef CANVAS_COUNT
#define CANVAS_COUNT 2
#endif

#define FONT_WIDTH 9
#define FONT_HEIGHT 12

#define TAU 6.28f
#define COLOR_COMPONENT_TO_FLOAT 0.00392156862f
#define FLOAT_TO_COLOR_COMPONENT 255.0f
#define DEG_TO_RAD 0.0174533f

#define MAX_COLOR_VALUE 0xFF

#define Kilobytes(value) ((value)*1024LL)
#define Megabytes(value) (Kilobytes(value)*1024LL)
#define Gigabytes(value) (Megabytes(value)*1024LL)
#define Terabytes(value) (Gigabytes(value)*1024LL)

#define MEMORY_SIZE Gigabytes(1)
#define MEMORY_BASE Terabytes(2)

#define MAX_WIDTH 3840
#define MAX_HEIGHT 2160
#define MAX_WINDOW_SIZE (MAX_WIDTH * MAX_HEIGHT)

#define DEFAULT_WIDTH 480
#define DEFAULT_HEIGHT 360

#define WINDOW_CONTENT_PIXEL_SIZE 4
#define WINDOW_CONTENT_SIZE (MAX_WINDOW_SIZE * WINDOW_CONTENT_PIXEL_SIZE)

#define BOX__ALL_SIDES (Top | Bottom | Left | Right | Front | Back)
#define BOX__VERTEX_COUNT 8
#define BOX__EDGE_COUNT 12
#define GRID__MAX_SEGMENTS 101

#define CURVE_STEPS 360

#define CUBE_UV_COUNT 4
#define CUBE_NORMAL_COUNT 6
#define CUBE_VERTEX_COUNT 8
#define CUBE_TRIANGLE_COUNT 12

#define CAMERA_DEFAULT__FOCAL_LENGTH 2.0f
#define CAMERA_DEFAULT__TARGET_DISTANCE 10

#define NAVIGATION_DEFAULT__MAX_VELOCITY 5
#define NAVIGATION_DEFAULT__ACCELERATION 10
#define NAVIGATION_SPEED_DEFAULT__TURN   1
#define NAVIGATION_SPEED_DEFAULT__ORIENT 0.002f
#define NAVIGATION_SPEED_DEFAULT__ORBIT  0.002f
#define NAVIGATION_SPEED_DEFAULT__ZOOM   0.003f
#define NAVIGATION_SPEED_DEFAULT__DOLLY  1
#define NAVIGATION_SPEED_DEFAULT__PAN    0.03f

#define VIEWPORT_DEFAULT__NEAR_CLIPPING_PLANE_DISTANCE 0.001f
#define VIEWPORT_DEFAULT__FAR_CLIPPING_PLANE_DISTANCE 1000.0f

#define fractionOf(x) ((x) - floorf(x))
#define oneMinusFractionOf(x) (1 - fractionOf(x))

INLINE f32 clampedValue(f32 value, f32 from, f32 to) {
    f32 mn = value < to ? value : to;
    return mn > from ? mn : from;
}

INLINE i32 clampedValue(i32 value, i32 from, i32 to) {
    i32 mn = value < to ? value : to;
    return mn > from ? mn : from;
}

INLINE f32 clampedValue(f32 value, f32 to) {
    return value < to ? value : to;
}

INLINE i32 clampedValue(i32 value, i32 to) {
    return value < to ? value : to;
}

INLINE f32 clampedValue(f32 value) {
    f32 mn = value < 1.0f ? value : 1.0f;
    return mn > 0.0f ? mn : 0.0f;
}

INLINE i32 clampedValue(i32 value) {
    i32 mn = value < 1 ? value : 1;
    return mn > 0 ? mn : 0;
}

f32 smoothstep(f32 from, f32 to, f32 t) {
    t = (t - from) / (to - from);
    return t * t * (3 - 2 * t);
}

template <typename T>
INLINE void swap(T *a, T *b) {
    T t = *a;
    *a = *b;
    *b = t;
}

template <typename T>
struct RangeOf {
    T first, last;

    RangeOf() : RangeOf{0, 0} {}
    RangeOf(T first, T last) : first{first}, last{last} {}
    RangeOf(const RangeOf<T> &other) : RangeOf{other.first, other.last} {}

    INLINE bool contains(i32 v) const { return (first <= v) && (v <= last); }
    INLINE bool bounds(i32 v) const { return (first < v) && (v < last); }
    INLINE bool operator!() const { return last < first; }
    INLINE bool operator[](T v) const { return contains(v); }
    INLINE bool operator()(T v) const { return bounds(v); }
    INLINE void operator+=(T offset) {first += offset; last += offset;}
    INLINE void operator-=(T offset) {first -= offset; last -= offset;}
    INLINE void operator*=(T factor) {first *= factor; last *= factor;}
    INLINE void operator/=(T factor) {factor = 1 / factor; first *= factor; last *= factor;}
    INLINE void operator-=(const RangeOf<T> &rhs) { sub(rhs.first, rhs.last); }
    INLINE RangeOf<T> operator-(const RangeOf<T> &rhs) const {
        RangeOf<T> result{first, last};
        result.sub(rhs.first, rhs.last);
        return result;
    }
    INLINE void sub(T sub_first, T sub_last) {
        if (sub_last < sub_first) {
            T tmp = sub_last;
            sub_last = sub_first;
            sub_first = tmp;
        }
        if (last < sub_first || sub_last < first) {
            first = -1;
            last = -2;
        } else {
            first = first < sub_first ? sub_first : first;
            last = sub_last < last ? sub_last : last;
        }
    }
};
typedef RangeOf<f32> Range;
typedef RangeOf<i32> RangeI;

template <typename T>
struct RectOf {
    union {
        struct {
            T left;
            T right;
            T top;
            T bottom;
        };
        struct {
            RangeOf<T> x_range, y_range;
        };
    };

    RectOf(const RectOf<T> &other) : RectOf{other.x_range, other.y_range} {}
    RectOf(const RangeOf<T> &x_range, const RangeOf<T> &y_range) : x_range{x_range}, y_range{y_range} {}
    RectOf(T left = 0, T right = 0, T top = 0, T bottom = 0) : left{left}, right{right}, top{top}, bottom{bottom} {}

    INLINE bool contains(T x, T y) const { return x_range.contains(x) && y_range.contains(y); }
    INLINE bool bounds(T x, T y) const { return x_range.bounds(x) && y_range.bounds(y); }
    INLINE bool operator!() const { return !x_range || !y_range; }
    INLINE bool isOutsideOf(const RectOf<T> &other) {
        return (
                other.right < left || right < other.left ||
                other.bottom < top || bottom < other.top
        );
    }
    INLINE void operator+=(T offset) {x_range += offset; y_range += offset;}
    INLINE void operator-=(T offset) {x_range -= offset; y_range -= offset;}
    INLINE void operator*=(T factor) {x_range *= factor; y_range *= factor;}
    INLINE void operator/=(T factor) {x_range /= factor; y_range /= factor;}
    INLINE void operator-=(const RectOf<T> &rhs) { sub(rhs.x_range, rhs.y_range); }
    INLINE RectOf<T> operator-(const RectOf<T> &rhs) const {
        RectOf<T> result{x_range, y_range};
        result.sub(rhs.x_range, rhs.y_range);
        return result;
    }
    INLINE void sub(const RangeOf<T> &other_x_range, const RangeOf<T> &other_y_range) {
        x_range -= other_x_range;
        y_range -= other_y_range;
    }
};
typedef RectOf<f32> Rect;
typedef RectOf<i32> RectI;

struct Turn {
    bool right{false};
    bool left{false};
};

struct Move {
    bool right{false};
    bool left{false};
    bool up{false};
    bool down{false};
    bool forward{false};
    bool backward{false};
};

INLINE f32 smoothStep(f32 from, f32 to, f32 t) {
    t = (t - from) / (to - from);
    return t * t * (3.0f - 2.0f * t);
}

INLINE f32 approach(f32 src, f32 trg, f32 diff) {
    f32 out;

    out = src + diff; if (trg > out) return out;
    out = src - diff; if (trg < out) return out;

    return trg;
}

enum class CurveType {
    None = 0,

    Helix,
    Coil,

    Count
};
struct Curve {
    CurveType type{CurveType::None};
    f32 revolution_count{1}, thickness{0.1f};
};

enum GeometryType {
    GeometryType_None = 0,

    GeometryType_Mesh,
    GeometryType_Grid,
    GeometryType_Box,
    GeometryType_Curve,

    GeometryType_Count
};

enum BoxSide {
    NoSide = 0,
    Top    = 1,
    Bottom = 2,
    Left   = 4,
    Right  = 8,
    Front  = 16,
    Back   = 32
};

INLINE BoxSide getBoxSide(f32 x, f32 y, f32 z, u8 axis) {
    switch (axis) {
        case 0 : return x > 0 ? Right : Left;
        case 3 : return x > 0 ? Left : Right;
        case 1 : return y > 0 ? Top : Bottom;
        case 4 : return y > 0 ? Bottom : Top;
        case 2 : return z > 0 ? Front : Back;
        default: return z > 0 ? Back : Front;
    }
}

template <class T>
struct Orientation {
    T rotation{};

    Orientation() : rotation{T::Identity} {}
    explicit Orientation(f32 x_radians, f32 y_radians = 0, f32 z_radians = 0) {
        setRotation(x_radians, y_radians, z_radians);
    }

    INLINE void rotate(f32 x_radians, f32 y_radians, f32 z_radians) {
        setRotation(x + x_radians, y + y_radians, x + z_radians);
    }

    INLINE void rotate(f32 x_radians, f32 y_radians) {
        setRotation(x + x_radians, y + y_radians);
    }

    INLINE void setRotation(f32 x_radians, f32 y_radians, f32 z_radians) {
        x = x_radians;
        y = y_radians;
        z = z_radians;
        _update();
    }

    INLINE void setRotation(f32 x_radians, f32 y_radians) {
        x = x_radians;
        y = y_radians;
        _update();
    }

    INLINE void rotateAroundX(f32 radians) {
        setRotationAroundX(x + radians);
    }

    INLINE void rotateAroundY(f32 radians) {
        setRotationAroundY(y + radians);
    }

    INLINE void rotateAroundZ(f32 radians) {
        setRotationAroundZ(z + radians);
    }

    INLINE void setRotationAroundX(f32 radians) {
        x = radians;
        _update();
    }

    INLINE void setRotationAroundY(f32 radians) {
        y = radians;
        _update();
    }

    INLINE void setRotationAroundZ(f32 radians) {
        z = radians;
        _update();
    }

protected:
    f32 x, y, z;

    void _update() {
        rotation = T::Identity;
        if (z != 0.0f) rotation = T::RotationAroundZ(z);
        if (x != 0.0f) rotation *= T::RotationAroundX(x);
        if (y != 0.0f) rotation *= T::RotationAroundY(y);
    }
};

enum ColorID {
    Black,
    White,
    Grey,

    Red,
    Green,
    Blue,

    Cyan,
    Magenta,
    Yellow,

    DarkRed,
    DarkGreen,
    DarkBlue,
    DarkGrey,

    BrightRed,
    BrightGreen,
    BrightBlue,
    BrightGrey,

    BrightCyan,
    BrightMagenta,
    BrightYellow,

    DarkCyan,
    DarkMagenta,
    DarkYellow
};

struct Color {
    union {
        struct { f32 red, green, blue; };
        struct { f32 r  , g    , b   ; };
    };

    Color(f32 red = 0.0f, f32 green = 0.0f, f32 blue = 0.0f) : red{red}, green{green}, blue{blue} {}
    Color(enum ColorID color_id) : Color{} {
        switch (color_id) {
            case Black: break;
            case White:
                red = 1.0f;
                green = 1.0f;
                blue = 1.0f;
                break;
            case Grey:
                red = 0.5f;
                green = 0.5f;
                blue = 0.5f;
                break;
            case DarkGrey:
                red = 0.25f;
                green = 0.25f;
                blue = 0.25f;
                break;
            case BrightGrey:
                red = 0.75f;
                green = 0.75f;
                blue = 0.75f;
                break;

            case Red:
                red = 1.0f;
                break;
            case Green:
                green = 1.0f;
                break;
            case Blue:
                blue = 1.0f;
                break;

            case DarkRed:
                red = 0.5f;
                break;
            case DarkGreen:
                green = 0.5f;
                break;
            case DarkBlue:
                blue = 0.5f;
                break;

            case DarkCyan:
                green = 0.5f;
                blue = 0.5f;
                break;
            case DarkMagenta:
                red = 0.5f;
                blue = 0.5f;
                break;
            case DarkYellow:
                red = 0.5f;
                green = 0.5f;
                break;

            case BrightRed:
                red = 1.0f;
                green = 0.5f;
                blue = 0.5f;
                break;
            case BrightGreen:
                red = 0.5f;
                green = 1.0f;
                blue = 0.5f;
                break;
            case BrightBlue:
                red = 0.5f;
                green = 0.5f;
                blue = 1.0f;
                break;

            case Cyan:
                blue = 1.0f;
                green = 1.0f;
                break;
            case Magenta:
                red = 1.0f;
                blue = 1.0f;
                break;
            case Yellow:
                red = 1.0f;
                green = 1.0f;
                break;

            case BrightCyan:
                green = 0.75f;
                blue = 0.75f;
                break;
            case BrightMagenta:
                red = 0.75f;
                blue = 0.75f;
                break;
            case BrightYellow:
                red = 0.75f;
                green = 0.75f;
                break;
        }
    }

    INLINE Color& gammaCorrect() {
        r *= r;
        g *= g;
        b *= b;
        return *this;
    }

    INLINE Color operator + (const Color &rhs) const {
        return {
                r + rhs.r,
                g + rhs.g,
                b + rhs.b
        };
    }

    INLINE Color operator + (f32 scalar) const {
        return {
                r + scalar,
                g + scalar,
                b + scalar
        };
    }

    INLINE Color& operator += (const Color &rhs) {
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        return *this;
    }

    INLINE Color& operator += (f32 scalar) {
        r += scalar;
        g += scalar;
        b += scalar;
        return *this;
    }

    INLINE Color operator - (const Color &rhs) const {
        return {
                r - rhs.r,
                g - rhs.g,
                b - rhs.b
        };
    }

    INLINE Color operator - (f32 scalar) const {
        return {
                r - scalar,
                g - scalar,
                b - scalar
        };
    }

    INLINE Color& operator -= (const Color &rhs) {
        r -= rhs.r;
        g -= rhs.g;
        b -= rhs.b;
        return *this;
    }

    INLINE Color& operator -= (f32 scalar) {
        r -= scalar;
        g -= scalar;
        b -= scalar;
        return *this;
    }

    INLINE Color operator * (const Color &rhs) const {
        return {
                r * rhs.r,
                g * rhs.g,
                b * rhs.b
        };
    }

    INLINE Color operator * (f32 scalar) const {
        return {
                r * scalar,
                g * scalar,
                b * scalar
        };
    }

    INLINE Color& operator *= (const Color &rhs) {
        r *= rhs.r;
        g *= rhs.g;
        b *= rhs.b;
        return *this;
    }

    INLINE Color& operator *= (f32 scalar) {
        r *= scalar;
        g *= scalar;
        b *= scalar;
        return *this;
    }

    INLINE Color operator / (const Color &rhs) const {
        return {
                r / rhs.r,
                g / rhs.g,
                b / rhs.b
        };
    }

    INLINE Color operator / (f32 scalar) const {
        scalar = 1.0f / scalar;
        return {
                r * scalar,
                g * scalar,
                b * scalar
        };
    }

    INLINE Color& operator /= (const Color &rhs) {
        r /= rhs.r;
        g /= rhs.g;
        b /= rhs.b;
        return *this;
    }

    INLINE Color& operator /= (f32 scalar) {
        scalar = 1.0f / scalar;
        r *= scalar;
        g *= scalar;
        b *= scalar;
        return *this;
    }

    INLINE Color lerpTo(const Color &to, f32 by) const {
        return (to - *this).scaleAdd(by, *this);
    }

    INLINE Color scaleAdd(f32 factor, const Color &to_be_added) const {
        return {
                fast_mul_add(r, factor, to_be_added.r),
                fast_mul_add(g, factor, to_be_added.g),
                fast_mul_add(b, factor, to_be_added.b)
        };
    }

    INLINE Color blendWith(const Color &other_color, f32 factor, f32 other_factor) const {
        return {
                fast_mul_add(r, factor, other_color.r * other_factor),
                fast_mul_add(g, factor, other_color.g * other_factor),
                fast_mul_add(b, factor, other_color.b * other_factor)
        };
    }
};

struct Pixel {
    Color color;
    f32 opacity;

    Pixel(Color color, f32 opacity = 1.0f) : color{color}, opacity{opacity} {}
    Pixel(f32 red = 0.0f, f32 green = 0.0f, f32 blue = 0.0f, f32 opacity = 0.0f) : color{red, green, blue}, opacity{opacity} {}
    Pixel(enum ColorID color_id, f32 opacity = 1.0f) : Pixel{Color(color_id), opacity} {}

    INLINE Pixel operator * (f32 factor) const {
        return {
                color * factor,
                opacity * factor
        };
    }

    INLINE Pixel& operator += (const Pixel &rhs) {
        color += rhs.color;
        opacity += rhs.opacity;
        return *this;
    }

    INLINE Pixel alphaBlendOver(const Pixel &background, bool premultiplied) const {
        const f32 one_minus_opacity = 1.0f - opacity;
        const f32 background_opacity = background.opacity * one_minus_opacity;
        const f32 new_opacity = background_opacity + opacity;
        if (premultiplied)
            return {color + (background.color * one_minus_opacity), new_opacity};

        f32 one_over_opacity = new_opacity == 0 ? 0.0f : 1.0f / new_opacity;
        return {
                color.blendWith(
                        background.color,
                        one_over_opacity * opacity,
                        one_over_opacity * background_opacity
                ),
                new_opacity
        };
    }

    INLINE u32 asContent(bool premultiplied, bool gamma_corrected = false) const {
        if (premultiplied) {
            u8 R = (u8)(color.r > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * (gamma_corrected ? color.r : sqrtf(color.r))));
            u8 G = (u8)(color.g > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * (gamma_corrected ? color.g : sqrtf(color.g))));
            u8 B = (u8)(color.b > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * (gamma_corrected ? color.b : sqrtf(color.b))));
            return R << 16 | G << 8 | B;
        } else {
            u8 R = (u8)(color.r > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * (gamma_corrected ? (color.r * opacity) : sqrtf(color.r * opacity))));
            u8 G = (u8)(color.g > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * (gamma_corrected ? (color.g * opacity) : sqrtf(color.g * opacity))));
            u8 B = (u8)(color.b > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * (gamma_corrected ? (color.b * opacity) : sqrtf(color.b * opacity))));
            return R << 16 | G << 8 | B;
        }
    }
};

#define PIXEL_SIZE (sizeof(Pixel))
#define CANVAS_PIXELS_SIZE (MAX_WINDOW_SIZE * PIXEL_SIZE * 4)
#define CANVAS_DEPTHS_SIZE (MAX_WINDOW_SIZE * sizeof(f32) * 4)
#define CANVAS_SIZE (CANVAS_PIXELS_SIZE + CANVAS_DEPTHS_SIZE)

struct Dimensions {
    u32 width_times_height{(u32)DEFAULT_WIDTH * (u32)DEFAULT_HEIGHT};
    f32 width_over_height{(f32)DEFAULT_WIDTH / (f32)DEFAULT_HEIGHT};
    f32 height_over_width{(f32)DEFAULT_HEIGHT / (f32)DEFAULT_WIDTH};
    f32 f_width{(f32)DEFAULT_WIDTH};
    f32 f_height{(f32)DEFAULT_HEIGHT};
    f32 h_width{(f32)DEFAULT_WIDTH * 0.5f};
    f32 h_height{(f32)DEFAULT_HEIGHT * 0.5f};
    u16 width{DEFAULT_WIDTH};
    u16 height{DEFAULT_HEIGHT};
    u16 stride{DEFAULT_WIDTH};

    Dimensions() = default;
    Dimensions(u16 Width, u16 Height, u16 Stride) :
            width_times_height{(u32)Width * (u32)Height},
            width_over_height{(f32)Width / (f32)Height},
            height_over_width{(f32)Height / (f32)Width},
            f_width{(f32)Width},
            f_height{(f32)Height},
            h_width{(f32)Width * 0.5f},
            h_height{(f32)Height * 0.5f},
            width{Width},
            height{Height},
            stride{Stride}
    {}

    void update(u16 Width, u16 Height) {
        width = Width;
        height = Height;
        stride = Width;
        width_times_height = width * height;
        f_width  = (f32)width;
        f_height = (f32)height;
        h_width  = f_width  * 0.5f;
        h_height = f_height * 0.5f;
        width_over_height  = f_width  / f_height;
        height_over_width  = f_height / f_width;
    }
};

namespace controls {
    namespace key_map {
        u8 ctrl{0};
        u8 alt{0};
        u8 shift{0};
        u8 space{0};
        u8 tab{0};
        u8 escape{0};
        u8 left{0};
        u8 right{0};
        u8 up{0};
        u8 down{0};
    }

    namespace is_pressed {
        bool ctrl{false};
        bool alt{false};
        bool shift{false};
        bool space{false};
        bool tab{false};
        bool escape{false};
        bool left{false};
        bool right{false};
        bool up{false};
        bool down{false};
    }
}

namespace os {
    void* getMemory(u64 size);
    void setWindowTitle(char* str);
    void setWindowCapture(bool on);
    void setCursorVisibility(bool on);
    void closeFile(void *handle);
    void* openFileForReading(const char* file_path);
    void* openFileForWriting(const char* file_path);
    bool readFromFile(void *out, unsigned long, void *handle);
    bool writeToFile(void *out, unsigned long, void *handle);
}

namespace time {
    u64 getTicks();
    u64 ticks_per_second;
    f64 seconds_per_tick;
    f64 milliseconds_per_tick;
    f64 microseconds_per_tick;
    f64 nanoseconds_per_tick;

    struct Timer {
        f32 delta_time{0};

        u64 ticks_before{0};
        u64 ticks_after{0};
        u64 ticks_diff{0};
        u64 ticks_of_last_report{0};

        u64 accumulated_ticks{0};
        u64 accumulated_frame_count{0};

        u64 seconds{0};
        u64 milliseconds{0};
        u64 microseconds{0};
        u64 nanoseconds{0};

        f64 average_frames_per_tick{0.0};
        f64 average_ticks_per_frame{0.0};
        u16 average_frames_per_second{0};
        u16 average_milliseconds_per_frame{0};
        u16 average_microseconds_per_frame{0};
        u16 average_nanoseconds_per_frame{0};

        Timer() noexcept : ticks_before{getTicks()}, ticks_of_last_report{getTicks()} {};

        INLINE void accumulate() {
            ticks_diff = ticks_after - ticks_before;
            accumulated_ticks += ticks_diff;
            accumulated_frame_count++;

            seconds = (u64) (seconds_per_tick * (f64) (ticks_diff));
            milliseconds = (u64) (milliseconds_per_tick * (f64) (ticks_diff));
            microseconds = (u64) (microseconds_per_tick * (f64) (ticks_diff));
            nanoseconds = (u64) (nanoseconds_per_tick * (f64) (ticks_diff));
        }

        INLINE void average() {
            average_frames_per_tick = (f64) accumulated_frame_count / (f64) accumulated_ticks;
            average_ticks_per_frame = (f64) accumulated_ticks / (f64) accumulated_frame_count;
            average_frames_per_second = (u16) (average_frames_per_tick * (f64) ticks_per_second);
            average_milliseconds_per_frame = (u16) (average_ticks_per_frame * milliseconds_per_tick);
            average_microseconds_per_frame = (u16) (average_ticks_per_frame * microseconds_per_tick);
            average_nanoseconds_per_frame = (u16) (average_ticks_per_frame * nanoseconds_per_tick);
            accumulated_ticks = accumulated_frame_count = 0;
        }

        INLINE void beginFrame() {
            ticks_after = ticks_before;
            ticks_before = getTicks();
            ticks_diff = ticks_before - ticks_after;
            delta_time = (f32) ((f64) ticks_diff * seconds_per_tick);
        }

        INLINE void endFrame() {
            ticks_after = getTicks();
            accumulate();
            if (accumulated_ticks >= (ticks_per_second / 8))
                average();
        }
    };
}

namespace mouse {
    struct Button {
        i32 down_pos_x, down_pos_y, up_pos_x, up_pos_y, double_click_pos_x, double_click_pos_y;

        bool is_pressed{false}, double_clicked{false};

        void down(i32 x, i32 y) {
            is_pressed = true;

            down_pos_x = x;
            down_pos_y = y;
        }

        void up(i32 x, i32 y) {
            is_pressed = false;

            up_pos_x = x;
            up_pos_y = y;
        }

        void doubleClick(i32 x, i32 y) {
            double_clicked = true;
            double_click_pos_x = x;
            double_click_pos_y = y;
        }
    };

    Button middle_button, right_button, left_button;

    i32 pos_x, pos_y, pos_raw_diff_x, pos_raw_diff_y, movement_x, movement_y;
    f32 wheel_scroll_amount{0};

    bool moved{false};
    bool is_captured{false};
    bool double_clicked{false};
    bool wheel_scrolled{false};

    void resetChanges() {
        moved = false;
        double_clicked = false;
        wheel_scrolled = false;
        wheel_scroll_amount = 0;
        pos_raw_diff_x = 0;
        pos_raw_diff_y = 0;
        right_button.double_clicked = false;
        left_button.double_clicked = false;
        middle_button.double_clicked = false;
    }

    void scroll(f32 amount) {
        wheel_scroll_amount += amount * 100;
        wheel_scrolled = true;
    }

    void setPosition(i32 x, i32 y) {
        pos_x = x;
        pos_y = y;
    }

    void move(i32 x, i32 y) {
        movement_x = x - pos_x;
        movement_y = y - pos_y;
        moved = true;
    }

    void moveRaw(i32 x, i32 y) {
        pos_raw_diff_x += x;
        pos_raw_diff_y += y;
        moved = true;
    }
}

namespace os {
    void* getMemory(u64 size);
    void setWindowTitle(char* str);
    void setWindowCapture(bool on);
    void setCursorVisibility(bool on);
    void closeFile(void *handle);
    void* openFileForReading(const char* file_path);
    void* openFileForWriting(const char* file_path);
    bool readFromFile(void *out, unsigned long, void *handle);
    bool writeToFile(void *out, unsigned long, void *handle);
}

namespace memory {
    u8 *canvas_memory{nullptr};
    u64 canvas_memory_capacity = CANVAS_SIZE * CANVAS_COUNT;

    typedef void* (*AllocateMemory)(u64 size);

    struct MonotonicAllocator {
        u8* address{nullptr};
        u64 capacity{0};
        u64 occupied{0};

        MonotonicAllocator() = default;

        explicit MonotonicAllocator(u64 Capacity) {
            capacity = Capacity;
            address = (u8*)os::getMemory(Capacity);
        }

        void* allocate(u64 size) {
            if (!address) return nullptr;
            occupied += size;
            if (occupied > capacity) return nullptr;

            void* current_address = address;
            address += size;
            return current_address;
        }
    };
}

namespace window {
    u16 width{DEFAULT_WIDTH};
    u16 height{DEFAULT_HEIGHT};
    char* title{(char*)""};
    u32 *content{nullptr};
}

struct String {
    u32 length;
    char *char_ptr;

    String() noexcept : String{(char*)"", 0} {}
    String(char *char_ptr) noexcept : length{getLength(char_ptr)}, char_ptr{char_ptr} {}
    String(char *char_ptr, u32 length) noexcept : length{length}, char_ptr{char_ptr} {}

    static String getFilePath(char *file_name, char *buffer, char *adjacent_file) {
        String str(buffer);
        u32 offset = getDirectoryLength(adjacent_file);
        str.copyFrom(adjacent_file, file_name, offset);
        return str;
    }

    void copyFrom(char* CharPtr, u32 offset) {
        length = offset;
        char *source_char = CharPtr;
        char *string_char = char_ptr + offset;
        while (source_char[0]) {
            *string_char = *source_char;
            string_char++;
            source_char++;
            length++;
        }
        *string_char = 0;
    }

    void copyFrom(char* CharPtr1, char* CharPtr2, u32 offset) {
        copyFrom(CharPtr1, 0);
        copyFrom(CharPtr2, offset);
    }

    String& operator = (char* CharPtr) {
        char_ptr = CharPtr;
        length = getLength(char_ptr);
        return *this;
    }

    static u32 getLength(char *string) {
        char *ptr = string;
        u32 length = 0;
        if (ptr) while (ptr[length]) length++;
        return length;
    }

    static u32 getDirectoryLength(char *path) {
        u32 path_len = getLength(path);
        u32 dir_len = path_len;
        while ((path[dir_len] != '/') && (path[dir_len] != '\\')) dir_len--;
        return dir_len + 1;
    }
};

struct NumberString {
    u8 float_digits_count = 3;
    String string{_buffer, 0};
    char _buffer[13]{};

    explicit NumberString(u8 digit_count = 3) : float_digits_count{digit_count}, string{_buffer, 1} {
        _buffer[12] = 0;
        for (u8 i = 0; i < 12; i++)
            _buffer[i] = ' ';
    }

    explicit NumberString(const char *str, u8 digits_count = 3) : float_digits_count{digits_count}, string{_buffer, 1} {
        _buffer[12] = 0;
        char *char_ptr = (char*)str;
        string.length = (u8)String::getLength(char_ptr);
        if (string.length > 12) string.length = 12;
        if ((*str >= '0') && (*str <= '9')) {
            char_ptr += string.length;
            char_ptr--;
            for (i32 i = 11; i >= 0; i--, char_ptr--)
                _buffer[i] = (11 - i) < float_digits_count ? *char_ptr : ' ';
        } else {
            for (u8 i = 0; i < string.length; i++, char_ptr++) _buffer[i] = *char_ptr;
            _buffer[string.length] = 0;
        }
    }

    NumberString& operator = (i32 number) {
        char *buffer = _buffer;
        buffer[12] = 0;

        bool is_negative = number < 0;
        if (is_negative) number = -number;

        if (number) {
            u32 temp;
            buffer += 11;
            string.char_ptr = buffer;
            string.length = 0;

            for (u8 i = 0; i < 11; i++) {
                temp = number;
                number /= 10;
                string.length++;
                *buffer-- = (char)('0' + temp - number * 10);
                if (!number) {
                    if (is_negative) {
                        *buffer = '-';
                        string.char_ptr--;
                        string.length++;
                    }

                    break;
                }
                string.char_ptr--;
            }
        } else {
            buffer[11] = '0';
            string.length = 1;
            string.char_ptr = buffer + 11;
        }

        return *this;
    }

    NumberString& operator = (f32 number) {
        f32 factor = 1;
        for (u8 i = 0; i < float_digits_count; i++) factor *= 10;
        i32 int_num = (i32)(number * factor);
        if (int_num == 0) {
            *this = (i32)factor;
            string.length++;
            string.char_ptr[0] = '.';
            string.char_ptr--;
            string.char_ptr[0] = '0';
            return *this;
        }

        bool is_negative = number < 0;
        bool is_fraction = is_negative ? number > -1 : number < 1;

        *this = int_num;

        if (is_fraction) {
            u32 len = string.length;
            string.length++;
            string.char_ptr--;
            if (is_negative) {
                string.char_ptr[0] = '-';
                string.char_ptr[1] = '0';
            } else
                string.char_ptr[0] = '0';

            if (len < float_digits_count) {
                for (u32 i = 0; i < (float_digits_count - len); i++) {
                    string.length++;
                    string.char_ptr--;
                    string.char_ptr[0] = '0';
                }
            }
        }

        static char tmp[13];
        tmp[string.length + 1] = 0;
        for (u8 i = 0; i < (u8)string.length; i++) {
            u8 char_count_from_right_to_left = (u8)string.length - i - 1;
            if (char_count_from_right_to_left >= float_digits_count) tmp[i] = string.char_ptr[i];
            else                                                     tmp[i + 1] = string.char_ptr[i];
        }
        tmp[string.length - float_digits_count] = '.';
        string.copyFrom(tmp, 0);
        if (is_negative) string.length++;

        return *this;
    }
};

struct HUDLine {
    String title{}, alternate_value{};
    NumberString value{};
    enum ColorID title_color{BrightGrey};
    enum ColorID value_color{BrightGrey};
    enum ColorID alternate_value_color{Grey};
    bool *use_alternate{nullptr};
    bool invert_alternate_use{false};

    HUDLine(enum ColorID default_color = BrightGrey) :
            title{},
            alternate_value{},
            value{},
            title_color{default_color},
            value_color{default_color} {}
    HUDLine(char* title_char_ptr,
            enum ColorID default_color = BrightGrey) :
            title{title_char_ptr},
            alternate_value{},
            value{},
            title_color{default_color},
            value_color{default_color} {}
    HUDLine(char* title_char_ptr,
            char* value_char_ptr,
            enum ColorID default_color = BrightGrey) :
            title{title_char_ptr},
            alternate_value{},
            value{value_char_ptr},
            title_color{default_color},
            alternate_value_color{default_color}
    {}
    HUDLine(char* title_char_ptr,
            char* value_char_ptr,
            char* alternate_value_char_ptr,
            bool *use_alternate = nullptr,
            bool invert_alternate_use = false,
            enum ColorID value_color = BrightGrey,
            enum ColorID alternate_value_color = Grey,
            enum ColorID title_color = BrightGrey) :
            title{title_char_ptr},
            alternate_value{alternate_value_char_ptr},
            value{value_char_ptr},
            title_color{title_color},
            value_color{value_color},
            alternate_value_color{alternate_value_color},
            use_alternate{use_alternate},
            invert_alternate_use{invert_alternate_use}
    {}
};

struct HUDSettings {
    u32 line_count{0};
    f32 line_height{1.0f};
    enum ColorID default_color{BrightGrey};

    HUDSettings(u32 line_count = 0,
                f32 line_height = 1.0f,
                ColorID default_color = BrightGrey) : line_count{line_count}, line_height{line_height}, default_color{default_color} {}
};
struct HUD {
    HUDSettings settings;
    HUDLine *lines = nullptr;
    i32 left = 10, top = 10;
    bool enabled{true};

    HUD() = default;
    HUD(HUDSettings settings,
        HUDLine *lines,
        i32 left = 10,
        i32 top = 10) :
            settings{settings}, lines{lines}, left{left}, top{top} {
        if (settings.default_color != BrightGrey) for (u32 i = 0; i < settings.line_count; i++)
                lines[i].title_color = lines[i].alternate_value_color = lines[i].value_color = settings.default_color;
    }
    HUD(HUDSettings settings, memory::AllocateMemory allocate_memory = nullptr, i32 left = 10, i32 top = 10) : settings{settings}, left{left}, top{top} {
        if (settings.line_count) {
            lines = (HUDLine*)allocate_memory(settings.line_count * sizeof(HUDLine));
            for (u32 i = 0; i < settings.line_count; i++)
                lines[i] = HUDLine{settings.default_color};
        }
    }
};



#define SLIM_VEC2

struct vec2i {
    i32 x, y;

    vec2i() noexcept : vec2i{0} {}
    vec2i(i32 x, i32 y) noexcept : x(x), y(y) {}
    vec2i(vec2i &other) noexcept : vec2i{other.x, other.y} {}
    vec2i(const vec2i &other) noexcept : vec2i{other.x, other.y} {}
    explicit vec2i(i32 value) noexcept : vec2i{value, value} {}

    INLINE bool operator == (const vec2i &other) const {
        return (other.x == x) &&
               (other.y == y);
    }

    INLINE vec2i & operator = (f32 value) {
        x = y = (i32)value;
        return *this;
    }

    INLINE vec2i & operator = (i32 value) {
        x = y = value;
        return *this;
    }

    INLINE bool operator ! () const {
        return nonZero();
    }

    INLINE vec2i operator - () const {
        return {
                -x,
                -y
        };
    }

    INLINE vec2i operator - (const vec2i &rhs) const {
        return {
                x - rhs.x,
                y - rhs.y
        };
    }

    INLINE vec2i operator + (const vec2i &rhs) const {
        return {
                x + rhs.x,
                y + rhs.y
        };
    }

    INLINE vec2i operator * (const vec2i &rhs) const {
        return {
                x * rhs.x,
                y * rhs.y
        };
    }

    INLINE vec2i operator / (const vec2i &rhs) const {
        return {
                x / rhs.x,
                y / rhs.y
        };
    }

    INLINE vec2i& operator -= (const vec2i &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    INLINE vec2i& operator += (const vec2i &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    INLINE vec2i& operator *= (const vec2i &rhs) {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    INLINE vec2i& operator /= (const vec2i &rhs) {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    INLINE vec2i operator - (i32 rhs) const {
        return {
                x - rhs,
                y - rhs
        };
    }

    INLINE vec2i operator + (i32 rhs) const {
        return {
                x + rhs,
                y + rhs
        };
    }

    INLINE vec2i operator * (i32 rhs) const {
        return {
                x * rhs,
                y * rhs
        };
    }

    INLINE vec2i operator / (i32 rhs) const {
        return {
                x / rhs,
                y / rhs
        };
    }

    INLINE vec2i& operator -= (i32 rhs) {
        x -= rhs;
        y -= rhs;
        return *this;
    }

    INLINE vec2i& operator += (i32 rhs) {
        x += rhs;
        y += rhs;
        return *this;
    }

    INLINE vec2i& operator *= (i32 rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    INLINE vec2i& operator /= (i32 rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    INLINE vec2i operator - (f32 rhs) const {
        return {
                (i32)((f32)x - rhs),
                (i32)((f32)y - rhs)
        };
    }

    INLINE vec2i operator + (f32 rhs) const {
        return {
                (i32)((f32)x + rhs),
                (i32)((f32)y + rhs)
        };
    }

    INLINE vec2i operator * (f32 rhs) const {
        return {
                (i32)((f32)x * rhs),
                (i32)((f32)y * rhs)
        };
    }

    INLINE vec2i operator / (f32 rhs) const {
        return {
                (i32)((f32)x / rhs),
                (i32)((f32)y / rhs)
        };
    }

    INLINE vec2i& operator -= (f32 rhs) {
        x -= (i32)rhs;
        y -= (i32)rhs;
        return *this;
    }

    INLINE vec2i& operator += (f32 rhs) {
        x += (i32)rhs;
        y += (i32)rhs;
        return *this;
    }

    INLINE vec2i& operator *= (f32 rhs) {
        x *= (i32)rhs;
        y *= (i32)rhs;
        return *this;
    }

    INLINE vec2i& operator /= (f32 rhs) {
        x /= (i32)rhs;
        y /= (i32)rhs;
        return *this;
    }

    INLINE bool nonZero() const {
        return x != 0 ||
               y != 0;
    }

    INLINE i32 minimum() const {
        return x < y ? x : y;
    }

    INLINE i32 maximum() const {
        return x > y ? x : y;
    }

    INLINE vec2i clamped() const {
        return {
                clampedValue(x),
                clampedValue(y)
        };
    }

    INLINE vec2i clamped(const vec2i &upper) const {
        return {
                clampedValue(x, upper.x),
                clampedValue(y, upper.y)
        };
    }

    INLINE vec2i clamped(const f32 min_value, const f32 max_value) const {
        return {
                (i32)(clampedValue((f32)x, min_value, max_value)),
                (i32)(clampedValue((f32)y, min_value, max_value))
        };
    }

    INLINE vec2i clamped(const i32 min_value, const i32 max_value) const {
        return {
                clampedValue(x, min_value, max_value),
                clampedValue(y, min_value, max_value)
        };
    }

    INLINE vec2i approachTo(const vec2i &trg, f32 diff) const {
        return {
                (i32)(approach((f32)x, (f32)trg.x, diff)),
                (i32)(approach((f32)y, (f32)trg.y, diff))
        };
    }

    INLINE vec2i scaleAdd(f32 factor, const vec2i &to_be_added) const {
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

    vec2() : vec2{0} {}
    vec2(f32 x, f32 y) noexcept : x(x), y(y) {}
    vec2(i32 x, i32 y) noexcept : x((f32)x), y((f32)y) {}
    vec2(vec2 &other) noexcept : vec2{other.x, other.y} {}
    vec2(const vec2 &other) noexcept : vec2{other.x, other.y} {}
    explicit vec2(f32 value) noexcept : vec2{value, value} {}
    explicit vec2(vec2i &other) noexcept : vec2{(f32)other.x, (f32)other.y} {}
    explicit vec2(const vec2i &other) noexcept : vec2{(f32)other.x, (f32)other.y} {}

    INLINE bool operator == (const vec2 &other) const {
        return (other.x == x) &&
               (other.y == y);
    }

    INLINE vec2 & operator = (f32 value) {
        x = y = value;
        return *this;
    }

    INLINE vec2 & operator = (i32 value) {
        x = y = (f32)value;
        return *this;
    }

    INLINE bool operator ! () const {
        return nonZero();
    }

    INLINE vec2 operator - () const {
        return {
                -x,
                -y
        };
    }

    INLINE vec2 operator - (const vec2 &rhs) const {
        return {
                x - rhs.x,
                y - rhs.y
        };
    }

    INLINE vec2 operator + (const vec2 &rhs) const {
        return {
                x + rhs.x,
                y + rhs.y
        };
    }

    INLINE vec2 operator * (const vec2 &rhs) const {
        return {
                x * rhs.x,
                y * rhs.y
        };
    }

    INLINE vec2 operator / (const vec2 &rhs) const {
        return {
                x / rhs.x,
                y / rhs.y
        };
    }

    INLINE vec2& operator -= (const vec2 &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    INLINE vec2& operator += (const vec2 &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    INLINE vec2& operator *= (const vec2 &rhs) {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    INLINE vec2& operator /= (const vec2 &rhs) {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    INLINE vec2 operator - (f32 rhs) const {
        return {
                x - rhs,
                y - rhs
        };
    }

    INLINE vec2 operator + (f32 rhs) const {
        return {
                x + rhs,
                y + rhs
        };
    }

    INLINE vec2 operator * (f32 rhs) const {
        return {
                x * rhs,
                y * rhs
        };
    }

    INLINE vec2 operator / (f32 rhs) const {
        f32 factor = 1.0f / rhs;
        return {
                x * factor,
                y * factor
        };
    }

    INLINE vec2& operator -= (f32 rhs) {
        x -= rhs;
        y -= rhs;
        return *this;
    }

    INLINE vec2& operator += (f32 rhs) {
        x += rhs;
        y += rhs;
        return *this;
    }

    INLINE vec2& operator *= (f32 rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    INLINE vec2& operator /= (f32 rhs) {
        f32 factor = 1.0f / rhs;
        x *= factor;
        y *= factor;
        return *this;
    }

    INLINE vec2 operator - (i32 rhs) const {
        return {
                x - (f32)rhs,
                y - (f32)rhs
        };
    }

    INLINE vec2 operator + (i32 rhs) const {
        return {
                x + (f32)rhs,
                y + (f32)rhs
        };
    }

    INLINE vec2 operator * (i32 rhs) const {
        return {
                x * (f32)rhs,
                y * (f32)rhs
        };
    }

    INLINE vec2 operator / (i32 rhs) const {
        f32 factor = 1.0f / (f32)rhs;
        return {
                x * factor,
                y * factor
        };
    }

    INLINE vec2& operator -= (i32 rhs) {
        x -= (f32)rhs;
        y -= (f32)rhs;
        return *this;
    }

    INLINE vec2& operator += (i32 rhs) {
        x += (f32)rhs;
        y += (f32)rhs;
        return *this;
    }

    INLINE vec2& operator *= (i32 rhs) {
        x *= (f32)rhs;
        y *= (f32)rhs;
        return *this;
    }

    INLINE vec2& operator /= (i32 rhs) {
        f32 factor = 1.0f / (f32)rhs;
        x *= factor;
        y *= factor;
        return *this;
    }

    INLINE vec2 operator - (const vec2i &rhs) const {
        return {
                x - (f32)rhs.x,
                y - (f32)rhs.y
        };
    }

    INLINE vec2 operator + (const vec2i &rhs) const {
        return {
                x + (f32)rhs.x,
                y + (f32)rhs.y
        };
    }

    INLINE vec2 operator * (const vec2i &rhs) const {
        return {
                x * (f32)rhs.x,
                y * (f32)rhs.y
        };
    }

    INLINE vec2 operator / (const vec2i &rhs) const {
        return {
                x / (f32)rhs.x,
                y / (f32)rhs.y
        };
    }

    INLINE vec2& operator -= (const vec2i &rhs) {
        x -= (f32)rhs.x;
        y -= (f32)rhs.y;
        return *this;
    }

    INLINE vec2& operator += (const vec2i &rhs) {
        x += (f32)rhs.x;
        y += (f32)rhs.y;
        return *this;
    }

    INLINE vec2& operator *= (const vec2i &rhs) {
        x *= (f32)rhs.x;
        y *= (f32)rhs.y;
        return *this;
    }

    INLINE vec2& operator /= (const vec2i &rhs) {
        x /= (f32)rhs.x;
        y /= (f32)rhs.y;
        return *this;
    }

    INLINE bool nonZero() const {
        return x != 0.0f ||
               y != 0.0f;
    }

    INLINE f32 minimum() const {
        return x < y ? x : y;
    }

    INLINE f32 maximum() const {
        return x > y ? x : y;
    }

    INLINE vec2 perp() const {
        return {
                -y,
                x
        };
    }

    INLINE f32 dot(const vec2 &rhs) const {
        return (x * rhs.x) + (y * rhs.y);
    }

    INLINE f32 cross(const vec2 &rhs) const {
        return (x * rhs.y) - (y * rhs.x);
    }

    INLINE f32 squaredLength() const {
        return x*x + y*y;
    }

    INLINE f32 length() const {
        return sqrtf(squaredLength());
    }

    INLINE vec2 normalized() const {
        return *this / length();
    }

    INLINE vec2 reflectedAround(const vec2 &N) const {
        return N.scaleAdd(-2 * dot(N), *this);
    }

    INLINE vec2 clamped() const {
        return {
                clampedValue(x),
                clampedValue(y)
        };
    }

    INLINE vec2 clamped(const vec2 &upper) const {
        return {
                clampedValue(x, upper.x),
                clampedValue(y, upper.y)
        };
    }

    INLINE vec2 clamped(const vec2 &lower, const vec2 &upper) const {
        return {
                clampedValue(x, lower.x, upper.x),
                clampedValue(y, lower.y, upper.y)
        };
    }

    INLINE vec2 clamped(const f32 min_value, const f32 max_value) const {
        return {
                clampedValue(x, min_value, max_value),
                clampedValue(y, min_value, max_value)
        };
    }

    INLINE vec2 approachTo(const vec2 &trg, f32 diff) const {
        return {
                approach(x, trg.x, diff),
                approach(y, trg.y, diff)
        };
    }

    INLINE vec2 lerpTo(const vec2 &to, f32 by) const {
        return (to - *this).scaleAdd(by, *this);
    }

    INLINE vec2 scaleAdd(f32 factor, const vec2 &to_be_added) const {
        return {
                fast_mul_add(x, factor, to_be_added.x),
                fast_mul_add(y, factor, to_be_added.y)
        };
    }

    INLINE vec2 mulAdd(const vec2 &factors, const vec2 &to_be_added) const {
        return {
                fast_mul_add(x, factors.x, to_be_added.x),
                fast_mul_add(y, factors.y, to_be_added.y)
        };
    }
};

vec2 vec2::X{1.0f, 0.0f};
vec2 vec2::Y{0.0f, 1.0f};

INLINE vec2 minimum(const vec2 &a, const vec2 &b) {
    return {
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y
    };
}

INLINE vec2 maximum(const vec2 &a, const vec2 &b) {
    return {
            a.x > b.x ? a.x : b.x,
            a.y > b.y ? a.y : b.y
    };
}

INLINE vec2i minimum(const vec2i &a, const vec2i &b) {
    return {
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y
    };
}

INLINE vec2i maximum(const vec2i &a, const vec2i &b) {
    return {
            a.x > b.x ? a.x : b.x,
            a.y > b.y ? a.y : b.y
    };
}

INLINE vec2i operator - (const vec2i &lhs, const vec2 &rhs) {
    return {
            lhs.x - (i32)rhs.x,
            lhs.y - (i32)rhs.y
    };
}

INLINE vec2i operator + (const vec2i &lhs, const vec2 &rhs) {
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

INLINE vec2i operator / (const vec2i &lhs, const vec2 &rhs) {
    return {
            lhs.x / (i32)rhs.x,
            lhs.y / (i32)rhs.y
    };
}

INLINE vec2i operator - (i32 lhs, const vec2 &rhs) {
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

INLINE vec2i operator / (i32 lhs, const vec2 &rhs) {
    return {
            lhs / (i32)rhs.x,
            lhs / (i32)rhs.y
    };
}

INLINE vec2i operator - (i32 lhs, const vec2i &rhs) {
    return {
            lhs - rhs.x,
            lhs - rhs.y
    };
}

INLINE vec2i operator + (i32 lhs, const vec2i &rhs) {
    return {
            lhs + rhs.x,
            lhs + rhs.y
    };
}

INLINE vec2i operator * (i32 lhs, const vec2i &rhs) {
    return {
            lhs * rhs.x,
            lhs * rhs.y
    };
}

INLINE vec2i operator / (i32 lhs, const vec2i &rhs) {
    return {
            lhs / rhs.x,
            lhs / rhs.y
    };
}

INLINE vec2 operator - (f32 lhs, const vec2 &rhs) {
    return {
            lhs - rhs.x,
            lhs - rhs.y
    };
}

INLINE vec2 operator + (f32 lhs, const vec2 &rhs) {
    return {
            lhs + rhs.x,
            lhs + rhs.y
    };
}

INLINE vec2 operator / (f32 lhs, const vec2 &rhs) {
    return {
            lhs / rhs.x,
            lhs / rhs.y
    };
}

INLINE vec2 operator * (f32 lhs, const vec2 &rhs) {
    return {
            lhs * rhs.x,
            lhs * rhs.y
    };
}

INLINE vec2 lerp(const vec2 &from, const vec2 &to, f32 by) {
    return (to - from).scaleAdd(by, from);
}


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
    vec3(f32 value) noexcept : vec3{value, value, value} {}
    vec3(enum ColorID color_id) noexcept : vec3{Color{color_id}} {}
    vec3(const Color &color) noexcept : vec3{color.red, color.green, color.blue} {}

    INLINE vec3& operator = (f32 value) {
        x = y = z = value;
        return *this;
    }

    INLINE vec3& operator = (i32 value) {
        x = y = z = (f32)value;
        return *this;
    }

    INLINE Color toColor() const {
        return {r, g, b};
    }

    INLINE bool operator == (const vec3 &other) const {
        return (other.x == x) &&
               (other.y == y) &&
               (other.z == z);
    }

    INLINE bool operator ! () const {
        return nonZero();
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

    INLINE f32 minimum() const {
        return x < y ? (x < z ? x : z) : (y < z ? y : z);
    }

    INLINE f32 maximum() const {
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

    INLINE vec3 reflectedAround(const vec3 &N) const {
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

INLINE vec3 minimum(const vec3 &a, const vec3 &b) {
    return {
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y,
            a.z < b.y ? a.z : b.z
    };
}

INLINE vec3 maximum(const vec3 &a, const vec3 &b) {
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


struct Edge {
    vec3 from, to;
};

struct AABB {
    vec3 min = -1;
    vec3 max = +1;

    AABB(f32 min_x, f32 min_y, f32 min_z,
         f32 max_x, f32 max_y, f32 max_z) : AABB{
            vec3{min_x, min_y, min_z},
            vec3{max_x, max_y, max_z}
    } {}
    AABB(f32 min_value, f32 max_value) : AABB{vec3{min_value}, vec3{max_value}} {}
    AABB(const vec3 &min, const vec3 &max) : min{min}, max{max} {}
    AABB() : AABB{0, 0} {}
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


struct vec4 {
    union {
        struct {f32 components[4]; };
        struct {f32 x, y, z, w; };
        struct {f32 r, g, b, a; };
    };
    static vec4 X, Y, Z, W;

    vec4() noexcept : vec4{0} {}
    vec4(f32 x, f32 y, f32 z, f32 w) noexcept : x(x), y(y), z(z), w(w) {}
    vec4(vec4 &other) noexcept : vec4{other.x, other.y, other.z, other.w} {}
    vec4(const vec4 &other) noexcept : vec4{other.x, other.y, other.z, other.w} {}
    explicit vec4(f32 value) noexcept : vec4{value, value, value, value} {}

    INLINE bool operator == (const vec4 &other) const {
        return (other.x == x) &&
               (other.y == y) &&
               (other.z == z) &&
               (other.w == w);
    }

    INLINE vec4& operator = (f32 value) {
        x = y = z = value;
        return *this;
    }

    INLINE vec4& operator = (i32 value) {
        x = y = z = (f32)value;
        return *this;
    }

    INLINE bool operator ! () const {
        return nonZero();
    }

    INLINE vec4 operator - () const {
        return {
                -x,
                -y,
                -z,
                -w
        };
    }

    INLINE vec4 operator - (const vec4 &rhs) const {
        return {
                x - rhs.x,
                y - rhs.y,
                z - rhs.z,
                w - rhs.w
        };
    }

    INLINE vec4 operator + (const vec4 &rhs) const {
        return {
                x + rhs.x,
                y + rhs.y,
                z + rhs.z,
                w + rhs.w
        };
    }

    INLINE vec4 operator * (const vec4 &rhs) const {
        return {
                x * rhs.x,
                y * rhs.y,
                z * rhs.z,
                w * rhs.w
        };
    }

    INLINE vec4 operator / (const vec4 &rhs) const {
        return {
                x / rhs.x,
                y / rhs.y,
                z / rhs.z,
                w / rhs.w
        };
    }

    INLINE vec4& operator -= (const vec4 &rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    INLINE vec4& operator += (const vec4 &rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    INLINE vec4& operator *= (const vec4 &rhs) {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;
        return *this;
    }

    INLINE vec4& operator /= (const vec4 &rhs) {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        w /= rhs.w;
        return *this;
    }

    INLINE vec4 operator - (f32 rhs) const {
        return {
                x - rhs,
                y - rhs,
                z - rhs,
                w - rhs
        };
    }

    INLINE vec4 operator + (f32 rhs) const {
        return {
                x + rhs,
                y + rhs,
                z + rhs,
                w + rhs
        };
    }

    INLINE vec4 operator * (f32 rhs) const {
        return {
                x * rhs,
                y * rhs,
                z * rhs,
                w * rhs
        };
    }

    INLINE vec4 operator / (f32 rhs) const {
        f32 factor = 1.0f / rhs;
        return {
                x * factor,
                y * factor,
                z * factor,
                w * factor
        };
    }

    INLINE vec4& operator -= (f32 rhs) {
        x -= rhs;
        y -= rhs;
        z -= rhs;
        w -= rhs;
        return *this;
    }

    INLINE vec4& operator += (f32 rhs) {
        x += rhs;
        y += rhs;
        z += rhs;
        w += rhs;
        return *this;
    }

    INLINE vec4& operator *= (f32 rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }

    INLINE vec4& operator /= (f32 rhs) {
        f32 factor = 1.0f / rhs;
        x *= factor;
        y *= factor;
        z *= factor;
        w *= factor;
        return *this;
    }

    INLINE vec4 operator - (i32 rhs) const {
        return {
                x - (f32)rhs,
                y - (f32)rhs,
                z - (f32)rhs,
                w - (f32)rhs
        };
    }

    INLINE vec4 operator + (i32 rhs) const {
        return {
                x + (f32)rhs,
                y + (f32)rhs,
                z + (f32)rhs,
                w + (f32)rhs
        };
    }

    INLINE vec4 operator * (i32 rhs) const {
        return {
                x * (f32)rhs,
                y * (f32)rhs,
                z * (f32)rhs,
                w * (f32)rhs
        };
    }

    INLINE vec4 operator / (i32 rhs) const {
        f32 factor = 1.0f / (f32)rhs;
        return {
                x * factor,
                y * factor,
                z * factor,
                w * factor
        };
    }

    INLINE vec4& operator -= (i32 rhs) {
        x -= (f32)rhs;
        y -= (f32)rhs;
        z -= (f32)rhs;
        w -= (f32)rhs;
        return *this;
    }

    INLINE vec4& operator += (i32 rhs) {
        x += (f32)rhs;
        y += (f32)rhs;
        z += (f32)rhs;
        w += (f32)rhs;
        return *this;
    }

    INLINE vec4& operator *= (i32 rhs) {
        x *= (f32)rhs;
        y *= (f32)rhs;
        z *= (f32)rhs;
        w *= (f32)rhs;
        return *this;
    }

    INLINE vec4& operator /= (i32 rhs) {
        f32 factor = 1.0f / (f32)rhs;
        x *= factor;
        y *= factor;
        z *= factor;
        w *= factor;
        return *this;
    }

    INLINE bool nonZero() const {
        return x != 0.0f ||
               y != 0.0f ||
               z != 0.0f ||
               w != 0.0f;
    }

    INLINE f32 minimum() const {
        f32 mn = x < y ? x : y;
        mn = mn < z ? mn : z;
        mn = mn < w ? mn : w;
        return mn;
    }

    INLINE f32 maximum() const {
        f32 mx = x > y ? x : y;
        mx = mx > z ? mx : z;
        mx = mx > w ? mx : w;
        return mx;
    }

    INLINE f32 dot(const vec4 &rhs) const {
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z) + (z * rhs.w);
    }

    INLINE f32 squaredLength() const {
        return x*x + y*y + z*z + w*w;
    }

    INLINE f32 length() const {
        return sqrtf(squaredLength());
    }

    INLINE vec4 normalized() const {
        return *this / length();
    }

    INLINE vec4 reflectedAround(const vec4 &N) const {
        return N.scaleAdd(-2 * dot(N), *this);
    }

    INLINE vec4 clamped() const {
        return {
                clampedValue(x),
                clampedValue(y),
                clampedValue(z),
                clampedValue(w)
        };
    }

    INLINE vec4 clamped(const vec4 &upper) const {
        return {
                clampedValue(x, upper.x),
                clampedValue(y, upper.y),
                clampedValue(z, upper.z),
                clampedValue(w, upper.w)
        };
    }

    INLINE vec4 clamped(const vec4 &lower, const vec4 &upper) const {
        return {
                clampedValue(x, lower.x, upper.x),
                clampedValue(y, lower.y, upper.y),
                clampedValue(z, lower.z, upper.z),
                clampedValue(w, lower.w, upper.w)
        };
    }

    INLINE vec4 clamped(const f32 min_value, const f32 max_value) const {
        return {
                clampedValue(x, min_value, max_value),
                clampedValue(y, min_value, max_value),
                clampedValue(z, min_value, max_value),
                clampedValue(w, min_value, max_value)
        };
    }

    INLINE vec4 approachTo(const vec4 &trg, f32 diff) const {
        return {
                approach(x, trg.x, diff),
                approach(y, trg.y, diff),
                approach(z, trg.z, diff),
                approach(w, trg.w, diff)
        };
    }

    INLINE vec4 lerpTo(const vec4 &to, f32 by) const {
        return (to - *this).scaleAdd(by, *this);
    }

    INLINE vec4 scaleAdd(f32 factor, const vec4 &to_be_added) const {
        return {
                fast_mul_add(x, factor, to_be_added.x),
                fast_mul_add(y, factor, to_be_added.y),
                fast_mul_add(z, factor, to_be_added.z),
                fast_mul_add(w, factor, to_be_added.w)
        };
    }

    INLINE vec4 mulAdd(const vec4 &factors, const vec4 &to_be_added) const {
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

INLINE vec4 minimum(const vec4 &a, const vec4 &b) {
    return {
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y,
            a.z < b.y ? a.z : b.z,
            a.w < b.w ? a.w : b.w
    };
}

INLINE vec4 maximum(const vec4 &a, const vec4 &b) {
    return {
            a.x > b.x ? a.x : b.x,
            a.y > b.y ? a.y : b.y,
            a.z > b.z ? a.z : b.z,
            a.w > b.w ? a.w : b.w
    };
}

INLINE vec4 operator - (f32 lhs, const vec4 &rhs) {
    return {
            lhs - rhs.x,
            lhs - rhs.y,
            lhs - rhs.z,
            lhs - rhs.w
    };
}

INLINE vec4 operator + (f32 lhs, const vec4 &rhs) {
    return {
            lhs + rhs.x,
            lhs + rhs.y,
            lhs + rhs.z,
            lhs + rhs.w
    };
}

INLINE vec4 operator / (f32 lhs, const vec4 &rhs) {
    return {
            lhs / rhs.x,
            lhs / rhs.y,
            lhs / rhs.z,
            lhs / rhs.w
    };
}

INLINE vec4 operator * (f32 lhs, const vec4 &rhs) {
    return {
            lhs * rhs.x,
            lhs * rhs.y,
            lhs * rhs.z,
            lhs * rhs.w
    };
}

INLINE vec4 lerp(const vec4 &from, const vec4 &to, f32 by) {
    return (to - from).scaleAdd(by, from);
}


struct quat {
    vec3 axis;
    f32 amount;

    static quat Identity;

    quat() noexcept : quat{vec3{0}, 1.0f} {}
    quat(f32 axis_x, f32 axis_y, f32 axis_z, f32 amount) noexcept : axis{axis_x, axis_y, axis_z}, amount{amount} {}
    quat(vec3 axis, f32 amount) noexcept : axis{axis}, amount{amount} {}
    quat(quat &other) noexcept : quat{other.axis, other.amount} {}
    quat(const quat &other) noexcept : quat{other.axis, other.amount} {}

    INLINE f32 length() const {
        return sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z + amount * amount);
    }

    INLINE quat normalized() const {
        return *this / length();
    }

    INLINE quat operator * (f32 rhs) const {
        return {
                axis * rhs,
                amount * rhs
        };
    }

    INLINE quat operator / (f32 rhs) const {
        return *this * (1.0f / rhs);
    }

    INLINE vec3 operator * (const vec3 &rhs) const {
        vec3 out{axis.cross(rhs)};
        vec3 qqv{axis.cross(out)};
        out = out.scaleAdd(amount, qqv);
        out = out.scaleAdd(2, rhs);
        return out;
    }

    INLINE quat operator * (const quat &rhs) const {
        return {
                {
                        amount * rhs.axis.x + axis.x * rhs.amount + axis.y * rhs.axis.z - axis.z * rhs.axis.y,
                        amount * rhs.axis.y - axis.x * rhs.axis.z + axis.y * rhs.amount + axis.z * rhs.axis.x,
                        amount * rhs.axis.z + axis.x * rhs.axis.y - axis.y * rhs.axis.x + axis.z * rhs.amount
                },
                amount * rhs.amount - axis.x * rhs.axis.x - axis.y * rhs.axis.y - axis.z * rhs.axis.z
        };
    }

    INLINE void operator *= (const quat &rhs) {
        f32 x = axis.x; f32 X = rhs.axis.x;
        f32 y = axis.y; f32 Y = rhs.axis.y;
        f32 z = axis.z; f32 Z = rhs.axis.z;
        f32 w = amount; f32 W = rhs.amount;
        axis.x = X*w + W*x + Z*y - Y*z;
        axis.y = Y*w - Z*x + W*y + X*z;
        axis.z = Z*w + Y*x - X*y + W*z;
        amount = W*w - X*x - Y*y - Z*z;
    }

    INLINE quat conjugate() const {
        return {
                -axis,
                amount
        };
    }

    INLINE quat rotated(const vec3 &Axis, f32 radians) const {
        return *this * AxisAngle(Axis, radians);
    }

    static INLINE quat AxisAngle(const vec3 &axis, f32 radians) {
        radians *= 0.5f;
        return quat{
                axis * sinf(radians),
                cosf(radians)
        }.normalized();
    }

    static INLINE quat RotationAroundX(f32 radians) { return AxisAngle(vec3::X, radians); }
    static INLINE quat RotationAroundY(f32 radians) { return AxisAngle(vec3::Y, radians); }
    static INLINE quat RotationAroundZ(f32 radians) { return AxisAngle(vec3::Z, radians); }
    INLINE void setRotationAroundX(f32 radians) { *this = RotationAroundX(radians); }
    INLINE void setRotationAroundY(f32 radians) { *this = RotationAroundY(radians); }
    INLINE void setRotationAroundZ(f32 radians) { *this = RotationAroundZ(radians); }

    INLINE void setXYZ(vec3 &X, vec3 &Y, vec3 &Z) const {
        f32 q0 = amount;
        f32 q1 = -axis.x;
        f32 q2 = -axis.y;
        f32 q3 = -axis.z;

        X.x = 2 * (q0 * q0 + q1 * q1) - 1;
        X.y = 2 * (q1 * q2 - q0 * q3);
        X.z = 2 * (q1 * q3 + q0 * q2);

        Y.x = 2 * (q1 * q2 + q0 * q3);
        Y.y = 2 * (q0 * q0 + q2 * q2) - 1;
        Y.z = 2 * (q2 * q3 - q0 * q1);

        Z.x = 2 * (q1 * q3 - q0 * q2);
        Z.y = 2 * (q2 * q3 + q0 * q1);
        Z.z = 2 * (q0 * q0 + q3 * q3) - 1;
    }
};
quat quat::Identity = {};

struct OrientationUsingQuaternion : Orientation<quat> {
    OrientationUsingQuaternion(f32 x_radians = 0.0f, f32 y_radians = 0.0f, f32 z_radians = 0.0f) :
            Orientation<quat>{x_radians, y_radians, z_radians} {}
};


struct mat2 {
    vec2 X, Y;

    static mat2 Identity;

    mat2() : mat2{
            vec2{1.0f, 0.0f},
            vec2{0.0f, 1.0f}
    } {}
    mat2(vec2i x, vec2i y) noexcept : X{x}, Y{y} {}
    mat2(vec2i x, vec2 y) noexcept : X{x}, Y{y} {}
    mat2(vec2 x, vec2i y) noexcept : X{x}, Y{y} {}
    mat2(vec2 x, vec2 y) noexcept : X{x}, Y{y} {}
    mat2(f32 Xx, f32 Xy, f32 Yx, f32 Yy) noexcept : X{Xx, Xy}, Y{Yx, Yy} {}
    mat2(i32 Xx, i32 Xy, i32 Yx, i32 Yy) noexcept : X{(f32)Xx, (f32)Xy}, Y{(f32)Yx, (f32)Yy} {}
    mat2(mat2 &other) noexcept : mat2{other.X, other.Y} {}
    mat2(const mat2 &other) noexcept : mat2{other.X, other.Y} {}

    INLINE f32 det() const {
        return X.x*Y.y - X.y*Y.x;
    }

    INLINE bool has_inverse() const {
        return det() != 0;
    }

    INLINE mat2 operator ! () const {
        return inverted();
    }

    INLINE mat2 operator ~ () const {
        return transposed();
    }

    INLINE mat2 operator + (f32 rhs) const {
        return {
                X.x + rhs, X.y + rhs,
                Y.x + rhs, Y.y + rhs
        };
    }

    INLINE mat2 operator - (f32 rhs) const {
        return {
                X.x - rhs, X.y - rhs,
                Y.x - rhs, Y.y - rhs
        };
    }

    INLINE mat2 operator * (f32 rhs) const {
        return {
                X.x * rhs, X.y * rhs,
                Y.x * rhs, Y.y * rhs
        };
    }

    INLINE mat2 operator / (f32 rhs) const {
        f32 factor = 1.0f / rhs;
        return {
                X.x * factor, X.y * factor,
                Y.x * factor, Y.y * factor
        };
    }

    INLINE mat2 operator + (const mat2 &rhs) const {
        return {
                X.x + rhs.X.x, X.y + rhs.X.y,
                Y.x + rhs.Y.x, Y.y + rhs.Y.y
        };
    }

    INLINE mat2 operator - (const mat2 &rhs) const {
        return {
                X.x - rhs.X.x, X.y - rhs.X.y,
                Y.x - rhs.Y.x, Y.y - rhs.Y.y
        };
    }

    INLINE mat2 operator * (const mat2 &rhs) const {
        return {
                X.x*rhs.X.x + X.y*rhs.Y.x, // Row 1 | Column 1
                X.x*rhs.X.y + X.y*rhs.Y.y, // Row 1 | Column 2
                Y.x*rhs.X.x + Y.y*rhs.Y.x, // Row 2 | Column 1
                Y.x*rhs.X.y + Y.y*rhs.Y.y  // Row 2 | Column 2
        };
    }

    INLINE vec2 operator * (const vec2 &rhs) const {
        return {
                X.x*rhs.x + Y.x*rhs.y,
                X.y*rhs.x + Y.y*rhs.y
        };
    }

    INLINE void operator += (const mat2 &rhs) {
        X.x += rhs.X.x;
        X.y += rhs.X.y;
        Y.x += rhs.Y.x;
        Y.y += rhs.Y.y;
    }

    INLINE void operator -= (const mat2 &rhs) {
        X.x -= rhs.X.x;
        X.y -= rhs.X.y;
        Y.x -= rhs.Y.x;
        Y.y -= rhs.Y.y;
    }

    INLINE void operator *= (const mat2 &rhs) {
        mat2 lhs{*this};
        X.x = lhs.X.x*rhs.X.x + lhs.X.y*rhs.Y.x; // Row 1 | Column 1
        X.y = lhs.X.x*rhs.X.y + lhs.X.y*rhs.Y.y; // Row 1 | Column 2
        Y.x = lhs.Y.x*rhs.X.x + lhs.Y.y*rhs.Y.x; // Row 2 | Column 1
        Y.y = lhs.Y.x*rhs.X.y + lhs.Y.y*rhs.Y.y; // Row 2 | Column 2
    }

    INLINE void operator += (f32 rhs) {
        X.x += rhs;
        X.y += rhs;
        Y.x += rhs;
        Y.y += rhs;
    }

    INLINE void operator -= (f32 rhs) {
        X.x -= rhs;
        X.y -= rhs;
        Y.x -= rhs;
        Y.y -= rhs;
    }

    INLINE void operator *= (f32 rhs) {
        X.x *= rhs;
        X.y *= rhs;
        Y.x *= rhs;
        Y.y *= rhs;
    }

    INLINE void operator /= (f32 rhs) {
        f32 factor = 1.0f / rhs;
        X.x *= factor;
        X.y *= factor;
        Y.x *= factor;
        Y.y *= factor;
    }

    INLINE void setRotation(f32 angle) {
        X.x = Y.y = cos(angle);
        Y.x = X.y = sin(angle);
        X.y = -X.y;
    }

    INLINE void rotate(f32 angle) {
        f32 c = cos(angle);
        f32 s = sin(angle);
        mat2 lhs{*this};
        X.x = c*lhs.X.x + s*lhs.X.y; // Row 1 | Column 1
        X.y = c*lhs.X.y - s*lhs.X.x; // Row 1 | Column 2
        Y.x = c*lhs.Y.x + s*lhs.Y.y; // Row 2 | Column 1
        Y.y = c*lhs.Y.y - s*lhs.Y.x; // Row 2 | Column 2
    }

    INLINE mat2 rotated_by(f32 angle) const {
        f32 c = cos(angle);
        f32 s = sin(angle);
        return {
                c*X.x + s*X.y, c*Y.x + s*Y.y,
                c*X.y - s*X.x, c*Y.y - s*Y.x
        };
    }

    INLINE mat2 transposed() const {
        return {
                X.x, Y.x,
                X.y, Y.y
        };
    }

    INLINE mat2 inverted() const {
        return mat2{
                Y.y, -X.y,
                -Y.x, X.x
        } / det();
    }
};
mat2 mat2::Identity = {};

INLINE mat2 operator * (f32 lhs, const mat2 &rhs) {
    return rhs * lhs;
}

INLINE mat2 operator + (f32 lhs, const mat2 &rhs) {
    return rhs + lhs;
}

INLINE mat2 outer(const vec2 &lhs, const vec2 &rhs) {
    return {
            lhs * rhs.x,
            lhs * rhs.y
    };
}


struct mat3 {
    vec3 X, Y, Z;

    static mat3 Identity;

    mat3() noexcept : X{1.0f, 0.0f, 0.0f},
                      Y{0.0f, 1.0f, 0.0f},
                      Z{0.0f, 0.0f, 1.0f} {}
    mat3(vec3 X, vec3 Y, vec3 Z) noexcept : X{X}, Y{Y}, Z{Z} {}
    mat3(f32 Xx, f32 Xy, f32 Xz,
         f32 Yx, f32 Yy, f32 Yz,
         f32 Zx, f32 Zy, f32 Zz) noexcept :
            X{Xx, Xy, Xz},
            Y{Yx, Yy, Yz},
            Z{Zx, Zy, Zz} {}
    mat3(mat3 &other) noexcept : mat3{other.X, other.Y, other.Z} {}
    mat3(const mat3 &other) noexcept : mat3{other.X, other.Y, other.Z} {}

    static INLINE mat3 RotationAroundX(f32 radians) {
        f32 c = cos(radians);
        f32 s = sin(radians);
        return {
                {1, 0, 0},
                {0, c, -s},
                {0, s, c}
        };
    }

    static INLINE mat3 RotationAroundY(f32 radians) {
        f32 c = cos(radians);
        f32 s = sin(radians);
        return {
                {c, 0, s},
                {0, 1, 0},
                {-s, 0, c}
        };
    }

    static INLINE mat3 RotationAroundZ(f32 radians) {
        f32 c = cos(radians);
        f32 s = sin(radians);
        return {
                {c, -s, 0},
                {s, c, 0},
                {0, 0, 1}
        };
    }

    INLINE void rotateAroundX(f32 radians) {
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

    INLINE void rotateAroundY(f32 radians) {
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

    INLINE void rotateAroundZ(f32 radians) {
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

    INLINE mat3 rotatedAroundXby(f32 radians) const {
        mat3 out{*this};
        out.rotateAroundX(radians);
        return out;
    }

    INLINE mat3 rotatedAroundYby(f32 radians) const {
        mat3 out{*this};
        out.rotateAroundY(radians);
        return out;
    }

    INLINE mat3 rotatedAroundZby(f32 radians) const {
        mat3 out{*this};
        out.rotateAroundZ(radians);
        return out;
    }

    INLINE f32 det() const {
        return (
                + X.x * (Y.y * Z.z - Z.y * Y.z)
                - Y.x * (X.y * Z.z - Z.y * X.z)
                + Z.x * (X.y * Y.z - Y.y * X.z)
        );
    }

    INLINE bool has_inverse() const {
        return det() != 0;
    }

    INLINE mat3 transposed() const {
        return {
                X.x, Y.x, Z.x,
                X.y, Y.y, Z.y,
                X.z, Y.z, Z.z
        };
    }

    INLINE mat3 inverted() const {
        return mat3{
                +(Y.y * Z.z - Z.y * Y.z),
                -(Y.x * Z.z - Z.x * Y.z),
                +(Y.x * Z.y - Z.x * Y.y),

                -(X.y * Z.z - Z.y * X.z),
                +(X.x * Z.z - Z.x * X.z),
                -(X.x * Z.y - Z.x * X.y),

                +(X.y * Y.z - Y.y * X.z),
                -(X.x * Y.z - Y.x * X.z),
                +(X.x * Y.y - Y.x * X.y)
        } / det();
    }

    INLINE mat3 operator ! () const {
        return inverted();
    }

    INLINE mat3 operator ~ () const {
        return transposed();
    }

    INLINE mat3 operator + (f32 rhs) const {
        return {
                X.x + rhs, X.y + rhs, X.z + rhs,
                Y.x + rhs, Y.y + rhs, Y.z + rhs,
                Z.x + rhs, Z.y + rhs, Z.z + rhs
        };
    }

    INLINE mat3 operator - (f32 rhs) const {
        return {
                X.x - rhs, X.y - rhs, X.z - rhs,
                Y.x - rhs, Y.y - rhs, Y.z - rhs,
                Z.x - rhs, Z.y - rhs, Z.z - rhs
        };
    }

    INLINE mat3 operator * (f32 rhs) const {
        return {
                X.x * rhs, X.y * rhs, X.z * rhs,
                Y.x * rhs, Y.y * rhs, Y.z * rhs,
                Z.x * rhs, Z.y * rhs, Z.z * rhs
        };
    }

    INLINE mat3 operator / (f32 rhs) const {
        f32 factor = 1.0f / rhs;
        return mat3{
                X.x, X.y, X.z,
                Y.x, Y.y, Y.z,
                Z.x, Z.y, Z.z
        } * factor;
    }

    INLINE mat3 operator + (const mat3 &rhs) const {
        return {
                X.x + rhs.X.x, X.y + rhs.X.y, X.z + rhs.X.z,
                Y.x + rhs.Y.x, Y.y + rhs.Y.y, Y.z + rhs.Y.z,
                Z.x + rhs.Z.x, Z.y + rhs.Z.y, Z.z + rhs.Z.z
        };
    }

    INLINE mat3 operator - (const mat3 &rhs) const {
        return {
                X.x - rhs.X.x, X.y - rhs.X.y, X.z - rhs.X.z,
                Y.x - rhs.Y.x, Y.y - rhs.Y.y, Y.z - rhs.Y.z,
                Z.x - rhs.Z.x, Z.y - rhs.Z.y, Z.z - rhs.Z.z
        };
    }

    INLINE mat3 operator * (const mat3 &rhs) const {
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

    INLINE vec3 operator * (const vec3 &rhs) const {
        return {
                X.x*rhs.x + Y.x*rhs.y + Z.x*rhs.z,
                X.y*rhs.x + Y.y*rhs.y + Z.y*rhs.z,
                X.z*rhs.x + Y.z*rhs.y + Z.z*rhs.z
        };
    }

    INLINE void operator += (const mat3 &rhs) {
        X.x += rhs.X.x; Y.x += rhs.Y.x; Z.x += rhs.Z.x;
        X.y += rhs.X.y; Y.y += rhs.Y.y; Z.y += rhs.Z.y;
        X.z += rhs.X.z; Y.z += rhs.Y.z; Z.z += rhs.Z.z;
    }

    INLINE void operator -= (const mat3 &rhs) {
        X.x -= rhs.X.x; Y.x -= rhs.Y.x; Z.x -= rhs.Z.x;
        X.y -= rhs.X.y; Y.y -= rhs.Y.y; Z.y -= rhs.Z.y;
        X.z -= rhs.X.z; Y.z -= rhs.Y.z; Z.z -= rhs.Z.z;
    }

    INLINE void operator *= (const mat3 &rhs) {
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

    INLINE void operator += (f32 rhs) {
        X.x += rhs; Y.x += rhs; Z.x += rhs;
        X.y += rhs; Y.y += rhs; Z.y += rhs;
        X.z += rhs; Y.z += rhs; Z.z += rhs;
    }

    INLINE void operator -= (f32 rhs) {
        X.x -= rhs; Y.x -= rhs; Z.x -= rhs;
        X.y -= rhs; Y.y -= rhs; Z.y -= rhs;
        X.z -= rhs; Y.z -= rhs; Z.z -= rhs;
    }

    INLINE void operator *= (f32 rhs) {
        X.x *= rhs; Y.x *= rhs; Z.x *= rhs;
        X.y *= rhs; Y.y *= rhs; Z.y *= rhs;
        X.z *= rhs; Y.z *= rhs; Z.z *= rhs;
    }

    INLINE void operator /= (f32 rhs) {
        f32 factor = 1.0f / rhs;
        X.x *= factor; Y.x *= factor; Z.x *= factor;
        X.y *= factor; Y.y *= factor; Z.y *= factor;
        X.z *= factor; Y.z *= factor; Z.z *= factor;
    }
};
mat3 mat3::Identity = {};

INLINE mat3 operator * (f32 lhs, const mat3 &rhs) {
    return rhs * lhs;
}

INLINE mat3 operator + (f32 lhs, const mat3 &rhs) {
    return rhs + lhs;
}

INLINE mat3 outerVec3(const vec3 &lhs, const vec3 &rhs) {
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

    OrientationUsing3x3Matrix(f32 x_radians = 0.0f, f32 y_radians = 0.0f, f32 z_radians = 0.0f) :
            Orientation<mat3>{x_radians, y_radians, z_radians} {}
};


struct mat4 {
    vec4 X, Y, Z, W;

    static mat4 Identity;

    mat4() noexcept :
            X{1, 0, 0, 0},
            Y{0, 1, 0, 0},
            Z{0, 0, 1, 0},
            W{0, 0, 0, 1} {}
    mat4(vec4 X, vec4 Y, vec4 Z, vec4 W) noexcept : X{X}, Y{Y}, Z{Z}, W{W} {}
    mat4(f32 Xx, f32 Xy, f32 Xz, f32 Xw,
         f32 Yx, f32 Yy, f32 Yz, f32 Yw,
         f32 Zx, f32 Zy, f32 Zz, f32 Zw,
         f32 Wx, f32 Wy, f32 Wz, f32 Ww) noexcept :
            X{Xx, Xy, Xz, Xw},
            Y{Yx, Yy, Yz, Yw},
            Z{Zx, Zy, Zz, Zw},
            W{Wx, Wy, Wz, Ww} {}
    mat4(mat4 &other) noexcept : mat4{other.X, other.Y, other.Z, other.W} {}
    mat4(const mat4 &other) noexcept : mat4{other.X, other.Y, other.Z, other.W} {}

    INLINE void setRotationAroundX(f32 angle) {
        Z.z = Y.y = cos(angle);
        Y.z = Z.y = sin(angle);
        Y.z = -Y.z;
        X.z = X.y = Y.x = Z.x = 0;
        X.x = 1;
        W = vec4{0, 0, 0, 1};
        X.w = Y.w = Z.w = 0;
    };

    INLINE void setRotationAroundY(f32 angle) {
        X.x = Z.z = cos(angle);
        Z.x = X.z = sin(angle);
        Z.x = -Z.x;
        Y.x = Y.z = X.y = Z.y = 0;
        Y.y = 1;
        W = vec4{0, 0, 0, 1};
        X.w = Y.w = Z.w = 0;
    }

    INLINE void setRotationAroundZ(f32 angle) {
        X.x = Y.y = cos(angle);
        Y.x = X.y = sin(angle);
        X.y = -X.y;
        X.z = Y.z = Z.x = Z.y = 0;
        Z.z = 1;
        W = vec4{0, 0, 0, 1};
        X.w = Y.w = Z.w = 0;
    }

    INLINE void rotateAroundX(f32 angle) {
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

    INLINE void rotateAroundY(f32 angle) {
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

    INLINE void rotateAroundZ(f32 angle) {
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

    INLINE mat4 rotatedAroundXby(f32 angle) const {
        mat4 out{*this};
        out.rotateAroundX(angle);
        return out;
    }

    INLINE mat4 rotatedAroundYby(f32 angle) const {
        mat4 out{*this};
        out.rotateAroundY(angle);
        return out;
    }

    INLINE mat4 rotatedAroundZby(f32 angle) const {
        mat4 out{*this};
        out.rotateAroundZ(angle);
        return out;
    }

    INLINE f32 det() const {
        return (
                X.x * (+Y.y*Z.z*W.w - Y.y*Z.w*W.z - Z.y*Y.z*W.w + Z.y*Y.w*W.z + W.y*Y.z*Z.w - W.y*Y.w*Z.z)
                + X.y * (-Y.x*Z.z*W.w + Y.x*Z.w*W.z + Z.x*Y.z*W.w - Z.x*Y.w*W.z - W.x*Y.z*Z.w + W.x*Y.w*Z.z)
                + X.z * (+Y.x*Z.y*W.w - Y.x*Z.w*W.y - Z.x*Y.y*W.w + Z.x*Y.w*W.y + W.x*Y.y*Z.w - W.x*Y.w*Z.y)
                + X.w * (-Y.x*Z.y*W.z + Y.x*Z.z*W.y + Z.x*Y.y*W.z - Z.x*Y.z*W.y - W.x*Y.y*Z.z + W.x*Y.z*Z.y)
        );
    }

    INLINE bool has_inverse() const {
        return det() != 0;
    }

    INLINE mat4 transposed() const {
        return {
                X.x, Y.x, Z.x, W.x,
                X.y, Y.y, Z.y, W.y,
                X.z, Y.z, Z.z, W.z,
                X.w, Y.w, Z.w, W.w
        };
    }

    INLINE mat4 inverted() const {
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

    INLINE mat4 operator ! () const {
        return inverted();
    }

    INLINE mat4 operator ~ () const {
        return transposed();
    }

    INLINE mat4 operator + (f32 rhs) const {
        return {
                X.x + rhs, X.y + rhs, X.z + rhs, X.w + rhs,
                Y.x + rhs, Y.y + rhs, Y.z + rhs, Y.w + rhs,
                Z.x + rhs, Z.y + rhs, Z.z + rhs, Z.w + rhs,
                W.x + rhs, W.y + rhs, W.z + rhs, W.w + rhs
        };
    }

    INLINE mat4 operator - (f32 rhs) const {
        return {
                X.x - rhs, X.y - rhs, X.z - rhs, X.w - rhs,
                Y.x - rhs, Y.y - rhs, Y.z - rhs, Y.w - rhs,
                Z.x - rhs, Z.y - rhs, Z.z - rhs, Z.w - rhs,
                W.x - rhs, W.y - rhs, W.z - rhs, W.w - rhs
        };
    }

    INLINE mat4 operator * (f32 rhs) const {
        return {
                X.x * rhs, X.y * rhs, X.z * rhs, X.w * rhs,
                Y.x * rhs, Y.y * rhs, Y.z * rhs, Y.w * rhs,
                Z.x * rhs, Z.y * rhs, Z.z * rhs, Z.w * rhs,
                W.x * rhs, W.y * rhs, W.z * rhs, W.w * rhs
        };
    }

    INLINE mat4 operator / (f32 rhs) const {
        f32 factor = 1.0f / rhs;
        return {
                X.x * factor, X.y * factor, X.z * factor, X.w * factor,
                Y.x * factor, Y.y * factor, Y.z * factor, Y.w * factor,
                Z.x * factor, Z.y * factor, Z.z * factor, Z.w * factor,
                W.x * factor, W.y * factor, W.z * factor, W.w * factor
        };
    }

    INLINE mat4 operator + (const mat4 &rhs) const {
        return {
                X.x + rhs.X.x, X.y + rhs.X.y, X.z + rhs.X.z, X.w + rhs.X.w,
                Y.x + rhs.Y.x, Y.y + rhs.Y.y, Y.z + rhs.Y.z, Y.w + rhs.Y.w,
                Z.x + rhs.Z.x, Z.y + rhs.Z.y, Z.z + rhs.Z.z, Z.w + rhs.Z.w,
                W.x + rhs.W.x, W.y + rhs.W.y, W.z + rhs.W.z, W.w + rhs.W.w
        };
    }
    INLINE mat4 operator - (const mat4 &rhs) const {
        return {
                X.x - rhs.X.x, X.y - rhs.X.y, X.z - rhs.X.z, X.w - rhs.X.w,
                Y.x - rhs.Y.x, Y.y - rhs.Y.y, Y.z - rhs.Y.z, Y.w - rhs.Y.w,
                Z.x - rhs.Z.x, Z.y - rhs.Z.y, Z.z - rhs.Z.z, Z.w - rhs.Z.w,
                W.x - rhs.W.x, W.y - rhs.W.y, W.z - rhs.W.z, W.w - rhs.W.w
        };
    }

    INLINE mat4 operator * (const mat4 &rhs) const {
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

    INLINE vec4 operator * (const vec4 &rhs) const {
        return {
                X.x*rhs.x + Y.x*rhs.y + Z.x*rhs.z + W.x*rhs.w,
                X.y*rhs.x + Y.y*rhs.y + Z.y*rhs.z + W.y*rhs.w,
                X.z*rhs.x + Y.z*rhs.y + Z.z*rhs.z + W.z*rhs.w,
                X.w*rhs.x + Y.w*rhs.y + Z.w*rhs.z + W.w*rhs.w
        };
    }

    INLINE void operator += (const mat4 &rhs) {
        X.x += rhs.X.x; Y.x += rhs.Y.x; Z.x += rhs.Z.x; W.x += rhs.W.x;
        X.y += rhs.X.y; Y.y += rhs.Y.y; Z.y += rhs.Z.y; W.y += rhs.W.y;
        X.z += rhs.X.z; Y.z += rhs.Y.z; Z.z += rhs.Z.z; W.z += rhs.W.z;
        X.w += rhs.X.w; Y.w += rhs.Y.w; Z.w += rhs.Z.w; W.w += rhs.W.w;
    }

    INLINE void operator -= (const mat4 &rhs) {
        X.x -= rhs.X.x; Y.x -= rhs.Y.x; Z.x -= rhs.Z.x;
        X.y -= rhs.X.y; Y.y -= rhs.Y.y; Z.y -= rhs.Z.y;
        X.z -= rhs.X.z; Y.z -= rhs.Y.z; Z.z -= rhs.Z.z;
    }

    INLINE void operator *= (const mat4 &rhs) {
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

    INLINE void operator += (f32 rhs) {
        X.x += rhs; Y.x += rhs; Z.x += rhs; W.x += rhs;
        X.y += rhs; Y.y += rhs; Z.y += rhs; W.y += rhs;
        X.z += rhs; Y.z += rhs; Z.z += rhs; W.z += rhs;
        X.w += rhs; Y.w += rhs; Z.w += rhs; W.w += rhs;
    }

    INLINE void operator -= (f32 rhs) {
        X.x -= rhs; Y.x -= rhs; Z.x -= rhs; W.x -= rhs;
        X.y -= rhs; Y.y -= rhs; Z.y -= rhs; W.y -= rhs;
        X.z -= rhs; Y.z -= rhs; Z.z -= rhs; W.z -= rhs;
        X.w -= rhs; Y.w -= rhs; Z.w -= rhs; W.w -= rhs;
    }

    INLINE void operator *= (f32 rhs) {
        X.x *= rhs; Y.x *= rhs; Z.x *= rhs; W.x *= rhs;
        X.y *= rhs; Y.y *= rhs; Z.y *= rhs; W.y *= rhs;
        X.z *= rhs; Y.z *= rhs; Z.z *= rhs; W.z *= rhs;
        X.w *= rhs; Y.w *= rhs; Z.w *= rhs; W.w *= rhs;
    }

    INLINE void operator /= (f32 rhs) {
        f32 factor = 1.0f / rhs;
        X.x *= factor; Y.x *= factor; Z.x *= factor; W.x *= factor;
        X.y *= factor; Y.y *= factor; Z.y *= factor; W.y *= factor;
        X.z *= factor; Y.z *= factor; Z.z *= factor; W.z *= factor;
        X.w *= factor; Y.w *= factor; Z.w *= factor; W.w *= factor;
    }
};
mat4 mat4::Identity = {};

INLINE mat4 operator * (f32 lhs, const mat4 &rhs) {
    return rhs * lhs;
}

INLINE mat4 operator + (f32 lhs, const mat4 &rhs) {
    return rhs + lhs;
}


INLINE vec4 Vec4(const vec3 &v3, f32 w = 0.0f) {
    return {v3.x, v3.y, v3.z, w};
}

INLINE vec3 Vec3(const vec4 &v4) {
    return {v4.x, v4.y, v4.z};
}

INLINE vec2 Vec2(const vec3 &v3) {
    return {v3.x, v3.y};
}

INLINE mat3 Mat3(const quat &q) {
    mat3 mat;
    q.setXYZ(mat.X, mat.Y, mat.Z);
    return mat;
}

INLINE quat Quat(const mat3 &m) {
    f32 fourXSquaredMinus1 = m.X.x - m.Y.y - m.Z.z;
    f32 fourYSquaredMinus1 = m.Y.y - m.X.x - m.Z.z;
    f32 fourZSquaredMinus1 = m.Z.z - m.X.x - m.Y.y;
    f32 fourWSquaredMinus1 = m.X.x + m.Y.y + m.Z.z;

    int biggestIndex = 0;
    f32 fourBiggestSquaredMinus1 = fourWSquaredMinus1;
    if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
        fourBiggestSquaredMinus1 = fourXSquaredMinus1;
        biggestIndex = 1;
    }
    if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
        fourBiggestSquaredMinus1 = fourYSquaredMinus1;
        biggestIndex = 2;
    }
    if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) {
        fourBiggestSquaredMinus1 = fourZSquaredMinus1;
        biggestIndex = 3;
    }

    f32 biggestVal = sqrtf(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
    f32 mult = 0.25f / biggestVal;

    switch(biggestIndex) {
        case 0:
            return {
                    {
                            (m.Y.z - m.Z.y) * mult,
                            (m.Z.x - m.X.z) * mult,
                            (m.X.y - m.Y.x) * mult
                    },
                    biggestVal
            };
        case 1:
            return {
                    {
                            biggestVal,
                            (m.X.y + m.Y.x) * mult,
                            (m.Z.x + m.X.z) * mult
                    },
                    (m.Y.z - m.Z.y) * mult
            };
        case 2:
            return {
                    {
                            (m.X.y + m.Y.x) * mult,
                            biggestVal,
                            (m.Y.z + m.Z.y) * mult
                    },
                    (m.Z.x - m.X.z) * mult
            };
        case 3:
            return {
                    {
                            (m.Z.x + m.X.z) * mult,
                            (m.Y.z + m.Z.y) * mult,
                            biggestVal
                    },
                    (m.X.y - m.Y.x) * mult
            };
    }

    return {};
}

INLINE mat3 Mat3I(const quat &rotation)  {
    return Mat3(rotation.conjugate());
}

mat4 Mat4(const quat &rotation, const vec3 &scale, const vec3 &position) {
    mat3 rotation_matrix{Mat3(rotation)};
    return {
            Vec4(rotation_matrix.X * scale.x),
            Vec4(rotation_matrix.Y * scale.y),
            Vec4(rotation_matrix.Z * scale.z),
            Vec4(position, 1)
    };
}

mat4 Mat4(const mat3 &m3, const vec4 &W = {0, 0, 0, 1}) {
    return {
            Vec4(m3.X),
            Vec4(m3.Y),
            Vec4(m3.Z),
            W
    };
}

INLINE mat4 Mat4(const mat3 &rotation, const vec3 &position) {
    return {
            Vec4(rotation.X),
            Vec4(rotation.Y),
            Vec4(rotation.Z),
            Vec4(position, 1.0f)
    };
}


struct Transform : OrientationUsingQuaternion {
    vec3 position{0.0f};
    vec3 scale{1.0f};

    Transform() : OrientationUsingQuaternion{}, position{0.0f}, scale{1.0f} {}
    Transform(const vec3 &position) : OrientationUsingQuaternion{}, position{position}, scale{1.0f} {}
    Transform(const vec3 &position, const vec3 &orientation, const vec3 &scale = vec3{1.0f}) :
            OrientationUsingQuaternion{
                    orientation.x,
                    orientation.y,
                    orientation.z
            }, position{position}, scale{scale} {}

    void externPosAndDir(const vec3 &pos, const vec3 &dir, vec3 &out_pos, vec3 &out_dir) const {
        out_pos = position + (rotation * (scale * position));
        out_dir = (rotation * (scale * dir)).normalized();
    }

    void internPosAndDir(const vec3 &pos, const vec3 &dir, vec3 &out_pos, vec3 &out_dir) const {
        vec3 inv_scale = 1.0f / scale;
        quat inv_rotation = rotation.conjugate();
        out_pos = inv_scale * (inv_rotation * (pos - position));
        out_dir = (inv_scale * (inv_rotation * dir)).normalized();
    }

    INLINE vec3 externPos(const vec3 &pos) const { return _translate(_rotate(_scale(pos))); }
    INLINE vec3 internPos(const vec3 &pos) const { return _unscale(_unrotate(_untranslate(pos))); }

    INLINE vec3 externDir(const vec3 &dir) const { return _rotate(_scale(dir)).normalized(); }
    INLINE vec3 internDir(const vec3 &dir) const { return _unscale(_unrotate(dir)).normalized(); }

private:
    INLINE vec3 _scale(const vec3 &pos) const { return scale * pos; }
    INLINE vec3 _rotate(const vec3 &pos) const { return rotation * pos; }
    INLINE vec3 _translate(const vec3 &pos) const { return pos + position; }
    INLINE vec3 _unscale(const vec3 &pos) const { return pos / scale; }
    INLINE vec3 _unrotate(const vec3 &pos) const { return rotation.conjugate() * pos; }
    INLINE vec3 _untranslate(const vec3 &pos) const { return pos - position; }
};

struct Geometry {
    Transform transform;
    enum GeometryType type{GeometryType_None};
    enum ColorID color{White};
    u32 id{0};
};

AABB operator * (const AABB &aabb, const Transform &transform) {
    const vec3 vertices[8] = {
            {aabb.min.x, aabb.min.y, aabb.min.z},
            {aabb.min.x, aabb.min.y, aabb.max.z},
            {aabb.min.x, aabb.max.y, aabb.min.z},
            {aabb.min.x, aabb.max.y, aabb.max.z},
            {aabb.max.x, aabb.min.y, aabb.min.z},
            {aabb.max.x, aabb.min.y, aabb.max.z},
            {aabb.max.x, aabb.max.y, aabb.min.z},
            {aabb.max.x, aabb.max.y, aabb.max.z}
    };

    vec3 min{+INFINITY};
    vec3 max{-INFINITY};

    vec3 pos;
    for (const auto &vertex : vertices) {
        pos = transform.internPos(vertex);

        if (pos.x < min.x) min.x = pos.x;
        if (pos.y < min.y) min.y = pos.y;
        if (pos.z < min.z) min.z = pos.z;

        if (pos.x > max.x) max.x = pos.x;
        if (pos.y > max.y) max.y = pos.y;
        if (pos.z > max.z) max.z = pos.z;
    }

    return {min, max};
}

struct RayHit {
    vec3 position, normal;
    f32 distance, distance_squared;
    u32 geo_id;
    enum GeometryType geo_type = GeometryType_None;
    bool from_behind = false;
};

struct Ray {
    vec3 origin, direction;
    RayHit hit;

    INLINE vec3 at(f32 t) const { return origin + t*direction; }
    INLINE vec3 operator [](f32 t) const { return at(t); }

    INLINE BoxSide hitsCube() {
        vec3 octant, RD_rcp = 1.0f / direction;
        f32 x = signbit(direction.x) ? 1.0f : -1.0f;
        f32 y = signbit(direction.y) ? 1.0f : -1.0f;
        f32 z = signbit(direction.z) ? 1.0f : -1.0f;

        f32 t[6];
        t[0] = (+x - origin.x) * RD_rcp.x;
        t[1] = (+y - origin.y) * RD_rcp.y;
        t[2] = (+z - origin.z) * RD_rcp.z;
        t[3] = (-x - origin.x) * RD_rcp.x;
        t[4] = (-y - origin.y) * RD_rcp.y;
        t[5] = (-z - origin.z) * RD_rcp.z;

        u8 max_axis = t[3] < t[4] ? 3 : 4; if (t[5] < t[max_axis]) max_axis = 5;
        f32 max_t = t[max_axis];
        if (max_t < 0) // Further-away hit is behind the ray - intersection can not occur.
            return NoSide;

        u8 min_axis = t[0] > t[1] ? 0 : 1; if (t[2] > t[min_axis]) min_axis = 2;
        f32 min_t = t[min_axis];
        if (max_t < (min_t > 0 ? min_t : 0))
            return NoSide;

        hit.from_behind = min_t < 0; // Further-away hit is in front of the ray, closer one is behind it.
        if (hit.from_behind) {
            min_t = max_t;
            min_axis = max_axis;
        }

        BoxSide side = getBoxSide(x, y, z, min_axis);
        hit.position = direction.scaleAdd(min_t, origin);
        hit.normal = 0.0f;
        switch (side) {
            case Left:   hit.normal.x = hit.from_behind ? +1.0f : -1.0f; break;
            case Right:  hit.normal.x = hit.from_behind ? -1.0f : +1.0f; break;
            case Bottom: hit.normal.y = hit.from_behind ? +1.0f : -1.0f; break;
            case Top:    hit.normal.y = hit.from_behind ? -1.0f : +1.0f; break;
            case Back:   hit.normal.z = hit.from_behind ? +1.0f : -1.0f; break;
            case Front:  hit.normal.z = hit.from_behind ? -1.0f : +1.0f; break;
            default: return NoSide;
        }

        return side;
    }

    INLINE bool hitsPlane(const vec3 &P, const vec3 &N) {
        f32 NdotRd = N.dot(direction);
        if (NdotRd == 0) // The ray is parallel to the plane
            return false;

        f32 NdotRoP = N.dot(P - origin);
        if (NdotRoP == 0) return false; // The ray originated within the plane

        bool ray_is_facing_the_plane = NdotRd < 0;
        hit.from_behind = NdotRoP > 0;
        if (hit.from_behind == ray_is_facing_the_plane) // The ray can't hit the plane
            return false;

        hit.distance = NdotRoP / NdotRd;
        hit.position = at(hit.distance);
        hit.normal = N;

        return true;
    }
};


struct BoxCorners {
    vec3 front_top_left;
    vec3 front_top_right;
    vec3 front_bottom_left;
    vec3 front_bottom_right;
    vec3 back_top_left;
    vec3 back_top_right;
    vec3 back_bottom_left;
    vec3 back_bottom_right;

    BoxCorners() :
            front_top_left{-1, 1, 1},
            front_top_right{1, 1, 1},
            front_bottom_left{-1, -1, 1},
            front_bottom_right{1, -1, 1},
            back_top_left{-1, 1, -1},
            back_top_right{1, 1, -1},
            back_bottom_left{-1, -1, -1},
            back_bottom_right{1, -1, -1}
    {}
};

union BoxVertices {
    BoxCorners corners;
    vec3 buffer[BOX__VERTEX_COUNT];
};

struct BoxEdgeSides {
    Edge front_top,
            front_bottom,
            front_left,
            front_right,
            back_top,
            back_bottom,
            back_left,
            back_right,
            left_bottom,
            left_top,
            right_bottom,
            right_top;

    explicit BoxEdgeSides(const BoxCorners &corners) { setFrom(corners); }
    explicit BoxEdgeSides(const BoxVertices &vertices) : BoxEdgeSides(vertices.corners) {}

    void setFrom(const BoxVertices &vertices) {
        setFrom(vertices.corners);
    }

    void setFrom(const BoxCorners &corners) {
        front_top.from    = corners.front_top_left;
        front_top.to      = corners.front_top_right;
        front_bottom.from = corners.front_bottom_left;
        front_bottom.to   = corners.front_bottom_right;
        front_left.from   = corners.front_bottom_left;
        front_left.to     = corners.front_top_left;
        front_right.from  = corners.front_bottom_right;
        front_right.to    = corners.front_top_right;

        back_top.from     = corners.back_top_left;
        back_top.to       = corners.back_top_right;
        back_bottom.from  = corners.back_bottom_left;
        back_bottom.to    = corners.back_bottom_right;
        back_left.from    = corners.back_bottom_left;
        back_left.to      = corners.back_top_left;
        back_right.from   = corners.back_bottom_right;
        back_right.to     = corners.back_top_right;

        left_bottom.from  = corners.front_bottom_left;
        left_bottom.to    = corners.back_bottom_left;
        left_top.from     = corners.front_top_left;
        left_top.to       = corners.back_top_left;
        right_bottom.from = corners.front_bottom_right;
        right_bottom.to   = corners.back_bottom_right;
        right_top.from    = corners.front_top_right;
        right_top.to      = corners.back_top_right;
    }
};

union BoxEdges {
    BoxEdgeSides sides;
    Edge buffer[BOX__EDGE_COUNT];

    explicit BoxEdges(const BoxEdgeSides &sides) : sides{sides} {}
    explicit BoxEdges(const BoxVertices &vertices) : sides{vertices} {}
    explicit BoxEdges(const BoxCorners &corners) : sides{corners} {}

    void setFrom(const BoxVertices &vertices) {
        sides.setFrom(vertices);
    }

    void setFrom(const BoxCorners &corners) {
        sides.setFrom(corners);
    }
};

struct Box {
    BoxVertices vertices;
    BoxEdges edges;

    Box() : vertices{}, edges{vertices} {}
};



struct GridAxisVertices {
    vec3 from[GRID__MAX_SEGMENTS];
    vec3 to[  GRID__MAX_SEGMENTS];

    explicit GridAxisVertices(u8 segments, bool is_u) {
        update(segments, is_u);
    }

    bool update(u8 segments, bool is_u) {
        if (!segments || segments > GRID__MAX_SEGMENTS) return false;

        f32 step = segments > 1 ? (2.0f / ((f32)segments - 1.0f)) : 0.0f;
        for (u8 i = 0; i < segments; i++) {
            from[i].y = to[i].y = 0.0f;
            if (is_u) {
                from[i].x = to[i].x = -1.0f + (f32)i * step;
                from[i].z = -1;
                to[  i].z = +1;
            } else {
                from[i].z = to[i].z = -1.0f + (f32)i * step;
                from[i].x = -1;
                to[  i].x = +1;
            }
        }

        return true;
    }
};

struct GridVertices {
    GridAxisVertices u, v;

    GridVertices(u8 U_segments = GRID__MAX_SEGMENTS, u8 V_segments = GRID__MAX_SEGMENTS) : u{U_segments, true}, v{V_segments, false} {}

    bool update(u8 U_segments, u8 V_segments) {
        return u.update(U_segments, true) && v.update(V_segments, false);
    }
};

struct GridAxisEdges {
    Edge edges[GRID__MAX_SEGMENTS];

    GridAxisEdges(const GridAxisVertices &vertices, u8 segments) {
        update(vertices, segments);
    }

    bool update(const GridAxisVertices &vertices, u8 segments) {
        if (segments > GRID__MAX_SEGMENTS) return false;

        for (u8 i = 0; i < segments; i++) {
            edges[i].from = vertices.from[i];
            edges[i].to   = vertices.to[i];
        }

        return true;
    }
};

struct GridEdges {
    GridAxisEdges u, v;

    GridEdges(const GridVertices &vertices, u8 u_segments, u8 v_segments) :
            u{vertices.u, u_segments},
            v{vertices.v, v_segments}
    {
        update(vertices, u_segments, v_segments);
    }

    bool update(const GridVertices &vertices, u8 u_segments, u8 v_segments) {
        return update(vertices.u, vertices.v, u_segments, v_segments);
    }

    bool update(const GridAxisVertices &u_vertices, const GridAxisVertices &v_vertices, u8 u_segments, u8 v_segments) {
        if (u_segments > GRID__MAX_SEGMENTS || v_segments > GRID__MAX_SEGMENTS) return false;

        u.update(u_vertices, u_segments);
        v.update(v_vertices, v_segments);

        return true;
    }
};

struct Grid {
    u8 u_segments;
    u8 v_segments;
    GridVertices vertices;
    GridEdges edges;

    Grid(u8 u_segments = GRID__MAX_SEGMENTS, u8 v_segments = GRID__MAX_SEGMENTS) :
            u_segments{u_segments},
            v_segments{v_segments},
            vertices{u_segments, v_segments},
            edges{vertices, u_segments, v_segments} {}

    bool update(u8 U_segments, u8 V_segments) {
        if (!U_segments || U_segments > GRID__MAX_SEGMENTS ||
            !V_segments || V_segments > GRID__MAX_SEGMENTS)
            return false;

        u_segments = U_segments;
        v_segments = V_segments;

        vertices.update(U_segments, V_segments);
        edges.update(vertices, U_segments, V_segments);

        return true;
    }
};



struct Camera : OrientationUsing3x3Matrix {
    vec3 position{0};
    vec3 current_velocity{0};
    f32 focal_length{  CAMERA_DEFAULT__FOCAL_LENGTH};
    f32 zoom_amount{   CAMERA_DEFAULT__FOCAL_LENGTH};
    f32 target_distance{CAMERA_DEFAULT__TARGET_DISTANCE};
    f32 dolly_amount{0};

    Camera() : OrientationUsing3x3Matrix{}, position{0.0f} {}
    explicit Camera(const vec3 &position) : OrientationUsing3x3Matrix{}, position{position} {}
    explicit Camera(const vec3 &position, const vec3 &orientation = vec3{0.0f}, f32 zoom_amount = CAMERA_DEFAULT__FOCAL_LENGTH) :
            OrientationUsing3x3Matrix{orientation.x, orientation.y, orientation.z},
            position{position}, current_velocity{vec3{0}}, focal_length{zoom_amount}, zoom_amount{zoom_amount} {}

    void zoom(f32 amount) {
        f32 new_amount = zoom_amount + amount;
        focal_length = new_amount > 1 ? new_amount : (new_amount < -1.0f ? (-1.0f / new_amount) : 1.0f);
        zoom_amount = new_amount;
    }

    void dolly(f32 amount) {
        vec3 target_position = forward.scaleAdd(target_distance, position);

        // Compute new target distance:
        dolly_amount += amount;
        target_distance = powf(2.0f, dolly_amount / -200.0f) * CAMERA_DEFAULT__TARGET_DISTANCE;

        // Back-track from target position_x to new current position_x:
        position = target_position - (forward * target_distance);
    }

    void orbit(f32 azimuth, f32 altitude) {
        // Move the camera forward to the position_x of its target:
        position += forward * target_distance;

        // Reorient the camera while it is at the position_x of its target:
        rotate(altitude, azimuth);

        // Back the camera away from its target position_x using the updated forward direction:
        position -= forward * target_distance;
    }

    void pan(f32 right_amount, f32 up_amount) {
        position += up * up_amount + right * right_amount;
    }

    INLINE vec3 getRayDirectionAt(f32 x, f32 y, f32 width, f32 height) const {
        vec3 start{forward.scaleAdd(focal_length * height,up.scaleAdd(height,right * -width))};
        return right.scaleAdd(x * 2.0f + 1,up.scaleAdd(1 - 2.0f * y, start)).normalized();
    }

    INLINE vec3 internPos(const vec3 &pos) const { return _unrotate(_untranslate(pos)); }
    INLINE vec3 internDir(const vec3 &dir) const { return _unrotate(dir); }
    INLINE vec3 externPos(const vec3 &pos) const { return _translate(_rotate(pos)); }
    INLINE vec3 externDir(const vec3 &dir) const { return _rotate(dir); }

private:
    INLINE vec3 _rotate(const vec3 &pos) const { return rotation * pos; }
    INLINE vec3 _unrotate(const vec3 &pos) const { return rotation.transposed() * pos; }
    INLINE vec3 _translate(const vec3 &pos) const { return pos + position; }
    INLINE vec3 _untranslate(const vec3 &pos) const { return pos - position; }
};


struct EdgeVertexIndices {
    u32 from, to;
};

union TriangleVertexIndices {
    u32 ids[3];
    struct {
        u32 v1, v2, v3;
    };
};

struct Mesh {
    AABB aabb;

    vec3 *vertex_positions{nullptr};
    vec3 *vertex_normals{nullptr};
    vec2 *vertex_uvs{nullptr};

    TriangleVertexIndices *vertex_position_indices{nullptr};
    TriangleVertexIndices *vertex_normal_indices{nullptr};
    TriangleVertexIndices *vertex_uvs_indices{nullptr};

    EdgeVertexIndices *edge_vertex_indices{nullptr};

    u32 triangle_count{0};
    u32 vertex_count{0};
    u32 edge_count{0};
    u32 normals_count{0};
    u32 uvs_count{0};

    Mesh() = default;

    Mesh(u32 triangle_count,
         u32 vertex_count,
         u32 normals_count,
         u32 uvs_count,
         u32 edge_count,

         vec3 *vertex_positions,
         vec3 *vertex_normals,
         vec2 *vertex_uvs,

         TriangleVertexIndices *vertex_position_indices,
         TriangleVertexIndices *vertex_normal_indices,
         TriangleVertexIndices *vertex_uvs_indices,

         EdgeVertexIndices *edge_vertex_indices,
         AABB aabb
    ) :
            triangle_count{triangle_count},
            vertex_count{vertex_count},
            normals_count{normals_count},
            uvs_count{uvs_count},
            edge_count{edge_count},

            vertex_positions{vertex_positions},
            vertex_normals{vertex_normals},
            vertex_uvs{vertex_uvs},

            vertex_position_indices{vertex_position_indices},
            vertex_normal_indices{vertex_normal_indices},
            vertex_uvs_indices{vertex_uvs_indices},

            edge_vertex_indices{edge_vertex_indices},
            aabb{aabb}
    {}
};

struct CubeMesh : Mesh {
    const vec3 CUBE_VERTEX_POSITIONS[CUBE_VERTEX_COUNT] = {
            {-1, -1, -1},
            {1, -1, -1},
            {1, 1, -1},
            {-1, 1, -1},
            {-1, -1, 1},
            {1, -1, 1},
            {1, 1, 1},
            {-1, 1, 1}
    };

    const TriangleVertexIndices CUBE_VERTEX_POSITION_INDICES[CUBE_TRIANGLE_COUNT] = {
            {0, 1, 2},
            {1, 5, 6},
            {5, 4, 7},
            {4, 0, 3},
            {3, 2, 6},
            {1, 0, 4},
            {0, 2, 3},
            {1, 6, 2},
            {5, 7, 6},
            {4, 3, 7},
            {3, 6, 7},
            {1, 4, 5}
    };

    const vec3 CUBE_VERTEX_NORMALS[CUBE_NORMAL_COUNT] = {
            {0, 0, -1},
            {1, 0, 0},
            {0, 0, 1},
            {-1, 0, 0},
            {0, 1, 0},
            {0, -1, 0}
    };

    const TriangleVertexIndices CUBE_VERTEX_NORMAL_INDICES[CUBE_TRIANGLE_COUNT] = {
            {0, 0, 0},
            {1, 1, 1},
            {2, 2, 2},
            {3, 3, 3},
            {4, 4, 4},
            {5, 5, 5},
            {0, 0, 0},
            {1, 1, 1},
            {2, 2, 2},
            {3, 3, 3},
            {4, 4, 4},
            {5, 5, 5}
    };

    const vec2 CUBE_VERTEX_UVS[CUBE_UV_COUNT] = {
            {0.0f, 0.0f},
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
    };

    const TriangleVertexIndices CUBE_VERTEX_UV_INDICES[CUBE_TRIANGLE_COUNT] = {
            {0, 1, 2},
            {0, 1, 2},
            {0, 1, 2},
            {0, 1, 2},
            {0, 1, 2},
            {0, 1, 2},
            {0, 2, 3},
            {0, 2, 3},
            {0, 2, 3},
            {0, 2, 3},
            {0, 2, 3},
            {0, 2, 3}
    };

    CubeMesh() : Mesh{
            CUBE_TRIANGLE_COUNT,
            CUBE_VERTEX_COUNT,
            CUBE_NORMAL_COUNT,
            CUBE_UV_COUNT,
            0,

            (vec3*)CUBE_VERTEX_POSITIONS,
            (vec3*)CUBE_VERTEX_NORMALS,
            (vec2*)CUBE_VERTEX_UVS,

            (TriangleVertexIndices*)CUBE_VERTEX_POSITION_INDICES,
            (TriangleVertexIndices*)CUBE_VERTEX_NORMAL_INDICES,
            (TriangleVertexIndices*)CUBE_VERTEX_UV_INDICES,
            nullptr,

            {-1 , +1}
    } {}
};


u32 getSizeInBytes(const Mesh &mesh) {
    u32 memory_size = sizeof(vec3) * mesh.vertex_count;
    memory_size += sizeof(TriangleVertexIndices) * mesh.triangle_count;
    memory_size += sizeof(EdgeVertexIndices) * mesh.edge_count;

    if (mesh.uvs_count) {
        memory_size += sizeof(vec2) * mesh.uvs_count;
        memory_size += sizeof(TriangleVertexIndices) * mesh.triangle_count;
    }
    if (mesh.normals_count) {
        memory_size += sizeof(vec3) * mesh.normals_count;
        memory_size += sizeof(TriangleVertexIndices) * mesh.triangle_count;
    }

    return memory_size;
}

bool allocateMemory(Mesh &mesh, memory::MonotonicAllocator *memory_allocator) {
    if (getSizeInBytes(mesh) > (memory_allocator->capacity - memory_allocator->occupied)) return false;
    mesh.vertex_positions        = (vec3*                 )memory_allocator->allocate(sizeof(vec3)                  * mesh.vertex_count);
    mesh.vertex_position_indices = (TriangleVertexIndices*)memory_allocator->allocate(sizeof(TriangleVertexIndices) * mesh.triangle_count);
    mesh.edge_vertex_indices     = (EdgeVertexIndices*    )memory_allocator->allocate(sizeof(EdgeVertexIndices)     * mesh.edge_count);
    if (mesh.uvs_count) {
        mesh.vertex_uvs         = (vec2*                 )memory_allocator->allocate(sizeof(vec2)                  * mesh.uvs_count);
        mesh.vertex_uvs_indices = (TriangleVertexIndices*)memory_allocator->allocate(sizeof(TriangleVertexIndices) * mesh.triangle_count);
    }
    if (mesh.normals_count) {
        mesh.vertex_normals          = (vec3*                 )memory_allocator->allocate(sizeof(vec3)                  * mesh.normals_count);
        mesh.vertex_normal_indices   = (TriangleVertexIndices*)memory_allocator->allocate(sizeof(TriangleVertexIndices) * mesh.triangle_count);
    }
    return true;
}

void writeHeader(const Mesh &mesh, void *file) {
    os::writeToFile((void*)&mesh.vertex_count,   sizeof(u32),  file);
    os::writeToFile((void*)&mesh.triangle_count, sizeof(u32),  file);
    os::writeToFile((void*)&mesh.edge_count,     sizeof(u32),  file);
    os::writeToFile((void*)&mesh.uvs_count,      sizeof(u32),  file);
    os::writeToFile((void*)&mesh.normals_count,  sizeof(u32),  file);
}
void readHeader(Mesh &mesh, void *file) {
    os::readFromFile(&mesh.vertex_count,   sizeof(u32),  file);
    os::readFromFile(&mesh.triangle_count, sizeof(u32),  file);
    os::readFromFile(&mesh.edge_count,     sizeof(u32),  file);
    os::readFromFile(&mesh.uvs_count,      sizeof(u32),  file);
    os::readFromFile(&mesh.normals_count,  sizeof(u32),  file);
}

bool saveHeader(const Mesh &mesh, char *file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeHeader(mesh, file);
    os::closeFile(file);
    return true;
}

bool loadHeader(Mesh &mesh, char *file_path) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;
    readHeader(mesh, file);
    os::closeFile(file);
    return true;
}

void readContent(Mesh &mesh, void *file) {
    os::readFromFile(&mesh.aabb.min,       sizeof(vec3), file);
    os::readFromFile(&mesh.aabb.max,       sizeof(vec3), file);
    os::readFromFile(mesh.vertex_positions,             sizeof(vec3)                  * mesh.vertex_count,   file);
    os::readFromFile(mesh.vertex_position_indices,      sizeof(TriangleVertexIndices) * mesh.triangle_count, file);
    os::readFromFile(mesh.edge_vertex_indices,          sizeof(EdgeVertexIndices)     * mesh.edge_count,     file);
    if (mesh.uvs_count) {
        os::readFromFile(mesh.vertex_uvs,               sizeof(vec2)                  * mesh.uvs_count,      file);
        os::readFromFile(mesh.vertex_uvs_indices,       sizeof(TriangleVertexIndices) * mesh.triangle_count, file);
    }
    if (mesh.normals_count) {
        os::readFromFile(mesh.vertex_normals,                sizeof(vec3)                  * mesh.normals_count,  file);
        os::readFromFile(mesh.vertex_normal_indices,         sizeof(TriangleVertexIndices) * mesh.triangle_count, file);
    }
}
void writeContent(const Mesh &mesh, void *file) {
    os::writeToFile((void*)&mesh.aabb.min,       sizeof(vec3), file);
    os::writeToFile((void*)&mesh.aabb.max,       sizeof(vec3), file);
    os::writeToFile((void*)mesh.vertex_positions,        sizeof(vec3)                  * mesh.vertex_count,   file);
    os::writeToFile((void*)mesh.vertex_position_indices, sizeof(TriangleVertexIndices) * mesh.triangle_count, file);
    os::writeToFile((void*)mesh.edge_vertex_indices,     sizeof(EdgeVertexIndices)     * mesh.edge_count,     file);
    if (mesh.uvs_count) {
        os::writeToFile(mesh.vertex_uvs,          sizeof(vec2)                  * mesh.uvs_count,      file);
        os::writeToFile(mesh.vertex_uvs_indices,  sizeof(TriangleVertexIndices) * mesh.triangle_count, file);
    }
    if (mesh.normals_count) {
        os::writeToFile(mesh.vertex_normals,        sizeof(vec3)                  * mesh.normals_count,  file);
        os::writeToFile(mesh.vertex_normal_indices, sizeof(TriangleVertexIndices) * mesh.triangle_count, file);
    }
}

bool saveContent(const Mesh &mesh, char *file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeContent(mesh, file);
    os::closeFile(file);
    return true;
}

bool loadContent(Mesh &mesh, char *file_path) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;
    readContent(mesh, file);
    os::closeFile(file);
    return true;
}

bool save(const Mesh &mesh, char* file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeHeader(mesh, file);
    writeContent(mesh, file);
    os::closeFile(file);
    return true;
}

bool load(Mesh &mesh, char *file_path, memory::MonotonicAllocator *memory_allocator = nullptr) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;

    if (memory_allocator) {
        mesh = Mesh{};
        readHeader(mesh, file);
        if (!allocateMemory(mesh, memory_allocator)) return false;
    } else if (!mesh.vertex_positions) return false;
    readContent(mesh, file);
    os::closeFile(file);
    return true;
}

u32 getTotalMemoryForMeshes(String *mesh_files, u32 mesh_count) {
    u32 memory_size{0};
    for (u32 i = 0; i < mesh_count; i++) {
        Mesh mesh;
        loadHeader(mesh, mesh_files[i].char_ptr);
        memory_size += getSizeInBytes(mesh);
    }
    return memory_size;
}


struct SceneCounts {
    u32 cameras{1};
    u32 geometries{0};
    u32 grids{0};
    u32 boxes{0};
    u32 curves{0};
    u32 meshes{0};
};

struct Scene {
    SceneCounts counts;
    String file_path;

    Geometry *geometries{nullptr};
    Curve *curves{nullptr};
    Grid *grids{nullptr};
    Box *boxes{nullptr};
    Camera *cameras{nullptr};
    Mesh *meshes{nullptr};
    u64 last_io_ticks{0};
    bool last_io_is_save{false};

    Scene(SceneCounts counts,
          char *file_path = nullptr,
          Camera *cameras = nullptr,
          Geometry *geometries = nullptr,
          Grid *grids = nullptr,
          Box *boxes = nullptr,
          Curve *curves = nullptr,
          Mesh *meshes = nullptr,
          String *mesh_files = nullptr,
          String *texture_files = nullptr,
          memory::MonotonicAllocator *memory_allocator = nullptr
    ) : counts{counts},
        file_path{file_path},
        cameras{cameras},
        geometries{geometries},
        grids{grids},
        boxes{boxes},
        curves{curves},
        meshes{meshes}
    {
        if (meshes && mesh_files && counts.meshes) {
            for (u32 i = 0; i < counts.meshes; i++)
                meshes[i] = Mesh{};
            memory::MonotonicAllocator temp_allocator;
            if (!memory_allocator) {
                u32 capacity = getTotalMemoryForMeshes(mesh_files, 2);
                temp_allocator = memory::MonotonicAllocator{capacity};
                memory_allocator = &temp_allocator;
            }
            for (u32 i = 0; i < counts.meshes; i++) load(meshes[i], mesh_files[i].char_ptr, memory_allocator);
        }
    }

    INLINE bool castRay(Ray &ray) const {
        static Ray local_ray;
        static Transform xform;

        bool found{false};
        bool current_found{false};

        Geometry *geo = geometries;

        for (u32 i = 0; i < counts.geometries; i++, geo++) {
            xform = geo->transform;
            if (geo->type == GeometryType_Mesh)
                xform.scale *= meshes[geo->id].aabb.max;

            xform.internPosAndDir(ray.origin, ray.direction, local_ray.origin, local_ray.direction);

            current_found = local_ray.hitsCube();
            if (current_found) {
                local_ray.hit.position         = xform.externPos(local_ray.hit.position);
                local_ray.hit.distance_squared = (local_ray.hit.position - ray.origin).squaredLength();
                if (local_ray.hit.distance_squared < ray.hit.distance_squared) {
                    ray.hit = local_ray.hit;
                    ray.hit.geo_type = geo->type;
                    ray.hit.geo_id = i;
                    found = true;
                }
            }
        }

        if (found) {
            ray.hit.distance = sqrtf(ray.hit.distance_squared);
            ray.hit.normal = geometries[ray.hit.geo_id].transform.externDir(ray.hit.normal).normalized();
        }

        return found;
    }
};


void load(Scene &scene, char* scene_file_path = nullptr) {
    if (scene_file_path)
        scene.file_path = scene_file_path;
    else
        scene_file_path = scene.file_path.char_ptr;

    void *file_handle = os::openFileForReading(scene_file_path);

    os::readFromFile(&scene.counts, sizeof(SceneCounts), file_handle);

    if (scene.counts.cameras) {
        Camera *camera = scene.cameras;
        for (u32 i = 0; i < scene.counts.cameras; i++, camera++) {
            os::readFromFile(&camera->focal_length, sizeof(f32), file_handle);
            os::readFromFile(&camera->zoom_amount, sizeof(f32), file_handle);
            os::readFromFile(&camera->dolly_amount, sizeof(f32), file_handle);
            os::readFromFile(&camera->target_distance, sizeof(f32), file_handle);
            os::readFromFile(&camera->current_velocity, sizeof(vec3), file_handle);
            os::readFromFile(&camera->position, sizeof(vec3), file_handle);
            os::readFromFile(&camera->rotation, sizeof(Orientation<mat3>), file_handle);
        }
    }

    if (scene.counts.geometries)
        for (u32 i = 0; i < scene.counts.geometries; i++)
            os::readFromFile(scene.geometries + i, sizeof(Geometry), file_handle);

    if (scene.counts.grids)
        for (u32 i = 0; i < scene.counts.grids; i++)
            os::readFromFile(scene.grids + i, sizeof(Grid), file_handle);

    if (scene.counts.boxes)
        for (u32 i = 0; i < scene.counts.boxes; i++)
            os::readFromFile(scene.boxes + i, sizeof(Box), file_handle);

    if (scene.counts.curves)
        for (u32 i = 0; i < scene.counts.curves; i++)
            os::readFromFile(scene.curves + i, sizeof(Curve), file_handle);

    if (scene.counts.meshes) {
        Mesh *mesh = scene.meshes;
        for (u32 i = 0; i < scene.counts.meshes; i++, mesh++) {
            readHeader(*mesh, file_handle);
            readContent(*mesh, file_handle);
        }
    }

    os::closeFile(file_handle);
}

void save(Scene &scene, char* scene_file_path = nullptr) {
    if (scene_file_path)
        scene.file_path = scene_file_path;
    else
        scene_file_path = scene.file_path.char_ptr;

    void *file_handle = os::openFileForWriting(scene_file_path);

    os::writeToFile(&scene.counts, sizeof(SceneCounts), file_handle);

    if (scene.counts.cameras) {
        Camera *camera = scene.cameras;
        for (u32 i = 0; i < scene.counts.cameras; i++, camera++) {
            os::writeToFile(&camera->focal_length, sizeof(f32), file_handle);
            os::writeToFile(&camera->zoom_amount, sizeof(f32), file_handle);
            os::writeToFile(&camera->dolly_amount, sizeof(f32), file_handle);
            os::writeToFile(&camera->target_distance, sizeof(f32), file_handle);
            os::writeToFile(&camera->current_velocity, sizeof(vec3), file_handle);
            os::writeToFile(&camera->position, sizeof(vec3), file_handle);
            os::writeToFile(&camera->rotation, sizeof(Orientation<mat3>), file_handle);
        }
    }

    if (scene.counts.geometries)
        for (u32 i = 0; i < scene.counts.geometries; i++)
            os::writeToFile(scene.geometries + i, sizeof(Geometry), file_handle);

    if (scene.counts.grids)
        for (u32 i = 0; i < scene.counts.grids; i++)
            os::writeToFile(scene.grids + i, sizeof(Grid), file_handle);

    if (scene.counts.boxes)
        for (u32 i = 0; i < scene.counts.boxes; i++)
            os::writeToFile(scene.boxes + i, sizeof(Box), file_handle);

    if (scene.counts.curves)
        for (u32 i = 0; i < scene.counts.curves; i++)
            os::writeToFile(scene.curves + i, sizeof(Curve), file_handle);

    if (scene.counts.meshes) {
        Mesh *mesh = scene.meshes;
        for (u32 i = 0; i < scene.counts.meshes; i++, mesh++) {
            writeHeader(*mesh, file_handle);
            writeContent(*mesh, file_handle);
        }
    }

    os::closeFile(file_handle);
}

struct Frustum {
    enum class ProjectionType {
        Orthographic = 0,
        PerspectiveGL,
        PerspectiveDX
    };

    struct Projection {
        vec3 scale;
        f32 shear;
        ProjectionType type;

        Projection(f32 focal_length, f32 height_over_width, f32 n, f32 f,
                   ProjectionType projection_type = ProjectionType::PerspectiveGL) :
                scale{0}, shear{0}, type{projection_type} {
            update(focal_length, height_over_width, n, f);
        }
        Projection(const Projection &other) : scale{other.scale}, shear{other.shear} {}


        void update(f32 focal_length, f32 height_over_width, f32 n, f32 f) {
            scale.x = focal_length * height_over_width;
            scale.y = focal_length;
            if (type != ProjectionType::Orthographic) {
                scale.z = shear = 1.0f / (f - n);
                if (type == ProjectionType::PerspectiveGL) {
                    scale.z *= f + n;
                    shear *= f * n * -2;
                } else {
                    scale.z *= f;
                    shear *= f * -n;
                }
            }
        }

        vec3 project(const vec3 &position) const {
            vec3 projected_position{
                    position.x * scale.x,
                    position.y * scale.y,
                    position.z * scale.z + shear
            };
            return projected_position / position.z;
        }
    };
    Projection projection{
            CAMERA_DEFAULT__FOCAL_LENGTH,
            (f32)DEFAULT_HEIGHT / (f32)DEFAULT_WIDTH,
            VIEWPORT_DEFAULT__NEAR_CLIPPING_PLANE_DISTANCE,
            VIEWPORT_DEFAULT__FAR_CLIPPING_PLANE_DISTANCE
    };

    f32 near_clipping_plane_distance{VIEWPORT_DEFAULT__NEAR_CLIPPING_PLANE_DISTANCE};
    f32 far_clipping_plane_distance{ VIEWPORT_DEFAULT__FAR_CLIPPING_PLANE_DISTANCE};
    bool flip_z{false}, cull_back_faces{true};

    void updateProjection(f32 focal_length, f32 height_over_width) {
        projection.update(focal_length,
                          height_over_width,
                          near_clipping_plane_distance,
                          far_clipping_plane_distance);
    }

    bool cullAndClipEdge(Edge &edge, f32 focal_length, f32 aspect_ratio) const {
        f32 distance = near_clipping_plane_distance;

        vec3 A{edge.from};
        vec3 B{edge.to};

        u8 out = (A.z < distance) | ((B.z < distance) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, (distance - A.z) / (B.z - A.z));
            else         B = B.lerpTo(A, (distance - B.z) / (A.z - B.z));
        }

        distance = far_clipping_plane_distance;
        out = (A.z > distance) | ((B.z > distance) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, (A.z - distance) / (A.z - B.z));
            else         B = B.lerpTo(A, (B.z - distance) / (B.z - A.z));
        }

        // Left plane (facing to the right):
        vec3 N{focal_length, 0, aspect_ratio};
        f32 NdotA = N.dot(A);
        f32 NdotB = N.dot(B);

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        // Right plane (facing to the left):
        N.x = -N.x;
        NdotA = N.dot(A);
        NdotB = N.dot(B);

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        // Bottom plane (facing up):
        N = {0, focal_length, 1};
        NdotA = N.dot(A);
        NdotB = N.dot(B);

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        // Top plane (facing down):
        N.y = -N.y;
        NdotA = N.dot(A);
        NdotB = N.dot(B);

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        edge.from = A;
        edge.to   = B;

        return true;
    }

    void projectEdge(Edge &edge, const Dimensions &dimensions) const {
        // Project:
        vec3 A{projection.project(edge.from)};
        vec3 B{projection.project(edge.to)};

        // NDC->screen:
        A.x += 1;
        B.x += 1;
        A.y += 1;
        B.y += 1;
        A.x *= dimensions.h_width;
        B.x *= dimensions.h_width;
        A.y *= dimensions.h_height;
        B.y *= dimensions.h_height;

        // Flip Y:
        A.y = dimensions.f_height - A.y;
        B.y = dimensions.f_height - B.y;

        edge.from = A;
        edge.to   = B;
    }
};


struct Navigation {
    struct {
        struct {
            f32 turn{   NAVIGATION_SPEED_DEFAULT__TURN  };
            f32 zoom{   NAVIGATION_SPEED_DEFAULT__ZOOM  };
            f32 dolly{  NAVIGATION_SPEED_DEFAULT__DOLLY };
            f32 pan{    NAVIGATION_SPEED_DEFAULT__PAN   };
            f32 orbit{  NAVIGATION_SPEED_DEFAULT__ORBIT };
            f32 orient{ NAVIGATION_SPEED_DEFAULT__ORIENT};
        } speed;
        f32 max_velocity{NAVIGATION_DEFAULT__MAX_VELOCITY};
        f32 acceleration{NAVIGATION_DEFAULT__ACCELERATION};
    } settings;

    Move move{};
    Turn turn{};

    bool zoomed{false};
    bool moved{false};
    bool turned{false};

    void pan(Camera &camera) {
        camera.pan(settings.speed.pan * -(f32)mouse::pos_raw_diff_x,
                   settings.speed.pan * +(f32)mouse::pos_raw_diff_y);
        moved = true;
        mouse::moved = false;
    }
    void zoom(Camera &camera) {
        camera.zoom(settings.speed.zoom * mouse::wheel_scroll_amount);
        zoomed = true;
    }
    void dolly(Camera &camera) {
        camera.dolly(settings.speed.dolly * mouse::wheel_scroll_amount);
        moved = true;
    }
    void orient(Camera &camera) {
        camera.rotate(settings.speed.orient * -(f32)mouse::pos_raw_diff_y,
                      settings.speed.orient * -(f32)mouse::pos_raw_diff_x);
        mouse::moved = false;
        turned = true;
    }
    void orbit(Camera &camera) {
        camera.orbit(settings.speed.orbit * -(f32)mouse::pos_raw_diff_x,
                     settings.speed.orbit * -(f32)mouse::pos_raw_diff_y);
        turned = true;
        moved = true;
        mouse::moved = false;
    }
    void navigate(Camera &camera, f32 delta_time) {
        vec3 target_velocity;
        if (move.right)    target_velocity.x += settings.max_velocity;
        if (move.left)     target_velocity.x -= settings.max_velocity;
        if (move.up)       target_velocity.y += settings.max_velocity;
        if (move.down)     target_velocity.y -= settings.max_velocity;
        if (move.forward)  target_velocity.z += settings.max_velocity;
        if (move.backward) target_velocity.z -= settings.max_velocity;
        if (turn.left) {
            camera.rotateAroundY(delta_time * settings.speed.turn);
            turned = true;
        }
        if (turn.right) {
            camera.rotateAroundY(delta_time * -settings.speed.turn);
            turned = true;
        }

        // Update the current speed_x and position_x:
        vec3 &velocity = camera.current_velocity;
        velocity = velocity.approachTo(target_velocity,
                                       settings.acceleration * delta_time);
        vec3 movement = velocity * delta_time;
        moved = movement.nonZero();
        if (moved) camera.position += camera.rotation * movement;
    }
    void update(Camera &camera, f32 delta_time) {
        if (mouse::is_captured) {
            navigate(camera, delta_time);
            if (mouse::moved) orient(camera);
            if (mouse::wheel_scrolled) zoom(camera);
        } else {
            if (mouse::wheel_scrolled) dolly(camera);
            if (mouse::moved) {
                if (mouse::middle_button.is_pressed) pan(camera);
                if (mouse::right_button.is_pressed) orbit(camera);
            }
        }
    }
};

enum AntiAliasing {
    NoAA,
    MSAA,
    SSAA
};

struct Canvas {
    Dimensions dimensions;
    Pixel *pixels{nullptr};
    f32 *depths{nullptr};

    AntiAliasing antialias;
    bool premultiply = true;

    Canvas(AntiAliasing antialiasing = NoAA) : antialias{antialiasing} {
        if (memory::canvas_memory_capacity) {
            pixels = (Pixel*)memory::canvas_memory;
            memory::canvas_memory += CANVAS_PIXELS_SIZE;
            memory::canvas_memory_capacity -= CANVAS_PIXELS_SIZE;

            depths = (f32*)memory::canvas_memory;
            memory::canvas_memory += CANVAS_DEPTHS_SIZE;
            memory::canvas_memory_capacity -= CANVAS_DEPTHS_SIZE;
        } else {
            pixels = nullptr;
            depths = nullptr;
        }
    }

    Canvas(u16 width, u16 height, AntiAliasing antialiasing = NoAA) : Canvas{antialiasing} {
        dimensions.update(width, height);
    }

    Canvas(Pixel *pixels, f32 *depths) noexcept : pixels{pixels}, depths{depths} {}

    void clear(f32 red = 0, f32 green = 0, f32 blue = 0, f32 opacity = 1.0f, f32 depth = INFINITY) const {
        i32 pixels_width  = dimensions.width;
        i32 pixels_height = dimensions.height;
        i32 depths_width  = dimensions.width;
        i32 depths_height = dimensions.height;

        if (antialias != NoAA) {
            depths_width *= 2;
            depths_height *= 2;
            if (antialias == SSAA) {
                pixels_width *= 2;
                pixels_height *= 2;
            }
        }

        i32 pixels_count = pixels_width * pixels_height;
        i32 depths_count = depths_width * depths_height;

        Pixel pixel{red, green, blue, opacity};

        if (pixels) for (i32 i = 0; i < pixels_count; i++) pixels[i] = pixel;
        if (depths) for (i32 i = 0; i < depths_count; i++) depths[i] = depth;
    }

    void drawFrom(Canvas &source_canvas, bool blend = true, bool include_depths = false, const RectI *viewport_bounds = nullptr) {
        RectI bounds{
                0, dimensions.width < source_canvas.dimensions.width ? dimensions.width : source_canvas.dimensions.width,
                0, dimensions.height < source_canvas.dimensions.height ? dimensions.height : source_canvas.dimensions.height
        };
        if (viewport_bounds)
            bounds -= *viewport_bounds;

        if ((antialias == SSAA) && (source_canvas.antialias == SSAA))
            bounds *= 2;

        f32 depth;
        for (i32 y = bounds.top; y < bounds.bottom; y++) {
            for (i32 x = bounds.left; x < bounds.right; x++) {
                i32 src_offset = source_canvas.antialias == SSAA ? (
                        (source_canvas.dimensions.stride * (y >> 1) + (x >> 1)) * 4 + (2 * (y & 1)) + (x & 1)
                ) : (
                                         source_canvas.dimensions.stride * y + x
                                 );

                Pixel &pixel{source_canvas.pixels[src_offset]};
                if ((pixel.opacity == 0.0f) || (
                        (pixel.color.r == 0.0f) &&
                        (pixel.color.g == 0.0f) &&
                        (pixel.color.b == 0.0f)))
                    continue;

                if (include_depths)
                    depth = source_canvas.depths[src_offset];

                if (blend) {
                    setPixel(x, y, pixel.color, pixel.opacity, include_depths ? depth : 0.0f);
                } else {
                    i32 trg_offset = antialias == SSAA ? (
                            (dimensions.stride * (y >> 1) + (x >> 1)) * 4 + (2 * (y & 1)) + (x & 1)
                    ) : (
                                             dimensions.stride * y + x
                                     );
                    pixels[trg_offset] = source_canvas.pixels[src_offset];
                    if (include_depths && depth < depths[trg_offset])
                        depths[trg_offset] = depth;
                }
            }
        }
    }

    void drawToWindow() const {
        u32 *content_value = window::content;
        Pixel *pixel = pixels;
        for (u16 y = 0; y < window::height; y++)
            for (u16 x = 0; x < window::width; x++, content_value++) {
                *content_value = getPixelContent(pixel);

                if (antialias == SSAA)
                    pixel += 4;
                else
                    pixel++;
            }
    }

    INLINE void setPixel(i32 x, i32 y, const Color &color, f32 opacity = 1.0f, f32 depth = 0, f32 z_top = 0, f32 z_bottom = 0, f32 z_right = 0) const {
        u32 offset = antialias == SSAA ? ((dimensions.stride * (y >> 1) + (x >> 1)) * 4 + (2 * (y & 1)) + (x & 1)) : (dimensions.stride * y + x);
        Pixel pixel{color * color, opacity};
        if (premultiply && opacity != 1.0f)
            pixel.color *= pixel.opacity;

        Pixel *out_pixel = pixels + offset;
        f32 *out_depth = depths ? (depths + (antialias == MSAA ? offset * 4 : offset)) : nullptr;
        if (
                (
                        (out_depth == nullptr ||
                         *out_depth == INFINITY) &&
                        (out_pixel->color.r == 0) &&
                        (out_pixel->color.g == 0) &&
                        (out_pixel->color.b == 0)
                ) ||
                (
                        (opacity == 1.0f) &&
                        (depth == 0.0f) &&
                        (z_top == 0.0f) &&
                        (z_bottom == 0.0f) &&
                        (z_right == 0.0f)
                )
                ) {
            *out_pixel = pixel;
            if (depths) {
                out_depth[0] = depth;
                if (antialias == MSAA) out_depth[1] = out_depth[2] = out_depth[3] = 0;
            }

            return;
        }

        Pixel *bg{out_pixel}, *fg{&pixel};
        if (antialias == MSAA) {
            Pixel accumulated_pixel{};
            for (u8 i = 0; i < 4; i++) {
                if (depths) {
                    if (i) depth = i == 1 ? z_top : (i == 2 ? z_bottom : z_right);
                    _sortPixelsByDepth(depth, &pixel, out_depth, out_pixel, &bg, &fg);
                    out_depth++;
                }
                accumulated_pixel += fg->opacity == 1 ? *fg : fg->alphaBlendOver(*bg, premultiply);
            }
            *out_pixel = accumulated_pixel * 0.25f;
        } else {
            if (depths)
                _sortPixelsByDepth(depth, &pixel, out_depth, out_pixel, &bg, &fg);
            *out_pixel = fg->opacity == 1 ? *fg : fg->alphaBlendOver(*bg, premultiply);
        }
    }

    INLINE u32 getPixelContent(Pixel *pixel) const {
        return antialias == SSAA ?
               _isTransparentPixelQuad(pixel) ? 0 : _blendPixelQuad(pixel).asContent(premultiply) :
               pixel->opacity ? pixel->asContent(premultiply) : 0;
    }

#ifdef SLIM_ENABLE_CANVAS_TEXT_DRAWING
    INLINE void drawText(char *str, i32 x, i32 y, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#ifdef SLIM_VEC2
    INLINE void drawText(char *str, vec2i position, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawText(char *str, vec2 position, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#endif
#endif

#ifdef SLIM_ENABLE_CANVAS_NUMBER_DRAWING
    INLINE void drawNumber(i32 number, i32 x, i32 y, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#ifdef SLIM_VEC2
    INLINE void drawNumber(i32 number, vec2i position, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawNumber(i32 number, vec2 position, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#endif
#endif

#ifdef SLIM_ENABLE_CANVAS_HUD_DRAWING
    INLINE void drawHUD(const HUD &hud, const RectI *viewport_bounds = nullptr) const;
#endif

#ifdef SLIM_ENABLE_CANVAS_LINE_DRAWING
    INLINE void drawHLine(RangeI x_range, i32 y, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawHLine(i32 x_start, i32 x_end, i32 y, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawVLine(RangeI y_range, i32 x, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawVLine(i32 y_start, i32 y_end, i32 x, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawLine(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawLine(f32 x1, f32 y1, f32 x2, f32 y2, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) const;

#ifdef SLIM_VEC2
    INLINE void drawLine(vec2 from, vec2 to, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawLine(vec2i from, vec2i to, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) const;
#endif
#endif

#ifdef SLIM_ENABLE_CANVAS_RECTANGLE_DRAWING
    INLINE void drawRect(RectI rect, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawRect(Rect rect, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillRect(RectI rect, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillRect(Rect rect, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#endif

#ifdef SLIM_ENABLE_CANVAS_TRIANGLE_DRAWING
    INLINE void drawTriangle(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, const Color &color = White, f32 opacity = 0.5f, u8 line_width = 0, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillTriangle(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;

#ifdef SLIM_VEC2
    INLINE void drawTriangle(vec2 p1, vec2 p2, vec2 p3, const Color &color = White, f32 opacity = 0.5f, u8 line_width = 0, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillTriangle(vec2 p1, vec2 p2, vec2 p3, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawTriangle(vec2i p1, vec2i p2, vec2i p3, const Color &color = White, f32 opacity = 0.5f, u8 line_width = 0, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillTriangle(vec2i p1, vec2i p2, vec2i p3, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#endif
#endif

#ifdef SLIM_ENABLE_CANVAS_CIRCLE_DRAWING
    INLINE void fillCircle(i32 center_x, i32 center_y, i32 radius, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawCircle(i32 center_x, i32 center_y, i32 radius, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#ifdef SLIM_VEC2
    INLINE void drawCircle(vec2i center, i32 radius, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillCircle(vec2i center, i32 radius, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void drawCircle(vec2 center, i32 radius, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
    INLINE void fillCircle(vec2 center, i32 radius, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) const;
#endif
#endif

private:
    static INLINE bool _isTransparentPixelQuad(Pixel *pixel_quad) {
        return (
                (pixel_quad[0].opacity == 0.0f) &&
                (pixel_quad[1].opacity == 0.0f) &&
                (pixel_quad[2].opacity == 0.0f) &&
                (pixel_quad[3].opacity == 0.0f)
        );
    }

    INLINE Pixel _blendPixelQuad(Pixel *pixel_quad) const {
        Pixel TL{pixel_quad[0]}, TR{pixel_quad[1]}, BL{pixel_quad[2]}, BR{pixel_quad[3]};
        f32 TL_factor = 0.25f;
        f32 TR_factor = 0.25f;
        f32 BL_factor = 0.25f;
        f32 BR_factor = 0.25f;
        if (!premultiply) {
            TL_factor *= TL.opacity;
            TR_factor *= TR.opacity;
            BL_factor *= BL.opacity;
            BR_factor *= BR.opacity;
        }
        return {
                {
                        (
                                (TL.color.r * TL_factor) +
                                (TR.color.r * TR_factor) +
                                (BL.color.r * BL_factor) +
                                (BR.color.r * BR_factor)
                        ),
                        (
                                (TL.color.g * TL_factor) +
                                (TR.color.g * TR_factor) +
                                (BL.color.g * BL_factor) +
                                (BR.color.g * BR_factor)
                        ),
                        (
                                (TL.color.b * TL_factor) +
                                (TR.color.b * TR_factor) +
                                (BL.color.b * BL_factor) +
                                (BR.color.b * BR_factor)
                        )
                },
                (
                        TL_factor +
                        TR_factor +
                        BL_factor +
                        BR_factor
                )
        };
    }

    static INLINE void _sortPixelsByDepth(f32 depth, Pixel *pixel, f32 *out_depth, Pixel *out_pixel, Pixel **background, Pixel **foreground) {
        if (depth == 0.0f || depth < *out_depth) {
            *out_depth = depth;
            *background = out_pixel;
            *foreground = pixel;
        } else {
            *background = pixel;
            *foreground = out_pixel;
        }
    }
};



void _drawHLine(RangeI x_range, i32 y, const Canvas &canvas, const Color &color, f32 opacity, const RectI *viewport_bounds) {
    RangeI y_range{0, canvas.dimensions.height - 1};

    if (viewport_bounds) {
        y += viewport_bounds->top;
        y_range -= viewport_bounds->y_range;

        x_range += viewport_bounds->left;
        x_range -= viewport_bounds->x_range;
    }
    x_range.sub(0, canvas.dimensions.width - 1);
    if (!x_range || !y_range[y])
        return;

    if (canvas.antialias == SSAA) {
        y *= 2;
        x_range *= 2;
        for (i32 x = x_range.first; x <= x_range.last; x += 2) {
            canvas.setPixel(x, y, color, opacity);
            canvas.setPixel(x+1, y, color, opacity);
            canvas.setPixel(x, y+1, color, opacity);
            canvas.setPixel(x+1, y+1, color, opacity);
        }
    } else
        for (i32 x = x_range.first; x <= x_range.last; x += 1)
            canvas.setPixel(x, y, color, opacity);
}

void _drawVLine(RangeI y_range, i32 x, const Canvas &canvas, const Color &color, f32 opacity, const RectI *viewport_bounds) {
    RangeI x_range{0, canvas.dimensions.width - 1};

    if (viewport_bounds) {
        x += viewport_bounds->left;
        x_range -= viewport_bounds->x_range;

        y_range += viewport_bounds->top;
        y_range -= viewport_bounds->y_range;
    }
    y_range.sub(0, canvas.dimensions.height - 1);
    if (!y_range || !x_range[x])
        return;

    if (canvas.antialias == SSAA) {
        x *= 2;
        y_range *= 2;
        for (i32 y = y_range.first; y <= y_range.last; y += 2) {
            canvas.setPixel(x, y, color, opacity);
            canvas.setPixel(x+1, y, color, opacity);
            canvas.setPixel(x, y+1, color, opacity);
            canvas.setPixel(x+1, y+1, color, opacity);
        }
    } else
        for (i32 y = y_range.first; y <= y_range.last; y += 1)
            canvas.setPixel(x, y, color, opacity);
}

void _drawLine(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, const Canvas &canvas,
               const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) {
    Range float_x_range{x1 <= x2 ? x1 : x2, x1 <= x2 ? x2 : x1};
    Range float_y_range{y1 <= y2 ? y1 : y2, y1 <= y2 ? y2 : y1};
    if (viewport_bounds) {
        const f32 left = (f32)viewport_bounds->left;
        const f32 right = (f32)viewport_bounds->right;
        const f32 top = (f32)viewport_bounds->top;
        const f32 bottom = (f32)viewport_bounds->bottom;
        x1 += left;
        x2 += left;
        y1 += top;
        y2 += top;
        float_x_range += left;
        float_y_range += top;
        float_x_range.sub(left, right);
        float_y_range.sub(top, bottom);
    }
    float_x_range.sub(0, canvas.dimensions.f_width - 1.0f);
    float_y_range.sub(0, canvas.dimensions.f_height - 1.0f);
    if (!float_x_range || !float_y_range)
        return;

    RangeI x_range{(i32)float_x_range.first, (i32)(ceilf(float_x_range.last))};
    RangeI y_range{(i32)float_y_range.first, (i32)(ceilf(float_y_range.last))};
    if (x_range.last == (i32)canvas.dimensions.width) x_range.last--;
    if (y_range.last == (i32)canvas.dimensions.height) y_range.last--;

    i32 x, y;
    if (canvas.antialias == SSAA) {
        x1 += x1;
        x2 += x2;
        y1 += y1;
        y2 += y2;
        x_range.first <<= 1;
        y_range.first <<= 1;
        x_range.last <<= 1;
        y_range.last <<= 1;
        line_width <<= 1;
        line_width++;
    }
    f32 tmp, z_range, range_remap;
    f32 dx = x2 - x1;
    f32 dy = y2 - y1;
    f32 gap, grad, first_offset, last_offset;
    f32 z, z_curr, z_step;
    f32 first_x, last_x;
    f32 first_y, last_y;
    i32 start_x, end_x;
    i32 start_y, end_y;
    bool has_depth = (canvas.depths != nullptr) && ((z1 != 0.0f) || (z2 != 0.0f));
    if (fabsf(dx) > fabsf(dy)) { // Shallow:
        if (x2 < x1) { // Left to right:
            tmp = x2; x2 = x1; x1 = tmp;
            tmp = y2; y2 = y1; y1 = tmp;
            tmp = z2; z2 = z1; z1 = tmp;
        }

        grad = dy / dx;

        first_x = roundf(x1);
        last_x  = roundf(x2);
        first_offset = first_x - x1;
        last_offset  = last_x  - x2;

        first_y = y1 + grad * first_offset;
        last_y  = y2 + grad * last_offset;

        start_x = (i32)first_x;
        start_y = (i32)first_y;
        end_x   = (i32)last_x;
        end_y   = (i32)last_y;

        x = start_x;
        y = start_y;
        gap = oneMinusFractionOf(x1 + 0.5f);

        if (x_range[x]) {
            if (y_range[y]) canvas.setPixel(x, y, color, oneMinusFractionOf(first_y) * gap * opacity, z1);
            for (u8 i = 0; i < line_width; i++) if (y_range[++y]) canvas.setPixel(x, y, color, opacity, z1);
            if (y_range[++y]) canvas.setPixel(x, y, color, fractionOf(first_y) * gap * opacity, z1);
        }

        x = end_x;
        y = end_y;
        gap = fractionOf(x2 + 0.5f);

        if (x_range[x]) {
            if (y_range[y]) canvas.setPixel(x, y, color, oneMinusFractionOf(last_y) * gap * opacity, z2);
            for (u8 i = 0; i < line_width; i++) if (y_range[++y]) canvas.setPixel(x, y, color, opacity, z2);
            if (y_range[++y]) canvas.setPixel(x, y, color, fractionOf(last_y) * gap * opacity, z2);
        }

        if (has_depth) { // Compute one-over-depth start and step
            z1 = 1.0f / z1;
            z2 = 1.0f / z2;
            z_range = z2 - z1;
            range_remap = z_range / (x2 - x1);
            z1 += range_remap * (first_offset + 1.0f);
            z2 += range_remap * (last_offset  - 1.0f);
            z_curr = z1;
            z_range = z2 - z1;
            z_step = z_range / (last_x - first_x - 1.0f);
        } else z = 0.0f;

        gap = first_y + grad;
        for (x = start_x + 1; x < end_x; x++) {
            if (x_range[x]) {
                y = (i32) gap;

                if (has_depth) z = 1.0f / z_curr;
                if (y_range[y]) canvas.setPixel(x, y, color, oneMinusFractionOf(gap) * opacity, z);
                for (u8 i = 0; i < line_width; i++) if (y_range[++y]) canvas.setPixel(x, y, color, opacity, z);
                if (y_range[++y]) canvas.setPixel(x, y, color, fractionOf(gap) * opacity, z);
            }

            gap += grad;
            if (has_depth) z_curr += z_step;
        }
    } else { // Steep:
        if (y2 < y1) { // Bottom up:
            tmp = x2; x2 = x1; x1 = (f32)tmp;
            tmp = y2; y2 = y1; y1 = (f32)tmp;
            tmp = z2; z2 = z1; z1 = tmp;
        }

        grad = dx / dy;

        first_y = roundf(y1);
        last_y  = roundf(y2);

        first_offset = y1 - first_y;
        last_offset  = last_y  - y2;

        first_x = x1 + grad * first_offset;
        last_x  = x2 + grad * last_offset;

        start_y = (i32)first_y;
        start_x = (i32)first_x;

        end_y = (i32)last_y;
        end_x = (i32)last_x;

        x = start_x;
        y = start_y;
        gap = oneMinusFractionOf(y1 + 0.5f);

        if (y_range[y]) {
            if (x_range[x]) canvas.setPixel(x, y, color, oneMinusFractionOf(first_x) * gap * opacity, z1);
            for (u8 i = 0; i < line_width; i++) if (x_range[++x]) canvas.setPixel(x, y, color, opacity, z1);
            if (x_range[++x]) canvas.setPixel(x, y, color, fractionOf(first_x) * gap * opacity, z1);
        }

        x = end_x;
        y = end_y;
        gap = fractionOf(y2 + 0.5f);

        if (y_range[y]) {
            if (x_range[x]) canvas.setPixel(x, y, color, oneMinusFractionOf(last_x) * gap * opacity, z2);
            for (u8 i = 0; i < line_width; i++) if (x_range[++x]) canvas.setPixel(x, y, color, opacity, z2);
            if (x_range[++x]) canvas.setPixel(x, y, color, fractionOf(last_x) * gap * opacity, z2);
        }

        if (has_depth) { // Compute one-over-depth start and step
            z1 = 1.0f / z1;
            z2 = 1.0f / z2;
            z_range = z2 - z1;
            range_remap = z_range / (y2 - y1);
            z1 += range_remap * (first_offset + 1.0f);
            z2 += range_remap * (last_offset  - 1.0f);
            z_range = z2 - z1;
            z_step = z_range / (last_y - first_y - 1.0f);
            z_curr = z1;
        } else z = 0.0f;

        gap = first_x + grad;
        for (y = start_y + 1; y < end_y; y++) {
            if (y_range[y]) {
                if (has_depth) z = 1.0f / z_curr;
                x = (i32)gap;

                if (x_range[x]) canvas.setPixel(x, y, color, oneMinusFractionOf(gap) * opacity, z);
                for (u8 i = 0; i < line_width; i++) if (x_range[++x]) canvas.setPixel(x, y, color, opacity, z);
                if (x_range[++x]) canvas.setPixel(x, y, color, fractionOf(gap) * opacity, z);
            }

            gap += grad;
            if (has_depth) z_curr += z_step;
        }
    }
}


#ifdef SLIM_ENABLE_CANVAS_LINE_DRAWING
INLINE void Canvas::drawHLine(RangeI x_range, i32 y, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawHLine(x_range, y, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawHLine(i32 x_start, i32 x_end, i32 y, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawHLine(RangeI{x_start, x_end}, y, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawVLine(RangeI y_range, i32 x, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawVLine(y_range, x, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawVLine(i32 y_start, i32 y_end, i32 x, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawVLine(RangeI{y_start, y_end}, x, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawLine(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawLine(x1, y1, z1, x2, y2, z2, *this, color, opacity, line_width, viewport_bounds);
}
INLINE void Canvas::drawLine(f32 x1, f32 y1, f32 x2, f32 y2, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawLine(x1, y1, 0, x2, y2, 0, *this, color, opacity, line_width, viewport_bounds);
}

#ifdef SLIM_VEC2
INLINE void Canvas::drawLine(vec2 from, vec2 to, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawLine(from.x, from.y, 0, to.x, to.y, 0, *this, color, opacity, line_width, viewport_bounds);
}
INLINE void Canvas::drawLine(vec2i from, vec2i to, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawLine((f32)from.x, (f32)from.y, 0, (f32)to.x, (f32)to.y, 0, *this, color, opacity, line_width, viewport_bounds);
}
#endif
#endif



INLINE void drawHLine(RangeI x_range, i32 y, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawHLine(x_range, y, canvas, color, opacity, viewport_bounds);
}

INLINE void drawHLine(i32 x_start, i32 x_end, i32 y, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawHLine(RangeI{x_start, x_end}, y, canvas, color, opacity, viewport_bounds);
}

INLINE void drawVLine(RangeI y_range, i32 x, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawVLine(y_range, x, canvas, color, opacity, viewport_bounds);
}
INLINE void drawVLine(i32 y_start, i32 y_end, i32 x, const Canvas &canvas, const Color &color, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawVLine(RangeI{y_start, y_end}, x, canvas, color, opacity, viewport_bounds);
}

INLINE void drawLine(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) {
    _drawLine(x1, y1, z1, x2, y2, z2, canvas, color, opacity, line_width, viewport_bounds);
}

INLINE void drawLine(f32 x1, f32 y1, f32 x2, f32 y2, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) {
    _drawLine(x1, y1, 0, x2, y2, 0, canvas, color, opacity, line_width, viewport_bounds);
}

#ifdef SLIM_VEC2
void drawLine(vec2 from, vec2 to, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) {
    _drawLine(from.x, from.y, 0, to.x, to.y, 0, canvas, color, opacity, line_width, viewport_bounds);
}
#endif



void _drawRect(RectI rect, const Canvas &canvas, const Color &color, f32 opacity, const RectI *viewport_bounds) {
    RectI bounds{0, canvas.dimensions.width - 1, 0, canvas.dimensions.height - 1};
    if (viewport_bounds) {
        bounds -= *viewport_bounds;
        rect.x_range += viewport_bounds->left;
        rect.y_range += viewport_bounds->top;
    }
    if (rect.isOutsideOf(bounds))
        return;

    const bool draw_top = bounds.y_range[rect.top];
    const bool draw_bottom = bounds.y_range[rect.bottom];
    const bool draw_left = bounds.x_range[rect.left];
    const bool draw_right = bounds.x_range[rect.right];

    const bool draw_horizontal = draw_top || draw_bottom;
    const bool draw_vertical = draw_left || draw_right;
    if (!(draw_horizontal || draw_vertical))
        return;

    rect = bounds - rect;
    if (!rect)
        return;

    if (canvas.antialias == SSAA) {
        rect *= 2;

        if (draw_horizontal) {
            for (i32 x = rect.left; x <= rect.right; x += 2) {
                if (draw_bottom) {
                    canvas.setPixel(x, rect.bottom, color, opacity);
                    canvas.setPixel(x+1, rect.bottom, color, opacity);
                    canvas.setPixel(x, rect.bottom+1, color, opacity);
                    canvas.setPixel(x+1, rect.bottom+1, color, opacity);
                }
                if (draw_top) {
                    canvas.setPixel(x, rect.top, color, opacity);
                    canvas.setPixel(x+1, rect.top, color, opacity);
                    canvas.setPixel(x, rect.top+1, color, opacity);
                    canvas.setPixel(x+1, rect.top+1, color, opacity);
                }
            }
        }

        if (draw_vertical) {
            for (i32 y = rect.top; y <= rect.bottom; y += 2) {
                if (draw_left) {
                    canvas.setPixel(rect.left, y, color, opacity);
                    canvas.setPixel(rect.left+1, y, color, opacity);
                    canvas.setPixel(rect.left, y+1, color, opacity);
                    canvas.setPixel(rect.left+1, y+1, color, opacity);
                }
                if (draw_right) {
                    canvas.setPixel(rect.right, y, color, opacity);
                    canvas.setPixel(rect.right+1, y, color, opacity);
                    canvas.setPixel(rect.right, y+1, color, opacity);
                    canvas.setPixel(rect.right+1, y+1, color, opacity);
                }
            }
        }
    } else {
        if (draw_horizontal) {
            for (i32 x = rect.left; x <= rect.right; x++) {
                if (draw_bottom) canvas.setPixel(x, rect.bottom, color, opacity);
                if (draw_top) canvas.setPixel(x, rect.top, color, opacity);
            }
        }

        if (draw_vertical) {
            for (i32 y = rect.top; y <= rect.bottom; y++) {
                if (draw_right) canvas.setPixel(rect.right, y, color, opacity);
                if (draw_left) canvas.setPixel(rect.left, y, color, opacity);
            }
        }
    }
}

void _fillRect(RectI rect, const Canvas &canvas, const Color &color, f32 opacity, const RectI *viewport_bounds) {
    RectI bounds{0, canvas.dimensions.width - 1, 0, canvas.dimensions.height - 1};
    if (viewport_bounds) {
        bounds -= *viewport_bounds;
        rect.x_range += viewport_bounds->left;
        rect.y_range += viewport_bounds->top;
    }
    if (rect.isOutsideOf(bounds))
        return;

    rect = bounds - rect;
    if (!rect)
        return;

    if (canvas.antialias == SSAA) {
        rect *= 2;

        for (i32 y = rect.top; y <= rect.bottom; y += 2) {
            for (i32 x = rect.left; x <= rect.right; x += 2) {
                canvas.setPixel(x, y, color, opacity);
                canvas.setPixel(x+1, y, color, opacity);
                canvas.setPixel(x, y+1, color, opacity);
                canvas.setPixel(x+1, y+1, color, opacity);
            }
        }
    } else
        for (i32 y = rect.top; y <= rect.bottom; y++)
            for (i32 x = rect.left; x <= rect.right; x++)
                canvas.setPixel(x, y, color, opacity);
}


#ifdef SLIM_ENABLE_CANVAS_RECTANGLE_DRAWING
INLINE void Canvas::drawRect(RectI rect, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawRect(rect, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawRect(Rect rect, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    RectI rectI{(i32)rect.left, (i32)rect.right, (i32)rect.top, (i32)rect.bottom};
    _drawRect(rectI, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::fillRect(RectI rect, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _fillRect(rect, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::fillRect(Rect rect, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    RectI rectI{(i32)rect.left, (i32)rect.right, (i32)rect.top, (i32)rect.bottom};
    _fillRect(rectI, *this, color, opacity, viewport_bounds);
}
#endif


INLINE void drawRect(RectI rect, const Canvas &canvas, Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawRect(rect, canvas, color, opacity, viewport_bounds);
}

INLINE void drawRect(Rect rect, const Canvas &canvas, Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    RectI rectI{(i32)rect.left, (i32)rect.right, (i32)rect.top, (i32)rect.bottom};
    _drawRect(rectI, canvas, color, opacity, viewport_bounds);
}


INLINE void fillRect(RectI rect, const Canvas &canvas, Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _fillRect(rect, canvas, color, opacity, viewport_bounds);
}

INLINE void fillRect(Rect rect, const Canvas &canvas, Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    RectI rectI{(i32)rect.left, (i32)rect.right, (i32)rect.top, (i32)rect.bottom};
    _fillRect(rectI, canvas, color, opacity, viewport_bounds);
}



void _paintCircle(bool fill, i32 center_x, i32 center_y, i32 radius, const Canvas &canvas,
                  const Color &color, f32 opacity, const RectI *viewport_bounds) {
    RectI bounds{0, canvas.dimensions.width - 1, 0, canvas.dimensions.height - 1};
    RectI rect{center_x - radius,
               center_x + radius,
               center_y - radius,
               center_y + radius};
    if (viewport_bounds) {
        center_x += viewport_bounds->left;
        center_y += viewport_bounds->top;
        rect.x_range += viewport_bounds->left;
        rect.y_range += viewport_bounds->top;
        rect -= *viewport_bounds;
        bounds -= *viewport_bounds;
    }
    rect -= bounds;
    if (!rect)
        return;

    if (radius <= 1) {
        if (canvas.antialias == SSAA) {
            center_x *= 2;
            center_y *= 2;
            canvas.setPixel(center_x, center_y, color, opacity);
            canvas.setPixel(center_x+1, center_y, color, opacity);
            canvas.setPixel(center_x, center_y+1, color, opacity);
            canvas.setPixel(center_x+1, center_y+1, color, opacity);
        } else
            canvas.setPixel(center_x, center_y, color, opacity);

        return;
    }

    if (canvas.antialias == SSAA) {
        center_x *= 2;
        center_y *= 2;
        radius *= 2;
        bounds *= 2;
    }

    i32 x = radius, y = 0, y2 = 0;
    i32 r2 = radius * radius;
    i32 x2 = r2;

    RangeI x_range1{center_x - radius, center_x + radius};
    RangeI y_range2{center_y - radius, center_y + radius};
    RangeI y_range1{center_y, center_y};
    RangeI x_range2{center_x, center_x};
    RangeI range1, range2;
    i32 i;

    while (y <= x) {
        if (fill) {
            range1 = bounds.x_range - x_range1;
            range2 = bounds.x_range - x_range2;
            if (bounds.y_range[y_range1.first]) for (i = range1.first; i <= range1.last; i++) canvas.setPixel(i, y_range1.first, color, opacity);
            if (bounds.y_range[y_range1.last])  for (i = range1.first; i <= range1.last; i++) canvas.setPixel(i, y_range1.last , color, opacity);
            if (bounds.y_range[y_range2.first]) for (i = range2.first; i <= range2.last; i++) canvas.setPixel(i, y_range2.first, color, opacity);
            if (bounds.y_range[y_range2.last])  for (i = range2.first; i <= range2.last; i++) canvas.setPixel(i, y_range2.last , color, opacity);
        } else {
            if (bounds.y_range[y_range1.first]) {
                if (bounds.x_range[x_range1.first]) canvas.setPixel(x_range1.first, y_range1.first, color, opacity);
                if (bounds.x_range[x_range1.last ]) canvas.setPixel(x_range1.last,  y_range1.first, color, opacity);
            }
            if (bounds.y_range[y_range1.last]) {
                if (bounds.x_range[x_range1.first]) canvas.setPixel(x_range1.first, y_range1.last, color, opacity);
                if (bounds.x_range[x_range1.last ]) canvas.setPixel(x_range1.last,  y_range1.last, color, opacity);
            }

            if (bounds.y_range[y_range2.first]) {
                if (bounds.x_range[x_range2.first]) canvas.setPixel(x_range2.first, y_range2.first, color, opacity);
                if (bounds.x_range[x_range2.last ]) canvas.setPixel(x_range2.last,  y_range2.first, color, opacity);
            }
            if (bounds.y_range[y_range2.last]) {
                if (bounds.x_range[x_range2.first]) canvas.setPixel(x_range2.first, y_range2.last, color, opacity);
                if (bounds.x_range[x_range2.last ]) canvas.setPixel(x_range2.last,  y_range2.last, color, opacity);
            }
        }

        if ((x2 + y2) > r2) {
            x--;
            x2 = x * x;

            x_range1.first++;
            y_range2.first++;
            x_range1.last--;
            y_range2.last--;
        }

        y++;
        y2 = y * y;

        y_range1.first--;
        x_range2.first--;
        y_range1.last++;
        x_range2.last++;
    }
}


#ifdef SLIM_ENABLE_CANVAS_CIRCLE_DRAWING
INLINE void Canvas::fillCircle(i32 center_x, i32 center_y, i32 radius, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _paintCircle(true, center_x, center_y, radius, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawCircle(i32 center_x, i32 center_y, i32 radius, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _paintCircle(false, center_x, center_y, radius, *this, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
INLINE void Canvas::drawCircle(vec2i center, i32 radius, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _paintCircle(false, center.x, center.y, radius, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::fillCircle(vec2i center, i32 radius, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _paintCircle(true, center.x, center.y, radius, *this, color, opacity, viewport_bounds);
}
INLINE void Canvas::drawCircle(vec2 center, i32 radius, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _paintCircle(false, (i32)center.x, (i32)center.y, radius, *this, color, opacity, viewport_bounds);
}
INLINE void Canvas::fillCircle(vec2 center, i32 radius, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _paintCircle(true, (i32)center.x, (i32)center.y, radius, *this, color, opacity, viewport_bounds);
}
#endif
#endif



INLINE void fillCircle(i32 center_x, i32 center_y, i32 radius, const Canvas &canvas,
                       Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _paintCircle(true, center_x, center_y, radius, canvas, color, opacity, viewport_bounds);
}

INLINE void drawCircle(i32 center_x, i32 center_y, i32 radius, const Canvas &canvas,
                       Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _paintCircle(false, center_x, center_y, radius, canvas, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
INLINE void drawCircle(vec2i center, i32 radius, const Canvas &canvas,
                       Color color = White, f32 opacity = 1.0f,
                       const RectI *viewport_bounds = nullptr) {
    _paintCircle(false, center.x, center.y, radius, canvas, color, opacity, viewport_bounds);
}

INLINE void fillCircle(vec2i center, i32 radius,
                       const Canvas &canvas,
                       Color color = White, f32 opacity = 1.0f,
                       const RectI *viewport_bounds = nullptr) {
    _paintCircle(true, center.x, center.y, radius, canvas, color, opacity, viewport_bounds);
}
INLINE void drawCircle(vec2 center, i32 radius, const Canvas &canvas,
                       Color color = White, f32 opacity = 1.0f,
                       const RectI *viewport_bounds = nullptr) {
    _paintCircle(false, (i32)center.x, (i32)center.y, radius, canvas, color, opacity, viewport_bounds);
}

INLINE void fillCircle(vec2 center, i32 radius,
                       const Canvas &canvas,
                       Color color = White, f32 opacity = 1.0f,
                       const RectI *viewport_bounds = nullptr) {
    _paintCircle(true, (i32)center.x, (i32)center.y, radius, canvas, color, opacity, viewport_bounds);
}
#endif


INLINE void _drawTriangle(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3,
                          const Canvas &canvas, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) {
    drawLine(x1, y1, 0, x2, y2, 0, canvas, color, opacity, line_width, viewport_bounds);
    drawLine(x2, y2, 0, x3, y3, 0, canvas, color, opacity, line_width, viewport_bounds);
    drawLine(x3, y3, 0, x1, y1, 0, canvas, color, opacity, line_width, viewport_bounds);
}


void _fillTriangle(f32 x1, f32 y1,
                   f32 x2, f32 y2,
                   f32 x3, f32 y3,
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
    for (u32 y = first_y; y <= last_y; y++, C_start += Cdy, B_start += Bdy) {
        B = B_start;
        C = C_start;

        for (u32 x = first_x; x <= last_x; x++, B += Bdx, C += Cdx) {
            if (((Bdx < 0) && (B < 0)) ||
                ((Cdx < 0) && (C < 0)))
                break;

            A = 1 - B - C;

            // Skip the pixel if it's outside:
            if (fminf(A, fminf(B, C)) < 0)
                continue;

            canvas.setPixel(x, y, color, opacity);
        }
    }
}


#ifdef SLIM_ENABLE_CANVAS_TRIANGLE_DRAWING
INLINE void Canvas::drawTriangle(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawTriangle(x1, y1, x2, y2, x3, y3, *this, color, opacity, line_width, viewport_bounds);
}

INLINE void Canvas::drawTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawTriangle((f32)x1, (f32)y1, (f32)x2, (f32)y2, (f32)x3, (f32)y3, *this, color, opacity, line_width, viewport_bounds);
}

INLINE void Canvas::fillTriangle(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _fillTriangle(x1, y1, x2, y2, x3, y3, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::fillTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _fillTriangle((f32)x1, (f32)y1, (f32)x2, (f32)y2, (f32)x3, (f32)y3, *this, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
INLINE void Canvas::drawTriangle(vec2 p1, vec2 p2, vec2 p3, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, *this, color, opacity, line_width, viewport_bounds);
}

INLINE void Canvas::fillTriangle(vec2 p1, vec2 p2, vec2 p3, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, *this, color, opacity, viewport_bounds);
}

INLINE void Canvas::drawTriangle(vec2i p1, vec2i p2, vec2i p3, const Color &color, f32 opacity, u8 line_width, const RectI *viewport_bounds) const {
    _drawTriangle((f32)p1.x, (f32)p1.y, (f32)p2.x, (f32)p2.y, (f32)p3.x, (f32)p3.y, *this, color, opacity, line_width, viewport_bounds);
}

INLINE void Canvas::fillTriangle(vec2i p1, vec2i p2, vec2i p3, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _fillTriangle((f32)p1.x, (f32)p1.y, (f32)p2.x, (f32)p2.y, (f32)p3.x, (f32)p3.y, *this, color, opacity, viewport_bounds);
}
#endif
#endif



INLINE void drawTriangle(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, const Canvas &canvas,
                         Color color = White, f32 opacity = 1.0f, u8 line_width = 1, const RectI *viewport_bounds = nullptr) {
    _drawTriangle(x1, y1, x2, y2, x3, y3, canvas, color, opacity, line_width, viewport_bounds);
}

INLINE void drawTriangle(i32 x1, i32 y1,
                         i32 x2, i32 y2,
                         i32 x3, i32 y3,
                         const Canvas &canvas,
                         Color color = White, f32 opacity = 0.5f, u8 line_width = 0,
                         const RectI *viewport_bounds = nullptr) {
    _drawTriangle((f32)x1, (f32)y1, (f32)x2, (f32)y2, (f32)x3, (f32)y3, canvas, color, opacity, line_width, viewport_bounds);
}


INLINE void fillTriangle(f32 x1, f32 y1,
                         f32 x2, f32 y2,
                         f32 x3, f32 y3,
                         const Canvas &canvas, Color color = White, f32 opacity = 1.0f,
                         const RectI *viewport_bounds = nullptr) {
    _fillTriangle(x1, y1, x2, y2, x3, y3, canvas, color, opacity, viewport_bounds);
}

INLINE void fillTriangle(i32 x1, i32 y1,
                         i32 x2, i32 y2,
                         i32 x3, i32 y3,
                         const Canvas &canvas, Color color = White, f32 opacity = 1.0f,
                         const RectI *viewport_bounds = nullptr) {
    _fillTriangle((f32)x1, (f32)y1, (f32)x2, (f32)y2, (f32)x3, (f32)y3, canvas, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
void drawTriangle(vec2 p1, vec2 p2, vec2 p3, const Canvas &canvas,
                  Color color = White, f32 opacity = 0.5f, u8 line_width = 0, const RectI *viewport_bounds = nullptr) {
    _drawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, canvas, color, opacity, line_width, viewport_bounds);
}

void fillTriangle(vec2 p1, vec2 p2, vec2 p3, const Canvas &canvas,
                  Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, canvas, color, opacity, viewport_bounds);
}

void drawTriangle(vec2i p1, vec2i p2, vec2i p3, const Canvas &canvas,
                  Color color = White, f32 opacity = 0.5f, u8 line_width = 0,
                  const RectI *viewport_bounds = nullptr) {
    _drawTriangle((f32)p1.x, (f32)p1.y, (f32)p2.x, (f32)p2.y, (f32)p3.x, (f32)p3.y, canvas, color, opacity, line_width, viewport_bounds);
}

void fillTriangle(vec2i p1, vec2i p2, vec2i p3,
                  const Canvas &canvas,
                  Color color = White, f32 opacity = 1.0f,
                  const RectI *viewport_bounds = nullptr) {
    _fillTriangle((f32)p1.x, (f32)p1.y, (f32)p2.x, (f32)p2.y, (f32)p3.x, (f32)p3.y, canvas, color, opacity, viewport_bounds);
}
#endif


#define LINE_HEIGHT 14
#define FIRST_CHARACTER_CODE 32
#define LAST_CHARACTER_CODE 126
#define INTERNAL_FONT_WIDTH (FONT_WIDTH * 2)
#define INTERNAL_FONT_HEIGHT (FONT_HEIGHT * 2)

// Header File for SSD1306 characters
// Generated with TTF2BMH
// Font JetBrains Mono Regular
// Font Size: 24
u8 bitmap_32[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_33[] = {0,0,0,0,0,0,62,254,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14,14,14,4,0,0,0,0,0,0,0,0};
u8 bitmap_34[] = {0,0,0,0,254,254,0,0,0,254,254,254,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_35[] = {0,0,192,192,192,252,254,192,192,192,192,252,254,192,192,0,0,0,0,96,96,240,255,111,96,96,96,240,255,111,96,96,0,0,0,0,0,0,12,15,1,0,0,0,12,15,1,0,0,0,0,0,0,0};
u8 bitmap_36[] = {0,0,112,248,252,14,6,255,255,6,14,60,56,32,0,0,0,0,0,0,128,129,3,7,6,255,255,12,28,248,240,192,0,0,0,0,0,0,3,7,14,12,12,255,255,12,12,15,7,1,0,0,0,0};
u8 bitmap_37[] = {0,252,206,2,2,6,252,120,0,0,0,128,192,64,96,0,0,0,0,192,97,49,17,25,13,4,246,59,9,9,24,240,224,0,0,0,0,0,0,0,0,0,0,0,7,14,8,8,12,7,3,0,0,0};
u8 bitmap_38[] = {0,0,0,252,252,142,6,6,6,14,28,24,0,0,0,0,0,0,0,248,252,254,3,3,15,28,120,240,224,248,60,12,4,0,0,0,0,1,7,7,14,12,12,14,6,7,1,7,14,12,8,0,0,0};
u8 bitmap_39[] = {0,0,0,0,0,0,126,254,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_40[] = {0,0,0,0,0,240,252,62,7,3,1,1,0,0,0,0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7,15,28,56,112,96,96,0,0,0,0,0};
u8 bitmap_41[] = {0,0,0,0,1,1,3,15,254,248,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,96,112,48,56,30,15,3,0,0,0,0,0,0,0,0};
u8 bitmap_42[] = {0,0,0,0,0,0,0,240,240,0,0,0,0,0,0,0,0,0,0,3,3,131,198,246,124,15,61,118,230,199,131,3,2,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0};
u8 bitmap_43[] = {0,0,0,0,0,0,0,224,224,0,0,0,0,0,0,0,0,0,0,0,24,24,24,24,24,255,255,24,24,24,24,24,0,0,0,0,0,0,0,0,0,0,0,3,3,0,0,0,0,0,0,0,0,0};
u8 bitmap_44[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,255,63,15,1,0,0,0,0,0,0,0,0};
u8 bitmap_45[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_46[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,7,0,0,0,0,0,0,0,0};
u8 bitmap_47[] = {0,0,0,0,0,0,0,0,224,248,127,15,1,0,0,0,0,0,0,0,0,0,0,224,248,127,15,1,0,0,0,0,0,0,0,0,0,0,96,120,127,15,1,0,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_48[] = {0,0,240,248,60,14,6,134,134,14,12,252,248,192,0,0,0,0,0,0,255,255,128,0,31,63,63,0,0,255,255,127,0,0,0,0,0,0,1,3,7,14,12,12,12,14,6,7,3,0,0,0,0,0};
u8 bitmap_49[] = {0,0,0,56,24,28,14,254,254,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,12,12,12,12,12,15,15,15,12,12,12,12,0,0,0,0};
u8 bitmap_50[] = {0,0,56,60,28,14,6,6,6,14,28,252,248,0,0,0,0,0,0,0,0,0,128,192,224,112,48,28,15,7,3,0,0,0,0,0,0,0,14,15,15,13,12,12,12,12,12,12,12,12,0,0,0,0};
u8 bitmap_51[] = {0,0,6,6,6,6,134,198,102,54,62,30,14,0,0,0,0,0,0,0,128,128,0,3,3,3,3,7,14,254,252,96,0,0,0,0,0,0,1,3,7,14,12,12,12,14,7,7,3,0,0,0,0,0};
u8 bitmap_52[] = {0,0,0,0,0,128,192,240,120,30,14,2,0,0,0,0,0,0,0,0,240,252,254,199,195,192,192,192,192,254,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,0,0,0,0,0};
u8 bitmap_53[] = {0,0,254,254,254,6,6,6,6,6,6,6,6,0,0,0,0,0,0,0,7,7,7,6,3,3,3,3,7,254,252,240,0,0,0,0,0,0,1,7,7,14,12,12,12,14,14,7,3,0,0,0,0,0};
u8 bitmap_54[] = {0,0,0,0,128,224,240,60,30,6,2,0,0,0,0,0,0,0,0,0,248,254,15,5,2,3,3,3,7,254,252,240,0,0,0,0,0,0,1,3,7,14,12,12,12,14,6,7,3,0,0,0,0,0};
u8 bitmap_55[] = {0,0,62,62,6,6,6,6,6,6,230,254,126,14,0,0,0,0,0,0,0,0,0,0,128,224,252,63,15,1,0,0,0,0,0,0,0,0,0,0,8,14,15,7,1,0,0,0,0,0,0,0,0,0};
u8 bitmap_56[] = {0,0,112,248,252,14,6,6,6,14,12,252,248,0,0,0,0,0,0,0,240,249,29,15,6,6,6,7,15,253,248,224,0,0,0,0,0,0,1,7,7,14,12,12,12,14,6,7,3,0,0,0,0,0};
u8 bitmap_57[] = {0,0,240,248,28,14,6,6,6,6,12,252,248,224,0,0,0,0,0,0,3,7,15,14,12,140,204,244,126,31,7,1,0,0,0,0,0,0,0,0,0,8,14,15,3,1,0,0,0,0,0,0,0,0};
u8 bitmap_58[] = {0,0,0,0,0,0,224,224,224,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,6,0,0,0,0,0,0,0,0};
u8 bitmap_59[] = {0,0,0,0,0,0,224,224,224,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,255,63,7,1,0,0,0,0,0,0,0,0};
u8 bitmap_60[] = {0,0,0,0,0,128,128,192,192,224,96,112,48,0,0,0,0,0,0,0,14,15,27,27,49,49,96,96,192,192,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0};
u8 bitmap_61[] = {0,0,192,192,192,192,192,192,192,192,192,192,192,192,0,0,0,0,0,0,96,96,96,96,96,96,96,96,96,96,96,96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_62[] = {0,0,48,112,96,96,192,192,128,128,0,0,0,0,0,0,0,0,0,0,128,192,192,224,96,112,49,57,27,27,14,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_63[] = {0,0,0,0,6,6,6,6,6,12,252,248,224,0,0,0,0,0,0,0,0,0,0,124,124,124,12,14,7,3,0,0,0,0,0,0,0,0,0,0,0,14,14,14,4,0,0,0,0,0,0,0,0,0};
u8 bitmap_64[] = {0,192,240,120,28,14,6,6,134,134,142,12,252,248,192,0,0,0,0,255,255,0,0,0,252,255,7,1,1,3,253,255,255,0,0,0,0,15,63,120,224,192,193,131,135,134,6,2,1,7,7,0,0,0};
u8 bitmap_65[] = {0,0,0,0,128,240,254,14,62,254,224,0,0,0,0,0,0,0,0,0,128,248,255,127,97,96,96,99,127,254,224,0,0,0,0,0,0,8,15,15,1,0,0,0,0,0,0,3,15,15,0,0,0,0};
u8 bitmap_66[] = {0,0,254,254,254,6,6,6,6,14,14,252,248,32,0,0,0,0,0,0,255,255,255,6,6,6,6,6,15,253,249,240,0,0,0,0,0,0,15,15,15,12,12,12,12,12,14,7,3,1,0,0,0,0};
u8 bitmap_67[] = {0,0,224,248,252,14,6,6,6,6,14,60,56,32,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,128,128,128,0,0,0,0,0,0,0,3,7,14,12,12,12,12,14,7,3,0,0,0,0,0};
u8 bitmap_68[] = {0,0,254,254,254,6,6,6,6,14,28,252,248,192,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,255,255,127,0,0,0,0,0,0,15,15,15,12,12,12,12,14,7,7,3,0,0,0,0,0};
u8 bitmap_69[] = {0,0,254,254,254,6,6,6,6,6,6,6,6,6,0,0,0,0,0,0,255,255,255,6,6,6,6,6,6,6,6,0,0,0,0,0,0,0,15,15,15,12,12,12,12,12,12,12,12,12,0,0,0,0};
u8 bitmap_70[] = {0,0,254,254,254,6,6,6,6,6,6,6,6,6,0,0,0,0,0,0,255,255,255,6,6,6,6,6,6,6,6,0,0,0,0,0,0,0,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_71[] = {0,0,240,248,60,14,6,6,6,14,14,60,56,32,0,0,0,0,0,0,255,255,128,0,0,12,12,12,12,252,252,124,0,0,0,0,0,0,1,3,7,14,12,12,12,14,6,7,3,0,0,0,0,0};
u8 bitmap_72[] = {0,0,254,254,254,0,0,0,0,0,0,254,254,0,0,0,0,0,0,0,255,255,255,6,6,6,6,6,6,255,255,0,0,0,0,0,0,0,15,15,15,0,0,0,0,0,0,15,15,0,0,0,0,0};
u8 bitmap_73[] = {0,0,6,6,6,6,6,254,254,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,0,12,12,12,12,12,15,15,12,12,12,12,0,0,0,0,0};
u8 bitmap_74[] = {0,0,0,6,6,6,6,6,6,6,6,254,254,0,0,0,0,0,0,128,128,128,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,3,7,14,12,12,12,12,14,7,3,1,0,0,0,0,0};
u8 bitmap_75[] = {0,0,254,254,254,0,0,0,128,224,240,124,30,6,2,0,0,0,0,0,255,255,255,6,6,6,31,127,248,224,128,0,0,0,0,0,0,0,15,15,15,0,0,0,0,0,1,7,15,14,8,0,0,0};
u8 bitmap_76[] = {0,0,0,254,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,12,12,12,12,12,12,12,12,12,0,0,0,0};
u8 bitmap_77[] = {0,0,254,254,14,248,192,0,0,224,124,230,254,254,0,0,0,0,0,0,255,255,0,0,7,14,15,3,0,255,255,255,0,0,0,0,0,0,15,15,0,0,0,0,0,0,0,15,15,15,0,0,0,0};
u8 bitmap_78[] = {0,0,254,254,14,124,240,128,0,0,0,254,254,0,0,0,0,0,0,0,255,255,0,0,3,15,126,240,128,255,255,0,0,0,0,0,0,0,15,15,0,0,0,0,0,3,15,15,15,0,0,0,0,0};
u8 bitmap_79[] = {0,0,240,248,60,14,6,6,6,14,12,252,248,192,0,0,0,0,0,0,255,255,128,0,0,0,0,0,0,255,255,127,0,0,0,0,0,0,1,3,7,14,12,12,12,14,6,7,3,0,0,0,0,0};
u8 bitmap_80[] = {0,0,254,254,254,6,6,6,6,6,14,28,252,248,0,0,0,0,0,0,255,255,255,12,12,12,12,12,14,7,7,1,0,0,0,0,0,0,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_81[] = {0,0,240,248,60,14,6,6,6,14,12,252,248,192,0,0,0,0,0,0,255,255,128,0,0,0,0,0,0,255,255,127,0,0,0,0,0,0,1,3,7,14,12,12,12,62,254,231,131,0,0,0,0,0};
u8 bitmap_82[] = {0,0,254,254,254,6,6,6,6,6,14,188,248,240,0,0,0,0,0,0,255,255,255,12,12,12,28,252,254,199,3,1,0,0,0,0,0,0,15,15,15,0,0,0,0,0,3,15,15,12,0,0,0,0};
u8 bitmap_83[] = {0,0,96,248,252,14,6,6,6,6,14,60,56,32,0,0,0,0,0,0,128,129,3,7,7,6,14,12,28,248,248,224,0,0,0,0,0,0,1,7,7,14,12,12,12,12,14,7,3,1,0,0,0,0};
u8 bitmap_84[] = {0,6,6,6,6,6,6,254,254,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,0,0,0,0,0,0,0,0,0};
u8 bitmap_85[] = {0,0,254,254,254,0,0,0,0,0,0,254,254,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,1,3,7,14,12,12,12,14,6,7,3,0,0,0,0,0};
u8 bitmap_86[] = {0,2,62,254,224,0,0,0,0,0,128,248,254,30,0,0,0,0,0,0,0,3,63,254,224,0,128,248,255,15,0,0,0,0,0,0,0,0,0,0,0,3,15,14,15,15,0,0,0,0,0,0,0,0};
u8 bitmap_87[] = {0,254,254,0,0,0,252,62,254,224,0,0,240,254,62,0,0,0,0,3,255,255,0,252,63,0,1,255,224,0,255,255,0,0,0,0,0,0,15,15,8,15,0,0,0,3,15,15,15,0,0,0,0,0};
u8 bitmap_88[] = {0,2,6,30,124,240,192,0,128,224,248,62,14,2,0,0,0,0,0,0,0,0,192,241,127,31,63,249,224,192,0,0,0,0,0,0,0,8,14,15,3,1,0,0,0,0,1,7,15,12,8,0,0,0};
u8 bitmap_89[] = {0,6,30,124,240,192,0,0,0,128,224,248,62,14,2,0,0,0,0,0,0,0,1,7,31,252,252,15,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,0,0,0,0,0,0,0,0,0};
u8 bitmap_90[] = {0,0,6,6,6,6,6,6,134,230,246,62,14,0,0,0,0,0,0,0,0,0,192,240,124,30,7,1,0,0,0,0,0,0,0,0,0,0,14,15,15,12,12,12,12,12,12,12,12,0,0,0,0,0};
u8 bitmap_91[] = {0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,63,63,48,48,48,48,0,0,0,0,0,0};
u8 bitmap_92[] = {0,0,0,3,31,254,240,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,31,254,240,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,31,126,112,64,0,0,0,0};
u8 bitmap_93[] = {0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,0,0,48,48,48,48,63,63,0,0,0,0,0,0,0,0};
u8 bitmap_94[] = {0,0,0,0,224,248,62,7,15,60,240,192,0,0,0,0,0,0,0,0,4,7,3,0,0,0,0,0,1,7,6,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_95[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,96,96,96,96,96,96,96,96,96,96,96,0,0,0,0};
u8 bitmap_96[] = {0,0,0,0,0,0,1,7,6,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_97[] = {0,0,0,192,192,224,96,96,96,224,192,192,128,0,0,0,0,0,0,0,225,241,57,24,24,24,24,24,24,255,255,0,0,0,0,0,0,0,3,7,14,12,12,12,12,6,3,15,15,0,0,0,0,0};
u8 bitmap_98[] = {0,0,254,254,254,192,64,96,96,224,224,192,128,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,255,255,124,0,0,0,0,0,0,15,15,15,6,12,12,12,14,14,7,3,0,0,0,0,0};
u8 bitmap_99[] = {0,0,0,128,192,224,96,96,96,96,224,192,128,0,0,0,0,0,0,0,254,255,255,0,0,0,0,0,0,131,131,130,0,0,0,0,0,0,0,3,7,14,12,12,12,12,14,7,3,0,0,0,0,0};
u8 bitmap_100[] = {0,0,0,192,192,224,96,96,96,192,128,254,254,0,0,0,0,0,0,0,255,255,131,0,0,0,0,0,1,255,255,0,0,0,0,0,0,0,1,7,7,14,12,12,12,6,3,15,15,0,0,0,0,0};
u8 bitmap_101[] = {0,0,0,128,192,224,96,96,96,96,224,192,128,0,0,0,0,0,0,0,255,255,155,24,24,24,24,24,24,31,31,28,0,0,0,0,0,0,1,3,7,14,12,12,12,12,14,7,3,0,0,0,0,0};
u8 bitmap_102[] = {0,0,128,128,128,128,252,254,142,134,134,134,134,134,0,0,0,0,0,0,1,1,1,1,255,255,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,15,15,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_103[] = {0,0,0,192,192,224,96,96,96,192,128,224,224,0,0,0,0,0,0,0,255,255,129,0,0,0,0,0,129,255,255,0,0,0,0,0,0,0,0,3,135,135,134,134,134,194,225,255,127,0,0,0,0,0};
u8 bitmap_104[] = {0,0,254,254,254,192,64,96,96,224,224,192,128,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,1,255,255,0,0,0,0,0,0,0,15,15,15,0,0,0,0,0,0,15,15,0,0,0,0,0};
u8 bitmap_105[] = {0,0,0,96,96,96,98,103,231,231,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,12,12,12,12,12,12,15,15,12,12,12,12,0,0,0,0};
u8 bitmap_106[] = {0,0,0,96,96,96,96,96,103,231,231,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0,128,128,128,128,128,192,224,127,63,0,0,0,0,0,0,0};
u8 bitmap_107[] = {0,0,254,254,254,0,0,0,0,0,128,192,224,96,32,0,0,0,0,0,255,255,255,48,48,48,124,254,199,3,0,0,0,0,0,0,0,0,15,15,15,0,0,0,0,1,3,7,14,12,8,0,0,0};
u8 bitmap_108[] = {0,6,6,6,6,6,254,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,7,14,12,12,12,12,12,0,0,0,0};
u8 bitmap_109[] = {0,0,224,224,64,96,224,192,0,192,96,224,224,192,0,0,0,0,0,0,255,255,0,0,0,255,255,0,0,0,255,255,0,0,0,0,0,0,15,15,0,0,0,15,15,0,0,0,15,15,0,0,0,0};
u8 bitmap_110[] = {0,0,224,224,224,192,64,96,96,224,224,192,128,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,1,255,255,0,0,0,0,0,0,0,15,15,15,0,0,0,0,0,0,15,15,0,0,0,0,0};
u8 bitmap_111[] = {0,0,0,128,192,224,96,96,96,224,192,192,128,0,0,0,0,0,0,0,255,255,131,0,0,0,0,0,0,255,255,124,0,0,0,0,0,0,1,3,7,14,12,12,12,14,6,7,3,0,0,0,0,0};
u8 bitmap_112[] = {0,0,224,224,224,192,64,96,96,224,224,192,128,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,255,255,124,0,0,0,0,0,0,255,255,255,6,12,12,12,14,14,7,3,0,0,0,0,0};
u8 bitmap_113[] = {0,0,0,128,192,224,96,96,96,192,128,224,224,224,0,0,0,0,0,0,254,255,255,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,3,7,14,12,12,12,6,2,255,255,255,0,0,0,0};
u8 bitmap_114[] = {0,0,0,224,224,128,192,96,96,96,224,224,192,128,0,0,0,0,0,0,0,255,255,1,0,0,0,0,0,1,7,7,0,0,0,0,0,0,0,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_115[] = {0,0,0,192,192,224,96,96,96,96,224,192,128,0,0,0,0,0,0,0,3,15,31,24,24,24,56,48,48,241,225,0,0,0,0,0,0,0,3,7,7,14,12,12,12,12,14,7,3,0,0,0,0,0};
u8 bitmap_116[] = {0,0,96,96,96,96,254,254,96,96,96,96,96,96,0,0,0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,14,12,12,12,12,0,0,0,0,0};
u8 bitmap_117[] = {0,0,224,224,224,0,0,0,0,0,0,224,224,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,3,7,14,12,12,12,14,7,7,3,0,0,0,0,0};
u8 bitmap_118[] = {0,32,224,224,128,0,0,0,0,0,0,192,224,96,0,0,0,0,0,0,0,7,63,252,224,0,128,240,254,31,3,0,0,0,0,0,0,0,0,0,0,1,15,15,15,7,0,0,0,0,0,0,0,0};
u8 bitmap_119[] = {0,224,224,0,0,0,192,224,224,0,0,0,128,224,96,0,0,0,0,3,255,252,0,224,255,1,15,254,0,128,255,127,0,0,0,0,0,0,3,15,8,15,0,0,0,7,15,15,15,0,0,0,0,0};
u8 bitmap_120[] = {0,0,32,224,224,128,0,0,0,0,192,224,96,32,0,0,0,0,0,0,0,0,129,199,255,124,126,239,195,1,0,0,0,0,0,0,0,8,12,14,7,3,0,0,0,1,3,15,14,8,0,0,0,0};
u8 bitmap_121[] = {0,32,224,224,128,0,0,0,0,0,0,192,224,96,0,0,0,0,0,0,0,7,31,254,240,128,128,240,254,31,3,0,0,0,0,0,0,0,0,0,0,128,243,255,63,7,0,0,0,0,0,0,0,0};
u8 bitmap_122[] = {0,0,96,96,96,96,96,96,96,96,96,224,224,0,0,0,0,0,0,0,0,0,192,224,112,56,28,14,7,3,1,0,0,0,0,0,0,0,14,15,15,13,12,12,12,12,12,12,12,12,0,0,0,0};
u8 bitmap_123[] = {0,0,0,0,0,0,0,135,255,255,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,7,253,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,31,31,56,48,48,0,0,0,0,0};
u8 bitmap_124[] = {0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,127,127,0,0,0,0,0,0,0,0,0};
u8 bitmap_125[] = {0,0,0,0,0,0,255,255,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,252,255,7,3,3,3,3,0,0,0,0,0,0,0,48,48,48,56,31,15,0,0,0,0,0,0,0,0,0,0};
u8 bitmap_126[] = {0,0,0,128,128,128,128,0,0,0,0,0,128,128,0,0,0,0,0,0,15,15,1,1,3,7,14,12,12,14,15,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 *char_addr[] = {bitmap_32,bitmap_33,bitmap_34,bitmap_35,bitmap_36,bitmap_37,bitmap_38,bitmap_39,bitmap_40,bitmap_41,bitmap_42,bitmap_43,bitmap_44,bitmap_45,bitmap_46,bitmap_47,bitmap_48,bitmap_49,bitmap_50,bitmap_51,bitmap_52,bitmap_53,bitmap_54,bitmap_55,bitmap_56,bitmap_57,bitmap_58,bitmap_59,bitmap_60,bitmap_61,bitmap_62,bitmap_63,bitmap_64,bitmap_65,bitmap_66,bitmap_67,bitmap_68,bitmap_69,bitmap_70,bitmap_71,bitmap_72,bitmap_73,bitmap_74,bitmap_75,bitmap_76,bitmap_77,bitmap_78,bitmap_79,bitmap_80,bitmap_81,bitmap_82,bitmap_83,bitmap_84,bitmap_85,bitmap_86,bitmap_87,bitmap_88,bitmap_89,bitmap_90,bitmap_91,bitmap_92,bitmap_93,bitmap_94,bitmap_95,bitmap_96,bitmap_97,bitmap_98,bitmap_99,bitmap_100,bitmap_101,bitmap_102,bitmap_103,bitmap_104,bitmap_105,bitmap_106,bitmap_107,bitmap_108,bitmap_109,bitmap_110,bitmap_111,bitmap_112,bitmap_113,bitmap_114,bitmap_115,bitmap_116,bitmap_117,bitmap_118,bitmap_119,bitmap_120,bitmap_121,bitmap_122,bitmap_123,bitmap_124,bitmap_125,bitmap_126};



void _drawText(char *str, i32 x, i32 y, const Canvas &canvas, const Color &color, f32 opacity, const RectI *viewport_bounds) {
    RectI bounds{
            0, canvas.dimensions.width - 1,
            0, canvas.dimensions.height - 1
    };
    if (viewport_bounds) {
        x += viewport_bounds->left;
        y += viewport_bounds->top;
        bounds -= *viewport_bounds;
    }

    if (x + FONT_WIDTH < bounds.left || x - FONT_WIDTH > bounds.right ||
        y + FONT_HEIGHT < bounds.top || y - FONT_HEIGHT > bounds.bottom)
        return;

    f32 pixel_opacity;
    u16 current_x = (u16)x;
    u16 current_y = (u16)y;
    u16 pixel_x, sub_pixel_x;
    u16 pixel_y, sub_pixel_y;
    u16 t_offset;
    u8 *byte_ptr, byte, next_column_byte;
    char character = *str;
    while (character) {
        if (character == '\n') {
            if (current_y > bounds.bottom)
                break;

            current_x = (u16)x;
            current_y += LINE_HEIGHT;
        } else if (character == '\t') {
            t_offset = FONT_WIDTH * (4 - ((current_x / FONT_WIDTH) & 3));
            current_x += t_offset;
        } else if ((character >= FIRST_CHARACTER_CODE) &&
                   (character <= LAST_CHARACTER_CODE)) {
            byte_ptr = char_addr[character - FIRST_CHARACTER_CODE];
            byte = *byte_ptr;
            next_column_byte = *(byte_ptr + 1);
            for (int i = 1; i < 4; i++) {
                pixel_x = current_x;
                pixel_y = current_y + i * FONT_HEIGHT / 3;
                for (int w = 0; w < INTERNAL_FONT_WIDTH ; w += 2) {
                    for (int h = 0; h < 8; h += 2) {
                        /* skip background bits */
                        if (bounds.contains(pixel_x, pixel_y)) {
                            if (canvas.antialias == SSAA) {
                                sub_pixel_x = pixel_x << 1;
                                sub_pixel_y = pixel_y << 1;

                                if (byte & (0x80 >> h)) canvas.setPixel(sub_pixel_x, sub_pixel_y + 1, color, opacity);
                                if (byte & (0x80 >> (h+1))) canvas.setPixel(sub_pixel_x, sub_pixel_y, color, opacity);
                                if (next_column_byte & (0x80 >> h)) canvas.setPixel(sub_pixel_x+1, sub_pixel_y + 1, color, opacity);
                                if (next_column_byte & (0x80 >> (h+1))) canvas.setPixel(sub_pixel_x+1, sub_pixel_y, color, opacity);
                            } else {
                                pixel_opacity = (byte & (0x80 >> h)) ? 0.25f : 0;
                                if (byte & (0x80 >> (h+1))) pixel_opacity += 0.25f;
                                if (next_column_byte & (0x80 >> h)) pixel_opacity += 0.25f;
                                if (next_column_byte & (0x80 >> (h+1))) pixel_opacity += 0.25f;
                                if (pixel_opacity != 0.0f) canvas.setPixel(pixel_x, pixel_y, color, pixel_opacity);
                            }
                        }

                        pixel_y--;
                    }
                    byte_ptr += 2;
                    byte = *byte_ptr;
                    next_column_byte = *(byte_ptr + 1);

                    pixel_y += FONT_HEIGHT / 3;
                    pixel_x++;
                }
            }

            current_x += FONT_WIDTH;
            if (current_x > bounds.right) {
                if (current_y > bounds.bottom)
                    break;

                while (character && (character != '\n')) character = *++str;
                if (!character)
                    break;

                current_x = (u16)x;
                current_y += LINE_HEIGHT;
            }
        }
        character = *++str;
    }
}

#ifdef SLIM_ENABLE_CANVAS_TEXT_DRAWING
INLINE void Canvas::drawText(char *str, i32 x, i32 y, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawText(str, x, y, *this, color, opacity, viewport_bounds);
}
#ifdef SLIM_VEC2
INLINE void Canvas::drawText(char *str, vec2i position, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawText(str, position.x, position.y, *this, color, opacity, viewport_bounds);
}
INLINE void Canvas::drawText(char *str, vec2 position, const Color &color, f32 opacity, const RectI *viewport_bounds) const  {
    _drawText(str, (i32)position.x, (i32)position.y, *this, color, opacity, viewport_bounds);
}
#endif
#endif


INLINE void drawText(char *str, i32 x, i32 y, const Canvas &canvas, Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawText(str, x, y, canvas, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
INLINE void drawText(char *str, vec2i position, const Canvas &canvas, Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawText(str, position.x, position.y, canvas, color, opacity, viewport_bounds);
}
INLINE void drawText(char *str, vec2 position, const Canvas &canvas, Color color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawText(str, (i32)position.x, (i32)position.y, canvas, color, opacity, viewport_bounds);
}
#endif

void _drawNumber(i32 number, i32 x, i32 y, const Canvas &canvas, const Color &color, f32 opacity, const RectI *viewport_bounds) {
    static NumberString number_string;
    number_string = number;
    _drawText(number_string.string.char_ptr, x - (i32)number_string.string.length * FONT_WIDTH, y, canvas, color, opacity, viewport_bounds);
}


#ifdef SLIM_ENABLE_CANVAS_NUMBER_DRAWING
INLINE void Canvas::drawNumber(i32 number, i32 x, i32 y, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawNumber(number, x, y, *this, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
INLINE void Canvas::drawNumber(i32 number, vec2i position, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawNumber(number, position.x, position.y, *this, color, opacity, viewport_bounds);
}
INLINE void Canvas::drawNumber(i32 number, vec2 position, const Color &color, f32 opacity, const RectI *viewport_bounds) const {
    _drawNumber(number, (i32)position.x, (i32)position.y, *this, color, opacity, viewport_bounds);
}
#endif
#endif


INLINE void drawNumber(i32 number, i32 x, i32 y, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawNumber(number, x, y, canvas, color, opacity, viewport_bounds);
}

#ifdef SLIM_VEC2
INLINE void drawNumber(i32 number, vec2i position, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawNumber(number, position.x, position.y, canvas, color, opacity, viewport_bounds);
}
INLINE void drawNumber(i32 number, vec2 position, const Canvas &canvas, const Color &color = White, f32 opacity = 1.0f, const RectI *viewport_bounds = nullptr) {
    _drawNumber(number, (i32)position.x, (i32)position.y, canvas, color, opacity, viewport_bounds);
}
#endif


void _drawHUD(const HUD &hud, const Canvas &canvas, const RectI *viewport_bounds) {
    i32 x = hud.left;
    i32 y = hud.top;

    HUDLine *line = hud.lines;
    bool alt;
    for (u32 i = 0; i < hud.settings.line_count; i++, line++) {
        if (line->use_alternate) {
            alt = *line->use_alternate;
            if (line->invert_alternate_use)
                alt = !alt;
        } else
            alt = false;

        ColorID color = alt ? line->alternate_value_color : line->value_color;
        char *text = alt ? line->alternate_value.char_ptr : line->value.string.char_ptr;
        _drawText(line->title.char_ptr, x, y, canvas, line->title_color, 1.0f, viewport_bounds);
        _drawText(text, x + (i32)line->title.length * FONT_WIDTH, y, canvas, color, 1.0f, viewport_bounds);
        y += (i32)(hud.settings.line_height * (f32)FONT_HEIGHT);
    }
}

#ifdef SLIM_ENABLE_CANVAS_HUD_DRAWING
INLINE void Canvas::drawHUD(const HUD &hud, const RectI *viewport_bounds) const {
    _drawHUD(hud, *this, viewport_bounds);
}
#endif

INLINE void drawHUD(const HUD &hud, const Canvas &canvas, const RectI *viewport_bounds = nullptr) {
    _drawHUD(hud, canvas, viewport_bounds);
}



struct Viewport {
    Canvas &canvas;
    Camera *camera{nullptr};
    Frustum frustum;
    Dimensions dimensions;
    Navigation navigation;
    RectI bounds{};

    Viewport(Canvas &canvas, Camera *camera) : canvas{canvas} {
        dimensions = canvas.dimensions;
        bounds.left = bounds.top = 0;
        bounds.right = dimensions.width - 1;
        bounds.bottom = dimensions.height - 1;
        setCamera(*camera);
    }

    void setCamera(Camera &cam) {
        camera = &cam;
        updateProjection();
    }

    void updateProjection() {
        frustum.updateProjection(camera->focal_length, dimensions.height_over_width);
    }

    void updateDimensions(u16 width, u16 height) {
        i32 dx = width - dimensions.width;
        i32 dy = height - dimensions.height;
        dimensions.update(width, height);
        dimensions.stride += (u16)bounds.left;
        bounds.right += dx;
        bounds.bottom += dy;
        updateProjection();
    }

    void updateNavigation(f32 delta_time) {
        navigation.update(*camera, delta_time);
        updateProjection();
    }

    INLINE void projectEdge(Edge &edge) const {
        frustum.projectEdge(edge, dimensions);
    }

    INLINE bool cullAndClipEdge(Edge &edge) const {
        return frustum.cullAndClipEdge(edge, camera->focal_length, dimensions.width_over_height);
    }

#ifdef SLIM_ENABLE_VIEWPORT_BOX_DRAWING
    INLINE void drawBox(const Box &box, const Transform &transform, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, u8 sides = BOX__ALL_SIDES) const;
#endif

#ifdef SLIM_ENABLE_VIEWPORT_CAMERA_DRAWING
    INLINE void drawCamera(const Camera &camera, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) const;
#endif

#ifdef SLIM_ENABLE_VIEWPORT_CURVE_DRAWING
    INLINE void drawCurve(const Curve &curve, const Transform &transform, const Color &color = White,
                          f32 opacity = 1.0f, u8 line_width = 0, u32 step_count = CURVE_STEPS) const;
#endif

#ifdef SLIM_ENABLE_VIEWPORT_EDGE_DRAWING
    INLINE void drawEdge(Edge edge, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) const;
#endif

#ifdef SLIM_ENABLE_VIEWPORT_GRID_DRAWING
    INLINE void drawGrid(const Grid &grid, const Transform &transform, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) const;
#endif

#ifdef SLIM_ENABLE_VIEWPORT_MESH_DRAWING
    INLINE void drawMesh(const Mesh &mesh, const Transform &transform, bool draw_normals, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) const;
#endif
};



struct Selection {
    Transform xform;
    quat object_rotation;
    vec3 transformation_plane_origin,
            transformation_plane_normal,
            transformation_plane_center,
            object_scale,
            world_offset,
            *world_position{nullptr};
    Geometry *geometry{nullptr};
    f32 object_distance = 0;
    u32 geo_id = 0;
    GeometryType geo_type = GeometryType_None;
    BoxSide box_side = NoSide;
    bool changed = false;
    bool left_mouse_button_was_pressed = false;

    void manipulate(const Viewport &viewport, const Scene &scene) {
        static Ray ray, local_ray;

        const Dimensions &dimensions = viewport.dimensions;
        Camera &camera = *viewport.camera;
        f32 x = (f32)(mouse::pos_x - viewport.bounds.left);
        f32 y = (f32)(mouse::pos_y - viewport.bounds.top);

        ray.origin = camera.position;
        ray.direction = camera.getRayDirectionAt(x, y, dimensions.f_width, dimensions.f_height);
        ray.hit.distance_squared = INFINITY;

        if (mouse::left_button.is_pressed && !left_mouse_button_was_pressed) {
            // This is the first frame after the left mouse button went down:
            // Cast a ray onto the scene to find the closest object behind the hovered pixel:
            if (scene.castRay(ray)) {
                // Detect if object scene->selection has changed:
                changed = (
                        geo_type != ray.hit.geo_type ||
                        geo_id != ray.hit.geo_id
                );

                // Track the object that is now selected:
                geo_type = ray.hit.geo_type;
                geo_id   = ray.hit.geo_id;
                geometry = nullptr;

                // Capture a pointer to the selected object's position for later use in transformations:
                geometry = scene.geometries + geo_id;
                world_position = &geometry->transform.position;
                transformation_plane_origin = ray.hit.position;

                world_offset = ray.hit.position - *world_position;

                // Track how far away the hit position is from the camera along the depth axis:
                object_distance = (camera.rotation.transposed() * (ray.hit.position - ray.origin)).z;
            } else {
                if (geo_type) changed = true;
                geo_type = GeometryType_None;
            }
        }
        left_mouse_button_was_pressed = mouse::left_button.is_pressed;
        if (geo_type) {
            if (controls::is_pressed::alt) {
                bool any_mouse_button_is_pressed = (
                        mouse::left_button.is_pressed ||
                        mouse::middle_button.is_pressed ||
                        mouse::right_button.is_pressed);
                if (geometry && !any_mouse_button_is_pressed) {
                    // Cast a ray onto the bounding box of the currently selected object:
                    xform = geometry->transform;
                    if (geometry->type == GeometryType_Mesh)
                        xform.scale *= scene.meshes[geometry->id].aabb.max;

                    xform.internPosAndDir(ray.origin, ray.direction, local_ray.origin, local_ray.direction);

                    box_side = local_ray.hitsCube();
                    if (box_side) {
                        transformation_plane_center = xform.externPos(local_ray.hit.normal);
                        transformation_plane_origin = xform.externPos(local_ray.hit.position);
                        transformation_plane_normal = xform.externDir(local_ray.hit.normal);
                        transformation_plane_normal = transformation_plane_normal.normalized();
                        world_offset = transformation_plane_origin - *world_position;
                        object_scale    = geometry->transform.scale;
                        object_rotation = geometry->transform.rotation;
                    }
                }

                if (box_side) {
                    if (geometry) {
                        if (any_mouse_button_is_pressed) {
                            if (ray.hitsPlane(transformation_plane_origin, transformation_plane_normal)) {
                                xform = geometry->transform;
                                if (geometry->type == GeometryType_Mesh)
                                    xform.scale *= scene.meshes[geometry->id].aabb.max;

                                if (mouse::left_button.is_pressed) {
                                    *world_position = ray.hit.position - world_offset;
                                } else if (mouse::middle_button.is_pressed) {
                                    ray.hit.position = xform.internPos(ray.hit.position);
                                    ray.hit.position /= xform.internPos(transformation_plane_origin);
                                    geometry->transform.scale = object_scale * ray.hit.position;
                                } else if (mouse::right_button.is_pressed) {
                                    vec3 v1{ ray.hit.position - transformation_plane_center };
                                    vec3 v2{ transformation_plane_origin - transformation_plane_center };
                                    quat rotation = quat{v2.cross(v1), (v1.dot(v2)) + sqrtf(v1.squaredLength() * v2.squaredLength())};
                                    geometry->transform.rotation = (rotation.normalized() * object_rotation).normalized();
                                }
                            }
                        }
                    }
                }
            } else {
                box_side = NoSide;
                if (mouse::left_button.is_pressed && mouse::moved) {
                    // Back-project the new mouse position onto a quad at a distance of the selected-object away from the camera

                    // Screen -> NDC:
                    x = (x + 0.5f) / dimensions.h_width  - 1;
                    y = (y + 0.5f) / dimensions.h_height - 1;

                    // NDC -> View:
                    x *= object_distance / (camera.focal_length * dimensions.height_over_width);
                    y *= object_distance / camera.focal_length;

                    // View -> World (Back-track by the world offset from the hit position back to the selected-object's center):
                    *world_position = camera.rotation * vec3{x, -y, object_distance} + camera.position - world_offset;
                }
            }
        }
    }
};

void _drawEdge(Edge edge, const Viewport &viewport, const Color &color, f32 opacity, u8 line_width) {
    if (!viewport.cullAndClipEdge(edge)) return;

    viewport.projectEdge(edge);
    drawLine(edge.from.x,
             edge.from.y,
             edge.from.z,
             edge.to.x,
             edge.to.y,
             edge.to.z,
             viewport.canvas, color, opacity, line_width, &viewport.bounds);
}

#ifdef SLIM_ENABLE_VIEWPORT_EDGE_DRAWING
INLINE void Viewport::drawEdge(Edge edge, const Color &color, f32 opacity, u8 line_width) const {
    _drawEdge(edge, *this, color, opacity, line_width);
}
#endif

INLINE void drawEdge(Edge edge, const Viewport &viewport, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) {
    _drawEdge(edge, viewport, color, opacity, line_width);
}


void _drawBox(const Box &box, const Transform &transform, const Viewport &viewport, const Color &color, f32 opacity, u8 line_width, u8 sides) {
    static Box view_space_box;

    // Transform vertices positions from local-space to world-space and then to view-space:
    for (u8 i = 0; i < BOX__VERTEX_COUNT; i++)
        view_space_box.vertices.buffer[i] = viewport.camera->internPos(transform.externPos(box.vertices.buffer[i]));

    // Distribute transformed vertices positions to edges:
    view_space_box.edges.setFrom(view_space_box.vertices);

    if (sides == BOX__ALL_SIDES) for (const auto &edge : view_space_box.edges.buffer)
            drawEdge(edge, viewport, color, opacity, line_width);
    else {
        BoxEdgeSides &box_edges = view_space_box.edges.sides;
        if (sides & Front | sides & Top   ) drawEdge(box_edges.front_top, viewport, color, opacity, line_width);
        if (sides & Front | sides & Bottom) drawEdge(box_edges.front_bottom, viewport, color, opacity, line_width);
        if (sides & Front | sides & Left  ) drawEdge(box_edges.front_left, viewport, color, opacity, line_width);
        if (sides & Front | sides & Right ) drawEdge(box_edges.front_right, viewport, color, opacity, line_width);
        if (sides & Back  | sides & Top   ) drawEdge(box_edges.back_top, viewport, color, opacity, line_width);
        if (sides & Back  | sides & Bottom) drawEdge(box_edges.back_bottom, viewport, color, opacity, line_width);
        if (sides & Back  | sides & Left  ) drawEdge(box_edges.back_left, viewport, color, opacity, line_width);
        if (sides & Back  | sides & Right ) drawEdge(box_edges.back_right, viewport, color, opacity, line_width);
        if (sides & Left  | sides & Top   ) drawEdge(box_edges.left_top, viewport, color, opacity, line_width);
        if (sides & Left  | sides & Bottom) drawEdge(box_edges.left_bottom, viewport, color, opacity, line_width);
        if (sides & Right | sides & Top   ) drawEdge(box_edges.right_top, viewport, color, opacity, line_width);
        if (sides & Right | sides & Bottom) drawEdge(box_edges.right_bottom, viewport, color, opacity, line_width);
    }
}

#ifdef SLIM_ENABLE_VIEWPORT_BOX_DRAWING
INLINE void Viewport::drawBox(const Box &box, const Transform &transform, const Color &color, f32 opacity, u8 line_width, u8 sides) const {
    _drawBox(box, transform, *this, color, opacity, line_width, sides);
}
#endif

INLINE void drawBox(const Box &box, const Transform &transform, const Viewport &viewport,
                    const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, u8 sides = BOX__ALL_SIDES) {
    _drawBox(box, transform, viewport, color, opacity, line_width, sides);
}


void _drawCamera(const Camera &camera, const Viewport &viewport, const Color &color, f32 opacity, u8 line_width) {
    static Transform transform;
    static Box box;

    transform.rotation = Quat(camera.rotation);
    transform.position = camera.position;
    transform.scale = 1.0f;

    box = Box{};
    drawBox(box, transform, viewport, color, opacity, line_width, BOX__ALL_SIDES);

    box.vertices.corners.back_bottom_left   *= 0.5f;
    box.vertices.corners.back_bottom_right  *= 0.5f;
    box.vertices.corners.back_top_left      *= 0.5f;
    box.vertices.corners.back_top_right     *= 0.5f;
    box.vertices.corners.front_bottom_left  *= 2.0f;
    box.vertices.corners.front_bottom_right *= 2.0f;
    box.vertices.corners.front_top_left     *= 2.0f;
    box.vertices.corners.front_top_right    *= 2.0f;

    for (auto &vertex : box.vertices.buffer)
        vertex.z += 1.5f;

    drawBox(box, transform, viewport, color, opacity, line_width, BOX__ALL_SIDES);
}

#ifdef SLIM_ENABLE_VIEWPORT_CAMERA_DRAWING
INLINE void Viewport::drawCamera(const Camera &camera, const Color &color, f32 opacity, u8 line_width) const {
    _drawCamera(camera, *this, color, opacity, line_width);
}
#endif

INLINE void drawCamera(const Camera &camera, const Viewport &viewport, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) {
    _drawCamera(camera, viewport, color, opacity, line_width);
}


void _drawCurve(const Curve &curve, const Transform &transform, const Viewport &viewport,
                const Color &color, f32 opacity, u8 line_width, u32 step_count) {
    const Camera &cam = *viewport.camera;

    f32 one_over_step_count = 1.0f / (f32)step_count;
    f32 rotation_step = one_over_step_count * TAU;
    f32 rotation_step_times_rev_count = rotation_step * (f32)curve.revolution_count;

    if (curve.type == CurveType::Helix)
        rotation_step = rotation_step_times_rev_count;

    vec3 center_to_orbit;
    center_to_orbit.x = 1;
    center_to_orbit.y = center_to_orbit.z = 0;

    vec3 orbit_to_curve;
    orbit_to_curve.x = curve.thickness;
    orbit_to_curve.y = orbit_to_curve.z = 0;

    mat3 rotation;
    rotation.X.x = rotation.Z.z = cosf(rotation_step);
    rotation.X.z = sinf(rotation_step);
    rotation.Z.x = -rotation.X.z;
    rotation.X.y = rotation.Z.y = rotation.Y.x = rotation.Y.z =  0;
    rotation.Y.y = 1;

    mat3 orbit_to_curve_rotation;
    if (curve.type == CurveType::Coil) {
        orbit_to_curve_rotation.X.x = orbit_to_curve_rotation.Y.y = cosf(rotation_step_times_rev_count);
        orbit_to_curve_rotation.X.y = sinf(rotation_step_times_rev_count);
        orbit_to_curve_rotation.Y.x = -orbit_to_curve_rotation.X.y;
        orbit_to_curve_rotation.X.z = orbit_to_curve_rotation.Y.z = orbit_to_curve_rotation.Z.x = orbit_to_curve_rotation.Z.y =  0;
        orbit_to_curve_rotation.Z.z = 1;
    }

    // Transform vertices positions of edges from view-space to screen-space (w/ culling and clipping):
    mat3 accumulated_orbit_rotation = rotation;
    vec3 current_position, previous_position;
    Edge edge;

    for (u32 i = 0; i < step_count; i++) {
        center_to_orbit = rotation * center_to_orbit;

        switch (curve.type) {
            case CurveType::Helix:
                current_position = center_to_orbit;
                current_position.y -= 1;
                break;
            case CurveType::Coil:
                orbit_to_curve  = orbit_to_curve_rotation * orbit_to_curve;
                current_position = accumulated_orbit_rotation * orbit_to_curve;
                current_position += center_to_orbit;
                break;
            default:
                break;
        }

        current_position = cam.internPos(transform.externPos(current_position));

        if (i) {
            edge.from = previous_position;
            edge.to   = current_position;
            drawEdge(edge, viewport, color, opacity, line_width);
        }

        switch (curve.type) {
            case CurveType::Helix:
                center_to_orbit.y += 2 * one_over_step_count;
                break;
            case CurveType::Coil:
                accumulated_orbit_rotation *= rotation;
                break;
            default:
                break;
        }

        previous_position = current_position;
    }
}

#ifdef SLIM_ENABLE_VIEWPORT_CURVE_DRAWING
INLINE void Viewport::drawCurve(const Curve &curve, const Transform &transform,
                                const Color &color, f32 opacity, u8 line_width, u32 step_count) const {
    _drawCurve(curve, transform, *this, color, opacity, line_width, step_count);
}
#endif

INLINE void drawCurve(const Curve &curve, const Transform &transform, const Viewport &viewport,
                      const Color &color = White, f32 opacity = 1.0f, u8 line_width = 0, u32 step_count = CURVE_STEPS) {
    _drawCurve(curve, transform, viewport, color, opacity, line_width, step_count);
}



void _drawGrid(const Grid &grid, const Transform &transform, const Viewport &viewport, const Color &color, f32 opacity, u8 line_width) {
    static Grid view_space_grid;

    // Transform vertices positions from local-space to world-space and then to view-space:
    for (u8 segment = 0; segment < grid.u_segments; segment++) {
        view_space_grid.vertices.u.from[segment] = (viewport.camera->internPos(transform.externPos(grid.vertices.u.from[segment])));
        view_space_grid.vertices.u.to[segment] = (viewport.camera->internPos(transform.externPos(grid.vertices.u.to[segment])));
    }
    for (u8 segment = 0; segment < grid.v_segments; segment++) {
        view_space_grid.vertices.v.from[segment] = (viewport.camera->internPos(transform.externPos(grid.vertices.v.from[segment])));
        view_space_grid.vertices.v.to[segment] = (viewport.camera->internPos(transform.externPos(grid.vertices.v.to[segment])));
    }

    // Distribute transformed vertices positions to edges:
    view_space_grid.edges.update(view_space_grid.vertices, grid.u_segments, grid.v_segments);

    for (u8 u = 0; u < grid.u_segments; u++) drawEdge(view_space_grid.edges.u.edges[u], viewport, color, opacity, line_width);
    for (u8 v = 0; v < grid.v_segments; v++) drawEdge(view_space_grid.edges.v.edges[v], viewport, color, opacity, line_width);
}

#ifdef SLIM_ENABLE_VIEWPORT_GRID_DRAWING
INLINE void Viewport::drawGrid(const Grid &grid, const Transform &transform, const Color &color, f32 opacity, u8 line_width) const {
    _drawGrid(grid, transform, *this, color, opacity, line_width);
}
#endif

INLINE void drawGrid(const Grid &grid, const Transform &transform, const Viewport &viewport, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) {
    _drawGrid(grid, transform, viewport, color, opacity, line_width);
}


void _drawMesh(const Mesh &mesh, const Transform &transform, bool draw_normals, const Viewport &viewport, const Color &color, f32 opacity, u8 line_width) {
    const Camera &cam = *viewport.camera;
    vec3 pos;
    Edge edge;
    EdgeVertexIndices *edge_index = mesh.edge_vertex_indices;
    for (u32 i = 0; i < mesh.edge_count; i++, edge_index++) {
        edge.from = cam.internPos(transform.externPos(mesh.vertex_positions[edge_index->from]));
        edge.to   = cam.internPos(transform.externPos(mesh.vertex_positions[edge_index->to]));
        drawEdge(edge, viewport, color, opacity, line_width);
    }

    if (draw_normals && mesh.normals_count && mesh.vertex_normals && mesh.vertex_normal_indices) {
        TriangleVertexIndices *normal_index = mesh.vertex_normal_indices;
        TriangleVertexIndices *position_index = mesh.vertex_position_indices;
        for (u32 t = 0; t < mesh.triangle_count; t++, normal_index++, position_index++) {
            for (u8 i = 0; i < 3; i++) {
                pos = mesh.vertex_positions[position_index->ids[i]];
                edge.to = mesh.vertex_normals[normal_index->ids[i]] * 0.1f + pos;
                edge.from = cam.internPos(transform.externPos(pos));
                edge.to = cam.internPos(transform.externPos(edge.to));
                drawEdge(edge, viewport, Red, opacity * 0.5f, line_width);
            }
        }
    }
}

#ifdef SLIM_ENABLE_VIEWPORT_MESH_DRAWING
INLINE void Viewport::drawMesh(const Mesh &mesh, const Transform &transform, bool draw_normals, const Color &color,
                               f32 opacity, u8 line_width) const {
    _drawMesh(mesh, transform, draw_normals, *this, color, opacity, line_width);
}
#endif

INLINE void drawMesh(const Mesh &mesh, const Transform &transform, bool draw_normals, const Viewport &viewport,
                     const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) {
    _drawMesh(mesh, transform, draw_normals, viewport, color, opacity, line_width);
}

void drawSelection(Selection &selection, const Viewport &viewport, const Scene &scene) {
    static Box box;

    if (controls::is_pressed::alt && !mouse::is_captured && selection.geo_type && selection.geometry) {
        selection.xform = selection.geometry->transform;
        if (selection.geometry->type == GeometryType_Mesh)
            selection.xform.scale *= scene.meshes[selection.geometry->id].aabb.max;

        drawBox(box, selection.xform, viewport, Yellow, 0.5f, 0);
        if (selection.box_side) {
            ColorID color = White;
            switch (selection.box_side) {
                case Left:  case Right:  color = Red;   break;
                case Top:   case Bottom: color = Green; break;
                case Front: case Back:   color = Blue;  break;
                case NoSide: break;
            }

            drawBox(box, selection.xform, viewport, color, 0.5f, 1, selection.box_side);
        }
    }
}


struct SlimApp {
    time::Timer update_timer, render_timer;
    bool is_running{true};

    virtual void OnWindowResize(u16 width, u16 height) {};
    virtual void OnKeyChanged(  u8 key, bool pressed) {};
    virtual void OnMouseButtonUp(  mouse::Button &mouse_button) {};
    virtual void OnMouseButtonDown(mouse::Button &mouse_button) {};
    virtual void OnMouseButtonDoubleClicked(mouse::Button &mouse_button) {};
    virtual void OnMouseWheelScrolled(f32 amount) {};
    virtual void OnMousePositionSet(i32 x, i32 y) {};
    virtual void OnMouseMovementSet(i32 x, i32 y) {};
    virtual void OnMouseRawMovementSet(i32 x, i32 y) {};
    virtual void OnRender() {};
    virtual void OnUpdate(f32 delta_time) {};
    virtual void OnWindowRedraw() {
        update_timer.beginFrame();
        OnUpdate(update_timer.delta_time);
        update_timer.endFrame();

        render_timer.beginFrame();
        OnRender();
        render_timer.endFrame();
    };

    void resize(u16 width, u16 height) {
        window::width = width;
        window::height = height;
        OnWindowResize(width, height);
        OnWindowRedraw();
    }
};

SlimApp* createApp();


#ifdef __linux__
//linux code goes here
#elif _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifndef NDEBUG
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <new.h>

void DisplayError(LPTSTR lpszFunction) {
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    unsigned int last_error = GetLastError();

    FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, last_error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf, 0, nullptr);

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));

    if (FAILED( StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                                TEXT("%s failed with error code %d as follows:\n%s"), lpszFunction, last_error, lpMsgBuf)))
        printf("FATAL ERROR: Unable to output error code.\n");

    _tprintf((LPTSTR)"ERROR: %s\n", (LPCTSTR)lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}
#endif

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

WNDCLASSA window_class;
HWND window_handle;
HDC win_dc;
BITMAPINFO info;
RECT win_rect;
RAWINPUT raw_inputs;
HRAWINPUT raw_input_handle;
RAWINPUTDEVICE raw_input_device;
UINT raw_input_size;
PUINT raw_input_size_ptr = (PUINT)(&raw_input_size);
UINT raw_input_header_size = sizeof(RAWINPUTHEADER);

inline UINT getRawInput(LPVOID data) {
    return GetRawInputData(raw_input_handle, RID_INPUT, data, raw_input_size_ptr, raw_input_header_size);
}
inline bool hasRawInput() {
    return (getRawInput(0) == 0) && (raw_input_size != 0);
}
inline bool hasRawMouseInput(LPARAM lParam) {
    raw_input_handle = (HRAWINPUT)(lParam);
    return (
            (hasRawInput()) &&
            (getRawInput((LPVOID)&raw_inputs) == raw_input_size) &&
            (raw_inputs.header.dwType == RIM_TYPEMOUSE)
    );
}

LARGE_INTEGER performance_counter;

void os::setWindowTitle(char* str) {
    window::title = str;
    SetWindowTextA(window_handle, str);
}

void os::setCursorVisibility(bool on) {
    ShowCursor(on);
}

void os::setWindowCapture(bool on) {
    if (on) SetCapture(window_handle);
    else ReleaseCapture();
}

u64 time::getTicks() {
    QueryPerformanceCounter(&performance_counter);
    return (u64)performance_counter.QuadPart;
}

void* os::getMemory(u64 size) {
    return VirtualAlloc((LPVOID)MEMORY_BASE, (SIZE_T)size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

void os::closeFile(void *handle) {
    CloseHandle(handle);
}

void* os::openFileForReading(const char* path) {
    HANDLE handle = CreateFileA(path,           // file to open
                                GENERIC_READ,          // open for reading
                                FILE_SHARE_READ,       // share for reading
                                nullptr,                  // default security
                                OPEN_EXISTING,         // existing file only
                                FILE_ATTRIBUTE_NORMAL, // normal file
                                nullptr);                 // no attr. template
#ifndef NDEBUG
    if (handle == INVALID_HANDLE_VALUE) {
        DisplayError((LPTSTR)"CreateFile");
        _tprintf((LPTSTR)"Terminal failure: unable to open file \"%s\" for read.\n", path);
        return nullptr;
    }
#endif
    return handle;
}

void* os::openFileForWriting(const char* path) {
    HANDLE handle = CreateFileA(path,           // file to open
                                GENERIC_WRITE,          // open for writing
                                0,                      // do not share
                                nullptr,                   // default security
                                OPEN_ALWAYS,            // create new or open existing
                                FILE_ATTRIBUTE_NORMAL,  // normal file
                                nullptr);
#ifndef NDEBUG
    if (handle == INVALID_HANDLE_VALUE) {
        DisplayError((LPTSTR)"CreateFile");
        _tprintf((LPTSTR)"Terminal failure: unable to open file \"%s\" for write.\n", path);
        return nullptr;
    }
#endif
    return handle;
}

bool os::readFromFile(LPVOID out, DWORD size, HANDLE handle) {
    DWORD bytes_read = 0;
    BOOL result = ReadFile(handle, out, size, &bytes_read, nullptr);
#ifndef NDEBUG
    if (result == FALSE) {
        DisplayError((LPTSTR)"ReadFile");
        printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", (unsigned int)GetLastError());
        CloseHandle(handle);
    }
#endif
    return result != FALSE;
}

bool os::writeToFile(LPVOID out, DWORD size, HANDLE handle) {
    DWORD bytes_written = 0;
    BOOL result = WriteFile(handle, out, size, &bytes_written, nullptr);
#ifndef NDEBUG
    if (result == FALSE) {
        DisplayError((LPTSTR)"WriteFile");
        printf("Terminal failure: Unable to write from file.\n GetLastError=%08x\n", (unsigned int)GetLastError());
        CloseHandle(handle);
    }
#endif
    return result != FALSE;
}

SlimApp *CURRENT_APP;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    bool pressed = message == WM_SYSKEYDOWN || message == WM_KEYDOWN;
    u8 key = (u8)wParam;
    i32 x, y;
    f32 scroll_amount;

    switch (message) {
        case WM_DESTROY:
            CURRENT_APP->is_running = false;
            PostQuitMessage(0);
            break;

        case WM_SIZE:
            GetClientRect(window_handle, &win_rect);

            info.bmiHeader.biWidth = win_rect.right - win_rect.left;
            info.bmiHeader.biHeight = win_rect.top - win_rect.bottom;
            CURRENT_APP->resize((u16)info.bmiHeader.biWidth, (u16)-info.bmiHeader.biHeight);

            break;

        case WM_PAINT:
            SetDIBitsToDevice(win_dc,
                              0, 0, window::width, window::height,
                              0, 0, 0, window::height,
                              (u32*)window::content, &info, DIB_RGB_COLORS);

            ValidateRgn(window_handle, nullptr);
            break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
            switch (key) {
                case VK_CONTROL: controls::is_pressed::ctrl   = pressed; break;
                case VK_MENU   : controls::is_pressed::alt    = pressed; break;
                case VK_SHIFT  : controls::is_pressed::shift  = pressed; break;
                case VK_SPACE  : controls::is_pressed::space  = pressed; break;
                case VK_TAB    : controls::is_pressed::tab    = pressed; break;
                case VK_ESCAPE : controls::is_pressed::escape = pressed; break;
                case VK_LEFT   : controls::is_pressed::left   = pressed; break;
                case VK_RIGHT  : controls::is_pressed::right  = pressed; break;
                case VK_UP     : controls::is_pressed::up     = pressed; break;
                case VK_DOWN   : controls::is_pressed::down   = pressed; break;
                default: break;
            }
            CURRENT_APP->OnKeyChanged(key, pressed);

            break;

        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        case WM_LBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_LBUTTONDBLCLK:
            x = GET_X_LPARAM(lParam);
            y = GET_Y_LPARAM(lParam);
            mouse::Button *mouse_button;
            switch (message) {
                case WM_MBUTTONUP:
                case WM_MBUTTONDOWN:
                case WM_MBUTTONDBLCLK:
                    mouse_button = &mouse::middle_button;
                    break;
                case WM_RBUTTONUP:
                case WM_RBUTTONDOWN:
                case WM_RBUTTONDBLCLK:
                    mouse_button = &mouse::right_button;
                    break;
                default:
                    mouse_button = &mouse::left_button;
            }

            switch (message) {
                case WM_MBUTTONDBLCLK:
                case WM_RBUTTONDBLCLK:
                case WM_LBUTTONDBLCLK:
                    mouse_button->doubleClick(x, y);
                    mouse::double_clicked = true;
                    CURRENT_APP->OnMouseButtonDoubleClicked(*mouse_button);
                    break;
                case WM_MBUTTONUP:
                case WM_RBUTTONUP:
                case WM_LBUTTONUP:
                    mouse_button->up(x, y);
                    CURRENT_APP->OnMouseButtonUp(*mouse_button);
                    break;
                default:
                    mouse_button->down(x, y);
                    CURRENT_APP->OnMouseButtonDown(*mouse_button);
            }

            break;

        case WM_MOUSEWHEEL:
            scroll_amount = (f32)(GET_WHEEL_DELTA_WPARAM(wParam)) / (f32)(WHEEL_DELTA);
            mouse::scroll(scroll_amount); CURRENT_APP->OnMouseWheelScrolled(scroll_amount);
            break;

        case WM_MOUSEMOVE:
            x = GET_X_LPARAM(lParam);
            y = GET_Y_LPARAM(lParam);
            mouse::move(x, y);        CURRENT_APP->OnMouseMovementSet(x, y);
            mouse::setPosition(x, y); CURRENT_APP->OnMousePositionSet(x, y);
            break;

        case WM_INPUT:
            if ((hasRawMouseInput(lParam)) && (
                    raw_inputs.data.mouse.lLastX != 0 ||
                    raw_inputs.data.mouse.lLastY != 0)) {
                x = raw_inputs.data.mouse.lLastX;
                y = raw_inputs.data.mouse.lLastY;
                mouse::moveRaw(x, y); CURRENT_APP->OnMouseRawMovementSet(x, y);
            }

        default:
            return DefWindowProcA(hWnd, message, wParam, lParam);
    }

    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow) {
    void* window_content_and_canvas_memory = GlobalAlloc(GPTR, WINDOW_CONTENT_SIZE + (CANVAS_SIZE * CANVAS_COUNT));
    if (!window_content_and_canvas_memory)
        return -1;

    window::content = (u32*)window_content_and_canvas_memory;
    memory::canvas_memory = (u8*)window_content_and_canvas_memory + WINDOW_CONTENT_SIZE;

    controls::key_map::ctrl = VK_CONTROL;
    controls::key_map::alt = VK_MENU;
    controls::key_map::shift = VK_SHIFT;
    controls::key_map::space = VK_SPACE;
    controls::key_map::tab = VK_TAB;
    controls::key_map::escape = VK_ESCAPE;
    controls::key_map::left = VK_LEFT;
    controls::key_map::right = VK_RIGHT;
    controls::key_map::up = VK_UP;
    controls::key_map::down = VK_DOWN;

    LARGE_INTEGER performance_frequency;
    QueryPerformanceFrequency(&performance_frequency);

    time::ticks_per_second = (u64)performance_frequency.QuadPart;
    time::seconds_per_tick = 1.0 / (f64)(time::ticks_per_second);
    time::milliseconds_per_tick = 1000.0 * time::seconds_per_tick;
    time::microseconds_per_tick = 1000.0 * time::milliseconds_per_tick;
    time::nanoseconds_per_tick  = 1000.0 * time::microseconds_per_tick;

    CURRENT_APP = createApp();
    if (!CURRENT_APP->is_running)
        return -1;

    info.bmiHeader.biSize        = sizeof(info.bmiHeader);
    info.bmiHeader.biCompression = BI_RGB;
    info.bmiHeader.biBitCount    = 32;
    info.bmiHeader.biPlanes      = 1;

    window_class.lpszClassName  = "RnDer";
    window_class.hInstance      = hInstance;
    window_class.lpfnWndProc    = WndProc;
    window_class.style          = CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    window_class.hCursor        = LoadCursorA(nullptr, MAKEINTRESOURCEA(32512));

    if (!RegisterClassA(&window_class)) return -1;

    win_rect.top = 0;
    win_rect.left = 0;
    win_rect.right  = window::width;
    win_rect.bottom = window::height;
    AdjustWindowRect(&win_rect, WS_OVERLAPPEDWINDOW, false);

    window_handle = CreateWindowA(
            window_class.lpszClassName,
            window::title,
            WS_OVERLAPPEDWINDOW,

            CW_USEDEFAULT,
            CW_USEDEFAULT,
            win_rect.right - win_rect.left,
            win_rect.bottom - win_rect.top,

            nullptr,
            nullptr,
            hInstance,
            nullptr
    );
    if (!window_handle)
        return -1;

    raw_input_device.usUsagePage = 0x01;
    raw_input_device.usUsage = 0x02;
    if (!RegisterRawInputDevices(&raw_input_device, 1, sizeof(raw_input_device)))
        return -1;

    win_dc = GetDC(window_handle);

    SetICMMode(win_dc, ICM_OFF);

    ShowWindow(window_handle, nCmdShow);

    MSG message;
    while (CURRENT_APP->is_running) {
        while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        CURRENT_APP->OnWindowRedraw();
        mouse::resetChanges();
        InvalidateRgn(window_handle, nullptr, false);
    }

    return 0;
}

#endif