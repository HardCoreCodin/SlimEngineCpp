#include "../slim/scene/selection.h"
#include "../slim/draw/selection.h"
#include "../slim/draw/hud.h"
#include "../slim/draw/grid.h"
#include "../slim/draw/mesh.h"
#include "../slim/app.h"
// Or using the single-header file:
//#include "../slim.h"

struct MeshApp : SlimApp {
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
    Grid grid{11,
              11}, *grids{&grid};
    Transform grid_transform{
          {0, 0, 0},
        {0, 45 * DEG_TO_RAD, 0},
        {5, 1, 5}
    };

    Geometry grid1{grid_transform,GeometryType_Grid, Green};
    Geometry mesh1{{{+8, 5, 0} }, GeometryType_Mesh, Blue};
    Geometry mesh2{{{-8, 5, 0} }, GeometryType_Mesh, Cyan}, *geometries{&grid1};

    char strings[2][100] = {};
    String mesh_files[2] = {
        String::getFilePath((char*)"suzanne.mesh",strings[0],(char*)__FILE__),
        String::getFilePath((char*)"dragon.mesh" ,strings[1],(char*)__FILE__)
    };
    Mesh meshes[2];

    SceneCounts counts{1, 3, 1, 0, 0, 2 };
    Scene scene{counts,nullptr, cameras, geometries, grids,nullptr,nullptr,
                meshes, mesh_files};
    Selection selection;

    // Drawing:
    f32 opacity = 0.5f;

    void OnRender() override {
        canvas.clear();

        drawGrid(grid, grid1.transform, viewport, grid1.color, opacity);

        bool draw_normals = controls::is_pressed::ctrl;
        Mesh &mesh{meshes[scene.geometries[1].id]};
        drawMesh(mesh, mesh1.transform, draw_normals, viewport, mesh1.color, opacity);
        drawMesh(mesh, mesh2.transform, draw_normals, viewport, mesh2.color, opacity);

        if (controls::is_pressed::alt) drawSelection(selection, viewport, scene);
        if (hud.enabled)
            drawHUD(hud, canvas);

        canvas.drawToWindow();
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
            } else if (key == 'M') {
                u32 old_mesh_id = scene.geometries[1].id;
                u32 new_mesh_id = (old_mesh_id + 1) % 2;
                scene.geometries[1].id = new_mesh_id;
                scene.geometries[2].id = new_mesh_id;
            }
        }
    }

    void OnUpdate(f32 delta_time) override {
        if (!mouse::is_captured) selection.manipulate(viewport, scene);
        if (!controls::is_pressed::alt) viewport.updateNavigation(delta_time);
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
    return new MeshApp();
}