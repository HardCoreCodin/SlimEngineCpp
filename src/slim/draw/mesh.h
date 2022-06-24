#pragma once

#include "./edge.h"
#include "../scene/mesh.h"
#include "../viewport/viewport.h"


void _drawMesh(const Mesh &mesh, const Transform &transform, bool draw_normals, const Viewport &viewport, const Color &color, f32 opacity, u8 line_width) {
    const Camera &cam = *viewport.camera;
    vec3 pos;
    Edge edge;
    EdgeVertexIndices *edge_index = mesh.edge_vertex_indices;
    for (u32 i = 0; i < mesh.edge_count; i++, edge_index++) {
        edge.from = cam.internPos(transform.externPos(mesh.vertex_positions[edge_index->from]));
        edge.to   = cam.internPos(transform.externPos(mesh.vertex_positions[edge_index->to]));
        drawEdge(edge, viewport, color, opacity, line_width);
    }

    if (draw_normals && mesh.normals_count && mesh.vertex_normals && mesh.vertex_normal_indices) {
        TriangleVertexIndices *normal_index = mesh.vertex_normal_indices;
        TriangleVertexIndices *position_index = mesh.vertex_position_indices;
        for (u32 t = 0; t < mesh.triangle_count; t++, normal_index++, position_index++) {
            for (u8 i = 0; i < 3; i++) {
                pos = mesh.vertex_positions[position_index->ids[i]];
                edge.to = mesh.vertex_normals[normal_index->ids[i]] * 0.1f + pos;
                edge.from = cam.internPos(transform.externPos(pos));
                edge.to = cam.internPos(transform.externPos(edge.to));
                drawEdge(edge, viewport, Red, opacity * 0.5f, line_width);
            }
        }
    }
}

#ifdef SLIM_ENABLE_VIEWPORT_MESH_DRAWING
INLINE void Viewport::drawMesh(const Mesh &mesh, const Transform &transform, bool draw_normals, const Color &color,
                               f32 opacity, u8 line_width) const {
    _drawMesh(mesh, transform, draw_normals, *this, color, opacity, line_width);
}
#endif

INLINE void drawMesh(const Mesh &mesh, const Transform &transform, bool draw_normals, const Viewport &viewport,
                     const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) {
    _drawMesh(mesh, transform, draw_normals, viewport, color, opacity, line_width);
}