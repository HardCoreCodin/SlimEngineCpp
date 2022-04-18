#include "../SlimEngineCpp/scene/grid.h"
#include "../SlimEngineCpp/draw/grid.h"
#include "../SlimEngineCpp/draw/camera.h"
#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"


struct CamerasExample : SlimEngine {
    Grid grid{11, 11};
    Geometry grid1{{{0, 0, 5}, {0, 45 * DEG_TO_RAD, 0}, {5, 1, 5}}, GeometryType_Grid, Green};
    Camera camera1{{0, 7, -11},{-25 * DEG_TO_RAD, 0, 0}};
    Camera camera2{{0, 6, 21},{-15 * DEG_TO_RAD, 190 * DEG_TO_RAD, 0},2};
    Camera *other_camera = &camera2;
    vec3 camera1_color = Color(Cyan);
    vec3 camera2_color = Color(Yellow);
    vec3 other_camera_color = camera2_color;
    Viewport viewport{window::canvas,&camera1};

    void OnUpdate(f32 delta_time) override { viewport.updateNavigation(delta_time); }
    void OnRender() override {
        draw(grid, grid1.transform, viewport, Color(White), 0.5f, 0);
        draw(*other_camera, viewport, other_camera_color, 0.5f, 0);
    }
    void OnKeyChanged(u8 key, bool is_pressed) override {
        NavigationMove &move = viewport.navigation.move;
        NavigationTurn &turn = viewport.navigation.turn;
        if (key == 'Q') turn.left     = is_pressed;
        if (key == 'E') turn.right    = is_pressed;
        if (key == 'R') move.up       = is_pressed;
        if (key == 'F') move.down     = is_pressed;
        if (key == 'W') move.forward  = is_pressed;
        if (key == 'S') move.backward = is_pressed;
        if (key == 'A') move.left     = is_pressed;
        if (key == 'D') move.right    = is_pressed;
        if (key == '1') viewport.setCamera(camera1);
        if (key == '2') viewport.setCamera(camera2);
        if (viewport.camera == &camera1) {
            other_camera = &camera2;
            other_camera_color = camera2_color;
        } else {
            other_camera = &camera1;
            other_camera_color = camera1_color;
        }
    }
    void OnWindowResize(u16 width, u16 height) override { viewport.updateDimensions(width, height); }
    void OnMouseButtonDown(mouse::Button &mouse_button) override { mouse::pos_raw_diff_x = mouse::pos_raw_diff_y = 0; }
    void OnMouseButtOnDoubleClicked(mouse::Button &mouse_button) override {
        if (&mouse_button == &mouse::left_button) {
            mouse::is_captured = !mouse::is_captured;
            os::setCursorVisibility(!mouse::is_captured);
            os::setWindowCapture(    mouse::is_captured);
            OnMouseButtonDown(mouse_button);
        }
    }
};

SlimEngine* createEngine() {
    return (SlimEngine*)new CamerasExample();
}