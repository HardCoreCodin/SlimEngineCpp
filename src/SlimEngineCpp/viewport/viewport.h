#pragma once

#include "./navigation.h"
#include "./frustum.h"
#include "./canvas.h"
#include "./hud.h"

struct Viewport {
    Canvas &canvas;
    HUD hud{};
    Dimensions dimensions;
    vec2i position{0, 0};
    Navigation navigation;
    Frustum frustum;
    Camera *camera{nullptr};

    Viewport(Canvas &canvas, Camera *cam, HUD hud = {}) : canvas{canvas}, hud{hud} {
        dimensions = canvas.dimensions;
        setCamera(*cam);
    }

    void setCamera(Camera &cam) {
        camera = &cam;
        updateProjectionMatrix();
    }

    void updateDimensions(u16 width, u16 height) {
        dimensions.update(width, height);
        dimensions.stride += (u16)position.x;
        updateProjectionMatrix();
    }

    void updateProjectionMatrix() {
        frustum.updateProjectionMatrix(camera->focal_length, dimensions.height_over_width);
    }

    void updateNavigation(f32 delta_time) {
        navigation.update(*camera, delta_time);
    }

    INLINE Ray getRayAt(const vec2i &coords) const {
        vec3 &up = camera->rotation.up;
        vec3 &right = camera->rotation.right;
        vec3 &forward = camera->rotation.forward;
        vec3 start = up * (dimensions.h_height - 0.5f)
                + forward * (dimensions.h_height * camera->focal_length)
                + right * (0.5f - dimensions.h_width);
        return {
                camera->position,
                up.scaleAdd(-((f32)coords.y), right.scaleAdd((f32)coords.x, start)).normalized()
        };
    }

    INLINE bool cullAndClipEdge(Edge &edge) const {
        return frustum.cullAndClipEdge(edge, camera->focal_length, dimensions.width_over_height);
    }

    INLINE void projectEdge(Edge &edge) const {
        frustum.projectEdge(edge, dimensions);
    }
};