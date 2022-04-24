#include "../SlimEngineCpp/scene/selection.h"
#include "../SlimEngineCpp/draw/grid.h"
#include "../SlimEngineCpp/draw/hud.h"
#include "../SlimEngineCpp/draw/mesh.h"
#include "../SlimEngineCpp/draw/selection.h"
#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngine.h"

struct MeshExample : SlimEngine {
    // Viewport:
    Camera camera{
        {0, 10, -15},
        {-25*DEG_TO_RAD,0, 0}
    }, *cameras{&camera};
    HUDLine Fps{(char*)"Fps    : "};
    HUDLine Mfs{(char*)"Mic-s/f: "};
    HUDSettings hud_settings{2,1.2f,Green};
    HUD hud{hud_settings, &Fps};
    Viewport viewport{window::canvas, &camera};

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
        String::getFilePath((char*)"dog.mesh"    ,strings[1],(char*)__FILE__)
    };
    Mesh meshes[2];

    SceneCounts counts{1, 3, 1, 0, 0, 2 };
    Scene scene{counts,nullptr, cameras, geometries, grids,nullptr,nullptr,
                meshes, mesh_files};
    Selection selection;

    // Drawing:
    f32 opacity = 0.5f;
    u8 line_width = 0;

    void OnRender() override {
        draw(grid,grid1.transform, viewport,Color(grid1.color), opacity, line_width);

        bool draw_normals = controls::is_pressed::ctrl;
        Mesh &mesh{meshes[scene.geometries[1].id]};
        draw(mesh,mesh1.transform, draw_normals, viewport,Color(mesh1.color), opacity, line_width);
        draw(mesh,mesh2.transform, draw_normals, viewport,Color(mesh2.color), opacity, line_width);

        if (controls::is_pressed::alt) draw(selection, viewport, scene);
        if (hud.enabled) draw(hud, viewport);
    }

    void OnKeyChanged(u8 key, bool is_pressed) override {
        if (key == 'M' && !is_pressed) {
            u32 old_mesh_id = scene.geometries[1].id;
            u32 new_mesh_id = (old_mesh_id + 1) % 2;
            scene.geometries[1].id = new_mesh_id;
            scene.geometries[2].id = new_mesh_id;
        }

        if (!is_pressed && key == controls::key_map::tab)
            hud.enabled = !hud.enabled;

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

    void OnUpdate(f32 delta_time) override {
        Fps.value = (i32)render_timer.average_frames_per_second;
        Mfs.value = (i32)render_timer.average_microseconds_per_frame;
        if (!mouse::is_captured) selection.manipulate(viewport, scene);
        if (!controls::is_pressed::alt) viewport.updateNavigation(delta_time);
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
    return (SlimEngine*)new MeshExample();
}