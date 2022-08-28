#pragma once

#include "../math/mat3.h"


struct Camera : OrientationUsing3x3Matrix {
    vec3 position{0};
    vec3 current_velocity{0};
    f32 focal_length{  CAMERA_DEFAULT__FOCAL_LENGTH};
    f32 zoom_amount{   CAMERA_DEFAULT__FOCAL_LENGTH};
    f32 target_distance{CAMERA_DEFAULT__TARGET_DISTANCE};
    f32 dolly_amount{0};

    XPU Camera() : OrientationUsing3x3Matrix{}, position{0.0f} {}
    XPU explicit Camera(const vec3 &position) : OrientationUsing3x3Matrix{}, position{position} {}
    XPU explicit Camera(const vec3 &position, const vec3 &orientation = vec3{0.0f}, f32 zoom_amount = CAMERA_DEFAULT__FOCAL_LENGTH) :
            OrientationUsing3x3Matrix{orientation.x, orientation.y, orientation.z},
            position{position}, current_velocity{vec3{0}}, focal_length{zoom_amount}, zoom_amount{zoom_amount} {}

    void zoom(f32 amount) {
        f32 new_amount = zoom_amount + amount;
        focal_length = new_amount > 1 ? new_amount : (new_amount < -1.0f ? (-1.0f / new_amount) : 1.0f);
        zoom_amount = new_amount;
    }

    void dolly(f32 amount) {
        vec3 target_position = forward.scaleAdd(target_distance, position);

        // Compute new target distance:
        dolly_amount += amount;
        target_distance = powf(2.0f, dolly_amount / -200.0f) * CAMERA_DEFAULT__TARGET_DISTANCE;

        // Back-track from target position_x to new current position_x:
        position = target_position - (forward * target_distance);
    }

    void orbit(f32 azimuth, f32 altitude) {
        // Move the camera forward to the position_x of its target:
        position += forward * target_distance;

        // Reorient the camera while it is at the position_x of its target:
        rotate(altitude, azimuth);

        // Back the camera away from its target position_x using the updated forward direction:
        position -= forward * target_distance;
    }

    void pan(f32 right_amount, f32 up_amount) {
        position += up * up_amount + right * right_amount;
    }

    INLINE_XPU vec3 getRayDirectionAt(f32 x, f32 y, f32 width, f32 height) const {
        vec3 start{forward.scaleAdd(focal_length * height,up.scaleAdd(height,right * -width))};
        return right.scaleAdd(x * 2.0f + 1,up.scaleAdd(1 - 2.0f * y, start)).normalized();
    }

    INLINE_XPU vec3 internPos(const vec3 &pos) const { return _unrotate(_untranslate(pos)); }
    INLINE_XPU vec3 internDir(const vec3 &dir) const { return _unrotate(dir); }
    INLINE_XPU vec3 externPos(const vec3 &pos) const { return _translate(_rotate(pos)); }
    INLINE_XPU vec3 externDir(const vec3 &dir) const { return _rotate(dir); }

private:
    INLINE_XPU vec3 _rotate(const vec3 &pos) const { return rotation * pos; }
    INLINE_XPU vec3 _unrotate(const vec3 &pos) const { return rotation.transposed() * pos; }
    INLINE_XPU vec3 _translate(const vec3 &pos) const { return pos + position; }
    INLINE_XPU vec3 _untranslate(const vec3 &pos) const { return pos - position; }
};