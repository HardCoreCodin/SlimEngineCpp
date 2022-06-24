#ifdef SLIMMER
#define SLIM_DISABLE_ALL_CANVAS_DRAWING
#define SLIM_ENABLE_CANVAS_HUD_DRAWING
#define SLIM_ENABLE_CANVAS_TEXT_DRAWING
#define SLIM_ENABLE_CANVAS_RECTANGLE_DRAWING

#define SLIM_DISABLE_ALL_VIEWPORT_DRAWING
#define SLIM_ENABLE_VIEWPORT_GRID_DRAWING
#endif

#include "../slim/draw/hud.h"
#include "../slim/draw/grid.h"
#include "../slim/app.h"
// Or using the single-header file:
//#include "../slim.h"

#include "./_common.h"

struct NavigationApp : SlimApp {
    // Viewport:
    Camera camera{
        {0, 7, -11},
        {-25*DEG_TO_RAD, 0, 0}
    };
    Canvas canvas;
    Viewport viewport{canvas, &camera};
    bool antialias = false;

    // HUD:
    HUDLine AA{(char*)"AA : ",
               (char*)"On",
               (char*)"Off",
               &antialias,
               true};
    HUDSettings hud_settings{1};
    HUD hud{hud_settings, &AA};

    // Scene:
    Grid grid{11, 11};
    Transform transform{
        {0, 0, 5},
        {0, 45*DEG_TO_RAD, 0},
        {5, 1, 5}
    };

    // Drawing:
    f32 opacity = 0.2f;
    Color color = White;

    void OnWindowResize(u16 width, u16 height) override {
        viewport.updateDimensions(width, height);
        canvas.dimensions.update(width, height);
    }
    void OnRender() override {
        canvas.clear();
        viewport.drawGrid(grid, transform, color, opacity);
        drawMouseAndKeyboard(viewport);
        if (hud.enabled)
            canvas.drawHUD(hud);
        canvas.drawToWindow();
    }
    void OnUpdate(f32 delta_time) override {
        viewport.updateNavigation(delta_time);
    }
    void OnMouseButtonDown(mouse::Button &mouse_button) override {
        mouse::pos_raw_diff_x = mouse::pos_raw_diff_y = 0;
    }
    void OnMouseButtonDoubleClicked(mouse::Button &mouse_button) override {
        if (&mouse_button == &mouse::left_button) {
            mouse::is_captured = !mouse::is_captured;
            os::setCursorVisibility(!mouse::is_captured);
            os::setWindowCapture(    mouse::is_captured);
            OnMouseButtonDown(mouse_button);
        }
    }
    void OnKeyChanged(u8 key, bool is_pressed) override {
        Move &move = viewport.navigation.move;
        Turn &turn = viewport.navigation.turn;
        if (key == 'X') turn.left     = is_pressed;
        if (key == 'C') turn.right    = is_pressed;
        if (key == 'R') move.up       = is_pressed;
        if (key == 'F') move.down     = is_pressed;
        if (key == 'W') move.forward  = is_pressed;
        if (key == 'S') move.backward = is_pressed;
        if (key == 'A') move.left     = is_pressed;
        if (key == 'D') move.right    = is_pressed;
        if (!is_pressed) {
            if (key == controls::key_map::tab)
                hud.enabled = !hud.enabled;
            else if (key == 'Q') {
                canvas.antialias = canvas.antialias == NoAA ? SSAA : NoAA;
                antialias = canvas.antialias == SSAA;
            }
        }
    }
};

SlimApp* createApp() {
    return new NavigationApp();
}