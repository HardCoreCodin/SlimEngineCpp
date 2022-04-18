#pragma once

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

//// Culling flags:
//// ======================
//#define IS_NEAR  0b00000001
//#define IS_FAR   0b00000010
//#define IS_BELOW 0b00000100
//#define IS_ABOVE 0b00001000
//#define IS_RIGHT 0b00010000
//#define IS_LEFT  0b00100000
//#define IS_OUT   0b00111111
//#define IS_NDC   0b01000000
//
//// Clipping flags:
//// ===============
//#define CULL    0b00000000
//#define INSIDE  0b00000010
//
//#define LAMBERT 1
//#define PHONG 2
//#define BLINN 4

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
    }

    namespace is_pressed {
        bool ctrl{false};
        bool alt{false};
        bool shift{false};
        bool space{false};
        bool tab{false};
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

    Timer update_timer, render_timer;
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