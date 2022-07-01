#pragma once

#include "./box.h"
#include "../math/utils.h"
#include "../scene/box.h"

void drawCamera(const Camera &camera, const Viewport &viewport, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) {
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