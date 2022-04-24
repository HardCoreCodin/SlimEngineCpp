#pragma once

#include "../math/quat.h"

struct Transform : public Orientation<quat> {
    vec3 position{0.0f};
    vec3 scale{1.0f};

    Transform() : Orientation<quat>{}, position{0.0f}, scale{1.0f} {}
    Transform(const vec3 &position) : Orientation<quat>{}, position{position}, scale{1.0f} {}
    Transform(const vec3 &position, const vec3 &orientation, const vec3 &scale = vec3{1.0f}) :
            Orientation{orientation.x, orientation.y, orientation.z}, position{position}, scale{scale} {}

    void externPosAndDir(const vec3 &pos, const vec3 &dir, vec3 &out_pos, vec3 &out_dir) const {
        out_pos = position + (rotation * (scale * position));
        out_dir = (rotation * (scale * dir)).normalized();
    }

    void internPosAndDir(const vec3 &pos, const vec3 &dir, vec3 &out_pos, vec3 &out_dir) const {
        vec3 inv_scale = 1.0f / scale;
        quat inv_rotation = rotation.conjugate();
        out_pos = inv_scale * (inv_rotation * (pos - position));
        out_dir = (inv_scale * (inv_rotation * dir)).normalized();
    }

    INLINE vec3 externPos(const vec3 &pos) const { return _translate(_rotate(_scale(pos))); }
    INLINE vec3 internPos(const vec3 &pos) const { return _unscale(_unrotate(_untranslate(pos))); }

    INLINE vec3 externDir(const vec3 &dir) const { return _rotate(_scale(dir)).normalized(); }
    INLINE vec3 internDir(const vec3 &dir) const { return _unscale(_unrotate(dir)).normalized(); }

private:
    INLINE vec3 _scale(const vec3 &pos) const { return scale * pos; }
    INLINE vec3 _rotate(const vec3 &pos) const { return rotation * pos; }
    INLINE vec3 _translate(const vec3 &pos) const { return pos + position; }
    INLINE vec3 _unscale(const vec3 &pos) const { return pos / scale; }
    INLINE vec3 _unrotate(const vec3 &pos) const { return rotation.conjugate() * pos; }
    INLINE vec3 _untranslate(const vec3 &pos) const { return pos - position; }
};

struct Geometry {
    Transform transform;
    enum GeometryType type{GeometryType_None};
    enum ColorID color{White};
    u32 id{0};
};

AABB operator * (const AABB &aabb, const Transform &transform) {
    const vec3 vertices[8] = {
            {aabb.min.x, aabb.min.y, aabb.min.z},
            {aabb.min.x, aabb.min.y, aabb.max.z},
            {aabb.min.x, aabb.max.y, aabb.min.z},
            {aabb.min.x, aabb.max.y, aabb.max.z},
            {aabb.max.x, aabb.min.y, aabb.min.z},
            {aabb.max.x, aabb.min.y, aabb.max.z},
            {aabb.max.x, aabb.max.y, aabb.min.z},
            {aabb.max.x, aabb.max.y, aabb.max.z}
    };

    vec3 min{+INFINITY};
    vec3 max{-INFINITY};

    vec3 pos;
    for (const auto &vertex : vertices) {
        pos = transform.internPos(vertex);

        if (pos.x < min.x) min.x = pos.x;
        if (pos.y < min.y) min.y = pos.y;
        if (pos.z < min.z) min.z = pos.z;

        if (pos.x > max.x) max.x = pos.x;
        if (pos.y > max.y) max.y = pos.y;
        if (pos.z > max.z) max.z = pos.z;
    }

    return {min, max};
}