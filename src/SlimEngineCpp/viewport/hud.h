#pragma once

#include "../core/types.h"

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

//    HUDLine(char* title_char_ptr, char* title_char_ptr, enum ColorID default_color = White) :
//            title_color{default_color},
//            value_color{default_color},
//            alternate_value_color{default_color}
//    {}
};

struct HUDSettings {
    u32 line_count{0};
    f32 line_height{1.0f};
    enum ColorID default_color{White};
    bool show{false};

    HUDSettings(u32 line_count = 0, f32 line_height = 1.0f, ColorID default_color = White, bool show = false) :
            line_count{line_count}, line_height{line_height}, default_color{default_color}, show{show} {}
};

struct HUD {
    HUDSettings settings;
    HUDLine *lines{nullptr};
    vec2i position{10, 10};

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
