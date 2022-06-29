#ifdef SLIMMER
#define SLIM_DISABLE_ALL_CANVAS_DRAWING
#define SLIM_ENABLE_CANVAS_HUD_DRAWING

#define SLIM_DISABLE_ALL_VIEWPORT_DRAWING
#define SLIM_ENABLE_VIEWPORT_EDGE_DRAWING
#define SLIM_ENABLE_VIEWPORT_GRID_DRAWING
#define SLIM_ENABLE_VIEWPORT_CAMERA_DRAWING
#define SLIM_ENABLE_VIEWPORT_CURVE_DRAWING
#define SLIM_ENABLE_VIEWPORT_BOX_DRAWING
#endif

#include "../slim/draw/hud.h"
#include "../slim/draw/grid.h"
#include "../slim/draw/curve.h"
#include "../slim/draw/box.h"
#include "../slim/app.h"
// Or using the single-header file:
//#include "../slim.h"

struct ShapesApp : SlimApp {
    // Viewport:
    Camera camera{
        {0, 7, -11},
        {-25 * DEG_TO_RAD, 0, 0}
    };
    Canvas canvas;
    Viewport viewport{canvas,&camera};
    bool antialias = false;

    // HUD:
    HUDLine AA{(char*)"AA : ",
               (char*)"On",
               (char*)"Off",
               &antialias,
               true};
    HUDSettings hud_settings{1};
    HUD hud{hud_settings, &AA};

    // Scene:
    Box box{};
    Grid grid{11,
              11};
    Curve helix{CurveType_Helix, 10};
    Curve coil{ CurveType_Coil,  30};

    Transform box_transform;
    Transform helix_transform{{-3, 4, 2}};
    Transform coil_transform{{4, 4, 2}};
    Transform grid_transform{
          {0, 0, 0},
        {0, 45 * DEG_TO_RAD, 0},
            {5, 1, 5}
    };

    // Drawing:
    f32 opacity = 0.2f;

    void OnRender() override {
        canvas.clear();
        viewport.drawGrid(grid, grid_transform, Green, opacity);
        viewport.drawBox(box, box_transform, Yellow, opacity);
        viewport.drawCurve(coil, coil_transform, Magenta, opacity);
        viewport.drawCurve(helix, helix_transform, Cyan, opacity);
        if (hud.enabled)
            canvas.drawHUD(hud);
        canvas.drawToWindow();
    }

    void OnWindowResize(u16 width, u16 height) override {
        viewport.updateDimensions(width, height);
        canvas.dimensions.update(width, height);
    }

    void OnKeyChanged(u8 key, bool is_pressed) override {
        if (!is_pressed) {
            if (key == controls::key_map::tab)
                hud.enabled = !hud.enabled;
            else if (key == 'Q') {
                canvas.antialias = canvas.antialias == NoAA ? SSAA : NoAA;
                antialias = canvas.antialias == SSAA;
            }
        }
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

SlimApp* createApp() {
    return new ShapesApp();
}