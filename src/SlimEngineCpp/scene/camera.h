#pragma once

#include "../core/types.h"

struct Camera : public Orientation<mat3> {
    vec3 position{0};
    vec3 current_velocity{0};
    f32 dolly_amount{0};
    f32 target_distance{CAMERA_DEFAULT__TARGET_DISTANCE};
    f32 focal_length{  CAMERA_DEFAULT__FOCAL_LENGTH};
    f32 zoom_amount{   CAMERA_DEFAULT__FOCAL_LENGTH};

    Camera() : Orientation<mat3>{}, position{0.0f} {}
    explicit Camera(const vec3 &position) : Orientation<mat3>{}, position{position} {}
    explicit Camera(const vec3 &position, const vec3 &orientation = vec3{0.0f}) :
            Orientation{orientation.x, orientation.y, orientation.z}, position{position} {}

    INLINE mat4 getMatrix() const {
        return {
            vec4{rotation.X},
            vec4{rotation.Y},
            vec4{rotation.Z},
            vec4{position, 1.0f}
        };
    }

    void zoom(f32 amount) {
        f32 new_amount = zoom_amount + amount;
        focal_length = new_amount > 1 ? new_amount : (new_amount < -1.0f ? (-1.0f / new_amount) : 1.0f);
        zoom_amount = new_amount;
    }

    void dolly(f32 amount) {
        vec3 target_position = rotation.forward.scaleAdd(target_distance, position);

        // Compute new target distance:
        dolly_amount += amount;
        target_distance = powf(2.0f, dolly_amount / -200.0f) * CAMERA_DEFAULT__TARGET_DISTANCE;

        // Back-track from target position to new current position:
        position = target_position - (rotation.forward * target_distance);
    }

    void orbit(f32 azimuth, f32 altitude) {
        // Move the camera forward to the position of its target:
        position += rotation.forward * target_distance;

        // Reorient the camera while it is at the position of its target:
        rotate(altitude, azimuth);

        // Back the camera away from its target position using the updated forward direction:
        position -= rotation.forward * target_distance;
    }

    void pan(f32 right_amount, f32 up_amount) {
        position += rotation.up * up_amount + rotation.right * right_amount;
    }

    INLINE vec3 internPos(const vec3 &pos) const { return _unrotate(_untranslate(pos)); }
    INLINE vec3 internDir(const vec3 &dir) const { return _unrotate(dir); }
    INLINE vec3 externPos(const vec3 &pos) const { return _translate(_rotate(pos)); }
    INLINE vec3 externDir(const vec3 &dir) const { return _rotate(dir); }

private:
    INLINE vec3 _rotate(const vec3 &pos) const { return rotation * pos; }
    INLINE vec3 _unrotate(const vec3 &pos) const { return rotation.transposed() * pos; }
    INLINE vec3 _translate(const vec3 &pos) const { return pos + position; }
    INLINE vec3 _untranslate(const vec3 &pos) const { return pos - position; }
};