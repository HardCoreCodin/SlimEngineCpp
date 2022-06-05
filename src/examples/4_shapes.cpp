#include "../SlimEngineCpp/scene/grid.h"
#include "../SlimEngineCpp/draw/grid.h"
#include "../SlimEngineCpp/draw/curve.h"
#include "../SlimEngineCpp/draw/box.h"
#include "../SlimEngineCpp/app.h"
// Or using the single-header file:
// #include "../SlimEngine.h"

struct ShapesExample : SlimEngine {
    // Viewport:
    Camera camera{
        {0, 7, -11},
        {-25 * DEG_TO_RAD, 0, 0}
    };
    Viewport viewport{window::canvas,&camera};

    // Scene:
    Box box{};
    Grid grid{11,
              11};
    Curve helix{CurveType::Helix, 10};
    Curve coil{ CurveType::Coil,  30};

    Transform helix_transform{{-3, 4, 2}};
    Transform coil_transform{{4, 4, 2}};
    Transform box_transform{};
    Transform grid_transform{
          {0, 0, 0},
        {0, 45 * DEG_TO_RAD, 0},
            {5, 1, 5}
    };

    // Drawing:
    f32 opacity = 0.5f;
    u8 line_width = 0;

    void OnRender() override {
        draw(      grid, grid_transform,  viewport,Green,   opacity, line_width);
        draw(      box,  box_transform,   viewport,Yellow,  opacity, line_width);
        draw(coil, coil_transform,  viewport,Magenta, opacity, line_width);
        draw(helix,helix_transform, viewport,Cyan,    opacity, line_width);
    }
    void OnWindowResize(u16 width, u16 height) override {
        viewport.updateDimensions(width, height);
    }
    void OnUpdate(f32 delta_time) override {
        static float elapsed = 0;
        elapsed += delta_time;

        helix_transform.scale.y = 3 + sinf(elapsed * 1.7f);
        helix_transform.scale.x = 1 - sinf(elapsed * 1.6f + 1) * 0.3f;
        coil_transform.scale.x  = 1 + sinf(elapsed * 1.5f + 2) * 0.2f;
        helix_transform.scale.z = helix_transform.scale.x;
        coil_transform.scale.z  = coil_transform.scale.x;
        coil.thickness = 0.3f - sinf(elapsed * 1.4f + 3) * 0.1f;

        f32 angle = delta_time * 0.5f;
        box_transform.scale = 1 + sinf(elapsed * 2) * 0.2f;
        box_transform.rotate(-angle, angle, angle);
        coil_transform.rotate(angle, -angle, -angle);
        helix_transform.rotate(angle, -angle, angle);
    }
};

SlimEngine* createEngine() {
    return (SlimEngine*)new ShapesExample();
}