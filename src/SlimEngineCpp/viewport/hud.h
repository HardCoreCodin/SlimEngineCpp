#pragma once

#include "../core/string.h"
#include "../math/vec2.h"

struct HUDLine {
    String title{}, alternate_value{};
    NumberString value{};
    enum ColorID title_color{White};
    enum ColorID value_color{White};
    enum ColorID alternate_value_color{White};
    bool *use_alternate{nullptr};
    bool invert_alternate_use{false};

    HUDLine(enum ColorID default_color = White) : title_color{default_color}, value_color{default_color}, alternate_value_color{default_color} {}
    HUDLine(char* title_char_ptr,
            enum ColorID default_color = White) : title_color{default_color}, value_color{default_color}, alternate_value_color{default_color}, title{title_char_ptr}{}
};

struct HUDSettings {
    u32 line_count{0};
    f32 line_height{1.0f};
    enum ColorID default_color{White};

    HUDSettings(u32 line_count = 0,
                f32 line_height = 1.0f,
                ColorID default_color = White) : line_count{line_count}, line_height{line_height}, default_color{default_color} {}
};
struct HUD {
    HUDSettings settings;
    HUDLine *lines{nullptr};
    vec2i position{10, 10};
    bool enabled{true};

    HUD() = default;
    HUD(HUDSettings settings,
        HUDLine *lines,
        vec2i position = {10, 10}) :
        settings{settings}, lines{lines}, position{position} {
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
