#include "../SlimEngineCpp/scene/grid.h"
#include "../SlimEngineCpp/draw/grid.h"
#include "../SlimEngineCpp/draw/curve.h"
#include "../SlimEngineCpp/draw/box.h"
#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngineCpp.h"

struct ShapesExample : SlimEngine {
    Box box{};
    Grid grid{11, 11};
    Curve helix{10}, coil{30};
    Geometry box1{{}, GeometryType_Coil, Yellow};
    Geometry grid1{{{0, 0, 0}, {0, 45 * DEG_TO_RAD, 0}, {5, 1, 5}}, GeometryType_Grid, Green};
    Geometry helix1{{{-3, 4, 2}}, GeometryType_Helix, Cyan};
    Geometry coil1{{{4, 4, 2}}, GeometryType_Coil, Magenta};
    Camera camera{{0, 7, -11},{-25 * DEG_TO_RAD, 0, 0}};
    Viewport viewport{window::canvas,&camera};

    void OnUpdate(f32 delta_time) override {
        static float elapsed = 0;
        elapsed += delta_time;

        helix1.transform.scale.y = 3 + sinf(elapsed * 1.7f);
        helix1.transform.scale.x = 1 - sinf(elapsed * 1.6f + 1) * 0.3f;
        coil1.transform.scale.x  = 1 + sinf(elapsed * 1.5f + 2) * 0.2f;
        helix1.transform.scale.z = helix1.transform.scale.x;
        coil1.transform.scale.z  = coil1.transform.scale.x;

        coil.thickness = 0.3f - sinf(elapsed * 1.4f + 3) * 0.1f;

        f32 rot_speed = delta_time * 0.5f;
        box1.transform.scale = 1 + sinf(elapsed * 2) * 0.2f;
        box1.transform.rotate(-rot_speed, rot_speed, rot_speed);
        coil1.transform.rotate(rot_speed, -rot_speed, -rot_speed);
        helix1.transform.rotate(rot_speed, -rot_speed, rot_speed);
    }
    void OnRender() override {
        f32 opacity = 0.5f;
        u8 line_width = 0;
        draw(grid,grid1.transform, viewport,Color(grid1.color), opacity, line_width);
        draw(box,box1.transform, viewport,Color(box1.color), opacity, line_width);
        draw(coil,coil1, viewport,Color(coil1.color), opacity, line_width);
        draw(helix,helix1, viewport,Color(helix1.color), opacity, line_width);
    }
    void OnWindowResize(u16 width, u16 height) override { viewport.updateDimensions(width, height); }
};

SlimEngine* createEngine() {
    return (SlimEngine*)new ShapesExample();
}