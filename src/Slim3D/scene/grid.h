#pragma once

#include "./edge.h"

struct GridAxisVertices {
    vec3 from[GRID__MAX_SEGMENTS];
    vec3 to[  GRID__MAX_SEGMENTS];

    explicit GridAxisVertices(u8 segments, bool is_u) {
        update(segments, is_u);
    }

    bool update(u8 segments, bool is_u) {
        if (!segments || segments > GRID__MAX_SEGMENTS) return false;

        f32 step = segments > 1 ? (2.0f / ((f32)segments - 1.0f)) : 0.0f;
        for (u8 i = 0; i < segments; i++) {
            from[i].y = to[i].y = 0.0f;
            if (is_u) {
                from[i].x = to[i].x = -1.0f + (f32)i * step;
                from[i].z = -1;
                to[  i].z = +1;
            } else {
                from[i].z = to[i].z = -1.0f + (f32)i * step;
                from[i].x = -1;
                to[  i].x = +1;
            }
        }

        return true;
    }
};

union GridVertices {
    vec3 buffer[2][2][GRID__MAX_SEGMENTS];
    struct {
        GridAxisVertices u, v;
    };

    GridVertices(u8 U_segments = GRID__MAX_SEGMENTS, u8 V_segments = GRID__MAX_SEGMENTS) : u{U_segments, true}, v{V_segments, false} {}

    bool update(u8 U_segments, u8 V_segments) {
        return u.update(U_segments, true) && v.update(V_segments, false);
    }

    void toViewSpace(const Viewport &viewport, const Transform &xform, u8 U_segments, u8 V_segments, GridVertices &transformed_vertices) const {
        const Camera &cam = *viewport.camera;
        for (u8 side = 0; side < 2; side++) {
            for (u8 axis = 0; axis < 2; axis++) {
                u8 segment_count = axis ? V_segments : U_segments;
                for (u8 segment = 0; segment < segment_count; segment++)
                    transformed_vertices.buffer[axis][side][segment] = (cam.internPos(xform.externPos(buffer[axis][side][segment])));
            }
        }
    }
};

struct GridAxisEdges {
    Edge edges[GRID__MAX_SEGMENTS];

    GridAxisEdges(const GridAxisVertices &vertices, u8 segments) {
        update(vertices, segments);
    }

    bool update(const GridAxisVertices &vertices, u8 segments) {
        if (segments > GRID__MAX_SEGMENTS) return false;

        for (u8 i = 0; i < segments; i++) {
            edges[i].from = vertices.from[i];
            edges[i].to   = vertices.to[i];
        }

        return true;
    }
};

union GridEdges {
    Edge buffer[2][GRID__MAX_SEGMENTS];

    struct {
        GridAxisEdges u, v;
    };

    GridEdges(const GridVertices &vertices, u8 u_segments, u8 v_segments) {
        update(vertices, u_segments, v_segments);
    }

    bool update(const GridVertices &vertices, u8 u_segments, u8 v_segments) {
        return update(vertices.u, vertices.v, u_segments, v_segments);
    }

    bool update(const GridAxisVertices &u_vertices, const GridAxisVertices &v_vertices, u8 u_segments, u8 v_segments) {
        if (u_segments > GRID__MAX_SEGMENTS || v_segments > GRID__MAX_SEGMENTS) return false;

        u.update(u_vertices, u_segments);
        v.update(v_vertices, v_segments);

        return true;
    }
};

struct Grid {
    u8 u_segments;
    u8 v_segments;
    GridVertices vertices;
    GridEdges edges;

    Grid(u8 u_segments = GRID__MAX_SEGMENTS, u8 v_segments = GRID__MAX_SEGMENTS) :
            u_segments{u_segments},
            v_segments{v_segments},
            vertices{u_segments, v_segments},
            edges{vertices, u_segments, v_segments} {}

    bool update(u8 U_segments, u8 V_segments) {
        if (!U_segments || U_segments > GRID__MAX_SEGMENTS ||
            !V_segments || V_segments > GRID__MAX_SEGMENTS)
            return false;

        u_segments = U_segments;
        v_segments = V_segments;

        vertices.update(U_segments, V_segments);
        edges.update(vertices, U_segments, V_segments);

        return true;
    }

    void draw(const Viewport &viewport, const Transform &transform, const vec3 &color = Color(White), f32 opacity = 1.0f, u8 line_width = 1) const {
        static Grid view_space_grid;

        // Transform vertices positions from local-space to world-space and then to view-space:
        vertices.toViewSpace(viewport, transform, u_segments, v_segments, view_space_grid.vertices);

        // Distribute transformed vertices positions to edges:
        view_space_grid.edges.update(view_space_grid.vertices, u_segments, v_segments);

        for (u8 u = 0; u < u_segments; u++) view_space_grid.edges.u.edges[u].draw(viewport, color, opacity, line_width);
        for (u8 v = 0; v < v_segments; v++) view_space_grid.edges.v.edges[v].draw(viewport, color, opacity, line_width);
    }
};