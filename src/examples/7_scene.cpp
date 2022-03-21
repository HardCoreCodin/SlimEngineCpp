#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"

struct MyApp : SlimEngine {
    Geometry &dragon   = scene.geometries[1];
    Geometry &suzanne1 = scene.geometries[2];
    Geometry &suzanne2 = scene.geometries[3];
    Geometry &grid     = scene.geometries[0];
    Geometry &helix    = scene.geometries[4];
    Geometry &coil     = scene.geometries[5];
    Geometry &box      = scene.geometries[6];

    MyApp() {
        scene.grids[0] = Grid{11, 11};
        grid.transform.scale = {5, 1, 5};

        helix.type = GeometryType_Helix;
        coil.type = GeometryType_Coil;
        grid.type = GeometryType_Grid;
        box.type = GeometryType_Box;
        box.color = Yellow;
        grid.color = Green;
        coil.color = Magenta;
        helix.color = Cyan;
        helix.transform.position = {-3, 4, 2};
        coil.transform.position = {4, 4, 2};
        grid.transform.scale = {5, 1, 5};
        helix.id = box.id = grid.id = 0;
        coil.id = 1;
        scene.curves[0].revolution_count = 10;
        scene.curves[1].revolution_count = 30;

        suzanne1.transform.position = {10, 5, 4};
        suzanne1.color = Magenta;
        suzanne1.type = GeometryType_Mesh;
        suzanne1.id = 0;

        suzanne2 = suzanne1;
        suzanne2.color = Cyan;
        suzanne2.transform.position.x = -10;

        dragon = suzanne1;
        dragon.id = 1;
        dragon.transform.position.z = 10;
        dragon.color = Blue;

        viewport.camera->position = {0, 7, -11};
        viewport.camera->setRotationAroundX(-25 * DEG_TO_RAD);
        viewport.hud.settings.line_height = 1.2f;
        viewport.hud.position = {10, 10};
        viewport.hud.lines[0].title = (char *) "Fps    : ";
        viewport.hud.lines[1].title = (char *) "mic-s/f: ";
        setCountersInHUD();
    }

    void OnWindowRedraw() override {
        if (!mouse::is_captured)
            scene.manipulateSelection(viewport);
        if (!controls::is_pressed::alt)
            viewport.updateNavigation(time::update_timer.delta_time);

        drawScene();
        scene.selection.draw(viewport, scene.mesh_aabbs);
        setCountersInHUD();

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
            viewport.canvas.drawText(text, x, y, color, 1);
        }
    }

    void drawScene() {
        Geometry *geo = scene.geometries;
        for (u32 i = 0; i < settings::scene::geometries; i++, geo++) {
            vec3 color{Color(geo->color)};
            switch (geo->type) {
                case GeometryType_Mesh:
                    scene.meshes[geo->id].draw(
                            viewport,geo->transform,
                            false, color,
                            0.5f, 0);
                    break;
                case GeometryType_Coil:
                case GeometryType_Helix:
                    scene.curves[geo->id].draw(viewport,*geo, color,0.5f,0);
                    break;
                case GeometryType_Box:
                    scene.boxes[geo->id].draw(viewport,geo->transform, color,0.5f, 0);
                    break;
                case GeometryType_Grid:
                    scene.grids[geo->id].draw(viewport,geo->transform, color,0.5f, 0);
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

    void setCountersInHUD() {
        viewport.hud.lines[0].value = (i32)time::update_timer.average_frames_per_second;
        viewport.hud.lines[1].value = (i32)time::update_timer.average_microseconds_per_frame;
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

        if (!is_pressed && key == controls::key_map::tab)
            viewport.hud.settings.show = !viewport.hud.settings.show;

        if (controls::is_pressed::ctrl &&
            !is_pressed && key == 'S' || key == 'Z') {
            scene.last_io_is_save = key == 'S';
            char *file = settings::scene::file_path.char_ptr;
            if (scene.last_io_is_save) scene.save(file);
            else                       scene.load(file);
            scene.last_io_ticks = time::getTicks();
        }
    }
};

SlimEngine* createEngine() {
    static String mesh_files[2];
    static char string_buffers[3][100];
    String &scene = settings::scene::file_path;
    String &mesh1 = mesh_files[0];
    String &mesh2 = mesh_files[1];
    mesh1.char_ptr = string_buffers[0];
    mesh2.char_ptr = string_buffers[1];
    scene.char_ptr = string_buffers[2];
    u32 offset = String::getDirectoryLength((char*)__FILE__);
    scene.copyFrom((char*)__FILE__,
                   (char*)"this.scene", offset);
    mesh1.copyFrom((char*)__FILE__,
                   (char*)"suzanne.mesh", offset);
    mesh2.copyFrom((char*)__FILE__,
                   (char*)"dog.mesh",  offset);
    settings::scene::mesh_files = mesh_files;
    settings::scene::meshes   = 2;
    settings::scene::curves   = 2;
    settings::scene::boxes    = 1;
    settings::scene::grids      = 1;
    settings::scene::geometries = 7;
    settings::hud::show = true;
    settings::hud::default_color = Green;
    settings::hud::line_count = 2;

    return (SlimEngine*)new MyApp();
}