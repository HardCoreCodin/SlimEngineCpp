#include "../SlimEngineCpp/scene/grid.h"
#include "../SlimEngineCpp/draw/grid.h"
#include "../SlimEngineCpp/draw/hud.h"
#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngine.h"

struct ViewportExample : SlimEngine {
    // Viewport:
    Camera camera{
        {0, 7, -11},
        {-25*DEG_TO_RAD,0, 0}
    };
    Viewport viewport{window::canvas,&camera};

    // HUD:
    HUDLine Fps{   (char*)"Fps    : "};
    HUDLine Mfs{   (char*)"Mic-s/f: "};
    HUDLine Width{ (char*)"Width  : "};
    HUDLine Height{(char*)"Height : "};
    HUDLine MouseX{(char*)"Mouse X: "};
    HUDLine MouseY{(char*)"Mouse X: "};
    HUDSettings hud_settings{
        6,
        1.2f,
        Green
    };
    HUD hud{hud_settings, &Fps};

    // Scene:
    Grid grid{11, 11};
    Transform transform{
        {0, 0, 5},
        {0, 45*DEG_TO_RAD, 0},
        {5, 1, 5}
    };

    // Drawing:
    f32 opacity = 0.5f;
    u8 line_width = 0;
    vec3 color{ Color(White) };

    void OnRender() override {
        draw(grid, transform, viewport, color, opacity, line_width);
        if (hud.enabled) draw(hud, viewport);
    }

    void OnWindowResize(u16 width, u16 height) override {
        viewport.updateDimensions(width, height);
        Width.value = (i32)width;
        Height.value = (i32)height;
    }

    void OnKeyChanged (u8 key, bool is_pressed) override {
        if (!is_pressed && key == controls::key_map::tab)
            hud.enabled = !hud.enabled;
    }

    void OnUpdate(f32 delta_time) override {
        Fps.value = (i32)render_timer.average_frames_per_second;
        Mfs.value = (i32)render_timer.average_microseconds_per_frame;
    }
    void OnMouseMovementSet(i32 x, i32 y) override {
        MouseX.value = mouse::pos_x;
        MouseY.value = mouse::pos_y;
    }
};

SlimEngine* createEngine() {
    return (SlimEngine*)new ViewportExample();
}