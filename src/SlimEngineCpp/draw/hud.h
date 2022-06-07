#pragma once

#include "./text.h"
#include "../core/hud.h"

void draw(const HUD &hud, const RectI &viewport_bounds, const Canvas &canvas) {
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
        drawText(line->title.char_ptr, x, y, viewport_bounds, canvas, line->title_color);
        drawText(text, x + (i32)line->title.length * FONT_WIDTH, y, viewport_bounds, canvas, color);
        y += (i32)(hud.settings.line_height * (f32)FONT_HEIGHT);
    }
}
