#pragma once

#include "../core/mouse.h"
#include "../scene/camera.h"

struct NavigationTurn {
    bool right{false};
    bool left{false};
};

struct NavigationMove {
    bool right{false};
    bool left{false};
    bool up{false};
    bool down{false};
    bool forward{false};
    bool backward{false};
};

struct Navigation {
    NavigationMove move{};
    NavigationTurn turn{};

    bool zoomed{false};
    bool moved{false};
    bool turned{false};

    void pan(Camera &camera) {
        camera.pan(settings::navigation::speed::pan * -(f32)mouse::pos_raw_diff.x,
                   settings::navigation::speed::pan * +(f32)mouse::pos_raw_diff.y);
        moved = true;
        mouse::raw_movement_handled = true;
        mouse::moved = false;
    }

    void zoom(Camera &camera) {
        camera.zoom(settings::navigation::speed::zoom * mouse::wheel_scroll_amount);
        zoomed = true;
        mouse::wheel_scroll_handled = true;
    }

    void dolly(Camera &camera) {
        camera.dolly(settings::navigation::speed::dolly * mouse::wheel_scroll_amount);
        moved = true;
        mouse::wheel_scroll_handled = true;
    }

    void orient(Camera &camera) {
        camera.rotateAroundXY(settings::navigation::speed::orient * -(f32)mouse::pos_raw_diff.y,
                              settings::navigation::speed::orient * -(f32)mouse::pos_raw_diff.x);
        mouse::moved = false;
        mouse::raw_movement_handled = true;
        turned = true;
    }

    void orbit(Camera &camera) {
        camera.orbit(settings::navigation::speed::orbit * -(f32)mouse::pos_raw_diff.x,
                     settings::navigation::speed::orbit * -(f32)mouse::pos_raw_diff.y);
        turned = true;
        moved = true;
        mouse::raw_movement_handled = true;
        mouse::moved = false;
    }

    void navigate(Camera &camera, f32 delta_time) {
        vec3 target_velocity;
        if (move.right)    target_velocity.x += settings::navigation::max_velocity;
        if (move.left)     target_velocity.x -= settings::navigation::max_velocity;
        if (move.up)       target_velocity.y += settings::navigation::max_velocity;
        if (move.down)     target_velocity.y -= settings::navigation::max_velocity;
        if (move.forward)  target_velocity.z += settings::navigation::max_velocity;
        if (move.backward) target_velocity.z -= settings::navigation::max_velocity;
        if (turn.left) {
            camera.rotateAroundY(delta_time * settings::navigation::speed::turn);
            turned = true;
        }
        if (turn.right) {
            camera.rotateAroundY(delta_time * -settings::navigation::speed::turn);
            turned = true;
        }

        // Update the current velocity and position:
        f32 velocity_difference = settings::navigation::acceleration * delta_time;
        camera.current_velocity = camera.current_velocity.approachTo(target_velocity, velocity_difference);
        vec3 position_difference = camera.current_velocity * delta_time;
        moved = position_difference.nonZero();
        if (moved) camera.position += camera.rotation * position_difference;
    }

    void update(Camera &camera, f32 delta_time) {
        if (mouse::is_captured) {
            navigate(camera, delta_time);
            if (mouse::moved) orient(camera);
            if (mouse::wheel_scrolled) zoom(camera);
        } else {
            if (mouse::wheel_scrolled) dolly(camera);
            if (mouse::moved) {
                if (mouse::middle_button.is_pressed) pan(camera);
                if (mouse::right_button.is_pressed) orbit(camera);
            }
        }
    }
};