#pragma once

#include "./box.h"
#include "../scene/scene.h"
#include "../scene/selection.h"
#include "../viewport/viewport.h"

void draw(Selection &selection, const Viewport &viewport, const Scene &scene) {
    static Box box;

    if (controls::is_pressed::alt && !mouse::is_captured && selection.geo_type && selection.geometry) {
        selection.xform = selection.geometry->transform;
        if (selection.geometry->type == GeometryType_Mesh)
            selection.xform.scale *= scene.meshes[selection.geometry->id].aabb.max;

        draw(box, selection.xform, viewport, Color(Yellow), 0.5f, 0);
        if (selection.box_side) {
            vec3 color = Color(White);
            switch (selection.box_side) {
                case Left:  case Right:  color = Color(Red);   break;
                case Top:   case Bottom: color = Color(Green); break;
                case Front: case Back:   color = Color(Blue);  break;
                case NoSide: break;
            }

            draw(box, selection.xform, viewport, color, 0.5f, 1, selection.box_side);
        }
    }
}
