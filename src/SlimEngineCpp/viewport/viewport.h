#pragma once

#include "./navigation.h"
#include "./canvas.h"
#include "./hud.h"

struct Viewport {
    Camera *camera{nullptr};
    Dimensions dimensions;
    vec2i position{0, 0};
    Canvas canvas{dimensions, position};
    HUD hud;
    Navigation navigation;
    Frustum frustum;
    ProjectionPlane projection_plane;
    bool show_wire_frame{false};

    explicit Viewport(Camera *cam) {
        setCamera(*cam);
    }

    void setCamera(Camera &cam) {
        camera = &cam;
        updateProjectionPlane();
        updateProjectionMatrix();
    }

    void updateProjectionMatrix() {
        frustum.updateProjectionMatrix(camera->focal_length, dimensions.height_over_width);
    }

    void updateProjectionPlane() {
        projection_plane.update(camera->right,
                                camera->up,
                                camera->forward,
                                camera->focal_length,
                                dimensions.h_width,
                                dimensions.h_height);
    }

    void updateNavigation(f32 delta_time) {
        navigation.update(*camera, delta_time);
    }

    INLINE Ray getRayAt(const vec2i &coords) const {
        return {
                coords,
                camera->position,
                projection_plane
        };
    }
};