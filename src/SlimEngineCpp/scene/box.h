#pragma once

#include "./edge.h"

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

    BoxVertices() : corners{} {};
    void toViewSpace(const Viewport &viewport, const Transform &xform, BoxVertices &transformed_vertices) const {
        const Camera &cam = *viewport.camera;
        for (u8 i = 0; i < BOX__VERTEX_COUNT; i++)
            transformed_vertices.buffer[i] = cam.internPos(xform.externPos(buffer[i]));
    }
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


    void draw(const Viewport &viewport, const Transform &transform, const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1, u8 sides = BOX__ALL_SIDES) const {
        static Box view_space_box;

        // Transform vertices positions from local-space to world-space and then to view-space:
        vertices.toViewSpace(viewport, transform, view_space_box.vertices);

        // Distribute transformed vertices positions to edges:
        view_space_box.edges.setFrom(view_space_box.vertices);

        if (sides == BOX__ALL_SIDES) {
            for (const auto &edge : edges.buffer)
                edge.draw(viewport, color, opacity, line_width);
        } else {
            BoxEdgeSides &box_edges = view_space_box.edges.sides;
            if (sides & Front | sides & Top   ) box_edges.front_top.draw(viewport, color, opacity, line_width);
            if (sides & Front | sides & Bottom) box_edges.front_bottom.draw(viewport, color, opacity, line_width);
            if (sides & Front | sides & Left  ) box_edges.front_left.draw(viewport, color, opacity, line_width);
            if (sides & Front | sides & Right ) box_edges.front_right.draw(viewport, color, opacity, line_width);
            if (sides & Back  | sides & Top   ) box_edges.back_top.draw(viewport, color, opacity, line_width);
            if (sides & Back  | sides & Bottom) box_edges.back_bottom.draw(viewport, color, opacity, line_width);
            if (sides & Back  | sides & Left  ) box_edges.back_left.draw(viewport, color, opacity, line_width);
            if (sides & Back  | sides & Right ) box_edges.back_right.draw(viewport, color, opacity, line_width);
            if (sides & Left  | sides & Top   ) box_edges.left_top.draw(viewport, color, opacity, line_width);
            if (sides & Left  | sides & Bottom) box_edges.left_bottom.draw(viewport, color, opacity, line_width);
            if (sides & Right | sides & Top   ) box_edges.right_top.draw(viewport, color, opacity, line_width);
            if (sides & Right | sides & Bottom) box_edges.right_bottom.draw(viewport, color, opacity, line_width);
        }
    }

    static void drawCamera(Camera &camera, const Viewport &viewport, const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1) {
        static Box box;
        static Transform transform;

        transform.rotation = camera.rotation;
        transform.position = camera.position;
        transform.scale = 1.0f;

        box.draw(viewport, transform, color, opacity, line_width, BOX__ALL_SIDES);

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

        box.draw(viewport, transform, color, opacity, line_width, BOX__ALL_SIDES);
    }
};
