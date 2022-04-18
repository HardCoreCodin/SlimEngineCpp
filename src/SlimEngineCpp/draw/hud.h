#pragma once

#include "./text.h"
#include "../viewport/hud.h"
#include "../viewport/viewport.h"

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
