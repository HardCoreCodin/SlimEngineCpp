#include "../Slim3D/app.h"
// Or using the single-header file:
//#include "../Slim3D.h"
#include "./_common.h"

struct MyApp : Slim3D {
    Geometry &helix = scene.geometries[0];
    Geometry &coil  = scene.geometries[1];
    Geometry &box   = scene.geometries[2];
    Geometry &grid  = scene.geometries[3];

    MyApp() {
        viewport.camera->position = {0, 8, -13};
        viewport.camera->setRotationAroundX(-25 * DEG_TO_RAD);
        grid.transform.setRotationAroundY(45 * DEG_TO_RAD);
        scene.grids[0] = Grid{11, 11};
        scene.curves[0].revolution_count = 10;
        scene.curves[1].revolution_count = 30;
        helix.type = GeometryType_Helix;
        coil.type  = GeometryType_Coil;
        grid.type  = GeometryType_Grid;
        box.type   = GeometryType_Box;
        box.color   = Yellow;
        grid.color  = Green;
        coil.color  = Magenta;
        helix.color = Cyan;
        helix.transform.position = {-3, 4, 2};
        coil.transform.position  = {4, 4, 2};
        grid.transform.scale     = {5, 1, 5};
        helix.id = box.id = grid.id = 0;
        coil.id  = 1;
    }

    void OnWindowRedraw() override {
        if (!mouse::is_captured)
            scene.manipulateSelection(viewport);
        if (!controls::is_pressed::alt)
            viewport.updateNavigation(
                    time::update_timer.delta_time);

        drawScene();
        scene.selection.draw(viewport,
                             scene.mesh_aabbs);
        drawMouseAndKeyboard(viewport);
    }

    void drawScene() {
        Geometry *geo = scene.geometries;
        for (u32 i = 0; i < settings::scene::geometries; i++, geo++) {
            vec3 color{Color(geo->color)};
            switch (geo->type) {
                case GeometryType_Coil:
                case GeometryType_Helix:
                    scene.curves[geo->id].draw(viewport,*geo, color,0.5f,0);
                    break;
                case GeometryType_Box:
                    scene.boxes[geo->id].draw(viewport,geo->transform, color,0.5f, 0);
                    break;
                case GeometryType_Grid:
                    scene.grids[geo->id].draw(viewport,geo->transform, color,0.5f, 0);
                    break;
                default:
                    break;
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

Slim3D* createEngine() {
    settings::scene::boxes      = 1;
    settings::scene::grids      = 1;
    settings::scene::curves     = 2;
    settings::scene::geometries = 4;

    return (Slim3D*)new MyApp();
}