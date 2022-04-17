#include "../SlimEngineCpp/scene/selection.h"
#include "../SlimEngineCpp/draw/grid.h"
#include "../SlimEngineCpp/draw/hud.h"
#include "../SlimEngineCpp/draw/mesh.h"
#include "../SlimEngineCpp/draw/curve.h"
#include "../SlimEngineCpp/draw/box.h"
#include "../SlimEngineCpp/draw/selection.h"
#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"

struct SceneExample : SlimEngine {
    char string_buffers[3][100];
    String scene_file = String::getFilePath((char*)"this.scene"    , string_buffers[0], (char*)__FILE__);
    String mesh_files[2] = {
            String::getFilePath((char*)"suzanne.mesh", string_buffers[1], (char*)__FILE__),
            String::getFilePath((char*)"dog.mesh"    , string_buffers[2], (char*)__FILE__)
    };
    Mesh monkey_mesh, dog_mesh, *meshes = &monkey_mesh;
    Curve helix{10}, coil{30}, *curves = &helix;
    Grid grid{11, 11};
    Box box{};
    Geometry grid_geo{
            {
                    {0, 0, 5},
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
    Geometry monkey1_geo{
            { {-10, 0, 0} },
            GeometryType_Mesh,
            Cyan
    };
    Geometry monkey2_geo{
            { {10, 0, 0} },
            GeometryType_Mesh,
            Blue
    };
    Geometry dog_geo{
            { {10, 5, 4} },
            GeometryType_Mesh,
            Magenta,
            1
    };
    Geometry *geometries = &grid_geo;

    Camera camera{
            {0, 7, -11},
            {-25 * DEG_TO_RAD, 0, 0}
    };
    HUDLine Fps{(char*)"Fps    : "};
    HUDLine Mfs{(char*)"Mic-s/f: "};
    HUDLine *hud_lines = &Fps;
    Viewport viewport{
            window::canvas,
            &camera,
            {
                    {2, 1.2f, Green, true},
                    hud_lines
            }
    };

    memory::MonotonicAllocator memory_allocator{Mesh::getTotalMemoryForMeshes(mesh_files, 2)};
    Scene scene{
            {1, 7, 1, 1, 2, 2 }, scene_file.char_ptr,
            &camera, geometries, &grid, &box, curves,
            meshes, mesh_files, &memory_allocator
    };
    Selection selection;

    void OnUpdate(f32 delta_time) override {
        setCountersInHUD();
        if (!mouse::is_captured) selection.manipulate(viewport, scene);
        if (!controls::is_pressed::alt) {
            viewport.updateNavigation(delta_time);
            viewport.updateProjectionMatrix();
        }
    }
    void OnRender() override {
        draw(grid, grid_geo.transform,
             viewport, Color(grid_geo.color), 0.5f, 0);
        draw(dog_mesh, dog_geo.transform, controls::is_pressed::ctrl,
             viewport, Color(dog_geo.color), 0.5f, 0);
        draw(monkey_mesh, monkey1_geo.transform, controls::is_pressed::ctrl,
             viewport, Color(monkey1_geo.color), 0.5f, 0);
        draw(monkey_mesh, monkey2_geo.transform, controls::is_pressed::ctrl,
             viewport, Color(monkey2_geo.color), 0.5f, 0);

        draw(grid, grid_geo.transform, viewport, Color(grid_geo.color), 0.5f, 0);
        draw(box,  box_geo.transform, viewport, Color(box_geo.color), 0.5f, 0);
        draw(coil, coil_geo, viewport, Color(coil_geo.color), 0.5f, 0);
        draw(helix, helix_geo, viewport, Color(helix_geo.color), 0.5f, 0);

        setCountersInHUD();
        if (controls::is_pressed::alt) draw(selection, viewport, scene);
        if (viewport.hud.settings.show) draw(viewport.hud, viewport);

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
            i32 x = viewport.dimensions.width / 2 - 150;
            i32 y = 20;
            drawText(text, x, y, viewport, color, 1);
        }
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
        if (key == 'M' && !is_pressed)
            scene.geometries[1].id = scene.geometries[2].id = (scene.geometries[1].id + 1) % 2;

        if (!is_pressed && key == controls::key_map::tab)
            viewport.hud.settings.show = !viewport.hud.settings.show;

        if (controls::is_pressed::ctrl &&
            !is_pressed && key == 'S' || key == 'Z') {
            scene.last_io_is_save = key == 'S';
            if (scene.last_io_is_save) scene.save(scene_file.char_ptr);
            else                       scene.load(scene_file.char_ptr);
            scene.last_io_ticks = time::getTicks();
        }
    }
    void setCountersInHUD() {
        Fps.value = (i32)time::render_timer.average_frames_per_second;
        Mfs.value = (i32)time::render_timer.average_microseconds_per_frame;
    }
};

SlimEngine* createEngine() {
    return (SlimEngine*)new SceneExample();
}