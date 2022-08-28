#pragma once

#include "./core/base.h"


struct SlimApp {
    timers::Timer update_timer, render_timer;
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

        render_timer.beginFrame();
        OnRender();
        render_timer.endFrame();
    };

    void resize(u16 width, u16 height) {
        window::width = width;
        window::height = height;
        OnWindowResize(width, height);
        OnWindowRedraw();
    }
};

SlimApp* createApp();

#include "./platforms/win32.h"