#pragma once

#include "../core/types.h"

struct HUDLine {
    enum ColorID title_color{White}, value_color{White}, alternate_value_color{White};
    bool invert_alternate_use{false}, *use_alternate{nullptr};
    String title{}, alternate_value{};
    NumberString value{};

    HUDLine(enum ColorID default_color = White) :
    title_color{default_color},
    value_color{default_color},
    alternate_value_color{default_color}
    {}
};

struct HUDSettings {
    enum ColorID default_color{White};
    u32 line_count{0};
    f32 line_height{1};
    bool show{false};
};

struct HUD {
    HUDSettings settings;
    vec2i position{0, 0};
    HUDLine *lines{nullptr};

    HUD() : settings{
        settings::hud::default_color,
        settings::hud::line_count,
        settings::hud::line_height,
        settings::hud::show
    } {
        if (settings.line_count) {
            lines = (HUDLine*)memory::allocate(settings.line_count * sizeof(HUDLine));
            for (u32 i = 0; i < settings.line_count; i++)
                new(lines + i) HUDLine{settings.default_color};
        }
    }

    void draw(const Canvas &canvas) const {
        u16 x = (u16)position.x;
        u16 y = (u16)position.y;

        HUDLine *line = lines;
        bool alt;
        for (u32 i = 0; i < settings.line_count; i++, line++) {
            if (line->use_alternate) {
                alt = *line->use_alternate;
                if (line->invert_alternate_use)
                    alt = !alt;
            } else
                alt = false;

            char *text = alt ? line->alternate_value.char_ptr : line->value.string.char_ptr;
            vec3 color{Color(alt ? line->alternate_value_color : line->value_color)};
            canvas.drawText(line->title.char_ptr, x, y, Color(line->title_color), 1);
            canvas.drawText(text, x + (u16)line->title.length * FONT_WIDTH, y, color, 1);
            y += (u16)(settings.line_height * (f32)FONT_HEIGHT);
        }
    }
};
