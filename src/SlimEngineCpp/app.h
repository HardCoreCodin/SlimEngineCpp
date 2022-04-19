#pragma once

#include "./viewport/canvas.h"
//#include "./renderer/mesh_shaders.h"


namespace window {
    u16 width{DEFAULT_WIDTH};
    u16 height{DEFAULT_HEIGHT};
    char* title{(char*)""};
    u32 *content{nullptr};
    Canvas canvas{nullptr};

    void renderCanvasToContent() {
        union RGBA2u32 {
            u32 value;
            RGBA rgba;

            RGBA2u32() : value{0} {}
        };
        PixelQuad *src_pixel = canvas.pixels;
        u32 *trg_value = content;
        vec3 color;
        RGBA2u32 background_pixel, trg_pixel;
        background_pixel.rgba.R = (u8)(canvas.background.color.r * canvas.background.color.r * FLOAT_TO_COLOR_COMPONENT);
        background_pixel.rgba.G = (u8)(canvas.background.color.g * canvas.background.color.g * FLOAT_TO_COLOR_COMPONENT);
        background_pixel.rgba.B = (u8)(canvas.background.color.b * canvas.background.color.b * FLOAT_TO_COLOR_COMPONENT);
        background_pixel.rgba.A = (u8)(canvas.background.opacity * canvas.background.opacity * FLOAT_TO_COLOR_COMPONENT);
        if (canvas.antialias) {
            for (u16 y = 0; y < canvas.dimensions.height; y++) {
                for (u16 x = 0; x < canvas.dimensions.width; x++, src_pixel++, trg_value++) {
                    if (src_pixel->TL.opacity != 0.0f ||
                        src_pixel->TR.opacity != 0.0f ||
                        src_pixel->BL.opacity != 0.0f ||
                        src_pixel->BR.opacity != 0.0f) {
                        color = src_pixel->TL.color * (src_pixel->TL.opacity * 0.25f);
                        color = src_pixel->TR.color.scaleAdd(src_pixel->TR.opacity * 0.25f, color);
                        color = src_pixel->BL.color.scaleAdd(src_pixel->BL.opacity * 0.25f, color);
                        color = src_pixel->BR.color.scaleAdd(src_pixel->BR.opacity * 0.25f, color);
                        trg_pixel.rgba.R = (u8)(color.r > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.r)));
                        trg_pixel.rgba.G = (u8)(color.g > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.g)));
                        trg_pixel.rgba.B = (u8)(color.b > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.b)));
                        trg_pixel.rgba.A = (u8)(clampedValue(src_pixel->TL.opacity) * FLOAT_TO_COLOR_COMPONENT);
                    } else trg_pixel = background_pixel;
                    *trg_value = trg_pixel.value;
                }
            }
        } else {
            for (u16 y = 0; y < canvas.dimensions.height; y++) {
                for (u16 x = 0; x < canvas.dimensions.width; x++, src_pixel++, trg_value++) {
                    if (src_pixel->TL.depth == INFINITY)
                        trg_pixel = background_pixel;
                    else {
                        color = src_pixel->TL.color * src_pixel->TL.opacity;
                        trg_pixel.rgba.R = (u8)(color.r > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.r)));
                        trg_pixel.rgba.G = (u8)(color.g > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.g)));
                        trg_pixel.rgba.B = (u8)(color.b > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.b)));
                        trg_pixel.rgba.A = (u8)(clampedValue(src_pixel->TL.opacity) * FLOAT_TO_COLOR_COMPONENT);
                    }
                    *trg_value = trg_pixel.value;
                }
            }
        }
    }
}

struct SlimEngine {
    time::Timer update_timer, render_timer;
    bool is_running{true};

    virtual void OnWindowResize(u16 width, u16 height) {};
    virtual void OnKeyChanged(  u8 key, bool pressed) {};
    virtual void OnMouseButtonUp(  mouse::Button &mouse_button) {};
    virtual void OnMouseButtonDown(mouse::Button &mouse_button) {};
    virtual void OnMouseButtonDoubleClicked(mouse::Button &mouse_button) {};
    virtual void OnMouseWheelScrolled(f32 amount) {};
    virtual void OnMousePositionSet(i32 x, i32 y) {};
    virtual void OnMouseMovementSet(i32 x, i32 y) {};
    virtual void OnMouseRawMovementSet(i32 x, i32 y) {};
    virtual void OnRender() {};
    virtual void OnUpdate(f32 delta_time) {};
    virtual void OnWindowRedraw() {
        update_timer.beginFrame();
        OnUpdate(update_timer.delta_time);
        update_timer.endFrame();

        window::canvas.clear();
        render_timer.beginFrame();
        OnRender();
        render_timer.endFrame();
        window::renderCanvasToContent();
        mouse::resetChanges();
    };

    void resize(u16 width, u16 height) {
        window::width = width;
        window::height = height;
        window::canvas.dimensions.update(width, height);

        OnWindowResize(width, height);
        OnWindowRedraw();
    }
};

SlimEngine* createEngine();

#include "./platforms/win32.h"