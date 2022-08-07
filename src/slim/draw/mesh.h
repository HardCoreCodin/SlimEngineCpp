#pragma once

#include "./edge.h"
#include "../scene/mesh.h"


void drawMesh(const Mesh &mesh, const Transform &transform, bool draw_normals, const Viewport &viewport,
              const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) {
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

void drawMeshRTreeQuery(const Mesh &mesh, const RTreeQuery *query, const Transform &transform, const Viewport &viewport,
                        const Color &color = White, f32 opacity = 1.0f, u8 line_width = 1) {
    const Camera &cam = *viewport.camera;
    vec3 pos;
    Edge edge;
    for (u32 result_index = 0; result_index < query->node_count; result_index++) {
        const RTreeNode &node = mesh.rtree.nodes[query->node_ids[result_index]];
        for (u16 i = 0; i < node.child_count; i++) {
            Triangle &triangle = mesh.triangles[node.first_child_id + i];
            pos = cam.internPos(transform.externPos(triangle.position));
            edge.from = pos;
            edge.to =  cam.internPos(transform.externPos(triangle.position + triangle.U));
            edge.to.z -= 0.01;
            drawEdge(edge, viewport, color, opacity, line_width);

            edge.from = cam.internPos(transform.externPos(triangle.position + triangle.V));
            edge.from.z -= 0.01;
            drawEdge(edge, viewport, color, opacity, line_width);

            edge.to = pos;
            edge.to.z -= 0.01;
            drawEdge(edge, viewport, color, opacity, line_width);
        }
    }
}