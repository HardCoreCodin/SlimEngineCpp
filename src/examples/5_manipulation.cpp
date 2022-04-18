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
    // Viewport:
    Camera camera{
        {0, 7, -11},
        {-25 * DEG_TO_RAD, 0, 0}
    }, *cameras{&camera};
    Viewport viewport{window::canvas,&camera};

    // Scene:
    Box box{}, *boxes{&box};
    Grid grid{11, 11}, *grids{&grid};
    Curve helix{CurveType_Helix, 10};
    Curve coil{CurveType_Coil, 30}, *curves{&helix};
    Transform grid_transform{
        {0, 0, 0},
        {0, 45 * DEG_TO_RAD, 0},
        {5, 1, 5}
    };
    Geometry grid1{grid_transform,GeometryType_Grid,Green};
    Geometry helix1{{ {-3, 4, 2} },GeometryType_Curve,Cyan};
    Geometry coil1{{ {4, 4, 2} }, GeometryType_Curve,Magenta,1};
    Geometry box1{{},GeometryType_Curve,Yellow}, *geometries{&grid1};
    SceneCounts counts{1, 4, 1, 1, 1 };
    Scene scene{counts,nullptr, cameras, geometries,grids, boxes, curves};
    Selection selection;

    // Drawing:
    f32 opacity = 0.5f;
    u8 line_width = 0;

    void OnUpdate(f32 delta_time) override {
        if (!mouse::is_captured) selection.manipulate(viewport, scene);
        if (!controls::is_pressed::alt) viewport.updateNavigation(delta_time);
    }
    void OnRender() override {
        draw(grid,grid1.transform, viewport,Color(grid1.color), opacity, line_width);
        draw(box,box1.transform, viewport,Color(box1.color), opacity, line_width);
        draw(coil,coil1.transform, viewport,Color(coil1.color), opacity, line_width);
        draw(helix,helix1.transform, viewport, Color(helix1.color), opacity, line_width);
        if (controls::is_pressed::alt) draw(selection, viewport, scene);
        drawMouseAndKeyboard(viewport);
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
    void OnWindowResize(u16 width, u16 height) override { viewport.updateDimensions(width, height); }
    void OnMouseButtonDown(mouse::Button &mouse_button) override { mouse::pos_raw_diff_x = mouse::pos_raw_diff_y = 0; }
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
    return (SlimEngine*)new ManipulationExample();
}