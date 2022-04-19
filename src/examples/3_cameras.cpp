#include "../SlimEngineCpp/scene/grid.h"
#include "../SlimEngineCpp/draw/grid.h"
#include "../SlimEngineCpp/draw/camera.h"
#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"


struct CamerasExample : SlimEngine {
    // Viewport:
    Camera camera1{
        {0, 7, -11},
        {-25*DEG_TO_RAD, 0, 0}
    };
    Camera camera2{
        {0, 6, 21},
        {-15*DEG_TO_RAD,190*DEG_TO_RAD, 0},
        2
    };
    Camera *other_camera = &camera2;
    vec3 camera1_color = Color(Cyan);
    vec3 camera2_color = Color(Yellow);
    vec3 other_camera_color = camera2_color;
    Viewport viewport{window::canvas,&camera1};

    // Geometry:
    Grid grid{11, 11};

    // Scene:
    Transform transform{
        {0, 0, 5},
        {0, 45*DEG_TO_RAD, 0},
        {5, 1, 5}
    };

    // Drawing:
    f32 opacity = 0.5f;
    u8 line_width = 0;
    vec3 color{ Color(White) };

    void OnUpdate(f32 delta_time) override {
        viewport.updateNavigation(delta_time);
    }
    void OnRender() override {
        draw(grid, transform, viewport, color, opacity, line_width);
        draw(*other_camera, viewport,
             other_camera_color, opacity, line_width);
    }
    void OnKeyChanged(u8 key, bool is_pressed) override {
        if (key == '1') viewport.setCamera(camera1);
        if (key == '2') viewport.setCamera(camera2);
        if (viewport.camera == &camera1) {
            other_camera = &camera2;
            other_camera_color = camera2_color;
        } else {
            other_camera = &camera1;
            other_camera_color = camera1_color;
        }

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
    }
    void OnWindowResize(u16 width, u16 height) override {
        viewport.updateDimensions(width, height);
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
};

SlimEngine* createEngine() {
    return (SlimEngine*)new CamerasExample();
}