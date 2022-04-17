#include "../SlimEngineCpp/scene/selection.h"
#include "../SlimEngineCpp/draw/grid.h"
#include "../SlimEngineCpp/draw/curve.h"
#include "../SlimEngineCpp/draw/box.h"
#include "../SlimEngineCpp/draw/selection.h"
#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"

#include "./_common.h"

struct ManipulationExample : SlimEngine {
    Grid grid{11, 11};
    Curve helix{10};
    Curve coil{30};
    Box box{};
    Geometry grid_geo{
            {
                    {0, 0, 0},
                    {0, 45 * DEG_TO_RAD, 0},
                    {5, 1, 5}
            },
            GeometryType_Grid,
            Green
    };
    Geometry helix_geo{
            {
                    {-3, 4, 2}
            },
            GeometryType_Helix,
            Cyan
    };
    Geometry coil_geo{
            {
                    {4, 4, 2}
            },
            GeometryType_Coil,
            Magenta,
            1
    };
    Geometry box_geo{
            {},
            GeometryType_Coil,
            Yellow
    };

    Camera camera{
            {0, 7, -11},
            {-25 * DEG_TO_RAD, 0, 0}
    };
    Viewport viewport{window::canvas, &camera};
    Selection selection;

    Scene scene{ {1, 4, 1, 1, 1 }, nullptr,
                 &camera, &grid_geo, &grid, &box, &helix };

    void OnUpdate(f32 delta_time) override {
        if (!mouse::is_captured) selection.manipulate(viewport, scene);
        if (!controls::is_pressed::alt) {
            viewport.updateNavigation(delta_time);
            viewport.updateProjectionMatrix();
        }
    }
    void OnRender() override {
        draw(grid, grid_geo.transform, viewport, Color(grid_geo.color), 0.5f, 0);
        draw(box,  box_geo.transform, viewport, Color(box_geo.color), 0.5f, 0);
        draw(coil, coil_geo, viewport, Color(coil_geo.color), 0.5f, 0);
        draw(helix, helix_geo, viewport, Color(helix_geo.color), 0.5f, 0);
        if (controls::is_pressed::alt) draw(selection, viewport, scene);
        drawMouseAndKeyboard(viewport);
    }
    void OnWindowResize(u16 width, u16 height) override {
        viewport.updateDimensions(width, height);
    }
    void OnMouseButtonDown(mouse::Button &mouse_button) override {
        mouse::pos_raw_diff_x = mouse::pos_raw_diff_y = 0;
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
    return (SlimEngine*)new ManipulationExample();
}