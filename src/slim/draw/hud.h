#pragma once

#include "./text.h"
#include "../core/hud.h"

void drawHUD(const HUD &hud, const Canvas &canvas, const RectI *viewport_bounds = nullptr) {
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