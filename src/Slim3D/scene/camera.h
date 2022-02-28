#pragma once

#include "../core/types.h"

struct Camera : Rotation {
    f32 focal_length{  CAMERA_DEFAULT__FOCAL_LENGTH};
    f32 zoom_amount{   CAMERA_DEFAULT__FOCAL_LENGTH};
    f32 dolly_amount{  CAMERA_DEFAULT__TARGET_DISTANCE};
    f32 target_distance{0};
    vec3 current_velocity{0};
    vec3 position{0};
    vec3 right{  vec3::X};
    vec3 up{     vec3::Y};
    vec3 forward{vec3::Z};

    INLINE mat4 getMatrix() const {
        return {
            vec4{right},
            vec4{up},
            vec4{forward},
            vec4{position, 1.0f}
        };
//        return {
//                right.x, right.y, right.z, 0.0f,
//                up.x, up.y, up.z, 0.0f,
//                forward.x, forward.y, forward.z, 0.0f,
//                position.x, position.y, position.z, 1.0f
//        };
    }

    INLINE mat4 getInverseMatrix() const {
        mat3 inv_rot{getInverseRotationMatrix()};
        return {
                vec4{inv_rot.X},
                vec4{inv_rot.Y},
                vec4{inv_rot.Z},
                vec4{-(inv_rot * position), 1.0f}
        };
//        return {
//                right.x, right.y, right.z, 0.0f,
//                up.x, up.y, up.z, 0.0f,
//                forward.x, forward.y, forward.z, 0.0f,
//                -(right.x * position.x + up.x * position.y + forward.x * position.y),
//                -(right.y * position.x + up.y * position.y + forward.y * position.y),
//                -(right.z * position.x + up.z * position.y + forward.z * position.y),
//                1.0f
//        };
    }

    void zoom(f32 amount) {
        f32 new_amount = zoom_amount + amount;
        focal_length = new_amount > 1 ? new_amount : (new_amount < -1.0f ? (-1.0f / new_amount) : 1.0f);
        zoom_amount = new_amount;
    }

    void dolly(f32 amount) {
        vec3 target_position = forward * target_distance;
        target_position = position + target_position;

        // Compute new target distance:
        dolly_amount += amount;
        target_distance = powf(2.0f, dolly_amount / -200.0f) * CAMERA_DEFAULT__TARGET_DISTANCE;

        // Back-track from target position to new current position:
        position = target_position - target_distance * forward;
    }

    void orbit(f32 azimuth, f32 altitude) {
        // Move the camera forward to the position of its target:
        position += forward * target_distance;

        // Reorient the camera while it is at the position of its target:
        rotateAroundXY(altitude, azimuth);

        // Back the camera away from its target position using the updated forward direction:
        position -= forward * target_distance;
    }

    void pan(f32 right_amount, f32 up_amount) {
        position += up * up_amount + right * right_amount;
    }

    INLINE vec3 internPos(const vec3 &pos) const { return _unrotate(_untranslate(pos)); }
    INLINE vec3 internDir(const vec3 &dir) const { return _unrotate(dir); }
    INLINE vec3 externPos(const vec3 &pos) const { return _translate(_rotate(pos)); }
    INLINE vec3 externDir(const vec3 &dir) const { return _rotate(dir); }

private:
    INLINE vec3 _rotate(const vec3 &pos) const { return rotation * pos; }
    INLINE vec3 _unrotate(const vec3 &pos) const { return rotation.conjugate() * pos; }
    INLINE vec3 _translate(const vec3 &pos) const { return pos + position; }
    INLINE vec3 _untranslate(const vec3 &pos) const { return pos - position; }
};