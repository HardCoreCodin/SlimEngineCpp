#include "../Slim3D/app.h"
// Or using the single-header file:
// #include "../Slim3D.h"

struct MyApp : Slim3D {
    void drawScene() {
        bool normals = controls::is_pressed::ctrl;
        Geometry *geo = scene.geometries;
        for (u32 i = 0; i < settings::scene::geometries; i++, geo++) {
            vec3 color{Color(geo->color)};
            switch (geo->type) {
                case GeometryType_Mesh:
                    scene.meshes[geo->id].draw(
                            viewport,geo->transform,
                            normals, color,
                            0.5f, 0);
                    break;
                case GeometryType_Grid:
                    scene.grids[geo->id].draw(
                            viewport,geo->transform,
                            color,0.5f, 0);
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
        if (key == 'M' && !is_pressed)
            scene.geometries[1].id = scene.geometries[2].id = (scene.geometries[1].id + 1) % 2;

        if (!is_pressed && key == controls::key_map::tab)
            viewport.hud.settings.show = !viewport.hud.settings.show;
    }

    void OnWindowRedraw() override {
        if (!mouse::is_captured) scene.manipulateSelection(viewport);
        if (!controls::is_pressed::alt) viewport.updateNavigation(time::update_timer.delta_time);

        drawScene();
        scene.selection.draw(viewport, scene.mesh_aabbs);
        setCountersInHUD();
    }

    void setCountersInHUD() {
        viewport.hud.lines[0].value = (i32)time::update_timer.average_frames_per_second;
        viewport.hud.lines[1].value = (i32)time::update_timer.average_microseconds_per_frame;
    }

    MyApp() {
        viewport.camera->position = {0, 7, -11};
        viewport.camera->setRotationAroundX(-25 * DEG_TO_RAD);
        viewport.hud.settings.line_height = 1.2f;
        viewport.hud.position = {10, 10};
        viewport.hud.lines[0].title = (char *) "Fps    : ";
        viewport.hud.lines[1].title = (char *) "mic-s/f: ";
        setCountersInHUD();

        scene.grids[0] = Grid{11, 11};
        Geometry &grid = scene.geometries[0];
        grid.type = GeometryType_Grid;
        grid.transform.scale = {5, 1, 5};

        mesh1.transform.position = {10, 5, 4};
        mesh1.color = Magenta;
        mesh1.type = GeometryType_Mesh;
        mesh1.id = 0;

        mesh2 = mesh1;
        mesh2.color = Cyan;
        mesh2.transform.position.x = -10;
    }
    Geometry &mesh1 = scene.geometries[1];
    Geometry &mesh2 = scene.geometries[2];
};

Slim3D* createEngine() {
    static String mesh_files[2];
    static char string_buffers[2][100];
    String *mesh1 = &mesh_files[0];
    String *mesh2 = &mesh_files[1];
    mesh1->char_ptr = string_buffers[0];
    mesh2->char_ptr = string_buffers[1];
    u32 offset = String::getDirectoryLength((char*)__FILE__);
    mesh1->copyFrom((char*)__FILE__,
                    (char*)"suzanne.mesh", offset);
    mesh2->copyFrom((char*)__FILE__,
                    (char*)"dragon.mesh",  offset);
    settings::scene::mesh_files = mesh_files;
    settings::scene::meshes   = 2;
    settings::scene::grids      = 1;
    settings::scene::geometries = 3;
    settings::hud::show = true;
    settings::hud::default_color = Green;
    settings::hud::line_count = 2;

    return (Slim3D*)new MyApp();
}