#pragma once

#include "../core/string.h"

struct HUDLine {
    String title, alternate_value;
    NumberString value;
    enum ColorID title_color = BrightGrey;
    enum ColorID value_color = BrightGrey;
    enum ColorID alternate_value_color = Grey;
    bool *use_alternate = nullptr;
    bool invert_alternate_use = false;

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
    u32 line_count = 0;
    f32 line_height = 1.2f;
    enum ColorID default_color = BrightGrey;

    HUDSettings(u32 line_count = 0,
                f32 line_height = 1.2f,
                ColorID default_color = BrightGrey) : line_count{line_count}, line_height{line_height}, default_color{default_color} {}
};
struct HUD {
    HUDSettings settings;
    HUDLine *lines = nullptr;
    i32 left = 10, top = 10;
    bool enabled = true;

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
