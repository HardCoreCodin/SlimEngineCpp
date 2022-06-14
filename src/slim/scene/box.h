#pragma once

#include "../math/vec3.h"

struct BoxCorners {
    vec3 front_top_left;
    vec3 front_top_right;
    vec3 front_bottom_left;
    vec3 front_bottom_right;
    vec3 back_top_left;
    vec3 back_top_right;
    vec3 back_bottom_left;
    vec3 back_bottom_right;

    BoxCorners() :
    front_top_left{-1, 1, 1},
    front_top_right{1, 1, 1},
    front_bottom_left{-1, -1, 1},
    front_bottom_right{1, -1, 1},
    back_top_left{-1, 1, -1},
    back_top_right{1, 1, -1},
    back_bottom_left{-1, -1, -1},
    back_bottom_right{1, -1, -1}
    {}
};

union BoxVertices {
    BoxCorners corners;
    vec3 buffer[BOX__VERTEX_COUNT];
};

struct BoxEdgeSides {
    Edge front_top,
         front_bottom,
         front_left,
         front_right,
         back_top,
         back_bottom,
         back_left,
         back_right,
         left_bottom,
         left_top,
         right_bottom,
         right_top;

    explicit BoxEdgeSides(const BoxCorners &corners) { setFrom(corners); }
    explicit BoxEdgeSides(const BoxVertices &vertices) : BoxEdgeSides(vertices.corners) {}

    void setFrom(const BoxVertices &vertices) {
        setFrom(vertices.corners);
    }

    void setFrom(const BoxCorners &corners) {
        front_top.from    = corners.front_top_left;
        front_top.to      = corners.front_top_right;
        front_bottom.from = corners.front_bottom_left;
        front_bottom.to   = corners.front_bottom_right;
        front_left.from   = corners.front_bottom_left;
        front_left.to     = corners.front_top_left;
        front_right.from  = corners.front_bottom_right;
        front_right.to    = corners.front_top_right;

        back_top.from     = corners.back_top_left;
        back_top.to       = corners.back_top_right;
        back_bottom.from  = corners.back_bottom_left;
        back_bottom.to    = corners.back_bottom_right;
        back_left.from    = corners.back_bottom_left;
        back_left.to      = corners.back_top_left;
        back_right.from   = corners.back_bottom_right;
        back_right.to     = corners.back_top_right;

        left_bottom.from  = corners.front_bottom_left;
        left_bottom.to    = corners.back_bottom_left;
        left_top.from     = corners.front_top_left;
        left_top.to       = corners.back_top_left;
        right_bottom.from = corners.front_bottom_right;
        right_bottom.to   = corners.back_bottom_right;
        right_top.from    = corners.front_top_right;
        right_top.to      = corners.back_top_right;
    }
};

union BoxEdges {
    BoxEdgeSides sides;
    Edge buffer[BOX__EDGE_COUNT];

    explicit BoxEdges(const BoxEdgeSides &sides) : sides{sides} {}
    explicit BoxEdges(const BoxVertices &vertices) : sides{vertices} {}
    explicit BoxEdges(const BoxCorners &corners) : sides{corners} {}

    void setFrom(const BoxVertices &vertices) {
        sides.setFrom(vertices);
    }

    void setFrom(const BoxCorners &corners) {
        sides.setFrom(corners);
    }
};

struct Box {
    BoxVertices vertices;
    BoxEdges edges;

    Box() : vertices{}, edges{vertices} {}
};
