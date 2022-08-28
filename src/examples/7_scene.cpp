#include "../slim/scene/selection.h"
#include "../slim/draw/text.h"
#include "../slim/draw/hud.h"
#include "../slim/draw/grid.h"
#include "../slim/draw/mesh.h"
#include "../slim/draw/curve.h"
#include "../slim/draw/box.h"
#include "../slim/draw/selection.h"
#include "../slim/serialization/scene.h"
#include "../slim/app.h"
// Or using the single-header file:
//#include "../slim.h"

struct SceneApp : SlimApp {
    // Viewport:
    Camera camera{
       {0, 10, -15},
       {-25*DEG_TO_RAD,0, 0}
    }, *cameras{&camera};
    Canvas canvas;
    Viewport viewport{canvas, &camera};
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

    Geometry grid_geo{{{0, 0, 0}, {0, 45 * DEG_TO_RAD, 0}, {5, 1, 5}}, GeometryType_Grid, Green};
    Geometry helix_geo{{{-3, 4, 2}}, GeometryType_Curve, Cyan};
    Geometry coil_geo{{{4, 4, 2}}, GeometryType_Curve, Magenta, 1};
    Geometry suzanne_geo1{{{+8, 5, 0}}, GeometryType_Mesh, Yellow};
    Geometry suzanne_geo2{{{-8, 5, 0}}, GeometryType_Mesh, Cyan};
    Geometry dog_geo{{{0, 5, 5}}, GeometryType_Mesh, Blue, 1};
    Geometry box_geo{{}, GeometryType_Box, Yellow}, *geometries{&grid_geo};

    char strings[3][100] = {};
    Mesh meshes[2];
    String mesh_files[2] = {
            String::getFilePath((char*)"suzanne.mesh",strings[1],(char*)__FILE__),
            String::getFilePath((char*)"dog.mesh" ,strings[2],(char*)__FILE__)
    };
    String scene_file = String::getFilePath((char*)"this.scene",strings[0],(char*)__FILE__);
    SceneCounts counts{1, 7, 1, 1, 2, 2 };
    Scene scene{counts,scene_file.char_ptr, cameras, geometries, grids, boxes, curves, meshes, mesh_files};
    Selection selection;

    // Drawing:
    f32 opacity = 0.2f;

    void OnRender() override {
        canvas.clear();

        bool draw_normals = controls::is_pressed::ctrl;
        for (u32 i = 0; i < counts.geometries; i++) {
            Geometry &geo{geometries[i]};
            Transform &transform{geo.transform};
            Curve &curve{curves[geo.id]};
            Mesh &mesh{meshes[geo.id]};
            Color color{geo.color};
            switch (geo.type) {
                case GeometryType_Grid : drawGrid(grid,   transform, viewport, color, opacity); break;
                case GeometryType_Box  : drawBox(box,     transform, viewport, color, opacity); break;
                case GeometryType_Curve: drawCurve(curve, transform, viewport, color, opacity); break;
                case GeometryType_Mesh : drawMesh(mesh,   transform, draw_normals, viewport, color, opacity); break;
                default: break;
            }
        }

        if (controls::is_pressed::alt)
            drawSelection(selection, viewport, scene);

        drawMessage();

        if (hud.enabled)
            drawHUD(hud, canvas);

        canvas.drawToWindow();
    }

    void OnUpdate(f32 delta_time) override {
        if (!mouse::is_captured) selection.manipulate(viewport, scene);
        if (!controls::is_pressed::alt) viewport.updateNavigation(delta_time);
    }

    void drawMessage() const {
        f64 now = (f64) timers::getTicks();
        f64 tps = (f64) timers::ticks_per_second;
        if ((now - (f64) scene.last_io_ticks) / tps <= 2.0) {
            char *text;
            Color color;
            if (scene.last_io_is_save) {
                text = (char*)"Scene saved to: this.scene";
                color = Yellow;
            } else {
                text = (char*)"Scene loaded from: this.scene";
                color = Cyan;
            }
            canvas.drawText(text, 50, 20, color);
        }
    }
    void OnKeyChanged(u8 key, bool is_pressed) override {
        if (!is_pressed) {
            if (key == controls::key_map::tab)
                hud.enabled = !hud.enabled;
            else if (key == 'Q') {
                canvas.antialias = canvas.antialias == NoAA ? SSAA : NoAA;
                antialias = canvas.antialias == SSAA;
            } else if (controls::is_pressed::ctrl &&
                       (key == 'Z' || key == 'S')) {
                scene.last_io_is_save = key == 'S';
                if (scene.last_io_is_save)
                    save(scene, scene_file.char_ptr);
                else
                    load(scene, scene_file.char_ptr);
                scene.last_io_ticks = timers::getTicks();
            }
        }
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
    return new SceneApp();
}