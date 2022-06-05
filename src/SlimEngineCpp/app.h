#pragma once

#include "./viewport/canvas.h"

namespace window {
    u16 width{DEFAULT_WIDTH};
    u16 height{DEFAULT_HEIGHT};
    char* title{(char*)""};
    u32 *content{nullptr};
    Canvas canvas{nullptr, nullptr};
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
        window::canvas.renderToContent(window::content);
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