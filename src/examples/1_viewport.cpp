#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"

enum HUDLineIndex { Fps, Mfs, Width, Height, MouseX, MouseY };
struct MyApp : SlimEngine {
    void OnWindowResize(u16 width, u16 height) override {
        setDimensionsInHUD(width, height);
    }
    void OnMouseMovementSet(i32 x, i32 y) override {
        setMouseInHUD();
    }
    void OnKeyChanged (u8 key, bool is_pressed) override {
        if (!is_pressed && key == controls::key_map::tab)
            viewport.hud.settings.show = !viewport.hud.settings.show;
    }
    void setDimensionsInHUD(i32 width, i32 height) {
        viewport.hud.lines[Width ].value = width;
        viewport.hud.lines[Height].value = height;
    }
    void setMouseInHUD() {
        viewport.hud.lines[MouseX].value = mouse::pos.x;
        viewport.hud.lines[MouseY].value = mouse::pos.y;
    }
    void setCountersInHUD() {
        u16 fps = time::update_timer.average_frames_per_second;
        u16 msf = time::update_timer.average_microseconds_per_frame;
        viewport.hud.lines[Fps].value = (i32)fps;
        viewport.hud.lines[Mfs].value = (i32)msf;
    }


    void OnWindowRedraw() override {
        scene.grids->draw(
                viewport,
                scene.geometries->transform,
                Color(scene.geometries->color),
                0.5f, 0);
        setCountersInHUD();
    }

    void setupViewport() {
        viewport.hud.position = {10, 10};
        viewport.hud.lines[Fps   ].title = (char*)"Fps    : ";
        viewport.hud.lines[Mfs   ].title = (char*)"mic-s/f: ";
        viewport.hud.lines[Width ].title = (char*)"Width  : ";
        viewport.hud.lines[Height].title = (char*)"Height : ";
        viewport.hud.lines[MouseX].title = (char*)"mouse X: ";
        viewport.hud.lines[MouseY].title = (char*)"mouse Y: ";
        viewport.camera->position = {0, 7, -11};
        viewport.camera->setRotationAroundX(-25 * DEG_TO_RAD);

        setDimensionsInHUD(viewport.dimensions.width,
                           viewport.dimensions.height);
        setCountersInHUD();
        setMouseInHUD();
    }

    MyApp() {
        scene.grids[0] = Grid{11, 11};
        Geometry &grid = scene.geometries[0];
        grid.type = GeometryType_Grid;
        grid.transform.scale = {5, 1, 5};
        grid.transform.position.z = 5;
        grid.transform.setRotationAroundY(45 * DEG_TO_RAD);
        setupViewport();
    }
};

SlimEngine* createEngine() {
    settings::hud::show = true;
    settings::hud::default_color = Green;
    settings::hud::line_count = 6;
    settings::hud::line_height = 1.2f;
    settings::scene::grids      = 1;
    settings::scene::geometries = 1;
    return (SlimEngine*)new MyApp();
}