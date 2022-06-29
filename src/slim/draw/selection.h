#pragma once

#ifndef SLIM_ENABLE_VIEWPORT_BOX_DRAWING
#define SLIM_ENABLE_VIEWPORT_BOX_DRAWING
#endif

#include "./box.h"
#include "../scene/scene.h"
#include "../scene/selection.h"
#include "../viewport/viewport.h"

void drawSelection(Selection &selection, const Viewport &viewport, const Scene &scene) {
    static Box box;

    if (controls::is_pressed::alt && !mouse::is_captured && selection.geo_type && selection.geometry) {
        selection.xform = selection.geometry->transform;
        if (selection.geometry->type == GeometryType_Mesh)
            selection.xform.scale *= scene.meshes[selection.geometry->id].aabb.max;

        viewport.drawBox(box, selection.xform, Yellow, 0.5f, 0);
        if (selection.box_side) {
            ColorID color = White;
            switch (selection.box_side) {
                case BoxSide_Left:  case BoxSide_Right: color = Red;   break;
                case BoxSide_Top:   case BoxSide_Bottom: color = Green; break;
                case BoxSide_Front: case BoxSide_Back: color = Blue;  break;
                case BoxSide_None: break;
            }

            viewport.drawBox(box, selection.xform, color, 0.5f, 1, selection.box_side);
        }
    }
}
