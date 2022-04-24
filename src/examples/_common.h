#pragma once

#ifndef SLIM_ENGINE_SHF
#include "../SlimEngineCpp/draw/text.h"
#include "../SlimEngineCpp/draw/rectangle.h"
#endif
// Or using the single-header file:
// #include "../SlimEngineCpp.h"

#define MOUSE_WHEEL__MAX 2000

void drawKeyboard(Viewport &viewport) {
    RectI rect;
    i32 right  = viewport.dimensions.width;
    i32 bottom = viewport.dimensions.height;
    NavigationMove move = viewport.navigation.move;
    Canvas &canvas = viewport.canvas;

    // Draw the keyboard contour:
    rect.right = right - 2;
    rect.left = rect.right - 230;
    rect.bottom = bottom - 2;
    rect.top = rect.bottom - 100;
    draw(rect, viewport, Color(White), 1);

    // Draw the 'A' key:
    rect.right = right - 172;
    rect.left = rect.right - 20;
    rect.bottom = bottom - 42;
    rect.top = rect.bottom - 20;
    fill(rect, viewport, Color(move.left ? White : Grey), 1);
    drawText((char*)"A", rect.left + 2, rect.top - 1, viewport, Color(move.left ? Grey : White), 1);

    // Draw the 'S' key:
    rect.left += 22;
    rect.right += 22;
    fill(rect, viewport, Color(move.backward ? White : Grey), 1);
    drawText((char*)"S", rect.left + 2, rect.top - 1, viewport, Color(move.backward ? Grey : White), 1);

    // Draw the 'D' key:
    rect.left += 22;
    rect.right += 22;
    fill(rect, viewport, Color(move.right ? White : Grey), 1);
    drawText((char*)"D", rect.left + 2, rect.top - 1, viewport, Color(move.right ? Grey : White), 1);

    // Draw the 'D' key:
    rect.left += 22;
    rect.right += 22;
    fill(rect, viewport, Color(move.down ? White : Grey), 1);
    drawText((char*)"F", rect.left + 2, rect.top - 1, viewport, Color(move.down ? Grey : White), 1);

    // Draw the 'Q' key:
    rect.left -= 28 * 3;
    rect.right -= 28 * 3;
    rect.top -= 22;
    rect.bottom -= 22;

//    fillRect(viewport, Color(turn.left ? White : Grey), &rect);
//    drawText(viewport, viewport, Color(turn.left ? Grey : White), "Q", rect.left + 5, rect.top + 5);

    // Draw the 'W' key:
    rect.left += 22;
    rect.right += 22;
    fill(rect, viewport, Color(move.forward ? White : Grey), 1);
    drawText((char*)"W", rect.left + 2, rect.top - 1, viewport, Color(move.forward ? Grey : White), 1);

    // Draw the 'E' key:
    rect.left += 22;
    rect.right += 22;
//    fillRect(viewport, Color(turn.right ? White : Grey), &rect);
//    drawText(viewport, viewport, Color(turn.right ? Grey : White), "E", rect.left + 5, rect.top + 5);

    // Draw the 'R' key:
    rect.left += 22;
    rect.right += 22;
    fill(rect, viewport, Color(move.up ? White : Grey), 1);
    drawText((char*)"R", rect.left + 2, rect.top - 1, viewport, Color(move.up ? Grey : White), 1);


    // Draw the left Ctrl key:
    rect.right = right - 202;
    rect.left = rect.right - 28;
    rect.bottom = bottom - 4;
    rect.top = rect.bottom - 18;
    fill(rect, viewport, Color(controls::is_pressed::ctrl ? Blue : Cyan), 1);

    // Draw the left Alt key:
    rect.left += 30;
    rect.right += 30;
    fill(rect, viewport, Color(controls::is_pressed::alt ? Red : Magenta), 1);

    // Draw the left Shift key:
    rect.left -= 30;
    rect.right -= 15;
    rect.top -= 20;
    rect.bottom -= 20;
    fill(rect, viewport, Color(controls::is_pressed::shift ? Green : Yellow), 1);

    // Draw the right Ctrl key:
    rect.right = right - 4;
    rect.left = rect.right - 28;
    rect.bottom = bottom - 4;
    rect.top = rect.bottom - 18;
    fill(rect, viewport, Color(controls::is_pressed::ctrl ? Blue : Cyan), 1);

    // Draw the right Alt key:
    rect.left -= 30;
    rect.right -= 30;
    fill(rect, viewport, Color(controls::is_pressed::alt ? Red : Magenta), 1);

    // Draw the right Shift key:
    rect.left += 15;
    rect.right += 30;
    rect.top -= 20;
    rect.bottom -= 20;
    fill(rect, viewport, Color(controls::is_pressed::shift ? Green : Yellow), 1);

    // Draw the Space key:
    rect.right = right - 62;
    rect.left = rect.right - 110;
    rect.bottom = bottom - 4;
    rect.top = rect.bottom - 18;
    fill(rect, viewport, Color(controls::is_pressed::space ? White : Grey), 1);
}

void drawMouse(Viewport &viewport) {
    RectI rect;
    Canvas &canvas = viewport.canvas;
    i32 bottom = viewport.dimensions.height;

    // Draw the mouse contour:
    rect.left = 2;
    rect.right = 62;
    rect.bottom = bottom - 1;
    rect.top = rect.bottom - 111;
    draw(rect, viewport, Color(White), 1);

    // Draw the left mouse button:
    rect.left = 3;
    rect.right = 24;
    rect.bottom = bottom - 70;
    rect.top = rect.bottom - 40;
    fill(rect, viewport, Color(mouse::left_button.is_pressed ? Blue : Cyan), 1);

    // Draw the middle mouse button:
    rect.left += 20;
    rect.right += 20;
    fill(rect, viewport, Color(mouse::middle_button.is_pressed ? Green : Yellow), 1);

    // Draw the right mouse button:
    rect.left += 20;
    rect.right += 20;
    fill(rect, viewport, Color(mouse::right_button.is_pressed ? Red : Magenta), 1);

    // Draw the mouse wheel:
    rect.left = 28;
    rect.right = 38;
    rect.bottom = bottom - 73;
    rect.top = rect.bottom - 33;
    fill(rect, viewport, Color(Grey), 1);

    // Draw a marker representing the state of the mouse wheel:
    rect.left += 1;
    rect.right -= 1;
    rect.top += 14;
    rect.bottom -= 14;

    static f32 accumulated_mouse_wheel_scroll_amount = 0;
    static f32 mouse_wheel_delta_y = 0;
    if (mouse::wheel_scrolled) {
        accumulated_mouse_wheel_scroll_amount += mouse::wheel_scroll_amount;
        if (accumulated_mouse_wheel_scroll_amount > MOUSE_WHEEL__MAX)
            accumulated_mouse_wheel_scroll_amount = -MOUSE_WHEEL__MAX;
        if (accumulated_mouse_wheel_scroll_amount < -MOUSE_WHEEL__MAX)
            accumulated_mouse_wheel_scroll_amount = MOUSE_WHEEL__MAX;

        mouse_wheel_delta_y = accumulated_mouse_wheel_scroll_amount;
        mouse_wheel_delta_y += MOUSE_WHEEL__MAX;
        mouse_wheel_delta_y /= MOUSE_WHEEL__MAX;
        mouse_wheel_delta_y -= 1;
        mouse_wheel_delta_y *= 38;
        mouse_wheel_delta_y /= -2;

        mouse::wheel_scroll_handled = true;
    }
    rect.top += (i32)mouse_wheel_delta_y;
    rect.bottom += (i32)mouse_wheel_delta_y;
    fill(rect, viewport, Color(White), 1);
}

void drawMouseAndKeyboard(Viewport &viewport) {
    drawMouse(viewport);
    drawKeyboard(viewport);
}