#include "../SlimEngineCpp/scene/selection.h"
#include "../SlimEngineCpp/draw/grid.h"
#include "../SlimEngineCpp/draw/hud.h"
#include "../SlimEngineCpp/draw/mesh.h"
#include "../SlimEngineCpp/draw/selection.h"
#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"

struct MeshExample : SlimEngine {
    char strings[2][100] = {};
    String mesh_files[2] = {
        String::getFilePath((char*)"suzanne.mesh",strings[0],(char*)__FILE__),
        String::getFilePath((char*)"dog.mesh", strings[1],(char*)__FILE__)
    };
    Mesh suz, dog, *meshes{&suz};
    Grid grid{11, 11}, *grids{&grid};
    Geometry grid1{{{0, 0, 5}, {0, 45 * DEG_TO_RAD, 0}, {5, 1, 5} }, GeometryType_Grid, Green};
    Geometry suz1{{{+8, 5, 0} }, GeometryType_Mesh, Yellow};
    Geometry suz2{{{-8, 5, 0} }, GeometryType_Mesh, Cyan};
    Geometry dog1{{{0, 5, 5} }, GeometryType_Mesh, Blue, 1}, *geometries{&grid1};
    Selection selection;
    Camera camera{{0, 10, -15}, {-25 * DEG_TO_RAD, 0, 0}}, *cameras{&camera};
    HUDLine Fps{(char*)"Fps    : "};
    HUDLine Mfs{(char*)"Mic-s/f: "}, *hud_lines{&Fps};
    HUDSettings hud_settings{2, 1.2f, Green, true};
    Viewport viewport{window::canvas,cameras, hud_settings, hud_lines};
    SceneCounts counts{1, 4, 1, 0, 0, 2 };
    Scene scene{counts,nullptr, cameras, geometries, grids,nullptr,nullptr, meshes, mesh_files};

    void OnUpdate(f32 delta_time) override {
        setCountersInHUD();
        if (!mouse::is_captured) selection.manipulate(viewport, scene);
        if (!controls::is_pressed::alt) viewport.updateNavigation(delta_time);
    }
    void OnRender() override {
        bool draw_normals = controls::is_pressed::ctrl;
        f32 opacity = 0.5f;
        u8 line_width = 0;
        draw(grid, grid1.transform, viewport, Color(grid1.color), opacity, 0);
        draw(dog,dog1.transform, draw_normals, viewport,Color(dog1.color), opacity, line_width);
        draw(suz,suz1.transform, draw_normals, viewport,Color(suz1.color), opacity, line_width);
        draw(suz,suz2.transform, draw_normals, viewport,Color(suz2.color), opacity, line_width);
        if (controls::is_pressed::alt) draw(selection, viewport, scene);
        if (viewport.hud.settings.show) draw(viewport.hud, viewport);
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
        if (key == 'M' && !is_pressed)
            scene.geometries[1].id = scene.geometries[2].id = (scene.geometries[1].id + 1) % 2;

        if (!is_pressed && key == controls::key_map::tab)
            viewport.hud.settings.show = !viewport.hud.settings.show;
    }
    void OnWindowResize(u16 width, u16 height) override { viewport.updateDimensions(width, height); }
    void OnMouseButtonDown(mouse::Button &mouse_button) override { mouse::pos_raw_diff_x = mouse::pos_raw_diff_y = 0; }
    void OnMouseButtOnDoubleClicked(mouse::Button &mouse_button) override {
        if (&mouse_button == &mouse::left_button) {
            mouse::is_captured = !mouse::is_captured;
            os::setCursorVisibility(!mouse::is_captured);
            os::setWindowCapture(    mouse::is_captured);
            OnMouseButtonDown(mouse_button);
        }
    }
    void setCountersInHUD() {
        Fps.value = (i32)time::render_timer.average_frames_per_second;
        Mfs.value = (i32)time::render_timer.average_microseconds_per_frame;
    }
};

SlimEngine* createEngine() {
    return (SlimEngine*)new MeshExample();
}