#ifdef SLIMMER
#define SLIM_DISABLE_ALL_CANVAS_DRAWING
#define SLIM_ENABLE_CANVAS_HUD_DRAWING

#define SLIM_DISABLE_ALL_VIEWPORT_DRAWING
#define SLIM_ENABLE_VIEWPORT_GRID_DRAWING
#endif

#include "../slim/draw/hud.h"
#include "../slim/draw/grid.h"
#include "../slim/app.h"
// Or using the single-header file:
//#include "../slim.h"

struct ViewportApp : SlimApp {
    // Viewport:
    Camera camera{
        {0, 7, -11},
        {-25*DEG_TO_RAD,0, 0}
    };
    Canvas canvas;
    Viewport viewport{canvas,&camera};
    bool antialias = false;

    // HUD:
    HUDLine Fps{   (char*)"Fps: "};
    HUDLine AA{    (char*)"AA : ",
                   (char*)"On",
                   (char*)"Off",
                   &antialias,
                   true};
    HUDLine Width{ (char*)"Width  : "};
    HUDLine Height{(char*)"Height : "};
    HUDLine MouseX{(char*)"Mouse X: "};
    HUDLine MouseY{(char*)"Mouse Y: "};
    HUDSettings hud_settings{6};
    HUD hud{hud_settings, &Fps};

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

    void OnRender() override {
        canvas.clear();
        viewport.drawGrid(grid, transform, color, opacity);
        if (hud.enabled)
            canvas.drawHUD(hud);
        canvas.drawToWindow();
    }

    void OnWindowResize(u16 width, u16 height) override {
        viewport.updateDimensions(width, height);
        canvas.dimensions.update(width, height);
        Width.value = (i32)width;
        Height.value = (i32)height;
    }

    void OnKeyChanged(u8 key, bool is_pressed) override {
        if (!is_pressed) {
            if (key == controls::key_map::tab)
                hud.enabled = !hud.enabled;
            else if (key == 'Q') {
                canvas.antialias = canvas.antialias == NoAA ? SSAA : NoAA;
                antialias = canvas.antialias == SSAA;
            }
        }
    }

    void OnUpdate(f32 delta_time) override {
        Fps.value = (i32)render_timer.average_frames_per_second;
    }
    void OnMouseMovementSet(i32 x, i32 y) override {
        MouseX.value = mouse::pos_x;
        MouseY.value = mouse::pos_y;
    }
};

SlimApp* createApp() {
    return new ViewportApp();
}