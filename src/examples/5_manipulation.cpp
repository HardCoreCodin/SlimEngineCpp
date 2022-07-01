#include "../slim/scene/selection.h"
#include "../slim/draw/hud.h"
#include "../slim/draw/grid.h"
#include "../slim/draw/curve.h"
#include "../slim/draw/box.h"
#include "../slim/draw/selection.h"
#include "../slim/app.h"
// Or using the single-header file:
//#include "../slim.h"

#include "./_common.h"

struct ManipulationApp : SlimApp {
    // Viewport:
    Camera camera{
        {0, 7, -11},
        {-25 * DEG_TO_RAD, 0, 0}
    }, *cameras{&camera};
    Canvas canvas;
    Viewport viewport{canvas,&camera};
    bool antialias = false;

    // HUD:
    HUDLine AA{(char*)"AA : ",
               (char*)"On",
               (char*)"Off",
               &antialias,
               true};
    HUDSettings hud_settings{1};
    HUD hud{hud_settings, &AA};

    // Scene:
    Box box, *boxes{&box};
    Grid grid{11, 11}, *grids{&grid};
    Curve helix{CurveType_Helix, 10};
    Curve coil{ CurveType_Coil,  30}, *curves{&helix};

    Geometry grid_geo{{
            {0, 0, 0},
            {0, 45 * DEG_TO_RAD, 0},
            {5, 1, 5}
    },GeometryType_Grid,Green};
    Geometry helix_geo{{{-3, 4, 2}},GeometryType_Curve,Cyan};
    Geometry coil_geo{{{4, 4, 2}}, GeometryType_Curve,Magenta,1};
    Geometry box_geo{{},GeometryType_Box,Yellow}, *geometries{&grid_geo};
    SceneCounts counts{1, 4, 1, 1, 1 };
    Scene scene{counts,nullptr, cameras, geometries, grids, boxes, curves};
    Selection selection;

    // Drawing:
    f32 opacity = 0.2f;

    void OnRender() override {
        canvas.clear();

        drawGrid(grid, grid_geo.transform, viewport, Green, opacity);
        drawBox(box, box_geo.transform, viewport, Yellow, opacity);
        drawCurve(coil, coil_geo.transform, viewport, Magenta, opacity);
        drawCurve(helix, helix_geo.transform, viewport, Cyan, opacity);

        if (controls::is_pressed::alt)
            drawSelection(selection, viewport, scene);

        drawMouseAndKeyboard(viewport);
        if (hud.enabled)
            drawHUD(hud, canvas);

        canvas.drawToWindow();
    }

    void OnUpdate(f32 delta_time) override {
        if (!mouse::is_captured) selection.manipulate(viewport, scene);
        if (!controls::is_pressed::alt) viewport.updateNavigation(delta_time);
    }

    void OnKeyChanged(u8 key, bool is_pressed) override {
        Move &move = viewport.navigation.move;
        Turn &turn = viewport.navigation.turn;
        if (key == 'X') turn.left     = is_pressed;
        if (key == 'C') turn.right    = is_pressed;
        if (key == 'R') move.up       = is_pressed;
        if (key == 'F') move.down     = is_pressed;
        if (key == 'W') move.forward  = is_pressed;
        if (key == 'S') move.backward = is_pressed;
        if (key == 'A') move.left     = is_pressed;
        if (key == 'D') move.right    = is_pressed;
        if (!is_pressed) {
            if (key == controls::key_map::tab)
                hud.enabled = !hud.enabled;
            else if (key == 'Q') {
                canvas.antialias = canvas.antialias == NoAA ? SSAA : NoAA;
                antialias = canvas.antialias == SSAA;
            }
        }
    }
    void OnWindowResize(u16 width, u16 height) override {
        viewport.updateDimensions(width, height);
        canvas.dimensions.update(width, height);
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

SlimApp* createApp() {
    return (SlimApp*)new ManipulationApp();
}