#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"

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

        Camera &camera = scene.cameras[1];
        camera.position = {0, 6, 21};
        camera.setRotationAroundXY(-15 * DEG_TO_RAD, 30);
        camera.zoom(-2);
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

    void OnWindowRedraw() override {
        vec3 camera_color = Color(Yellow);
        Camera *other_camera = scene.cameras;
        if (other_camera == viewport.camera) {
            other_camera++;
            camera_color = Color(Cyan);
        }
        viewport.updateNavigation(
                time::update_timer.delta_time);
        scene.grids->draw(
                viewport,
                scene.geometries->transform,
                Color(scene.geometries->color),
                0.5f, 0);
        Box::drawCamera(
                *other_camera, viewport,
                camera_color, 0.5f, 0);
    }

    void OnKeyChanged(u8 key, bool is_pressed) override {
        NavigationMove &move = viewport.navigation.move;
        if (key == '1') viewport.setCamera(scene.cameras[0]);
        if (key == '2') viewport.setCamera(scene.cameras[1]);
        if (key == 'R') move.up       = is_pressed;
        if (key == 'F') move.down     = is_pressed;
        if (key == 'W') move.forward  = is_pressed;
        if (key == 'A') move.left     = is_pressed;
        if (key == 'S') move.backward = is_pressed;
        if (key == 'D') move.right    = is_pressed;
    }
};

SlimEngine* createEngine() {
    settings::scene::cameras    = 2;
    settings::scene::grids      = 1;
    settings::scene::geometries = 1;

    return (SlimEngine*)new MyApp();
}