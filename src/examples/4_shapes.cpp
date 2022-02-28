#include "../Slim3D/app.h"
// Or using the single-header file:
// #include "../Slim3D.h"

struct MyApp : Slim3D {
    Geometry &helix = scene.geometries[0];
    Geometry &coil  = scene.geometries[1];
    Geometry &box   = scene.geometries[2];
    Geometry &grid  = scene.geometries[3];

    MyApp() {
        viewport.camera->position = {0, 8, -13};
        viewport.camera->setRotationAroundX(-25 * DEG_TO_RAD);
        grid.transform.setRotationAroundY(45 * DEG_TO_RAD);
        scene.grids[0] = Grid{11, 11};
        scene.curves[0].revolution_count = 10;
        scene.curves[1].revolution_count = 30;
        helix.type = GeometryType_Helix;
        coil.type  = GeometryType_Coil;
        grid.type  = GeometryType_Grid;
        box.type   = GeometryType_Box;
        box.color   = Yellow;
        grid.color  = Green;
        coil.color  = Magenta;
        helix.color = Cyan;
        helix.transform.position = {-3, 4, 2};
        coil.transform.position  = {4, 4, 2};
        grid.transform.scale     = {5, 1, 5};
        helix.id = box.id = grid.id = 0;
        coil.id  = 1;
    }

    void OnWindowRedraw() override {
        updateScene();
        drawScene();
    }

    void drawScene() {
        Geometry *geo = scene.geometries;
        for (u32 i = 0; i < settings::scene::geometries; i++, geo++) {
            vec3 color{Color(geo->color)};
            switch (geo->type) {
                case GeometryType_Coil:
                case GeometryType_Helix:
                    scene.curves[geo->id].draw(
                            viewport,*geo, color,
                            0.5f,0);
                    break;
                case GeometryType_Box:
                    scene.boxes[geo->id].draw(
                            viewport,geo->transform,
                            color,0.5f, 0);
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

    void updateScene() {
        static float elapsed = 0;
        elapsed += time::update_timer.delta_time;

        helix.transform.scale.y = 3 + sinf(elapsed * 1.7f);
        helix.transform.scale.x = 1 - sinf(elapsed * 1.6f + 1) * 0.3f;
        coil.transform.scale.x  = 1 + sinf(elapsed * 1.5f + 2) * 0.2f;
        helix.transform.scale.z = helix.transform.scale.x;
        coil.transform.scale.z  = coil.transform.scale.x;

        scene.curves[1].thickness = 0.3f - sinf(elapsed * 1.4f + 3) * 0.1f;

        f32 rot_speed = time::update_timer.delta_time * 0.5f;
        box.transform.scale = 1 + sinf(elapsed * 2) * 0.2f;
        box.transform.rotateAroundXYZ(-rot_speed, rot_speed, rot_speed);
        coil.transform.rotateAroundXYZ(rot_speed, -rot_speed, -rot_speed);
        helix.transform.rotateAroundXYZ(rot_speed, -rot_speed, rot_speed);
    }
};

Slim3D* createEngine() {
    settings::scene::boxes      = 1;
    settings::scene::grids      = 1;
    settings::scene::curves     = 2;
    settings::scene::geometries = 4;

    return (Slim3D*)new MyApp();
}