#pragma once


#define MOUSE_WHEEL__MAX 2000

void drawKeyboard(Viewport &viewport) {
    Rect rect;
    i32 right  = viewport.dimensions.width;
    i32 bottom = viewport.dimensions.height;
    NavigationMove move = viewport.navigation.move;
    Canvas &canvas = viewport.canvas;

    // Draw the keyboard contour:
    rect.max.x = right - 2;
    rect.min.x = rect.max.x - 230;
    rect.max.y = bottom - 2;
    rect.min.y = rect.max.y - 100;
    rect.draw(canvas, Color(White), 1);

    // Draw the 'A' key:
    rect.max.x = right - 172;
    rect.min.x = rect.max.x - 20;
    rect.max.y = bottom - 42;
    rect.min.y = rect.max.y - 20;
    rect.fill(canvas, Color(move.left ? White : Grey), 1);
    canvas.drawText((char*)"A", rect.min.x + 2, rect.min.y - 1, Color(move.left ? Grey : White), 1);

    // Draw the 'S' key:
    rect.min.x += 22;
    rect.max.x += 22;
    rect.fill(canvas, Color(move.backward ? White : Grey), 1);
    canvas.drawText((char*)"S", rect.min.x + 2, rect.min.y - 1, Color(move.backward ? Grey : White), 1);

    // Draw the 'D' key:
    rect.min.x += 22;
    rect.max.x += 22;
    rect.fill(canvas, Color(move.right ? White : Grey), 1);
    canvas.drawText((char*)"D", rect.min.x + 2, rect.min.y - 1, Color(move.right ? Grey : White), 1);

    // Draw the 'D' key:
    rect.min.x += 22;
    rect.max.x += 22;
    rect.fill(canvas, Color(move.down ? White : Grey), 1);
    canvas.drawText((char*)"F", rect.min.x + 2, rect.min.y - 1, Color(move.down ? Grey : White), 1);

    // Draw the 'Q' key:
    rect.min.x -= 28 * 3;
    rect.max.x -= 28 * 3;
    rect.min.y -= 22;
    rect.max.y -= 22;

//    fillRect(viewport, Color(turn.left ? White : Grey), &rect);
//    canvas.drawText(viewport, Color(turn.left ? Grey : White), "Q", rect.min.x + 5, rect.min.y + 5);

    // Draw the 'W' key:
    rect.min.x += 22;
    rect.max.x += 22;
    rect.fill(canvas, Color(move.forward ? White : Grey), 1);
    canvas.drawText((char*)"W", rect.min.x + 2, rect.min.y - 1, Color(move.forward ? Grey : White), 1);

    // Draw the 'E' key:
    rect.min.x += 22;
    rect.max.x += 22;
//    fillRect(viewport, Color(turn.right ? White : Grey), &rect);
//    canvas.drawText(viewport, Color(turn.right ? Grey : White), "E", rect.min.x + 5, rect.min.y + 5);

    // Draw the 'R' key:
    rect.min.x += 22;
    rect.max.x += 22;
    rect.fill(canvas, Color(move.up ? White : Grey), 1);
    canvas.drawText((char*)"R", rect.min.x + 2, rect.min.y - 1, Color(move.up ? Grey : White), 1);


    // Draw the left Ctrl key:
    rect.max.x = right - 202;
    rect.min.x = rect.max.x - 28;
    rect.max.y = bottom - 4;
    rect.min.y = rect.max.y - 18;
    rect.fill(canvas, Color(controls::is_pressed::ctrl ? Blue : Cyan), 1);

    // Draw the left Alt key:
    rect.min.x += 30;
    rect.max.x += 30;
    rect.fill(canvas, Color(controls::is_pressed::alt ? Red : Magenta), 1);

    // Draw the left Shift key:
    rect.min.x -= 30;
    rect.max.x -= 15;
    rect.min.y -= 20;
    rect.max.y -= 20;
    rect.fill(canvas, Color(controls::is_pressed::shift ? Green : Yellow), 1);

    // Draw the right Ctrl key:
    rect.max.x = right - 4;
    rect.min.x = rect.max.x - 28;
    rect.max.y = bottom - 4;
    rect.min.y = rect.max.y - 18;
    rect.fill(canvas, Color(controls::is_pressed::ctrl ? Blue : Cyan), 1);

    // Draw the right Alt key:
    rect.min.x -= 30;
    rect.max.x -= 30;
    rect.fill(canvas, Color(controls::is_pressed::alt ? Red : Magenta), 1);

    // Draw the right Shift key:
    rect.min.x += 15;
    rect.max.x += 30;
    rect.min.y -= 20;
    rect.max.y -= 20;
    rect.fill(canvas, Color(controls::is_pressed::shift ? Green : Yellow), 1);

    // Draw the Space key:
    rect.max.x = right - 62;
    rect.min.x = rect.max.x - 110;
    rect.max.y = bottom - 4;
    rect.min.y = rect.max.y - 18;
    rect.fill(canvas, Color(controls::is_pressed::space ? White : Grey), 1);
}

void drawMouse(Viewport &viewport) {
    Rect rect;
    Canvas &canvas = viewport.canvas;
    i32 bottom = viewport.dimensions.height;

    // Draw the mouse contour:
    rect.min.x = 2;
    rect.max.x = 62;
    rect.max.y = bottom - 1;
    rect.min.y = rect.max.y - 111;
    rect.draw(canvas, Color(White), 1);

    // Draw the left mouse button:
    rect.min.x = 3;
    rect.max.x = 24;
    rect.max.y = bottom - 70;
    rect.min.y = rect.max.y - 40;
    rect.fill(canvas, Color(mouse::left_button.is_pressed ? Blue : Cyan), 1);

    // Draw the middle mouse button:
    rect.min.x += 20;
    rect.max.x += 20;
    rect.fill(canvas, Color(mouse::middle_button.is_pressed ? Green : Yellow), 1);

    // Draw the right mouse button:
    rect.min.x += 20;
    rect.max.x += 20;
    rect.fill(canvas, Color(mouse::right_button.is_pressed ? Red : Magenta), 1);

    // Draw the mouse wheel:
    rect.min.x = 28;
    rect.max.x = 38;
    rect.max.y = bottom - 73;
    rect.min.y = rect.max.y - 33;
    rect.fill(canvas, Color(Grey), 1);

    // Draw a marker representing the state of the mouse wheel:
    rect.min.x += 1;
    rect.max.x -= 1;
    rect.min.y += 14;
    rect.max.y -= 14;

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
    rect.min.y += (i32)mouse_wheel_delta_y;
    rect.max.y += (i32)mouse_wheel_delta_y;
    rect.fill(canvas, Color(White), 1);
}

void drawMouseAndKeyboard(Viewport &viewport) {
    drawMouse(viewport);
    drawKeyboard(viewport);
}