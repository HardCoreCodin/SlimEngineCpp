#include "../SlimEngineCpp/scene/grid.h"
#include "../SlimEngineCpp/draw/grid.h"
#include "../SlimEngineCpp/draw/hud.h"
#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"

struct ViewportExample : SlimEngine {
    Grid grid{11, 11};
    Geometry grid1{{{0, 0, 5}, {0, 45 * DEG_TO_RAD, 0}, {5, 1, 5}}, GeometryType_Grid, Green};
    Camera camera{{0, 7, -11},{-25 * DEG_TO_RAD, 0, 0}};
    HUDLine Fps{   (char*)"Fps    : "};
    HUDLine Mfs{   (char*)"Mic-s/f: "};
    HUDLine Width{ (char*)"Width  : "};
    HUDLine Height{(char*)"Height : "};
    HUDLine MouseX{(char*)"Mouse X: "};
    HUDLine MouseY{(char*)"Mouse X: "}, *hud_lines{&Fps};
    HUDSettings hud_settings{6, 1.2f, Green, true};
    Viewport viewport{window::canvas,&camera, hud_settings, hud_lines};

    void OnUpdate(f32 delta_time) override { setCountersInHUD(); }
    void OnRender() override {
        draw(grid, grid1.transform, viewport, Color(White), 0.5f, 0);
        if (viewport.hud.settings.show) draw(viewport.hud, viewport);
    }
    void OnKeyChanged (u8 key, bool is_pressed) override {
        if (!is_pressed && key == controls::key_map::tab)
            viewport.hud.settings.show = !viewport.hud.settings.show;
    }
    void OnWindowResize(u16 width, u16 height) override { updateDimensions(width, height); }
    void OnMouseMovementSet(i32 x, i32 y) override { setMouseInHUD(); }
    void updateDimensions(u16 width, u16 height) {
        viewport.updateDimensions(width, height);
        Width.value = (i32)width;
        Height.value = (i32)height;
    }
    void setMouseInHUD() {
        MouseX.value = mouse::pos_x;
        MouseY.value = mouse::pos_y;
    }
    void setCountersInHUD() {
        Fps.value = (i32)time::render_timer.average_frames_per_second;
        Mfs.value = (i32)time::render_timer.average_microseconds_per_frame;
    }
};

SlimEngine* createEngine() {
    return (SlimEngine*)new ViewportExample();
}