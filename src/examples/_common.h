#pragma once

#ifndef SLIM_ENGINE_SHF
#include "../SlimEngineCpp/draw/text.h"
#include "../SlimEngineCpp/draw/rectangle.h"
#endif
// Or using the single-header file:
// #include "../SlimEngine.h"

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
    draw(rect, viewport.position, viewport.canvas, White);

    // Draw the 'A' key:
    rect.right = right - 172;
    rect.left = rect.right - 20;
    rect.bottom = bottom - 42;
    rect.top = rect.bottom - 20;
    fill(rect, viewport.position, viewport.canvas,  move.left ? White : Grey);
    drawText((char*)"A", rect.left + 2, rect.top - 1, viewport.canvas, move.left ? Grey : White);

    // Draw the 'S' key:
    rect.left += 22;
    rect.right += 22;
    fill(rect, viewport.position, viewport.canvas,  move.backward ? White : Grey);
    drawText((char*)"S", rect.left + 2, rect.top - 1, viewport.canvas, move.backward ? Grey : White);

    // Draw the 'D' key:
    rect.left += 22;
    rect.right += 22;
    fill(rect, viewport.position, viewport.canvas,  move.right ? White : Grey);
    drawText((char*)"D", rect.left + 2, rect.top - 1, viewport.canvas, move.right ? Grey : White);

    // Draw the 'D' key:
    rect.left += 22;
    rect.right += 22;
    fill(rect, viewport.position, viewport.canvas,  move.down ? White : Grey);
    drawText((char*)"F", rect.left + 2, rect.top - 1, viewport.canvas, move.down ? Grey : White);

    // Draw the 'Q' key:
    rect.left -= 28 * 3;
    rect.right -= 28 * 3;
    rect.top -= 22;
    rect.bottom -= 22;

    // Draw the 'W' key:
    rect.left += 22;
    rect.right += 22;
    fill(rect, viewport.position, viewport.canvas,  move.forward ? White : Grey);
    drawText((char*)"W", rect.left + 2, rect.top - 1, viewport.canvas, move.forward ? Grey : White);

    // Draw the 'E' key:
    rect.left += 22;
    rect.right += 22;

    // Draw the 'R' key:
    rect.left += 22;
    rect.right += 22;
    fill(rect, viewport.position, viewport.canvas,  move.up ? White : Grey);
    drawText((char*)"R", rect.left + 2, rect.top - 1, viewport.canvas, move.up ? Grey : White);


    // Draw the left Ctrl key:
    rect.right = right - 202;
    rect.left = rect.right - 28;
    rect.bottom = bottom - 4;
    rect.top = rect.bottom - 18;
    fill(rect, viewport.position, viewport.canvas,  controls::is_pressed::ctrl ? Blue : Cyan);

    // Draw the left Alt key:
    rect.left += 30;
    rect.right += 30;
    fill(rect, viewport.position, viewport.canvas,  controls::is_pressed::alt ? Red : Magenta);

    // Draw the left Shift key:
    rect.left -= 30;
    rect.right -= 15;
    rect.top -= 20;
    rect.bottom -= 20;
    fill(rect, viewport.position, viewport.canvas,  controls::is_pressed::shift ? Green : Yellow);

    // Draw the right Ctrl key:
    rect.right = right - 4;
    rect.left = rect.right - 28;
    rect.bottom = bottom - 4;
    rect.top = rect.bottom - 18;
    fill(rect, viewport.position, viewport.canvas,  controls::is_pressed::ctrl ? Blue : Cyan);

    // Draw the right Alt key:
    rect.left -= 30;
    rect.right -= 30;
    fill(rect, viewport.position, viewport.canvas,  controls::is_pressed::alt ? Red : Magenta);

    // Draw the right Shift key:
    rect.left += 15;
    rect.right += 30;
    rect.top -= 20;
    rect.bottom -= 20;
    fill(rect, viewport.position, viewport.canvas,  controls::is_pressed::shift ? Green : Yellow);

    // Draw the Space key:
    rect.right = right - 62;
    rect.left = rect.right - 110;
    rect.bottom = bottom - 4;
    rect.top = rect.bottom - 18;
    fill(rect, viewport.position, viewport.canvas,  controls::is_pressed::space ? White : Grey);
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
    draw(rect, viewport.position, viewport.canvas,  White);

    // Draw the left mouse button:
    rect.left = 3;
    rect.right = 24;
    rect.bottom = bottom - 70;
    rect.top = rect.bottom - 40;
    fill(rect, viewport.position, viewport.canvas,  mouse::left_button.is_pressed ? Blue : Cyan);

    // Draw the middle mouse button:
    rect.left += 20;
    rect.right += 20;
    fill(rect, viewport.position, viewport.canvas,  mouse::middle_button.is_pressed ? Green : Yellow);

    // Draw the right mouse button:
    rect.left += 20;
    rect.right += 20;
    fill(rect, viewport.position, viewport.canvas,  mouse::right_button.is_pressed ? Red : Magenta);

    // Draw the mouse wheel:
    rect.left = 28;
    rect.right = 38;
    rect.bottom = bottom - 73;
    rect.top = rect.bottom - 33;
    fill(rect, viewport.position, viewport.canvas,  Grey);

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
    fill(rect, viewport.position, viewport.canvas,  White);
}

void drawMouseAndKeyboard(Viewport &viewport) {
    drawMouse(viewport);
    drawKeyboard(viewport);
}