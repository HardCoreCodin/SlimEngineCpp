#pragma once

#include "./box.h"
#include "../math/utils.h"
#include "../scene/box.h"
#include "../viewport/viewport.h"

void _drawCamera(const Camera &camera, const Viewport &viewport, const Color &color, f32 opacity, u8 line_width) {
    static Transform transform;
    static Box box;

    transform.rotation = Quat(camera.rotation);
    transform.position = camera.position;
    transform.scale = 1.0f;

    box = Box{};
    drawBox(box, transform, viewport, color, opacity, line_width, BOX__ALL_SIDES);

    box.vertices.corners.back_bottom_left   *= 0.5f;
    box.vertices.corners.back_bottom_right  *= 0.5f;
    box.vertices.corners.back_top_left      *= 0.5f;
    box.vertices.corners.back_top_right     *= 0.5f;
    box.vertices.corners.front_bottom_left  *= 2.0f;
    box.vertices.corners.front_bottom_right *= 2.0f;
    box.vertices.corners.front_top_left     *= 2.0f;
    box.vertices.corners.front_top_right    *= 2.0f;

    for (auto &vertex : box.vertices.buffer)
        vertex.z += 1.5f;

    drawBox(box, transform, viewport, color, opacity, line_width, BOX__ALL_SIDES);
}

#ifdef SLIM_ENABLE_VIEWPORT_CAMERA_DRAWING
INLINE void Viewport::drawCamera(const Camera &camera, const Color &color, f32 opacity, u8 line_width) const {
    _drawCamera(camera, *this, color, opacity, line_width);
}
#endif

INLINE void drawCamera(const Camera &camera, const Viewport &viewport, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) {
    _drawCamera(camera, viewport, color, opacity, line_width);
}