#pragma once

#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/vec4.h"
#include "../math/mat3.h"
#include "../math/mat4.h"
#include "../math/quat.h"

enum CurveType {
    CurveType_None = 0,

    CurveType_Helix,
    CurveType_Coil,

    CurveType_Count
};

struct Edge { vec3 from, to; };
struct Rect { vec2i min, max; };
struct Curve { CurveType type{CurveType_None}; f32 revolution_count{1}, thickness{0.1f}; };

template <class T>
struct Orientation {
    T rotation{};

    Orientation() : rotation{T::Identity} {}
    explicit Orientation(f32 x_radians, f32 y_radians = 0, f32 z_radians = 0) {
        setRotation(x_radians, y_radians, z_radians);
    }

    INLINE void rotate(f32 x_radians, f32 y_radians, f32 z_radians) {
        setRotation(angles.x + x_radians, angles.y + y_radians, angles.x + z_radians);
    }

    INLINE void rotate(f32 x_radians, f32 y_radians) {
        setRotation(angles.x + x_radians, angles.y + y_radians);
    }

    INLINE void setRotation(f32 x_radians, f32 y_radians, f32 z_radians) {
        angles.x = x_radians;
        angles.y = y_radians;
        angles.z = z_radians;
        _update();
    }

    INLINE void setRotation(f32 x_radians, f32 y_radians) {
        angles.x = x_radians;
        angles.y = y_radians;
        _update();
    }

    INLINE void rotateAroundX(f32 radians) {
        setRotationAroundX(angles.x + radians);
    }

    INLINE void rotateAroundY(f32 radians) {
        setRotationAroundY(angles.y + radians);
    }

    INLINE void rotateAroundZ(f32 radians) {
        setRotationAroundZ(angles.z + radians);
    }

    INLINE void setRotationAroundX(f32 radians) {
        angles.x = radians;
        _update();
    }

    INLINE void setRotationAroundY(f32 radians) {
        angles.y = radians;
        _update();
    }

    INLINE void setRotationAroundZ(f32 radians) {
        angles.z = radians;
        _update();
    }

protected:
    vec3 angles;

    void _update() {
        rotation = T::Identity;
        if (angles.z != 0.0f) rotation = T::RotationAroundZ(angles.z);
        if (angles.x != 0.0f) rotation *= T::RotationAroundX(angles.x);
        if (angles.y != 0.0f) rotation *= T::RotationAroundY(angles.y);
    }
};

struct Transform : public Orientation<quat> {
    vec3 position{0.0f};
    vec3 scale{1.0f};

    Transform() : Orientation<quat>{}, position{0.0f}, scale{1.0f} {}
    Transform(const vec3 &position) : Orientation<quat>{}, position{position}, scale{1.0f} {}
    Transform(const vec3 &position, const vec3 &orientation, const vec3 &scale = vec3{1.0f}) :
            Orientation{orientation.x, orientation.y, orientation.z}, position{position}, scale{scale} {}

    mat3 getRotationMatrix() const {
        return mat3{rotation};
    }

    INLINE mat3 getInverseRotationMatrix() const {
        return mat3{rotation.conjugate()};
    }

    mat4 getMatrix() const {
        mat3 rotation_matrix{rotation};
        return mat4{
                vec4{rotation_matrix.X * scale.x},
                vec4{rotation_matrix.Y * scale.y},
                vec4{rotation_matrix.Z * scale.z},
                vec4{position, 1}
        };
    }

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

enum GeometryType {
    GeometryType_None = 0,

    GeometryType_Mesh,
    GeometryType_Grid,
    GeometryType_Box,
    GeometryType_Curve,

    GeometryType_Count
};

struct Geometry {
    Transform transform;
    enum GeometryType type{GeometryType_None};
    enum ColorID color{White};
    u32 id{0};
};

struct AABB {
    vec3 min{-1}, max{1};

    AABB(f32 min_x, f32 min_y, f32 min_z,
         f32 max_x, f32 max_y, f32 max_z) : AABB{
            vec3{min_x, min_y, min_z},
            vec3{max_x, max_y, max_z}
    } {}
    AABB(f32 min_value, f32 max_value) : AABB{vec3{min_value}, vec3{max_value}} {}
    AABB(const vec3 &min, const vec3 &max) : min{min}, max{max} {}
    AABB() : AABB{0, 0} {}

    AABB& operator *= (const Transform &transform) {
        const vec3 vertices[8] = {
                {min.x, min.y, min.z},
                {min.x, min.y, max.z},
                {min.x, max.y, min.z},
                {min.x, max.y, max.z},
                {max.x, min.y, min.z},
                {max.x, min.y, max.z},
                {max.x, max.y, min.z},
                {max.x, max.y, max.z}
        };

        min = +INFINITY;
        max = -INFINITY;

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

        return *this;
    }
};
//
//enum BRDFType {
//    phong,
//    ggx
//};
//
//struct Rasterizer;
//struct Shaded;
//struct Scene;
//struct Mesh;
//typedef void (*PixelShader)(Shaded &shaded, const Scene &scene);
//typedef u8 (  *MeshShader )(const Mesh &mesh, const Rasterizer &rasterizer);
//
//struct Material {
//    vec3 diffuse{1.0f};
//    vec3 specular{1.0f};
//    f32 shininess{1.0f};
//    f32 roughness{1.0f};
//    u8 texture_count{0};
//    u8 texture_ids[16]{0};
//    PixelShader pixel_shader{nullptr};
//    MeshShader mesh_shader{nullptr};
//    enum BRDFType brdf{phong};
//    u8 flags{PHONG | LAMBERT};
//};
//
//struct MaterialHas {
//    bool diffuse{false};
//    bool specular{false};
//
//    explicit MaterialHas(u8 flags) : diffuse{(bool)(flags & (u8)LAMBERT)}, specular{(bool)(flags & (u8)PHONG | flags & (u8)BLINN)} {};
//};
//
//struct MaterialUses {
//    bool blinn{false};
//    bool phong{false};
//
//    explicit MaterialUses(u8 flags) : blinn{(bool)(flags & (u8)BLINN)}, phong{(bool)(flags & (u8)PHONG)} {};
//};
//
//struct Shaded {
//    vec3 color, position, normal, viewing_direction, viewing_origin, reflected_direction, light_direction, diffuse;
//    vec2 UV, dUV;
//    vec2i coords;
//    f32 opacity;
//    f64 depth;
//    Material *material;
//    Geometry *geometry;
//};
//
//
//struct AmbientLight{ vec3 color; };
//struct Light {
//    vec3 attenuation, position_or_direction, color;
//    f32 intensity;
//    bool is_directional;
//};

enum BoxSide {
    NoSide = 0,
    Top    = 1,
    Bottom = 2,
    Left   = 4,
    Right  = 8,
    Front  = 16,
    Back   = 32
};

INLINE BoxSide getBoxSide(const vec3 &octant, u8 axis) {
    switch (axis) {
        case 0 : return octant.x > 0 ? Right : Left;
        case 3 : return octant.x > 0 ? Left : Right;
        case 1 : return octant.y > 0 ? Top : Bottom;
        case 4 : return octant.y > 0 ? Bottom : Top;
        case 2 : return octant.z > 0 ? Front : Back;
        default: return octant.z > 0 ? Back : Front;
    }
}

struct RayHit {
    vec3 position, normal;
    f32 distance, distance_squared;
    u32 geo_id{0};
    enum GeometryType geo_type{GeometryType_None};
    bool from_behind{false};
};

struct Ray {
    vec3 origin, direction;
    RayHit hit{};

    INLINE BoxSide hitCube() {
        vec3 octant, RD_rcp = 1.0f / direction;
        octant.x = signbit(direction.x) ? 1.0f : -1.0f;
        octant.y = signbit(direction.y) ? 1.0f : -1.0f;
        octant.z = signbit(direction.z) ? 1.0f : -1.0f;

        f32 t[6];
        t[0] = (+octant.x - origin.x) * RD_rcp.x;
        t[1] = (+octant.y - origin.y) * RD_rcp.y;
        t[2] = (+octant.z - origin.z) * RD_rcp.z;
        t[3] = (-octant.x - origin.x) * RD_rcp.x;
        t[4] = (-octant.y - origin.y) * RD_rcp.y;
        t[5] = (-octant.z - origin.z) * RD_rcp.z;

        u8 max_axis = t[3] < t[4] ? 3 : 4; if (t[5] < t[max_axis]) max_axis = 5;
        f32 max_t = t[max_axis];
        if (max_t < 0) // Further-away hit is behind the ray - intersection can not occur.
            return NoSide;

        u8 min_axis = t[0] > t[1] ? 0 : 1; if (t[2] > t[min_axis]) min_axis = 2;
        f32 min_t = t[min_axis];
        if (max_t < (min_t > 0 ? min_t : 0))
            return NoSide;

        hit.from_behind = min_t < 0; // Further-away hit is in front of the ray, closer one is behind it.
        if (hit.from_behind) {
            min_t = max_t;
            min_axis = max_axis;
        }

        BoxSide side = getBoxSide(octant, min_axis);
        hit.position = direction.scaleAdd(min_t, origin);
        hit.normal = 0.0f;
        switch (side) {
            case Left:   hit.normal.x = hit.from_behind ? +1.0f : -1.0f; break;
            case Right:  hit.normal.x = hit.from_behind ? -1.0f : +1.0f; break;
            case Bottom: hit.normal.y = hit.from_behind ? +1.0f : -1.0f; break;
            case Top:    hit.normal.y = hit.from_behind ? -1.0f : +1.0f; break;
            case Back:   hit.normal.z = hit.from_behind ? +1.0f : -1.0f; break;
            case Front:  hit.normal.z = hit.from_behind ? -1.0f : +1.0f; break;
            default: return NoSide;
        }

        return side;
    }

    INLINE bool hitPlane(const vec3 &P, const vec3 &N) {
        f32 NdotRd = N | direction;
        if (NdotRd == 0) // The ray is parallel to the plane
            return false;

        f32 NdotRoP = N | (P - origin);
        if (NdotRoP == 0) return false; // The ray originated within the plane

        bool ray_is_facing_the_plane = NdotRd < 0;
        hit.from_behind = NdotRoP > 0;
        if (hit.from_behind == ray_is_facing_the_plane) // The ray can't hit the plane
            return false;

        hit.distance = NdotRoP / NdotRd;
        hit.position = origin + direction*hit.distance;
        hit.normal = N;

        return true;
    }
};



//struct Rotation {
//    vec3 angle;
//    quat yaw_rotation{}, pitch_rotation{}, roll_rotation{};
//
//
//    quat rotation;
//
//    void _update() {
//        rotation = ((roll_rotation * yaw_rotation).normalized() * pitch_rotation).normalized();
//        matrix = mat3{rotation};
//    }
//
//
//    mat3 matrix;
//
//    Rotation() = default;
//    Rotation(f32 x_angle, f32 y_angle, f32 z_angle) : angle{0} {
//        yawPitchAndRoll(x_angle, y_angle, z_angle);
//    }
//
//    explicit Rotation(const vec3 &angles) : Rotation{angles.x, angles.y, angles.z} {}
//
//    INLINE quat asQuat() const {
//        return rotation;
//    }
//
//    INLINE void yawPitchAndRoll(f32 pitch_radians, f32 yaw_radians, f32 roll_radians) {
//        angle.x += pitch_radians;
//        angle.y += yaw_radians;
//        angle.z += roll_radians;
//        pitch_rotation = quat::getRotationAroundX(angle.x);
//        yaw_rotation   = quat::getRotationAroundY(angle.y);
//        roll_rotation = quat::getRotationAroundZ(angle.z);
//        _update();
//    }
//
//    INLINE vec3 rotate(const vec3 &pos) {
//        return rotation * pos;
//    }
//
//    INLINE vec3 unrotate(const vec3 &pos) {
//        return rotation.conjugate() * pos;
//    }
//
//    INLINE void yawAndPitch(f32 yaw_radians, f32 pitch_radians) {
//        angle.x += pitch_radians;
//        angle.y += yaw_radians;
//        pitch_rotation = quat::getRotationAroundX(angle.x);
//        yaw_rotation   = quat::getRotationAroundY(angle.y);
//        _update();
//    }
//
//    INLINE void pitch(f32 radians) {
//        angle.x += radians;
//        pitch_rotation = quat::getRotationAroundX(angle.x);
//        _update();
//    }
//
//    INLINE void yaw(f32 radians) {
//        angle.y += radians;
//        yaw_rotation = quat::getRotationAroundY(angle.y);
//        _update();
//    }
//
//    INLINE void roll(f32 radians) {
//        angle.z += radians;
//        roll_rotation = quat::getRotationAroundZ(angle.z);
//        _update();
//    }
//};

//struct Transform {
//    quat rotation{};
//    vec3 position; //, angle;
//    vec3 scale{1};
//
//    mat4 getMatrix() const {
//        mat3 rotation_matrix{Rotation{angle}.matrix};
//        return mat4{
//                vec4{rotation_matrix.X * size.x},
//                vec4{rotation_matrix.Y * size.y},
//                vec4{rotation_matrix.Z * size.z},
//                vec4{position, 1}
//        };
//    }
//
//    INLINE vec3 _rotate(const vec3 &pos) const {
//        return Rotation{angle}.rotate(pos);
//    }
//
//    INLINE vec3 _unrotate(const vec3 &pos) const {
//        return Rotation{angle}.unrotate(pos);
//    }
//
//    INLINE vec3 scale(const vec3 &pos) const {
//        return pos * size;
//    }
//
//    INLINE vec3 unscale(const vec3 &pos) const {
//        return pos / size;
//    }
//
//    INLINE vec3 translate(const vec3 &pos) const {
//        return pos + position;
//    }
//
//    INLINE vec3 untranslate(const vec3 &pos) const {
//        return pos - position;
//    }

//    void rotate(f32 yaw, f32 pitch, f32 roll) {
//        quat rot{};
//        if (roll)  rot  = quat::getRotationAroundAxis({0, 0, 1}, roll);
//        if (pitch) rot *= quat::getRotationAroundAxis({1, 0, 0}, pitch);
//        if (yaw)   rot *= quat::getRotationAroundAxis({0, 1, 0}, yaw);
//
////        rotation = rotation * rot;
//
//        rotation *= rot;
//
////        rotation = (quat::getRotationAroundAxis({0, 0, 1}, roll) *
////                    quat{1, 0, 0, pitch} *
////                    quat{0, 1, 0, yaw}).normalized() * rotation;
////        rotation = rotation.normalized();
//    }

//    vec3 externDir(const vec3 &dir) const {
//        return rotate(scale(dir));
//    }
//
//    vec3 externPos(const vec3 &pos) const {
//        return translate(rotate(scale(pos)));
//    }
//
//    vec3 internPos(const vec3 &pos) const {
//        return unscale(unrotate(untranslate(pos)));
//    }
//
//    void internPosAndDir(const vec3 &pos, const vec3 &dir, vec3 &out_position, vec3 &out_direction) const {
//        vec3 inv_scale = 1.0f / size;
//        quat inv_rotation = Rotation{angle}.asQuat().conjugate();
//        out_position = (inv_rotation * (pos - position)) * inv_scale;
//        out_direction = ((inv_rotation * dir) * inv_scale).normalized();
//    }
//};

//struct TransformWithMatrices : Transform {
//    mat3 yaw_matrix{}, pitch_matrix{}, roll_matrix{};
//    mat3 rotation_matrix{}; //, rotation_matrix_inverted{};
//    vec3 &right{rotation_matrix.X}, &up{rotation_matrix.Y}, &forward{rotation_matrix.Z};
//    quat yaw_rotation{}, pitch_rotation{}, roll_rotation{}; //, rotation_inverted{};
//
//    void rotate(f32 yaw, f32 pitch, f32 roll) {
//
//
////        xform->rotation_matrix_inverted = transposedMat3(xform->rotation_matrix);
//
////        xform->rotation          = convertRotationMatrixToQuaternion(xform->rotation_matrix);
////        xform->rotation_inverted = convertRotationMatrixToQuaternion(xform->rotation_matrix_inverted);
//
////        xform->matrix = mulMat3(xform->matrix, xform->rotation_matrix);
//
//        if (yaw)   yaw_matrix.rotateAroundY(yaw);
//        if (pitch) pitch_matrix.rotateAroundX(pitch);
//        if (roll)  roll_matrix.rotateAroundZ( roll);
//
//        rotation_matrix = pitch_matrix * yaw_matrix * roll_matrix;
////        rotation_matrix = (pitch_matrix * yaw_matrix) * roll_matrix;
////        rotation_matrix_inverted = rotation_matrix.transposed();
//
//        rotation = rotation_matrix.asQuat();
////        rotation_inverted = rotation_matrix_inverted.asQuat();
//
////        matrix = rotation_matrix * matrix;
//    }
//
//    void rotate2(f32 yaw, f32 pitch, f32 roll) {
//        if (pitch) pitch_rotation = pitch_rotation.rotatedAroundAxisBy({1, 0, 0}, pitch);
//        if (yaw)   yaw_rotation   = yaw_rotation.rotatedAroundAxisBy(  {0, 1, 0}, yaw);
//        if (roll)  roll_rotation  = roll_rotation.rotatedAroundAxisBy( {0, 0, 1}, roll);
//
//        rotation = ((roll_rotation * yaw_rotation).normalized() * pitch_rotation).normalized();
////        rotation_inverted = rotation.conjugate();
//        rotation_matrix = mat3{rotation};
////        rotation_matrix_inverted = mat3{rotation_inverted};
//    }
//
//    void writeToFile(void *file_handle) {
////        os::writeToFile(&matrix, sizeof(mat3), file_handle);
////        os::writeToFile(&yaw_matrix, sizeof(mat3), file_handle);
////        os::writeToFile(&pitch_matrix, sizeof(mat3), file_handle);
////        os::writeToFile(&roll_matrix, sizeof(mat3), file_handle);
//        os::writeToFile(&rotation_matrix, sizeof(mat3), file_handle);
////        os::writeToFile(&rotation_matrix_inverted, sizeof(mat3), file_handle);
//
//        os::writeToFile(&rotation, sizeof(quat), file_handle);
////        os::writeToFile(&rotation_inverted, sizeof(quat), file_handle);
//
//        os::writeToFile(&position, sizeof(vec3), file_handle);
//        os::writeToFile(&scale, sizeof(vec3), file_handle);
//    }
//
//    void readFromFile(void *file_handle) {
////        os::readFromFile(&matrix, sizeof(mat3), file_handle);
////        os::readFromFile(&yaw_matrix, sizeof(mat3), file_handle);
////        os::readFromFile(&pitch_matrix, sizeof(mat3), file_handle);
////        os::readFromFile(&roll_matrix, sizeof(mat3), file_handle);
//        os::readFromFile(&rotation_matrix, sizeof(mat3), file_handle);
////        os::readFromFile(&rotation_matrix_inverted, sizeof(mat3), file_handle);
//
//        os::readFromFile(&rotation, sizeof(quat), file_handle);
////        os::readFromFile(&rotation_inverted, sizeof(quat), file_handle);
//
//        os::readFromFile(&position, sizeof(vec3), file_handle);
//        os::readFromFile(&scale, sizeof(vec3), file_handle);
//    }
//};
