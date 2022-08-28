#pragma once

#include "./vec2.h"
#include "./quat.h"
#include "./vec4.h"
#include "./mat3.h"
#include "./mat4.h"

INLINE_XPU vec4 Vec4(const vec3 &v3, f32 w = 0.0f) {
    return {v3.x, v3.y, v3.z, w};
}

INLINE_XPU vec3 Vec3(const vec4 &v4) {
    return {v4.x, v4.y, v4.z};
}

INLINE_XPU vec2 Vec2(const vec3 &v3) {
    return {v3.x, v3.y};
}

INLINE_XPU mat3 Mat3(const quat &q) {
    mat3 mat;
    q.setXYZ(mat.X, mat.Y, mat.Z);
    return mat;
}

INLINE_XPU quat Quat(const mat3 &m) {
    f32 fourXSquaredMinus1 = m.X.x - m.Y.y - m.Z.z;
    f32 fourYSquaredMinus1 = m.Y.y - m.X.x - m.Z.z;
    f32 fourZSquaredMinus1 = m.Z.z - m.X.x - m.Y.y;
    f32 fourWSquaredMinus1 = m.X.x + m.Y.y + m.Z.z;

    int biggestIndex = 0;
    f32 fourBiggestSquaredMinus1 = fourWSquaredMinus1;
    if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
        fourBiggestSquaredMinus1 = fourXSquaredMinus1;
        biggestIndex = 1;
    }
    if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
        fourBiggestSquaredMinus1 = fourYSquaredMinus1;
        biggestIndex = 2;
    }
    if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) {
        fourBiggestSquaredMinus1 = fourZSquaredMinus1;
        biggestIndex = 3;
    }

    f32 biggestVal = sqrtf(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
    f32 mult = 0.25f / biggestVal;

    switch(biggestIndex) {
        case 0:
            return {
                    {
                            (m.Y.z - m.Z.y) * mult,
                            (m.Z.x - m.X.z) * mult,
                            (m.X.y - m.Y.x) * mult
                    },
                    biggestVal
            };
        case 1:
            return {
                    {
                            biggestVal,
                            (m.X.y + m.Y.x) * mult,
                            (m.Z.x + m.X.z) * mult
                    },
                    (m.Y.z - m.Z.y) * mult
            };
        case 2:
            return {
                    {
                            (m.X.y + m.Y.x) * mult,
                            biggestVal,
                            (m.Y.z + m.Z.y) * mult
                    },
                    (m.Z.x - m.X.z) * mult
            };
        case 3:
            return {
                    {
                            (m.Z.x + m.X.z) * mult,
                            (m.Y.z + m.Z.y) * mult,
                            biggestVal
                    },
                    (m.X.y - m.Y.x) * mult
            };
    }

    return {};
}

INLINE_XPU mat3 Mat3I(const quat &rotation)  {
    return Mat3(rotation.conjugate());
}

INLINE_XPU mat4 Mat4(const quat &rotation, const vec3 &scale, const vec3 &position) {
    mat3 rotation_matrix{Mat3(rotation)};
    return {
            Vec4(rotation_matrix.X * scale.x),
            Vec4(rotation_matrix.Y * scale.y),
            Vec4(rotation_matrix.Z * scale.z),
            Vec4(position, 1)
    };
}

INLINE_XPU mat4 Mat4(const mat3 &m3, const vec4 &W = {0, 0, 0, 1}) {
    return {
            Vec4(m3.X),
            Vec4(m3.Y),
            Vec4(m3.Z),
            W
    };
}

INLINE_XPU mat4 Mat4(const mat3 &rotation, const vec3 &position) {
    return {
            Vec4(rotation.X),
            Vec4(rotation.Y),
            Vec4(rotation.Z),
            Vec4(position, 1.0f)
    };
}