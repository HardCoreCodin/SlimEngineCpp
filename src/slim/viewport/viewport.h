#pragma once

#include "./navigation.h"
#include "./frustum.h"
#include "../draw/canvas.h"

#ifdef SLIM_ENABLE_VIEWPORT_BOX_DRAWING
#include "../core/transform.h"
#include "../scene/box.h"
#endif

#ifdef SLIM_ENABLE_VIEWPORT_GRID_DRAWING
#include "../core/transform.h"
#include "../scene/grid.h"
#endif

#ifdef SLIM_ENABLE_VIEWPORT_MESH_DRAWING
#include "../core/transform.h"
#include "../scene/mesh.h"
#endif

struct Viewport {
    Canvas &canvas;
    Camera *camera{nullptr};
    Frustum frustum;
    Dimensions dimensions;
    Navigation navigation;
    RectI bounds{};

    Viewport(Canvas &canvas, Camera *camera) : canvas{canvas} {
        dimensions = canvas.dimensions;
        bounds.left = bounds.top = 0;
        bounds.right = dimensions.width - 1;
        bounds.bottom = dimensions.height - 1;
        setCamera(*camera);
    }

    void setCamera(Camera &cam) {
        camera = &cam;
        updateProjection();
    }

    void updateProjection() {
        frustum.updateProjection(camera->focal_length, dimensions.height_over_width);
    }

    void updateDimensions(u16 width, u16 height) {
        i32 dx = width - dimensions.width;
        i32 dy = height - dimensions.height;
        dimensions.update(width, height);
        dimensions.stride += (u16)bounds.left;
        bounds.right += dx;
        bounds.bottom += dy;
        updateProjection();
    }

    void updateNavigation(f32 delta_time) {
        navigation.update(*camera, delta_time);
        updateProjection();
    }

    INLINE void projectEdge(Edge &edge) const {
        frustum.projectEdge(edge, dimensions);
    }

    INLINE bool cullAndClipEdge(Edge &edge) const {
        return frustum.cullAndClipEdge(edge, camera->focal_length, dimensions.width_over_height);
    }

#ifdef SLIM_ENABLE_VIEWPORT_BOX_DRAWING
    INLINE void drawBox(const Box &box, const Transform &transform, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1, u8 sides = BOX__ALL_SIDES) const;
#endif

#ifdef SLIM_ENABLE_VIEWPORT_CAMERA_DRAWING
    INLINE void drawCamera(const Camera &camera, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) const;
#endif

#ifdef SLIM_ENABLE_VIEWPORT_CURVE_DRAWING
    INLINE void drawCurve(const Curve &curve, const Transform &transform, const Color &color = White,
                          f32 opacity = 1.0f, u8 line_width = 0, u32 step_count = CURVE_STEPS) const;
#endif

#ifdef SLIM_ENABLE_VIEWPORT_EDGE_DRAWING
    INLINE void drawEdge(Edge edge, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) const;
#endif

#ifdef SLIM_ENABLE_VIEWPORT_GRID_DRAWING
    INLINE void drawGrid(const Grid &grid, const Transform &transform, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) const;
#endif

#ifdef SLIM_ENABLE_VIEWPORT_MESH_DRAWING
    INLINE void drawMesh(const Mesh &mesh, const Transform &transform, bool draw_normals, const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) const;
#endif
};