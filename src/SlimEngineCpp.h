#pragma once

#define SLIM_ENGINE_SHF

#ifdef __cplusplus
#include <cmath>
#else
#include <math.h>
#endif

#if defined(__clang__)
#define COMPILER_CLANG 1
#define COMPILER_CLANG_OR_GCC 1
#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPILER_GCC 1
    #define COMPILER_CLANG_OR_GCC 1
#elif defined(_MSC_VER)
#define COMPILER_MSVC 1
#endif

#ifndef NDEBUG
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

#ifndef false
#define false (u8)0
#endif

#ifndef true
#define true (u8)1
#endif


#define FONT_WIDTH 18
#define FONT_HEIGHT 24

typedef void* (*CallbackWithInt)(u64 size);
typedef void (*CallbackWithBool)(bool on);
typedef void (*CallbackWithCharPtr)(char* str);

#define TAU 6.28f
#define SQRT2_OVER_2 0.70710678118f
#define SQRT3 1.73205080757f
#define COLOR_COMPONENT_TO_FLOAT 0.00392156862f
#define FLOAT_TO_COLOR_COMPONENT 255.0f
#define DEG_TO_RAD 0.0174533f

#define MAX_COLOR_VALUE 0xFF
#define THREE_QUARTERS_COLOR_VALUE 0xC0
#define HALF_COLOR_VALUE 0x80
#define QUARTER_COLOR_VALUE 0x40

#define Kilobytes(value) ((value)*1024LL)
#define Megabytes(value) (Kilobytes(value)*1024LL)
#define Gigabytes(value) (Megabytes(value)*1024LL)
#define Terabytes(value) (Gigabytes(value)*1024LL)

#define MEMORY_SIZE Gigabytes(1)
#define MEMORY_BASE Terabytes(2)

#define MAX_WIDTH 3840
#define MAX_HEIGHT 2160
#define MAX_WINDOW_SIZE (MAX_WIDTH * MAX_HEIGHT)

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

#define PIXEL_SIZE 4
#define WINDOW_CONTENT_SIZE (MAX_WINDOW_SIZE * PIXEL_SIZE)

#define BOX__ALL_SIDES (Top | Bottom | Left | Right | Front | Back)
#define BOX__VERTEX_COUNT 8
#define BOX__EDGE_COUNT 12
#define GRID__MAX_SEGMENTS 101

#define CUBE_UV_COUNT 4
#define CUBE_NORMAL_COUNT 6
#define CUBE_VERTEX_COUNT 8
#define CUBE_TRIANGLE_COUNT 12

#define IS_VISIBLE ((u8)1)
#define IS_TRANSLATED ((u8)2)
#define IS_ROTATED ((u8)4)
#define IS_SCALED ((u8)8)
#define IS_SCALED_NON_UNIFORMLY ((u8)16)
#define ALL_FLAGS (IS_VISIBLE | IS_TRANSLATED | IS_ROTATED | IS_SCALED | IS_SCALED_NON_UNIFORMLY)

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

#include <cmath>

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

INLINE void swap(i32 *a, i32 *b) {
    i32 t = *a;
    *a = *b;
    *b = t;
}

INLINE void subRange(i32 from, i32 to, i32 end, i32 start, i32 *first, i32 *last) {
    *first = from;
    *last  = to;
    if (to < from) swap(first, last);
    *first = *first > start ? *first : start;
    *last  = (*last < end ? *last : end) - 1;
}

INLINE bool inRange(i32 value, i32 end, i32 start = 0) {
    return start <= value && value < end;
}

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

union RGBA {
    u32 value;
    struct {
        u8 B, G, R, A;
    };

    RGBA() : RGBA{0, 0, 0, MAX_COLOR_VALUE} {}
    RGBA(u8 r, u8 g, u8 b, u8 a) : B{b}, G{g}, R{r}, A{a} {}

    RGBA(enum ColorID color_id) : RGBA{0, 0, 0, MAX_COLOR_VALUE} {
        switch (color_id) {
            case Black: break;
            case White:
                R = MAX_COLOR_VALUE;
                G = MAX_COLOR_VALUE;
                B = MAX_COLOR_VALUE;
                break;
            case Grey:
                R = HALF_COLOR_VALUE;
                G = HALF_COLOR_VALUE;
                B = HALF_COLOR_VALUE;
                break;
            case DarkGrey:
                R = QUARTER_COLOR_VALUE;
                G = QUARTER_COLOR_VALUE;
                B = QUARTER_COLOR_VALUE;
                break;
            case BrightGrey:
                R = THREE_QUARTERS_COLOR_VALUE;
                G = THREE_QUARTERS_COLOR_VALUE;
                B = THREE_QUARTERS_COLOR_VALUE;
                break;

            case Red:
                R = MAX_COLOR_VALUE;
                break;
            case Green:
                G = MAX_COLOR_VALUE;
                break;
            case Blue:
                B = MAX_COLOR_VALUE;
                break;

            case DarkRed:
                R = HALF_COLOR_VALUE;
                break;
            case DarkGreen:
                G = HALF_COLOR_VALUE;
                break;
            case DarkBlue:
                B = HALF_COLOR_VALUE;
                break;

            case DarkCyan:
                G = HALF_COLOR_VALUE;
                B = HALF_COLOR_VALUE;
                break;
            case DarkMagenta:
                R = HALF_COLOR_VALUE;
                B = HALF_COLOR_VALUE;
                break;
            case DarkYellow:
                R = HALF_COLOR_VALUE;
                G = HALF_COLOR_VALUE;
                break;

            case BrightRed:
                R = MAX_COLOR_VALUE;
                G = HALF_COLOR_VALUE;
                B = HALF_COLOR_VALUE;
                break;
            case BrightGreen:
                R = HALF_COLOR_VALUE;
                G = MAX_COLOR_VALUE;
                B = HALF_COLOR_VALUE;
                break;
            case BrightBlue:
                R = HALF_COLOR_VALUE;
                G = HALF_COLOR_VALUE;
                B = MAX_COLOR_VALUE;
                break;

            case Cyan:
                B = MAX_COLOR_VALUE;
                G = MAX_COLOR_VALUE;
                break;
            case Magenta:
                R = MAX_COLOR_VALUE;
                B = MAX_COLOR_VALUE;
                break;
            case Yellow:
                R = MAX_COLOR_VALUE;
                G = MAX_COLOR_VALUE;
                break;

            case BrightCyan:
                G = THREE_QUARTERS_COLOR_VALUE;
                B = THREE_QUARTERS_COLOR_VALUE;
                break;
            case BrightMagenta:
                R = THREE_QUARTERS_COLOR_VALUE;
                B = THREE_QUARTERS_COLOR_VALUE;
                break;
            case BrightYellow:
                R = THREE_QUARTERS_COLOR_VALUE;
                G = THREE_QUARTERS_COLOR_VALUE;
                break;
        }
    }
};

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
        while (path[dir_len] != '/' && path[dir_len] != '\\') dir_len--;
        return dir_len + 1;
    }
};

struct NumberString {
    u8 float_digits_count = 3;
    String string{_buffer, 0};
    char _buffer[13]{};

    explicit NumberString(u8 digit_count = 3) : string{_buffer, 1}, float_digits_count{digit_count} {
        _buffer[12] = 0;
        for (u8 i = 0; i < 12; i++)
            _buffer[i] = ' ';
    }

    explicit NumberString(const char *str, u8 digits_count = 3) : float_digits_count{digits_count}, string{_buffer, 1} {
        _buffer[12] = 0;
        char *char_ptr = (char*)str;
        string.length = (u8)String::getLength(char_ptr);
        if (string.length > 12) string.length = 12;
        char_ptr += string.length;
        char_ptr--;
        for (u8 i = 11; i >= 0; i--, char_ptr--)
            _buffer[i] = (11 - i) < float_digits_count ? *char_ptr : ' ';
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

namespace controls {
    namespace key_map {
        u8 ctrl{0};
        u8 alt{0};
        u8 shift{0};
        u8 space{0};
        u8 tab{0};
        u8 escape{0};
    }

    namespace is_pressed {
        bool ctrl{false};
        bool alt{false};
        bool shift{false};
        bool space{false};
        bool tab{false};
        bool escape{false};
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

        bool is_pressed{false}, is_handled{false}, double_clicked{false};

        void down(i32 x, i32 y) {
            is_pressed = true;
            is_handled = false;

            down_pos_x = x;
            down_pos_y = y;
        }

        void up(i32 x, i32 y) {
            is_pressed = false;
            is_handled = false;

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
    bool move_handled{false};
    bool double_clicked{false};
    bool double_clicked_handled{false};
    bool wheel_scrolled{false};
    bool wheel_scroll_handled{false};
    bool raw_movement_handled{false};

    void resetChanges() {
        if (move_handled) {
            move_handled = false;
            moved = false;
        }
        if (double_clicked_handled) {
            double_clicked_handled = false;
            double_clicked = false;
        }
        if (raw_movement_handled) {
            raw_movement_handled = false;
            pos_raw_diff_x = 0;
            pos_raw_diff_y = 0;
        }
        if (wheel_scroll_handled) {
            wheel_scroll_handled = false;
            wheel_scrolled = false;
            wheel_scroll_amount = 0;
        }
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



struct vec2i {
    i32 x, y;

    vec2i() noexcept : vec2i{0} {}
    vec2i(i32 x, i32 y) noexcept : x(x), y(y) {}
    vec2i(vec2i &other) noexcept : vec2i{other.x, other.y} {}
    vec2i(const vec2i &other) noexcept : vec2i{other.x, other.y} {}
    explicit vec2i(i32 value) noexcept : vec2i{value, value} {}

    INLINE bool operator == (const vec2i &other) const {
        return other.x == x &&
               other.y == y;
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

    INLINE i32 min() const {
        return x < y ? x : y;
    }

    INLINE i32 max() const {
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
        return other.x == x &&
               other.y == y;
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

    INLINE f32 operator | (const vec2 &rhs) const {
        return dot(rhs);
    }

    INLINE vec2 operator % (const vec2 &rhs) const {
        return reflectAround(rhs);
    }

    INLINE vec2 operator ~ () const {
        return this->perp();
    }

    INLINE f32 operator ^ (const vec2 &rhs) const {
        return cross(rhs);
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

    INLINE f32 min() const {
        return x < y ? x : y;
    }

    INLINE f32 max() const {
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

    INLINE vec2 reflectAround(const vec2 &N) const {
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

INLINE vec2 min(const vec2 &a, const vec2 &b) {
    return {
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y
    };
}

INLINE vec2 max(const vec2 &a, const vec2 &b) {
    return {
            a.x > b.x ? a.x : b.x,
            a.y > b.y ? a.y : b.y
    };
}

INLINE vec2i min(const vec2i &a, const vec2i &b) {
    return {
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y
    };
}

INLINE vec2i max(const vec2i &a, const vec2i &b) {
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

    INLINE RGBA toRGBA() const {
        return {
                (u8)(255.0f * fmaxf(0, fminf(r, 1.0f))),
                (u8)(255.0f * fmaxf(0, fminf(g, 1.0f))),
                (u8)(255.0f * fmaxf(0, fminf(b, 1.0f))),
                (u8)(255.0f * fmaxf(0, fminf(a, 1.0f)))
        };
    }

    INLINE bool operator == (const vec4 &other) const {
        return other.x == x &&
               other.y == y &&
               other.z == z &&
               other.w == w;
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

    INLINE f32 operator | (const vec4 &rhs) const {
        return dot(rhs);
    }

    INLINE vec4 operator % (const vec4 &rhs) const {
        return reflectAround(rhs);
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

    INLINE f32 min() const {
        f32 mn = x < y ? x : y;
        mn = mn < z ? mn : z;
        mn = mn < w ? mn : w;
        return mn;
    }

    INLINE f32 max() const {
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

    INLINE vec4 reflectAround(const vec4 &N) const {
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

INLINE vec4 min(const vec4 &a, const vec4 &b) {
    return {
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y,
            a.z < b.y ? a.z : b.z,
            a.w < b.w ? a.w : b.w
    };
}

INLINE vec4 max(const vec4 &a, const vec4 &b) {
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
        f32 q1 = axis.x;
        f32 q2 = axis.y;
        f32 q3 = axis.z;

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


struct mat2 {
    union {
        f32 components[4];
        vec2 axis[2];
        struct { vec2 X, Y; };
        struct { vec2 right, up; };
    };

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
    union {
        f32 components[9];
        vec3 axis[3];
        struct { vec3 X, Y, Z; };
        struct { vec3 right, up, forward; };
    };

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

//    INLINE mat3 inverted() const {
//        return mat3{
//                +(Y.y * Z.z - Z.y * Y.z),
//                -(Y.x * Z.z - Z.x * Y.z),
//                +(Y.x * Z.y - Z.x * Y.y),
//
//                -(X.y * Z.z - Z.y * X.z),
//                +(X.x * Z.z - Z.x * X.z),
//                -(X.x * Z.y - Z.x * X.y),
//
//                +(X.y * Y.z - Y.y * X.z),
//                -(X.x * Y.z - Y.x * X.z),
//                +(X.x * Y.y - Y.x * X.y)
//        } / det();
//    }

//    INLINE mat3 operator ! () const {
//        return inverted();
//    }

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


struct mat4 {
    union {
        f32 components[16];
        vec4 axis[4];
        struct { vec4 X, Y, Z, W; };
    };
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

struct Edge {
    vec3 from, to;
};


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

template <class T>
struct Orientation {
    T rotation{};

    Orientation() : rotation{T::Identity} {}
    explicit Orientation(f32 x_radians, f32 y_radians = 0, f32 z_radians = 0) {
        setRotation(x_radians, y_radians, z_radians);
    }

    INLINE void rotate(f32 x_radians, f32 y_radians, f32 z_radians) {
        setRotation(angles.x + x_radians, angles.y + y_radians, angles.x + z_radians);
    }

    INLINE void rotate(f32 x_radians, f32 y_radians) {
        setRotation(angles.x + x_radians, angles.y + y_radians);
    }

    INLINE void setRotation(f32 x_radians, f32 y_radians, f32 z_radians) {
        angles.x = x_radians;
        angles.y = y_radians;
        angles.z = z_radians;
        _update();
    }

    INLINE void setRotation(f32 x_radians, f32 y_radians) {
        angles.x = x_radians;
        angles.y = y_radians;
        _update();
    }

    INLINE void rotateAroundX(f32 radians) {
        setRotationAroundX(angles.x + radians);
    }

    INLINE void rotateAroundY(f32 radians) {
        setRotationAroundY(angles.y + radians);
    }

    INLINE void rotateAroundZ(f32 radians) {
        setRotationAroundZ(angles.z + radians);
    }

    INLINE void setRotationAroundX(f32 radians) {
        angles.x = radians;
        _update();
    }

    INLINE void setRotationAroundY(f32 radians) {
        angles.y = radians;
        _update();
    }

    INLINE void setRotationAroundZ(f32 radians) {
        angles.z = radians;
        _update();
    }

protected:
    vec3 angles;

    void _update() {
        rotation = T::Identity;
        if (angles.z != 0.0f) rotation = T::RotationAroundZ(angles.z);
        if (angles.x != 0.0f) rotation *= T::RotationAroundX(angles.x);
        if (angles.y != 0.0f) rotation *= T::RotationAroundY(angles.y);
    }
};

struct Transform : public Orientation<quat> {
    vec3 position{0.0f};
    vec3 scale{1.0f};

    Transform() : Orientation<quat>{}, position{0.0f}, scale{1.0f} {}
    Transform(const vec3 &position) : Orientation<quat>{}, position{position}, scale{1.0f} {}
    Transform(const vec3 &position, const vec3 &orientation, const vec3 &scale = vec3{1.0f}) :
            Orientation{orientation.x, orientation.y, orientation.z}, position{position}, scale{scale} {}

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

enum GeometryType {
    GeometryType_None = 0,

    GeometryType_Mesh,
    GeometryType_Grid,
    GeometryType_Box,
    GeometryType_Curve,

    GeometryType_Count
};

struct Geometry {
    Transform transform;
    enum GeometryType type{GeometryType_None};
    enum ColorID color{White};
    u32 id{0};
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

    AABB& operator *= (const Transform &transform) {
        const vec3 vertices[8] = {
                {min.x, min.y, min.z},
                {min.x, min.y, max.z},
                {min.x, max.y, min.z},
                {min.x, max.y, max.z},
                {max.x, min.y, min.z},
                {max.x, min.y, max.z},
                {max.x, max.y, min.z},
                {max.x, max.y, max.z}
        };

        min = +INFINITY;
        max = -INFINITY;

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

        return *this;
    }
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

INLINE BoxSide getBoxSide(const vec3 &octant, u8 axis) {
    switch (axis) {
        case 0 : return octant.x > 0 ? Right : Left;
        case 3 : return octant.x > 0 ? Left : Right;
        case 1 : return octant.y > 0 ? Top : Bottom;
        case 4 : return octant.y > 0 ? Bottom : Top;
        case 2 : return octant.z > 0 ? Front : Back;
        default: return octant.z > 0 ? Back : Front;
    }
}

template <class VectorType>
struct RayHitOf {
    VectorType position{}, normal{};
    f32 distance{}, distance_squared{};
    u32 geo_id{};
    enum GeometryType geo_type{GeometryType_None};
    bool from_behind{false};
};

template<class VectorType>
struct RayOf {
    VectorType origin{}, direction{};
    RayHitOf<VectorType> hit{};

    INLINE VectorType at(f32 t) const { return origin + t*direction; }
    INLINE VectorType operator [](f32 t) const { return at(t); }
};

using Ray = RayOf<vec3>;
using Ray2D = RayOf<vec2>;


INLINE BoxSide rayHitsCube(Ray &ray) {
    vec3 octant, RD_rcp = 1.0f / ray.direction;
    octant.x = signbit(ray.direction.x) ? 1.0f : -1.0f;
    octant.y = signbit(ray.direction.y) ? 1.0f : -1.0f;
    octant.z = signbit(ray.direction.z) ? 1.0f : -1.0f;

    f32 t[6];
    t[0] = (+octant.x - ray.origin.x) * RD_rcp.x;
    t[1] = (+octant.y - ray.origin.y) * RD_rcp.y;
    t[2] = (+octant.z - ray.origin.z) * RD_rcp.z;
    t[3] = (-octant.x - ray.origin.x) * RD_rcp.x;
    t[4] = (-octant.y - ray.origin.y) * RD_rcp.y;
    t[5] = (-octant.z - ray.origin.z) * RD_rcp.z;

    u8 max_axis = t[3] < t[4] ? 3 : 4; if (t[5] < t[max_axis]) max_axis = 5;
    f32 max_t = t[max_axis];
    if (max_t < 0) // Further-away hit is behind the ray - intersection can not occur.
        return NoSide;

    u8 min_axis = t[0] > t[1] ? 0 : 1; if (t[2] > t[min_axis]) min_axis = 2;
    f32 min_t = t[min_axis];
    if (max_t < (min_t > 0 ? min_t : 0))
        return NoSide;

    ray.hit.from_behind = min_t < 0; // Further-away hit is in front of the ray, closer one is behind it.
    if (ray.hit.from_behind) {
        min_t = max_t;
        min_axis = max_axis;
    }

    BoxSide side = getBoxSide(octant, min_axis);
    ray.hit.position = ray.direction.scaleAdd(min_t, ray.origin);
    ray.hit.normal = 0.0f;
    switch (side) {
        case Left:   ray.hit.normal.x = ray.hit.from_behind ? +1.0f : -1.0f; break;
        case Right:  ray.hit.normal.x = ray.hit.from_behind ? -1.0f : +1.0f; break;
        case Bottom: ray.hit.normal.y = ray.hit.from_behind ? +1.0f : -1.0f; break;
        case Top:    ray.hit.normal.y = ray.hit.from_behind ? -1.0f : +1.0f; break;
        case Back:   ray.hit.normal.z = ray.hit.from_behind ? +1.0f : -1.0f; break;
        case Front:  ray.hit.normal.z = ray.hit.from_behind ? -1.0f : +1.0f; break;
        default: return NoSide;
    }

    return side;
}

INLINE bool rayHitsPlane(Ray &ray, const vec3 &P, const vec3 &N) {
    f32 NdotRd = N | ray.direction;
    if (NdotRd == 0) // The ray is parallel to the plane
        return false;

    f32 NdotRoP = N | (P - ray.origin);
    if (NdotRoP == 0) return false; // The ray originated within the plane

    bool ray_is_facing_the_plane = NdotRd < 0;
    ray.hit.from_behind = NdotRoP > 0;
    if (ray.hit.from_behind == ray_is_facing_the_plane) // The ray can't hit the plane
        return false;

    ray.hit.distance = NdotRoP / NdotRd;
    ray.hit.position = ray.origin + ray.direction*ray.hit.distance;
    ray.hit.normal = N;

    return true;
}

INLINE bool rightwardRayHitsBound(Ray2D &ray, const float x_bound) {
    f32 distance = x_bound - ray.origin.x;
    if (distance < 0 || // Rect is behind the ray
        distance > ray.direction.x) // Rect is too far in front of the ray
        return false;

    ray.hit.distance = distance;
    ray.hit.position = ray[distance / ray.direction.x];
    ray.hit.normal = {-1.0f, 0.0f};
    ray.hit.from_behind = false;

    return true;
}

INLINE bool leftwardRayHitsBound(Ray2D &ray, const float x_bound) {
    f32 distance = ray.origin.x - x_bound;
    if (distance < 0 || // Rect is behind the ray
        distance > -ray.direction.x) // Rect is too far in front of the ray
        return false;

    ray.hit.position = ray[distance / -ray.direction.x];
    ray.hit.normal = {1.0f, 0.0f};
    ray.hit.distance = distance;
    ray.hit.from_behind = false;

    return true;
}

INLINE bool horizontalRayHitsRect(Ray2D &ray, const Rect &rect) {
    if (ray.direction.x == 0) return false;
    return signbit(ray.direction.x) ?
           leftwardRayHitsBound(ray, rect.left) :
           rightwardRayHitsBound(ray, rect.right);
}

INLINE bool upwardRayHitsBound(Ray2D &ray, const float y_bound) {
    f32 distance = y_bound - ray.origin.y;
    if (distance < 0 || // Rect is behind the ray
        distance > ray.direction.y) // Rect is too far in front of the ray
        return false;

    ray.hit.distance = distance;
    ray.hit.position = ray[distance / ray.direction.y];
    ray.hit.normal = {0.0f, -1.0f};
    ray.hit.from_behind = false;

    return true;
}

INLINE bool downwardRayHitsBound(Ray2D &ray, const float y_bound) {
    f32 distance = ray.origin.y - y_bound;
    if (distance < 0 || // Rect is behind the ray
        distance > -ray.direction.y) // Rect is too far in front of the ray
        return false;

    ray.hit.position = ray[distance / -ray.direction.y];
    ray.hit.normal = {0.0f, 1.0f};
    ray.hit.distance = distance;
    ray.hit.from_behind = false;

    return true;
}

INLINE bool verticalRayHitsRect(Ray2D &ray, const Rect &rect) {
    if (ray.direction.y == 0) return false;
    return signbit(ray.direction.y) ?
           downwardRayHitsBound(ray, rect.bottom) :
           upwardRayHitsBound(ray, rect.top);
}

INLINE bool rayHitRect(Ray2D &ray, const Rect &rect) {
    if (ray.direction.x == 0) return verticalRayHitsRect(ray, rect);
    if (ray.direction.y == 0) return horizontalRayHitsRect(ray, rect);
    bool aiming_left{signbit(ray.direction.x)};
    bool aiming_down{signbit(ray.direction.y)};
    vec2 RD_rcp = 1.0f / ray.direction;
    vec2 near{aiming_left ? rect.right : rect.left, aiming_down ? rect.top : rect.bottom};
    vec2 far{ aiming_left ? rect.left : rect.right, aiming_down ? rect.bottom : rect.top};
    near -= ray.origin;
    near *= RD_rcp;
    far -= ray.origin;
    far *= RD_rcp;
    if (near.x > far.y || near.y > far.x) return false; // Early rejection

    f32 near_t = near.x > near.y ? near.x : near.y;
    f32 far_t  = far.x  < far.y  ? far.x  : far.y;
    if (far_t < 0) return false; // Reject if ray direction is pointing away from object

    // Contact point of collision from parametric line equation
    ray.hit.position = ray[near_t];
    ray.hit.normal = 0.0f;
    if (     near.x > near.y) ray.hit.normal.x = aiming_left ? 1.0f : -1.0f;
    else if (near.x < near.y) ray.hit.normal.y = aiming_down ? 1.0f : -1.0f;

    return true;
}


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

union GridVertices {
    vec3 buffer[2][2][GRID__MAX_SEGMENTS];
    struct {
        GridAxisVertices u, v;
    };

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

union GridEdges {
    Edge buffer[2][GRID__MAX_SEGMENTS];

    struct {
        GridAxisEdges u, v;
    };

    GridEdges(const GridVertices &vertices, u8 u_segments, u8 v_segments) {
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


struct Camera : public Orientation<mat3> {
    vec3 position{0};
    vec3 current_velocity{0};
    f32 focal_length{  CAMERA_DEFAULT__FOCAL_LENGTH};
    f32 zoom_amount{   CAMERA_DEFAULT__FOCAL_LENGTH};
    f32 target_distance{CAMERA_DEFAULT__TARGET_DISTANCE};
    f32 dolly_amount{0};

    Camera() : Orientation<mat3>{}, position{0.0f} {}
    explicit Camera(const vec3 &position) : Orientation<mat3>{}, position{position} {}
    explicit Camera(const vec3 &position, const vec3 &orientation = vec3{0.0f}, f32 zoom_amount = CAMERA_DEFAULT__FOCAL_LENGTH) :
            Orientation{orientation.x, orientation.y, orientation.z},
            position{position}, current_velocity{vec3{0}}, focal_length{zoom_amount}, zoom_amount{zoom_amount} {}

    void zoom(f32 amount) {
        f32 new_amount = zoom_amount + amount;
        focal_length = new_amount > 1 ? new_amount : (new_amount < -1.0f ? (-1.0f / new_amount) : 1.0f);
        zoom_amount = new_amount;
    }

    void dolly(f32 amount) {
        vec3 target_position = rotation.forward.scaleAdd(target_distance, position);

        // Compute new target distance:
        dolly_amount += amount;
        target_distance = powf(2.0f, dolly_amount / -200.0f) * CAMERA_DEFAULT__TARGET_DISTANCE;

        // Back-track from target position_x to new current position_x:
        position = target_position - (rotation.forward * target_distance);
    }

    void orbit(f32 azimuth, f32 altitude) {
        // Move the camera forward to the position_x of its target:
        position += rotation.forward * target_distance;

        // Reorient the camera while it is at the position_x of its target:
        rotate(altitude, azimuth);

        // Back the camera away from its target position_x using the updated forward direction:
        position -= rotation.forward * target_distance;
    }

    void pan(f32 right_amount, f32 up_amount) {
        position += rotation.up * up_amount + rotation.right * right_amount;
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
        new(&mesh) Mesh{};
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
//    String *mesh_files{nullptr};
//    String *texture_files{nullptr};
//    AmbientLight ambient_light;
//    Light *lights{nullptr};
//    Material *materials{nullptr};
//    Texture *textures{nullptr};

    Geometry *geometries{nullptr};
    Curve *curves{nullptr};
    Grid *grids{nullptr};
    Box *boxes{nullptr};
    Camera *cameras{nullptr};
    Mesh *meshes{nullptr};
//    AABB *mesh_aabbs{nullptr};
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
            meshes = new(meshes) Mesh[counts.meshes];
            memory::MonotonicAllocator temp_allocator;
            if (!memory_allocator) {
                u32 capacity = getTotalMemoryForMeshes(mesh_files, 2);
                temp_allocator = memory::MonotonicAllocator{capacity};
                memory_allocator = &temp_allocator;
            }
            for (u32 i = 0; i < counts.meshes; i++) load(meshes[i], mesh_files[i].char_ptr, memory_allocator);
        }
//        if (counts.lights) {
//            Light *light = lights = (Light*)memory::allocate(sizeof(Light) * counts.lights);
//            for (u32 i = 0; i < counts.lights; i++, light++) {
//                light = new(light) Light;
//                for (u8 c = 0; c < 3; c++) {
//                    light->position_or_direction.components[c] = 0;
//                    light->color.components[c]                 = 1;
//                    light->attenuation.components[c]           = 1;
//                }
//                light->intensity = 1;
//                light->is_directional = false;
//            }
//        }
//
//        if (counts.materials)   {
//            Material *material = materials = (Material*)memory::allocate(sizeof(Material) * counts.materials);
//            for (u32 i = 0; i < counts.materials; i++, material++) new(material) Material;
//        }
//
//        if (counts.textures && counts.texture_files) {
//            Texture *texture = textures = (Texture*)memory::allocate(sizeof(Texture) * counts.textures);
//            if (textures) for (u32 i = 0; i < counts.textures; i++, texture++) {
//                texture = new(texture) Texture;
//                texture->load(counts.texture_files[i].char_ptr);
//            }
//        }
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

            current_found = rayHitsCube(local_ray);
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

struct HUDLine {
    enum ColorID title_color{White}, value_color{White}, alternate_value_color{White};
    String title{}, alternate_value{};
    NumberString value{};
    bool invert_alternate_use{false}, *use_alternate{nullptr};

    HUDLine(enum ColorID default_color = White) :
            title_color{default_color},
            value_color{default_color},
            alternate_value_color{default_color}
    {}

    HUDLine(char* title_char_ptr, enum ColorID default_color = White) :
            title_color{default_color},
            value_color{default_color},
            alternate_value_color{default_color},
            title{title_char_ptr}
    {}
};

struct HUDSettings {
    u32 line_count{0};
    f32 line_height{1.0f};
    enum ColorID default_color{White};

    HUDSettings(u32 line_count = 0, f32 line_height = 1.0f, ColorID default_color = White) :
            line_count{line_count}, line_height{line_height}, default_color{default_color} {}
};

struct HUD {
    HUDSettings settings;
    HUDLine *lines{nullptr};
    vec2i position{10, 10};
    bool enabled{true};

    HUD() = default;
    HUD(HUDSettings settings, HUDLine *lines, vec2i position = {10, 10}) : settings{settings}, lines{lines}, position{position} {
        if (settings.default_color != White) for (u32 i = 0; i < settings.line_count; i++)
                lines[i].title_color = lines[i].alternate_value_color = lines[i].value_color = settings.default_color;
    }
    HUD(HUDSettings settings, memory::AllocateMemory allocate_memory = nullptr, vec2i position = {10, 10}) : settings{settings}, position{position} {
        if (settings.line_count) {
            lines = (HUDLine*)allocate_memory(settings.line_count * sizeof(HUDLine));
            for (u32 i = 0; i < settings.line_count; i++)
                new(lines + i) HUDLine{settings.default_color};
        }
    }
};


struct Frustum {
    struct Projection {
        union {
            struct {
                vec3 scale;
                f32 shear;
            };
            struct {
                vec3 projected_position;
                f32 w;
            };
        };

        Projection(f32 x, f32 y, f32 z, f32 w) : scale{x, y, z}, shear{w} {}
        Projection(f32 focal_length, f32 height_over_width, f32 n, f32 f, bool cube_NDC) : scale{0}, shear{0} {
            update(focal_length, height_over_width, n, f, cube_NDC);
        }
        Projection(const Projection &other) : scale{other.scale}, shear{other.shear} {}


        void update(f32 focal_length, f32 height_over_width, f32 n, f32 f, bool cube_NDC) {
            scale.x = focal_length * height_over_width;
            scale.y = focal_length;
            scale.z = shear = 1.0f / (f - n);
            if (cube_NDC) {
                scale.z *= f + n;
                shear *= f * n * -2;
            } else {
                scale.z *= f;
                shear *= f * -n;
            }
        }

        Projection project(const vec3 &position) const {
            return {
                    position.x * scale.x,
                    position.y * scale.y,
                    position.z * scale.z + shear,
                    position.z
            };
        }
    };
    Projection projection{
            CAMERA_DEFAULT__FOCAL_LENGTH,
            (f32)DEFAULT_HEIGHT / (f32)DEFAULT_WIDTH,
            VIEWPORT_DEFAULT__NEAR_CLIPPING_PLANE_DISTANCE,
            VIEWPORT_DEFAULT__FAR_CLIPPING_PLANE_DISTANCE,
            false
    };

    f32 near_clipping_plane_distance{VIEWPORT_DEFAULT__NEAR_CLIPPING_PLANE_DISTANCE};
    f32 far_clipping_plane_distance{ VIEWPORT_DEFAULT__FAR_CLIPPING_PLANE_DISTANCE};
    bool use_cube_NDC{false}, flip_z{false}, cull_back_faces{true};

    void updateProjection(f32 focal_length, f32 height_over_width) {
        projection.update(focal_length,
                          height_over_width,
                          near_clipping_plane_distance,
                          far_clipping_plane_distance,
                          use_cube_NDC);
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
        f32 NdotA = N | A;
        f32 NdotB = N | B;

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        // Right plane (facing to the left):
        N.x = -N.x;
        NdotA = N | A;
        NdotB = N | B;

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        // Bottom plane (facing up):
        N = {0, focal_length, 1};
        NdotA = N | A;
        NdotB = N | B;

        out = (NdotA < 0) | ((NdotB < 0) << 1);
        if (out) {
            if (out == 3) return false;
            if (out & 1) A = A.lerpTo(B, NdotA / (NdotA - NdotB));
            else         B = B.lerpTo(A, NdotB / (NdotB - NdotA));
        }

        // Top plane (facing down):
        N.y = -N.y;
        NdotA = N | A;
        NdotB = N | B;

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
        Projection Aproj{projection.project(edge.from)};
        Projection Bproj{projection.project(edge.to)};
        vec3 A{Aproj.projected_position / Aproj.w};
        vec3 B{Bproj.projected_position / Bproj.w};

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

struct NavigationTurn {
    bool right{false};
    bool left{false};
};

struct NavigationMove {
    bool right{false};
    bool left{false};
    bool up{false};
    bool down{false};
    bool forward{false};
    bool backward{false};
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

    NavigationMove move{};
    NavigationTurn turn{};

    bool zoomed{false};
    bool moved{false};
    bool turned{false};

    void pan(Camera &camera) {
        camera.pan(settings.speed.pan * -(f32)mouse::pos_raw_diff_x,
                   settings.speed.pan * +(f32)mouse::pos_raw_diff_y);
        moved = true;
        mouse::raw_movement_handled = true;
        mouse::moved = false;
    }

    void zoom(Camera &camera) {
        camera.zoom(settings.speed.zoom * mouse::wheel_scroll_amount);
        zoomed = true;
        mouse::wheel_scroll_handled = true;
    }

    void dolly(Camera &camera) {
        camera.dolly(settings.speed.dolly * mouse::wheel_scroll_amount);
        moved = true;
        mouse::wheel_scroll_handled = true;
    }

    void orient(Camera &camera) {
        camera.rotate(settings.speed.orient * -(f32)mouse::pos_raw_diff_y,
                      settings.speed.orient * -(f32)mouse::pos_raw_diff_x);
        mouse::moved = false;
        mouse::raw_movement_handled = true;
        turned = true;
    }

    void orbit(Camera &camera) {
        camera.orbit(settings.speed.orbit * -(f32)mouse::pos_raw_diff_x,
                     settings.speed.orbit * -(f32)mouse::pos_raw_diff_y);
        turned = true;
        moved = true;
        mouse::raw_movement_handled = true;
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

        // Update the current velocity and position:
        f32 velocity_difference = settings.acceleration * delta_time;
        camera.current_velocity = camera.current_velocity.approachTo(target_velocity, velocity_difference);
        vec3 position_difference = camera.current_velocity * delta_time;
        moved = position_difference.nonZero();
        if (moved) camera.position += camera.rotation * position_difference;
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


struct Pixel {
    f64 depth;
    f32 opacity;
    vec3 color;

    Pixel() : Pixel{vec3{0.0f}} {}
    explicit Pixel(const vec3 &color, f32 opacity = 1.0f, f64 depth = 0.0) : depth(depth), opacity(opacity), color{color} {}
    explicit Pixel(enum ColorID color_id, f32 opacity = 1.0f, f64 depth = 0.0) : Pixel{Color(color_id), opacity, depth} {}
};

union PixelQuad {
    Pixel quad[2][2];
    struct {
        Pixel TL, TR, BL, BR;
    };

    PixelQuad() noexcept : PixelQuad{
            Pixel{vec3{0}},
            Pixel{vec3{0}},
            Pixel{vec3{0}},
            Pixel{vec3{0}}
    } {}

    PixelQuad(const Pixel &top_left, const Pixel &top_right, const Pixel &bottom_left, const Pixel &bottom_right) :
            TL{top_left},
            TR{top_right},
            BL{bottom_left},
            BR{bottom_right}
    {}
};

#define PIXEL_QUAD_SIZE (sizeof(PixelQuad))
#define CANVAS_SIZE (MAX_WINDOW_SIZE * PIXEL_QUAD_SIZE)

struct Canvas {
    Dimensions dimensions;
    PixelQuad *pixels{nullptr};
    Pixel background{Black, 0, INFINITY};
    bool antialias{true};

    explicit Canvas(PixelQuad *pixels) noexcept : pixels{pixels} {}

    INLINE PixelQuad* row(u32 y) const { return pixels + y * (u32)dimensions.width; }
    INLINE PixelQuad* operator[](u32 y) const { return row(y); }

    INLINE void setPixel(i32 x, i32 y, const Pixel &pixel) const { setPixel(x, y, pixel.color, pixel.opacity, pixel.depth); }
    INLINE void setPixel(i32 x, i32 y, const vec3 &color, f32 opacity, f64 depth) const {
        Pixel *pixel;
        PixelQuad *pixel_quad;
        if (antialias) {
            pixel_quad = pixels + (dimensions.stride * (y >> 1)) + (x >> 1);
            pixel = &pixel_quad->quad[y & 1][x & 1];
        } else {
            pixel_quad = pixels + (dimensions.stride * y) + x;
            pixel = &pixel_quad->TL;
        }

        Pixel new_pixel;
        new_pixel.opacity = opacity;
        new_pixel.color = color;
        new_pixel.depth = depth;

        if (!(opacity == 1 && depth == 0)) {
            Pixel background_pixel, foreground_pixel, old_pixel = *pixel;

            if (old_pixel.depth < new_pixel.depth) {
                background_pixel = new_pixel;
                foreground_pixel = old_pixel;
            } else {
                background_pixel = old_pixel;
                foreground_pixel = new_pixel;
            }
            if (foreground_pixel.opacity != 1) {
                f32 one_minus_foreground_opacity = 1.0f - foreground_pixel.opacity;
                opacity = foreground_pixel.opacity + background_pixel.opacity * one_minus_foreground_opacity;
                f32 one_over_opacity = opacity == 0 ? 1.0f : 1.0f / opacity;
                f32 background_factor = background_pixel.opacity * one_over_opacity * one_minus_foreground_opacity;
                f32 foreground_factor = foreground_pixel.opacity * one_over_opacity;

                pixel->color.r = fast_mul_add(foreground_pixel.color.r, foreground_factor, background_pixel.color.r * background_factor);
                pixel->color.g = fast_mul_add(foreground_pixel.color.g, foreground_factor, background_pixel.color.g * background_factor);
                pixel->color.b = fast_mul_add(foreground_pixel.color.b, foreground_factor, background_pixel.color.b * background_factor);
                pixel->opacity = opacity;
                pixel->depth   = foreground_pixel.depth;
            } else *pixel = foreground_pixel;
        } else *pixel = new_pixel;

        if (!antialias) pixel_quad->BR = pixel_quad->BL = pixel_quad->TR = pixel_quad->TL;
    }

    void fill(const Pixel &pixel) const { fill(pixel.color, pixel.opacity, pixel.depth); }
    void fill(const vec3 &color, f32 opacity, f64 depth) const {
        PixelQuad fill_pixel;
        Pixel fill_sub_pixel;
        fill_sub_pixel.color = color;
        fill_sub_pixel.opacity = opacity;
        fill_sub_pixel.depth = depth;
        fill_pixel.TL = fill_pixel.TR = fill_pixel.BL = fill_pixel.BR = fill_sub_pixel;
        for (i32 y = 0; y < dimensions.height; y++)
            for (i32 x = 0; x < dimensions.width; x++)
                pixels[dimensions.stride * y + x] = fill_pixel;
    }

    void clear() const {
        fill(background.color,
             background.opacity,
             background.depth);
    }
};


struct Viewport {
    Canvas &canvas;
    Dimensions dimensions;
    vec2i position{0, 0};
    Navigation navigation;
    Frustum frustum;
    Camera *camera{nullptr};

    Viewport(Canvas &canvas, Camera *camera) : canvas{canvas} {
        dimensions = canvas.dimensions;
        setCamera(*camera);
    }

    void setCamera(Camera &cam) {
        camera = &cam;
        updateProjection();
    }

    void updateDimensions(u16 width, u16 height) {
        dimensions.update(width, height);
        dimensions.stride += (u16)position.x;
        updateProjection();
    }

    void updateProjection() {
        frustum.updateProjection(camera->focal_length, dimensions.height_over_width);
    }

    void updateNavigation(f32 delta_time) {
        navigation.update(*camera, delta_time);
        updateProjection();
    }

    INLINE Ray getRayAt(const vec2i &coords) const {
        vec3 start = (
                camera->rotation.up * (dimensions.h_height - 0.5f) +
                camera->rotation.forward * (dimensions.h_height * camera->focal_length) +
                camera->rotation.right * (0.5f - dimensions.h_width)
        );
        return {
                camera->position,
                camera->rotation.up.scaleAdd(-((f32)coords.y),
                                             camera->rotation.right.scaleAdd((f32)coords.x,start)).normalized()
        };
    }

    INLINE bool cullAndClipEdge(Edge &edge) const {
        return frustum.cullAndClipEdge(edge, camera->focal_length, dimensions.width_over_height);
    }

    INLINE void projectEdge(Edge &edge) const {
        frustum.projectEdge(edge, dimensions);
    }
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
    f32 object_distance{0};
    u32 geo_id{0};
    GeometryType geo_type{GeometryType_None};
    BoxSide box_side{NoSide};
    bool changed{false};

    void manipulate(const Viewport &viewport, const Scene &scene) {
        static Ray ray, local_ray;

        const Dimensions &dimensions = viewport.dimensions;
        Camera &camera = *viewport.camera;
        vec2i mouse_pos{mouse::pos_x - viewport.position.x,
                        mouse::pos_y - viewport.position.y};

        if (mouse::left_button.is_pressed && !mouse::left_button.is_handled) {
            // This is the first frame after the left mouse button went down:
            mouse::left_button.is_handled = true;

            // Cast a ray onto the scene to find the closest object behind the hovered pixel:
            ray = viewport.getRayAt(mouse_pos);

            ray.hit.distance_squared = INFINITY;
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
        if (geo_type) {
            if (controls::is_pressed::alt) {
                bool any_mouse_button_is_pressed = (
                        mouse::left_button.is_pressed ||
                        mouse::middle_button.is_pressed ||
                        mouse::right_button.is_pressed);
                if (geometry && !any_mouse_button_is_pressed) {
                    // Cast a ray onto the bounding box of the currently selected object:
                    ray = viewport.getRayAt(mouse_pos);

                    xform = geometry->transform;
                    if (geometry->type == GeometryType_Mesh)
                        xform.scale *= scene.meshes[geometry->id].aabb.max;

                    xform.internPosAndDir(ray.origin, ray.direction, local_ray.origin, local_ray.direction);

                    box_side = rayHitsCube(local_ray);
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
                            ray = viewport.getRayAt(mouse_pos);
                            if (rayHitsPlane(ray, transformation_plane_origin, transformation_plane_normal)) {
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
                                    quat rotation = quat{v2 ^ v1, v1 | v2 + sqrtf(v1.squaredLength() * v2.squaredLength())};
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
                    f32 x = ((f32)mouse_pos.x + 0.5f) / dimensions.h_width  - 1;
                    f32 y = ((f32)mouse_pos.y + 0.5f) / dimensions.h_height - 1;

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




void drawHLine(i32 x_start, i32 x_end, i32 y, const Viewport &viewport, const vec3 &color, f32 opacity = 1.0f) {
    x_start += viewport.position.x;
    x_end   += viewport.position.x;
    y       += viewport.position.y;

    if (!inRange(y, viewport.dimensions.height + viewport.position.y, viewport.position.y)) return;

    i32 first, last, step = 1;
    subRange(x_start, x_end, viewport.dimensions.width + viewport.position.x, viewport.position.x, &first, &last);

    Pixel pixel{color * color, opacity, 0};
    if (viewport.canvas.antialias) {
        y <<= 1;
        first <<= 1;
        last  <<= 1;
        step = 2;
        for (i32 x = first; x <= last; x += step) {
            viewport.canvas.setPixel(x+0, y+0, pixel);
            viewport.canvas.setPixel(x+1, y+0, pixel);
            viewport.canvas.setPixel(x+0, y+1, pixel);
            viewport.canvas.setPixel(x+1, y+1, pixel);
        }
    } else
        for (i32 x = first; x <= last; x += step)
            viewport.canvas.setPixel(x, y, pixel);
}

void drawVLine(i32 y_start, i32 y_end, i32 x, const Viewport &viewport, const vec3 &color, f32 opacity = 1.0f) {
    y_start += viewport.position.y;
    y_end   += viewport.position.y;
    x       += viewport.position.x;

    if (!inRange(x, viewport.dimensions.width + viewport.position.x, viewport.position.x)) return;
    i32 first, last, step = 1;

    subRange(y_start, y_end, viewport.dimensions.height + viewport.position.y, viewport.position.y, &first, &last);
    Pixel pixel{color * color, opacity, 0};
    if (viewport.canvas.antialias) {
        x <<= 1;
        first <<= 1;
        last  <<= 1;
        step = 2;
        for (i32 y = first; y <= last; y += step) {
            viewport.canvas.setPixel(x+0, y+0, pixel);
            viewport.canvas.setPixel(x+1, y+0, pixel);
            viewport.canvas.setPixel(x+0, y+1, pixel);
            viewport.canvas.setPixel(x+1, y+1, pixel);
        }
    } else
        for (i32 y = first; y <= last; y += step)
            viewport.canvas.setPixel(x, y, pixel);
}

void drawLine(f32 x1, f32 y1, f64 z1,
              f32 x2, f32 y2, f64 z2,
              const Viewport &viewport,
              vec3 color, f32 opacity, u8 line_width) {
    if (x1 < 0 &&
        y1 < 0 &&
        x2 < 0 &&
        y2 < 0)
        return;

    i32 x_left = viewport.position.x;
    i32 y_top  = viewport.position.y;
    x1 += (f32)x_left;
    x2 += (f32)x_left;
    y1 += (f32)y_top;
    y2 += (f32)y_top;

    i32 w = viewport.dimensions.width + x_left;
    i32 h = viewport.dimensions.height + y_top;

    color *= color;
    i32 x, y;
    if (viewport.canvas.antialias) {
        x1 += x1;
        x2 += x2;
        y1 += y1;
        y2 += y2;
        w <<= 1;
        h <<= 1;
        line_width <<= 1;
        line_width++;
    }
    f64 tmp, z_range, range_remap;
    f32 dx = x2 - x1;
    f32 dy = y2 - y1;
    f32 gap, grad, first_offset, last_offset;
    f64 z, z_curr, z_step;
    vec3 first, last;
    vec2i start, end;
    bool has_depth = z1 != 0.0 || z2 != 0.0;
    if (fabsf(dx) > fabsf(dy)) { // Shallow:
        if (x2 < x1) { // Left to right:
            tmp = x2; x2 = x1; x1 = (f32)tmp;
            tmp = y2; y2 = y1; y1 = (f32)tmp;
            tmp = z2; z2 = z1; z1 = tmp;
        }

        grad = dy / dx;

        first.x = roundf(x1);
        last.x  = roundf(x2);
        first_offset = first.x - x1;
        last_offset  = last.x  - x2;

        first.y = y1 + grad * first_offset;
        last.y  = y2 + grad * last_offset;

        start.x = (i32)first.x;
        start.y = (i32)first.y;
        end.x   = (i32)last.x;
        end.y   = (i32)last.y;

        x = start.x;
        y = start.y;
        gap = oneMinusFractionOf(x1 + 0.5f);

        if (inRange(x, w, x_left)) {
            if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, oneMinusFractionOf(first.y) * gap * opacity, z1);

            for (u8 i = 0; i < line_width; i++) {
                y++;
                if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, opacity, z1);
            }

            y++;
            if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, fractionOf(first.y) * gap * opacity, z1);
        }

        x = end.x;
        y = end.y;
        gap = fractionOf(x2 + 0.5f);

        if (inRange(x, w, x_left)) {
            if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, oneMinusFractionOf(last.y) * gap * opacity, z2);

            for (u8 i = 0; i < line_width; i++) {
                y++;
                if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, opacity, z2);
            }

            y++;
            if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, fractionOf(last.y) * gap * opacity, z2);
        }

        if (has_depth) { // Compute one-over-depth start and step
            z1 = 1.0 / z1;
            z2 = 1.0 / z2;
            z_range = z2 - z1;
            range_remap = z_range / (f64)(x2 - x1);
            z1 += range_remap * (f64)(first_offset + 1);
            z2 += range_remap * (f64)(last_offset  - 1);
            z_range = z2 - z1;
            z_step = z_range / (f64)(last.x - first.x - 1);
            z_curr = z1;
        } else z = 0;

        gap = first.y + grad;
        for (x = start.x + 1; x < end.x; x++) {
            if (inRange(x, w, x_left)) {
                if (has_depth) z = 1.0 / z_curr;
                y = (i32) gap;
                if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, oneMinusFractionOf(gap) * opacity, z);

                for (u8 i = 0; i < line_width; i++) {
                    y++;
                    if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, opacity, z);
                }

                y++;
                if (inRange(y, h, y_top)) viewport.canvas.setPixel(x, y, color, fractionOf(gap) * opacity, z);
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

        first.y = roundf(y1);
        last.y  = roundf(y2);

        first_offset = y1 - first.y;
        last_offset  = last.y  - y2;

        first.x = x1 + grad * first_offset;
        last.x  = x2 + grad * last_offset;

        start.y = (i32)first.y;
        start.x = (i32)first.x;

        end.y = (i32)last.y;
        end.x = (i32)last.x;

        x = start.x;
        y = start.y;
        gap = oneMinusFractionOf(y1 + 0.5f);

        if (inRange(y, h, y_top)) {
            if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, oneMinusFractionOf(first.x) * gap * opacity, z1);

            for (u8 i = 0; i < line_width; i++) {
                x++;
                if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, opacity, z1);
            }

            x++;
            if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, fractionOf(first.x) * gap * opacity, z1);
        }

        x = end.x;
        y = end.y;
        gap = fractionOf(y2 + 0.5f);

        if (inRange(y, h, y_top)) {
            if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, oneMinusFractionOf(last.x) * gap * opacity, z2);

            for (u8 i = 0; i < line_width; i++) {
                x++;
                if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, opacity, z2);
            }

            x++;
            if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, fractionOf(last.x) * gap * opacity, z2);
        }

        if (has_depth) { // Compute one-over-depth start and step
            z1 = 1.0 / z1;
            z2 = 1.0 / z2;
            z_range = z2 - z1;
            range_remap = z_range / (f64)(y2 - y1);
            z1 += range_remap * (f64)(first_offset + 1);
            z2 += range_remap * (f64)(last_offset  - 1);
            z_range = z2 - z1;
            z_step = z_range / (f64)(last.y - first.y - 1);
            z_curr = z1;
        } else z = 0;

        gap = first.x + grad;
        for (y = start.y + 1; y < end.y; y++) {
            if (inRange(y, h, y_top)) {
                if (has_depth) z = 1.0 / z_curr;
                x = (i32)gap;

                if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, oneMinusFractionOf(gap) * opacity, z);

                for (u8 i = 0; i < line_width; i++) {
                    x++;
                    if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, opacity, z);
                }

                x++;
                if (inRange(x, w, x_left)) viewport.canvas.setPixel(x, y, color, fractionOf(gap) * opacity, z);
            }

            gap += grad;
            if (has_depth) z_curr += z_step;
        }
    }
}


template<class VectorType = vec2i, typename ValueType = i32>
void draw(const RectOf<VectorType, ValueType> &rect, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f) {
    if (rect.bottom_right.x < 0 || rect.top_left.x >= viewport.dimensions.width ||
        rect.bottom_right.y < 0 || rect.top_left.y >= viewport.dimensions.height)
        return;

    drawHLine(rect.top_left.x, rect.bottom_right.x, rect.top_left.y, viewport, color, opacity);
    drawHLine(rect.top_left.x, rect.bottom_right.x, rect.bottom_right.y, viewport, color, opacity);
    drawVLine(rect.top_left.y, rect.bottom_right.y, rect.top_left.x, viewport, color, opacity);
    drawVLine(rect.top_left.y, rect.bottom_right.y, rect.bottom_right.x, viewport, color, opacity);
}

template<class VectorType = vec2i, typename ValueType = i32>
void fill(const RectOf<VectorType, ValueType> &rect, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f) {
    if (rect.bottom_right.x < 0 || rect.top_left.x >= viewport.dimensions.width ||
        rect.bottom_right.y < 0 || rect.top_left.y >= viewport.dimensions.height)
        return;

    i32 min_x, min_y, max_x, max_y;
    subRange(rect.top_left.x, rect.bottom_right.x, viewport.dimensions.width, 0, &min_x, &max_x);
    subRange(rect.top_left.y, rect.bottom_right.y, viewport.dimensions.height, 0, &min_y, &max_y);
    for (i32 y = min_y; y <= max_y; y++) drawHLine(min_x, max_x, y, viewport, color, opacity);
}


void draw(Edge edge, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1) {
    if (!viewport.cullAndClipEdge(edge)) return;

    viewport.projectEdge(edge);
    drawLine(edge.from.x,
             edge.from.y,
             edge.from.z,
             edge.to.x,
             edge.to.y,
             edge.to.z,
             viewport, color, opacity, line_width);
}


void draw(const Box &box, const Transform &transform, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1, u8 sides = BOX__ALL_SIDES) {
    static Box view_space_box;

    // Transform vertices positions from local-space to world-space and then to view-space:
    for (u8 i = 0; i < BOX__VERTEX_COUNT; i++)
        view_space_box.vertices.buffer[i] = viewport.camera->internPos(transform.externPos(box.vertices.buffer[i]));

    // Distribute transformed vertices positions to edges:
    view_space_box.edges.setFrom(view_space_box.vertices);

    if (sides == BOX__ALL_SIDES) for (const auto &edge : view_space_box.edges.buffer) draw(edge, viewport, color, opacity, line_width);
    else {
        BoxEdgeSides &box_edges = view_space_box.edges.sides;
        if (sides & Front | sides & Top   ) draw(box_edges.front_top,    viewport, color, opacity, line_width);
        if (sides & Front | sides & Bottom) draw(box_edges.front_bottom, viewport, color, opacity, line_width);
        if (sides & Front | sides & Left  ) draw(box_edges.front_left,   viewport, color, opacity, line_width);
        if (sides & Front | sides & Right ) draw(box_edges.front_right,  viewport, color, opacity, line_width);
        if (sides & Back  | sides & Top   ) draw(box_edges.back_top,     viewport, color, opacity, line_width);
        if (sides & Back  | sides & Bottom) draw(box_edges.back_bottom,  viewport, color, opacity, line_width);
        if (sides & Back  | sides & Left  ) draw(box_edges.back_left,    viewport, color, opacity, line_width);
        if (sides & Back  | sides & Right ) draw(box_edges.back_right,   viewport, color, opacity, line_width);
        if (sides & Left  | sides & Top   ) draw(box_edges.left_top,     viewport, color, opacity, line_width);
        if (sides & Left  | sides & Bottom) draw(box_edges.left_bottom,  viewport, color, opacity, line_width);
        if (sides & Right | sides & Top   ) draw(box_edges.right_top,    viewport, color, opacity, line_width);
        if (sides & Right | sides & Bottom) draw(box_edges.right_bottom, viewport, color, opacity, line_width);
    }
}


void draw(const Camera &camera, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1) {
    static Transform transform;
    static Box box;

    transform.rotation = Quat(camera.rotation);
    transform.position = camera.position;
    transform.scale = 1.0f;

    new(&box) Box();
    draw(box, transform, viewport, color, opacity, line_width, BOX__ALL_SIDES);

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

    draw(box, transform, viewport, color, opacity, line_width, BOX__ALL_SIDES);
}

#define CURVE_STEPS 360

void draw(const Curve &curve, const Transform &transform, const Viewport &viewport,
          const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1, u32 step_count = CURVE_STEPS) {
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
            draw(edge, viewport, color, opacity, line_width);
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


void draw(const Grid &grid, const Transform &transform, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1) {
    static Grid view_space_grid;

    // Transform vertices positions from local-space to world-space and then to view-space:
    for (u8 side = 0; side < 2; side++) {
        for (u8 axis = 0; axis < 2; axis++) {
            u8 segment_count = axis ? grid.v_segments : grid.u_segments;
            for (u8 segment = 0; segment < segment_count; segment++)
                view_space_grid.vertices.buffer[axis][side][segment] = (viewport.camera->internPos(transform.externPos(grid.vertices.buffer[axis][side][segment])));
        }
    }

    // Distribute transformed vertices positions to edges:
    view_space_grid.edges.update(view_space_grid.vertices, grid.u_segments, grid.v_segments);

    for (u8 u = 0; u < grid.u_segments; u++) draw(view_space_grid.edges.u.edges[u], viewport, color, opacity, line_width);
    for (u8 v = 0; v < grid.v_segments; v++) draw(view_space_grid.edges.v.edges[v], viewport, color, opacity, line_width);
}

#define LINE_HEIGHT 30
#define FIRST_CHARACTER_CODE 32
#define LAST_CHARACTER_CODE 126

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



void drawText(char *str, i32 x, i32 y, const Viewport &viewport, const vec3 &color, f32 opacity) {
    if (x < 0 || x > viewport.dimensions.width  - FONT_WIDTH ||
        y < 0 || y > viewport.dimensions.height - FONT_HEIGHT)
        return;

    Pixel pixel{color, opacity, 0};
    pixel.color.r *= pixel.color.r;
    pixel.color.g *= pixel.color.g;
    pixel.color.b *= pixel.color.b;

    u16 current_x = (u16)x;
    u16 current_y = (u16)y;
    u16 pixel_x, sub_pixel_x;
    u16 pixel_y, sub_pixel_y;
    u16 t_offset;
    u8* byte;
    char character = *str;
    while (character) {
        if (character == '\n') {
            if (current_y + FONT_HEIGHT > viewport.dimensions.height)
                break;

            current_x = (u16)x;
            current_y += LINE_HEIGHT;
        } else if (character == '\t') {
            t_offset = FONT_WIDTH * (4 - ((current_x / FONT_WIDTH) & 3));
            current_x += t_offset;
        } else if (character >= FIRST_CHARACTER_CODE &&
                   character <= LAST_CHARACTER_CODE) {
            byte = char_addr[character - FIRST_CHARACTER_CODE];
            for (int i = 1; i < 4; i++) {
                pixel_x = current_x;
                pixel_y = current_y + i * FONT_HEIGHT / 3;
                for (int w = 0; w < FONT_WIDTH ; w++) {
                    for (int h = 0; h < FONT_HEIGHT/3; h++) {
                        /* skip background bits */
                        if (*byte & (0x80  >> h)) {
                            if (viewport.canvas.antialias) {
                                sub_pixel_x = pixel_x << 1;
                                sub_pixel_y = pixel_y << 1;
                                viewport.canvas.setPixel(sub_pixel_x + 0, sub_pixel_y + 0, pixel);
                                viewport.canvas.setPixel(sub_pixel_x + 1, sub_pixel_y + 0, pixel);
                                viewport.canvas.setPixel(sub_pixel_x + 0, sub_pixel_y + 1, pixel);
                                viewport.canvas.setPixel(sub_pixel_x + 1, sub_pixel_y + 1, pixel);
                            } else
                                viewport.canvas.setPixel(pixel_x, pixel_y, pixel);
                        }

                        pixel_y--;
                    }
                    byte++;
                    pixel_y += FONT_HEIGHT / 3;
                    pixel_x++;
                }
            }
            current_x += FONT_WIDTH;
            if (current_x + FONT_WIDTH > viewport.dimensions.width)
                return;
        }
        character = *++str;
    }
}

void drawNumber(i32 number, i32 x, i32 y, const Viewport &viewport, const vec3 &color, f32 opacity) {
    static NumberString number_string;
    number_string = number;
    drawText(number_string.string.char_ptr, x - (i32)number_string.string.length * FONT_WIDTH, y, viewport, color, opacity);
}


void draw(const HUD &hud, const Viewport &viewport) {
    u16 x = (u16)hud.position.x;
    u16 y = (u16)hud.position.y;

    HUDLine *line = hud.lines;
    bool alt;
    for (u32 i = 0; i < hud.settings.line_count; i++, line++) {
        if (line->use_alternate) {
            alt = *line->use_alternate;
            if (line->invert_alternate_use)
                alt = !alt;
        } else
            alt = false;

        char *text = alt ? line->alternate_value.char_ptr : line->value.string.char_ptr;
        vec3 color{Color(alt ? line->alternate_value_color : line->value_color)};
        drawText(line->title.char_ptr, x, y, viewport, Color(line->title_color), 1);
        drawText(text, x + (u16)line->title.length * FONT_WIDTH, y, viewport, color, 1);
        y += (u16)(hud.settings.line_height * (f32)FONT_HEIGHT);
    }
}


void draw(const Mesh &mesh, const Transform &transform, bool draw_normals, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1) {
    const Camera &cam = *viewport.camera;
    vec3 pos;
    Edge edge;
    EdgeVertexIndices *edge_index = mesh.edge_vertex_indices;
    for (u32 i = 0; i < mesh.edge_count; i++, edge_index++) {
        edge.from = cam.internPos(transform.externPos(mesh.vertex_positions[edge_index->from]));
        edge.to   = cam.internPos(transform.externPos(mesh.vertex_positions[edge_index->to]));
        draw(edge, viewport, color, opacity, line_width);
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
                draw(edge, viewport, Color(Red), opacity * 0.5f, line_width);
            }
        }
    }
}


void draw(Selection &selection, const Viewport &viewport, const Scene &scene) {
    static Box box;

    if (controls::is_pressed::alt && !mouse::is_captured && selection.geo_type && selection.geometry) {
        selection.xform = selection.geometry->transform;
        if (selection.geometry->type == GeometryType_Mesh)
            selection.xform.scale *= scene.meshes[selection.geometry->id].aabb.max;

        draw(box, selection.xform, viewport, Color(Yellow), 0.5f, 0);
        if (selection.box_side) {
            vec3 color = Color(White);
            switch (selection.box_side) {
                case Left:  case Right:  color = Color(Red);   break;
                case Top:   case Bottom: color = Color(Green); break;
                case Front: case Back:   color = Color(Blue);  break;
                case NoSide: break;
            }

            draw(box, selection.xform, viewport, color, 0.5f, 1, selection.box_side);
        }
    }
}



namespace window {
    u16 width{DEFAULT_WIDTH};
    u16 height{DEFAULT_HEIGHT};
    char* title{(char*)""};
    u32 *content{nullptr};
    Canvas canvas{nullptr};

    union RGBA2u32 {
        u32 value;
        RGBA rgba;

        RGBA2u32() : value{0} {}
    };

    void renderCanvasToContent() {
        PixelQuad *src_pixel = canvas.pixels;
        u32 *trg_value = content;
        vec3 color;
        RGBA2u32 background_pixel, trg_pixel;
        background_pixel.rgba.R = (u8)(canvas.background.color.r * canvas.background.color.r * FLOAT_TO_COLOR_COMPONENT);
        background_pixel.rgba.G = (u8)(canvas.background.color.g * canvas.background.color.g * FLOAT_TO_COLOR_COMPONENT);
        background_pixel.rgba.B = (u8)(canvas.background.color.b * canvas.background.color.b * FLOAT_TO_COLOR_COMPONENT);
        background_pixel.rgba.A = (u8)(canvas.background.opacity * canvas.background.opacity * FLOAT_TO_COLOR_COMPONENT);
        if (canvas.antialias) {
            for (u16 y = 0; y < canvas.dimensions.height; y++) {
                for (u16 x = 0; x < canvas.dimensions.width; x++, src_pixel++, trg_value++) {
                    if (src_pixel->TL.opacity != 0.0f ||
                        src_pixel->TR.opacity != 0.0f ||
                        src_pixel->BL.opacity != 0.0f ||
                        src_pixel->BR.opacity != 0.0f) {
                        color = src_pixel->TL.color * (src_pixel->TL.opacity * 0.25f);
                        color = src_pixel->TR.color.scaleAdd(src_pixel->TR.opacity * 0.25f, color);
                        color = src_pixel->BL.color.scaleAdd(src_pixel->BL.opacity * 0.25f, color);
                        color = src_pixel->BR.color.scaleAdd(src_pixel->BR.opacity * 0.25f, color);
                        trg_pixel.rgba.R = (u8)(color.r > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.r)));
                        trg_pixel.rgba.G = (u8)(color.g > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.g)));
                        trg_pixel.rgba.B = (u8)(color.b > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.b)));
                        trg_pixel.rgba.A = (u8)(clampedValue(src_pixel->TL.opacity) * FLOAT_TO_COLOR_COMPONENT);
                    } else trg_pixel = background_pixel;
                    *trg_value = trg_pixel.value;
                }
            }
        } else {
            for (u16 y = 0; y < canvas.dimensions.height; y++) {
                for (u16 x = 0; x < canvas.dimensions.width; x++, src_pixel++, trg_value++) {
                    if (src_pixel->TL.depth == INFINITY)
                        trg_pixel = background_pixel;
                    else {
                        color = src_pixel->TL.color * src_pixel->TL.opacity;
                        trg_pixel.rgba.R = (u8)(color.r > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.r)));
                        trg_pixel.rgba.G = (u8)(color.g > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.g)));
                        trg_pixel.rgba.B = (u8)(color.b > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.b)));
                        trg_pixel.rgba.A = (u8)(clampedValue(src_pixel->TL.opacity) * FLOAT_TO_COLOR_COMPONENT);
                    }
                    *trg_value = trg_pixel.value;
                }
            }
        }
    }
}

struct SlimEngine {
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

        window::canvas.clear();
        render_timer.beginFrame();
        OnRender();
        render_timer.endFrame();
        window::renderCanvasToContent();
        mouse::resetChanges();
    };

    void resize(u16 width, u16 height) {
        window::width = width;
        window::height = height;
        window::canvas.dimensions.update(width, height);

        OnWindowResize(width, height);
        OnWindowRedraw();
    }
};

SlimEngine* createEngine();

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

    _tprintf(TEXT((LPTSTR)"ERROR: %s\n"), (LPCTSTR)lpDisplayBuf);

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
    return getRawInput(0) == 0 && raw_input_size != 0;
}
inline bool hasRawMouseInput(LPARAM lParam) {
    raw_input_handle = (HRAWINPUT)(lParam);
    return (
            hasRawInput() &&
            getRawInput((LPVOID)&raw_inputs) == raw_input_size &&
            raw_inputs.header.dwType == RIM_TYPEMOUSE
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
    HANDLE handle = CreateFile(path,           // file to open
                               GENERIC_READ,          // open for reading
                               FILE_SHARE_READ,       // share for reading
                               nullptr,                  // default security
                               OPEN_EXISTING,         // existing file only
                               FILE_ATTRIBUTE_NORMAL, // normal file
                               nullptr);                 // no attr. template
#ifndef NDEBUG
    if (handle == INVALID_HANDLE_VALUE) {
        DisplayError(TEXT((LPTSTR)"CreateFile"));
        _tprintf(TEXT("Terminal failure: unable to open file \"%s\" for read.\n"), path);
        return nullptr;
    }
#endif
    return handle;
}

void* os::openFileForWriting(const char* path) {
    HANDLE handle = CreateFile(path,           // file to open
                               GENERIC_WRITE,          // open for writing
                               0,                      // do not share
                               nullptr,                   // default security
                               OPEN_ALWAYS,            // create new or open existing
                               FILE_ATTRIBUTE_NORMAL,  // normal file
                               nullptr);
#ifndef NDEBUG
    if (handle == INVALID_HANDLE_VALUE) {
        DisplayError(TEXT((LPTSTR)"CreateFile"));
        _tprintf(TEXT("Terminal failure: unable to open file \"%s\" for write.\n"), path);
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
        DisplayError(TEXT((LPTSTR)"ReadFile"));
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
        DisplayError(TEXT((LPTSTR)"WriteFile"));
        printf("Terminal failure: Unable to write from file.\n GetLastError=%08x\n", (unsigned int)GetLastError());
        CloseHandle(handle);
    }
#endif
    return result != FALSE;
}

SlimEngine *CURRENT_ENGINE;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    bool pressed = message == WM_SYSKEYDOWN || message == WM_KEYDOWN;
    u8 key = (u8)wParam;
    i32 x, y;
    f32 scroll_amount;

    switch (message) {
        case WM_DESTROY:
            CURRENT_ENGINE->is_running = false;
            PostQuitMessage(0);
            break;

        case WM_SIZE:
            GetClientRect(window_handle, &win_rect);

            info.bmiHeader.biWidth = win_rect.right - win_rect.left;
            info.bmiHeader.biHeight = win_rect.top - win_rect.bottom;
            CURRENT_ENGINE->resize((u16)info.bmiHeader.biWidth, (u16)-info.bmiHeader.biHeight);

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
                default: break;
            }
            CURRENT_ENGINE->OnKeyChanged(key, pressed);

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
                    CURRENT_ENGINE->OnMouseButtonDoubleClicked(*mouse_button);
                    break;
                case WM_MBUTTONUP:
                case WM_RBUTTONUP:
                case WM_LBUTTONUP:
                    mouse_button->up(x, y);
                    CURRENT_ENGINE->OnMouseButtonUp(*mouse_button);
                    break;
                default:
                    mouse_button->down(x, y);
                    CURRENT_ENGINE->OnMouseButtonDown(*mouse_button);
            }

            break;

        case WM_MOUSEWHEEL:
            scroll_amount = (f32)(GET_WHEEL_DELTA_WPARAM(wParam)) / (f32)(WHEEL_DELTA);
            mouse::scroll(scroll_amount); CURRENT_ENGINE->OnMouseWheelScrolled(scroll_amount);
            break;

        case WM_MOUSEMOVE:
            x = GET_X_LPARAM(lParam);
            y = GET_Y_LPARAM(lParam);
            mouse::move(x, y);        CURRENT_ENGINE->OnMouseMovementSet(x, y);
            mouse::setPosition(x, y); CURRENT_ENGINE->OnMousePositionSet(x, y);
            break;

        case WM_INPUT:
            if ((hasRawMouseInput(lParam)) && (
                    raw_inputs.data.mouse.lLastX != 0 ||
                    raw_inputs.data.mouse.lLastY != 0)) {
                x = raw_inputs.data.mouse.lLastX;
                y = raw_inputs.data.mouse.lLastY;
                mouse::moveRaw(x, y); CURRENT_ENGINE->OnMouseRawMovementSet(x, y);
            }

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow) {
    void* window_content_and_canvas_memory = GlobalAlloc(GPTR, WINDOW_CONTENT_SIZE + CANVAS_SIZE);
    if (!window_content_and_canvas_memory)
        return -1;

    window::content = (u32*)window_content_and_canvas_memory;
    window::canvas.pixels = (PixelQuad*)((u8*)window_content_and_canvas_memory + WINDOW_CONTENT_SIZE);

    controls::key_map::ctrl = VK_CONTROL;
    controls::key_map::alt = VK_MENU;
    controls::key_map::shift = VK_SHIFT;
    controls::key_map::space = VK_SPACE;
    controls::key_map::tab = VK_TAB;
    controls::key_map::escape = VK_ESCAPE;

    LARGE_INTEGER performance_frequency;
    QueryPerformanceFrequency(&performance_frequency);

    time::ticks_per_second = (u64)performance_frequency.QuadPart;
    time::seconds_per_tick = 1.0 / (f64)(time::ticks_per_second);
    time::milliseconds_per_tick = 1000.0 * time::seconds_per_tick;
    time::microseconds_per_tick = 1000.0 * time::milliseconds_per_tick;
    time::nanoseconds_per_tick  = 1000.0 * time::microseconds_per_tick;

    CURRENT_ENGINE = createEngine();
    if (!CURRENT_ENGINE->is_running)
        return -1;

    info.bmiHeader.biSize        = sizeof(info.bmiHeader);
    info.bmiHeader.biCompression = BI_RGB;
    info.bmiHeader.biBitCount    = 32;
    info.bmiHeader.biPlanes      = 1;

    window_class.lpszClassName  = "RnDer";
    window_class.hInstance      = hInstance;
    window_class.lpfnWndProc    = WndProc;
    window_class.style          = CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    window_class.hCursor        = LoadCursorA(nullptr, IDC_ARROW);

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
    while (CURRENT_ENGINE->is_running) {
        while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        CURRENT_ENGINE->OnWindowRedraw();
        InvalidateRgn(window_handle, nullptr, false);
    }

    return 0;
}

#endif