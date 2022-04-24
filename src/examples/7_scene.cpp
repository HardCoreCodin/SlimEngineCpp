#include "../SlimEngineCpp/scene/selection.h"
#include "../SlimEngineCpp/draw/grid.h"
#include "../SlimEngineCpp/draw/hud.h"
#include "../SlimEngineCpp/draw/mesh.h"
#include "../SlimEngineCpp/draw/curve.h"
#include "../SlimEngineCpp/draw/box.h"
#include "../SlimEngineCpp/draw/selection.h"
#include "../SlimEngineCpp/serialization/scene.h"
#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"

struct SceneExample : SlimEngine {
    // Viewport:
    Camera camera{
       {0, 10, -15},
       {-25*DEG_TO_RAD,0, 0}
    }, *cameras{&camera};
    Viewport viewport{window::canvas, &camera};

    // Scene:
    Box box{}, *boxes{&box};
    Grid grid{11, 11}, *grids{&grid};
    Curve helix{CurveType::Helix, 10}, coil{CurveType::Coil, 30}, *curves{&helix};
    Mesh suz, dog, *meshes{&suz};
    Transform grid_transform{{0, 0, 0},{0, 45 * DEG_TO_RAD, 0},{5, 1, 5}};
    Geometry grid1{grid_transform,GeometryType_Grid, Green};
    Geometry helix1{{ {-3, 4, 2} },GeometryType_Curve,Cyan};
    Geometry coil1{{ {4, 4, 2} }, GeometryType_Curve,Magenta,1};
    Geometry suz1{{{+8, 5, 0} }, GeometryType_Mesh, Yellow};
    Geometry suz2{{{-8, 5, 0} }, GeometryType_Mesh, Cyan};
    Geometry dog1{{{0, 5, 5} }, GeometryType_Mesh, Blue, 1};
    Geometry box1{{},GeometryType_Curve,Yellow}, *geometries{&grid1};

    char strings[3][100] = {};
    String scene_file = String::getFilePath((char*)"this.scene",strings[0],(char*)__FILE__);
    String mesh_files[2] = {
            String::getFilePath((char*)"suzanne.mesh",strings[1],(char*)__FILE__),
            String::getFilePath((char*)"dog.mesh"    ,strings[2],(char*)__FILE__)
    };
    SceneCounts counts{1, 7, 1, 1, 2, 2 };
    Scene scene{counts,scene_file.char_ptr, cameras, geometries, grids, boxes, curves, meshes, mesh_files};
    Selection selection;

    // Drawing:
    f32 opacity = 0.5f;
    u8 line_width = 0;

    void OnUpdate(f32 delta_time) override {
        if (!mouse::is_captured) selection.manipulate(viewport, scene);
        if (!controls::is_pressed::alt) viewport.updateNavigation(delta_time);
    }
    void OnRender() override {
        bool draw_normals = controls::is_pressed::ctrl;
        draw(dog,dog1.transform, draw_normals, viewport,Color(dog1.color), opacity, line_width);
        draw(suz,suz1.transform, draw_normals, viewport,Color(suz1.color), opacity, line_width);
        draw(suz,suz2.transform, draw_normals, viewport,Color(suz2.color), opacity, line_width);
        draw(grid,grid1.transform, viewport,Color(grid1.color), opacity, line_width);
        draw(box,box1.transform, viewport,Color(box1.color), opacity, line_width);
        draw(coil,coil1.transform, viewport ,Color(coil1.color), opacity, line_width);
        draw(helix,helix1.transform, viewport, Color(helix1.color), opacity, line_width);
        if (controls::is_pressed::alt)
            draw(selection, viewport, scene);
        drawMessage();
    }
    void drawMessage() const {
        f64 now = (f64) time::getTicks();
        f64 tps = (f64) time::ticks_per_second;
        if ((now - (f64) scene.last_io_ticks) / tps <= 2.0) {
            char *text;
            vec3 color;
            if (scene.last_io_is_save) {
                text = (char *) "Scene saved to: this.scene";
                color = Color(Yellow);
            } else {
                text = (char *) "Scene loaded from: this.scene";
                color = Color(Cyan);
            }
            i32 x = 50;
            i32 y = 20;
            drawText(text, x, y, viewport, color, 1);
        }
    }
    void OnKeyChanged(u8 key, bool is_pressed) override {
        if (controls::is_pressed::ctrl &&
            !is_pressed && key == 'S' || key == 'Z') {
            scene.last_io_is_save = key == 'S';
            if (scene.last_io_is_save)
                save(scene, scene_file.char_ptr);
            else
                load(scene, scene_file.char_ptr);
            scene.last_io_ticks = time::getTicks();
        }
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
    return (SlimEngine*)new SceneExample();
}