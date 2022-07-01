#pragma once

#ifndef SLIM_SINGLE_HEADER_FILE
#include "../slim/draw/text.h"
#include "../slim/draw/rectangle.h"
#endif
// Or using the single-header file:
//#include "../slim.h"

#define MOUSE_WHEEL__MAX 2000

void drawKeyboard(Viewport &viewport) {
    RectI rect;
    i32 right  = viewport.dimensions.width;
    i32 bottom = viewport.dimensions.height;
    Move move = viewport.navigation.move;
    Canvas &canvas = viewport.canvas;

    // Draw the keyboard contour:
    rect.right = right - 2;
    rect.left = rect.right - 228;
    rect.bottom = bottom - 2;
    rect.top = rect.bottom - 98;
    canvas.drawRect(rect, White);

    // Draw the 'A' key:
    rect.right = right - 170;
    rect.left = rect.right - 18;
    rect.bottom = bottom - 46;
    rect.top = rect.bottom - 18;
    canvas.fillRect(rect, move.left ? White : Grey);
    canvas.drawText((char*)"A", rect.left + 6, rect.top + 4, move.left ? Grey : White);

    // Draw the 'S' key:
    rect.left += 22;
    rect.right += 22;
    canvas.fillRect(rect, move.backward ? White : Grey);
    canvas.drawText((char*)"S", rect.left + 6, rect.top + 4, move.backward ? Grey : White);

    // Draw the 'D' key:
    rect.left += 22;
    rect.right += 22;
    canvas.fillRect(rect, move.right ? White : Grey);
    canvas.drawText((char*)"D", rect.left + 6, rect.top + 4, move.right ? Grey : White);

    // Draw the 'D' key:
    rect.left += 22;
    rect.right += 22;
    canvas.fillRect(rect, move.down ? White : Grey);
    canvas.drawText((char*)"F", rect.left + 6, rect.top + 4, move.down ? Grey : White);

    // Draw the 'Q' key:
    rect.left -= 28 * 3;
    rect.right -= 28 * 3;
    rect.top -= 22;
    rect.bottom -= 22;

    // Draw the 'W' key:
    rect.left += 22;
    rect.right += 22;
    canvas.fillRect(rect, move.forward ? White : Grey);
    canvas.drawText((char*)"W", rect.left + 6, rect.top + 4, move.forward ? Grey : White);

    // Draw the 'E' key:
    rect.left += 22;
    rect.right += 22;

    // Draw the 'R' key:
    rect.left += 22;
    rect.right += 22;
    canvas.fillRect(rect, move.up ? White : Grey);
    canvas.drawText((char*)"R", rect.left + 6, rect.top + 4, move.up ? Grey : White);


    // Draw the left Ctrl key:
    rect.right = right - 200;
    rect.left = rect.right - 26;
    rect.bottom = bottom - 6;
    rect.top = rect.bottom - 16;
    canvas.fillRect(rect, controls::is_pressed::ctrl ? Blue : Cyan);

    // Draw the left Alt key:
    rect.left += 30;
    rect.right += 30;
    canvas.fillRect(rect, controls::is_pressed::alt ? Red : Magenta);

    // Draw the left Shift key:
    rect.left -= 30;
    rect.right -= 15;
    rect.top -= 20;
    rect.bottom -= 20;
    canvas.fillRect(rect, controls::is_pressed::shift ? Green : Yellow);

    // Draw the right Ctrl key:
    rect.right = right - 4;
    rect.left = rect.right - 26;
    rect.bottom = bottom - 6;
    rect.top = rect.bottom - 16;
    canvas.fillRect(rect, controls::is_pressed::ctrl ? Blue : Cyan);

    // Draw the right Alt key:
    rect.left -= 30;
    rect.right -= 30;
    canvas.fillRect(rect, controls::is_pressed::alt ? Red : Magenta);

    // Draw the right Shift key:
    rect.left += 15;
    rect.right += 30;
    rect.top -= 20;
    rect.bottom -= 20;
    canvas.fillRect(rect, controls::is_pressed::shift ? Green : Yellow);

    // Draw the Space key:
    rect.right = right - 64;
    rect.left = rect.right - 102;
    rect.bottom = bottom - 6;
    rect.top = rect.bottom - 16;
    canvas.fillRect(rect, controls::is_pressed::space ? White : Grey);
}

void drawMouse(Viewport &viewport) {
    RectI rect;
    Canvas &canvas = viewport.canvas;
    i32 bottom = viewport.dimensions.height;

    // Draw the mouse contour:
    rect.left = 2;
    rect.right = 60;
    rect.bottom = bottom - 1;
    rect.top = rect.bottom - 109;
    canvas.drawRect(rect, White);

    // Draw the left mouse button:
    rect.left = 3;
    rect.right = 20;
    rect.bottom = bottom - 70;
    rect.top = rect.bottom - 38;
    canvas.fillRect(rect, mouse::left_button.is_pressed ? Blue : Cyan);

    // Draw the middle mouse button:
    rect.left += 20;
    rect.right += 20;
    canvas.fillRect(rect, mouse::middle_button.is_pressed ? Green : Yellow);

    // Draw the right mouse button:
    rect.left += 20;
    rect.right += 20;
    canvas.fillRect(rect, mouse::right_button.is_pressed ? Red : Magenta);

    // Draw the mouse wheel:
    rect.left = 28;
    rect.right = 36;
    rect.bottom = bottom - 73;
    rect.top = rect.bottom - 31;
    canvas.fillRect(rect, Grey);

    // Draw a marker representing the state of the mouse wheel:
    rect.left += 1;
    rect.right -= 1;
    rect.top += 14;
    rect.bottom -= 12;

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
    }
    rect.top += (i32)mouse_wheel_delta_y;
    rect.bottom += (i32)mouse_wheel_delta_y;
    canvas.fillRect(rect, White);
}

void drawMouseAndKeyboard(Viewport &viewport) {
    drawMouse(viewport);
    drawKeyboard(viewport);
}