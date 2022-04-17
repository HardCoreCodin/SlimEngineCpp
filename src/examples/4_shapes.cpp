#include "../SlimEngineCpp/scene/grid.h"
#include "../SlimEngineCpp/draw/grid.h"
#include "../SlimEngineCpp/draw/curve.h"
#include "../SlimEngineCpp/draw/box.h"
#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"

struct ShapesExample : SlimEngine {
    Grid grid{11, 11};
    Curve helix{10};
    Curve coil{30};
    Box box{};
    Geometry grid_geo{
        {
            {0, 0, 0},
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
        Magenta
    };
    Geometry box_geo{
        {},
        GeometryType_Coil,
        Yellow
    };
    Camera camera{
        {0, 7, -11},
        {-25 * DEG_TO_RAD, 0, 0}
    };
    Viewport viewport{window::canvas, &camera};

    void OnRender() override {
        draw(grid, grid_geo.transform, viewport, Color(grid_geo.color), 0.5f, 0);
        draw(box,  box_geo.transform, viewport, Color(box_geo.color), 0.5f, 0);
        draw(coil, coil_geo, viewport, Color(coil_geo.color), 0.5f, 0);
        draw(helix, helix_geo, viewport, Color(helix_geo.color), 0.5f, 0);
    }
    void OnUpdate(f32 delta_time) override {
        static float elapsed = 0;
        elapsed += delta_time;

        helix_geo.transform.scale.y = 3 + sinf(elapsed * 1.7f);
        helix_geo.transform.scale.x = 1 - sinf(elapsed * 1.6f + 1) * 0.3f;
        coil_geo.transform.scale.x  = 1 + sinf(elapsed * 1.5f + 2) * 0.2f;
        helix_geo.transform.scale.z = helix_geo.transform.scale.x;
        coil_geo.transform.scale.z  = coil_geo.transform.scale.x;

        coil.thickness = 0.3f - sinf(elapsed * 1.4f + 3) * 0.1f;

        f32 rot_speed = delta_time * 0.5f;
        box_geo.transform.scale = 1 + sinf(elapsed * 2) * 0.2f;
        box_geo.transform.rotate(-rot_speed, rot_speed, rot_speed);
        coil_geo.transform.rotate(rot_speed, -rot_speed, -rot_speed);
        helix_geo.transform.rotate(rot_speed, -rot_speed, rot_speed);
    }
    void OnWindowResize(u16 width, u16 height) override {
        viewport.updateDimensions(width, height);
    }
};

SlimEngine* createEngine() {
    return (SlimEngine*)new ShapesExample();
}