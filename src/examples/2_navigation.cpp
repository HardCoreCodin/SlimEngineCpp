#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"

#include "./_common.h"

struct MyApp : SlimEngine {
    MyApp() {
        scene.grids[0] = Grid{11, 11};
        Geometry &grid = scene.geometries[0];
        grid.type = GeometryType_Grid;
        grid.transform.scale = {5, 1, 5};
        grid.transform.position.z = 5;
        grid.transform.setRotationAroundY(45 * DEG_TO_RAD);

        viewport.camera->position = {0, 7, -11};
        viewport.camera->setRotationAroundX(-25 * DEG_TO_RAD);
    }

    void OnWindowRedraw() override {
        updateViewport();
        scene.grids->draw(viewport, scene.geometries->transform,
                          Color(scene.geometries->color), 0.5f, 0);
        drawMouseAndKeyboard(viewport);
    }
    void updateViewport() {
        Navigation &nav = viewport.navigation;
        Camera &camera = *viewport.camera;
        if (mouse::is_captured) {
            nav.navigate(camera, time::update_timer.delta_time);
            if (mouse::moved) nav.orient(camera);
            if (mouse::wheel_scrolled) nav.zoom(camera);
        } else {
            if (mouse::wheel_scrolled) nav.dolly(camera);
            if (mouse::moved) {
                if (mouse::middle_button.is_pressed) nav.pan(camera);
                if (mouse::right_button.is_pressed) nav.orbit(camera);
            }
        }
    }
    void OnMouseButtonDown(mouse::Button &mouse_button) override {
        mouse::pos_raw_diff = {0, 0};
    }
    void OnMouseButtOnDoubleClicked(mouse::Button &mouse_button) override {
        if (&mouse_button == &mouse::left_button) {
            mouse::is_captured = !mouse::is_captured;
            os::setCursorVisibility(!mouse::is_captured);
            os::setWindowCapture(    mouse::is_captured);
            OnMouseButtonDown(mouse_button);
        }
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
    }
};

SlimEngine* createEngine() {
    settings::scene::grids      = 1;
    settings::scene::geometries = 1;

    return (SlimEngine*)new MyApp();
}