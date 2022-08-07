#pragma once

#include "../math/vec2.h"
#include "../math/mat3.h"

#include "./rtree.h"

struct EdgeVertexIndices {
    u32 from, to;
};

union TriangleVertexIndices {
    u32 ids[3];
    struct {
        u32 v1, v2, v3;
    };
};

enum TrianglePointOn {
    TrianglePointOn_None = 0,

    TrianglePointOn_Face,
    TrianglePointOn_Edge,
    TrianglePointOn_Vertex
};

struct Triangle {
    mat3 local_to_tangent;
    vec3 position, normal, U, V;

    TrianglePointOn findClosestPoint(const vec3 &point, f32 &closest_distance_so_far_squared, vec3 &closest_point_so_far) const {
        vec3 closest_point;
        vec3 tanget_space_point = local_to_tangent * (point - position);
        f32 u = tanget_space_point.x;
        f32 v = tanget_space_point.y;

        TrianglePointOn found_on;

        if (u < 0) { // Point is outside on the left, find the closest point on the left edge:
            // If ther point is below the triangle, choose the bottom-left corner
            // If ther point is above the triangle, choose the top-left corner
            // Otherwise it is within the height bounds of the triangle - find where on V
            closest_point = position + V * clampedValue(v);
            found_on = v < 0 || 1 < v ? TrianglePointOn_Vertex : TrianglePointOn_Edge;
        } else if (v < 0) { // Point is below the triangle, find the closest point on the bottom edge:
            // If the point is below the triangle on the right - choose the bottom-right corner
            // Otherwise it is within the width bounds of the triangle - find where on U
            closest_point = position + U * clampedValue(u);
            found_on = 1 < u ? TrianglePointOn_Vertex : TrianglePointOn_Edge;
        } else if ((u + v) <= 1) { // Point it over the triangle (within it's projection):
            closest_point = position + (U * u) + (V * v);
            found_on = TrianglePointOn_Face;
        } else if ((v - 1) >= u) { // Point is above the triangle's diagonal-edge projection - choose top corner:
            closest_point = position + V;
            found_on = TrianglePointOn_Vertex;
        } else if ((u - 1) >= v) { // Point is right of the triangle's diagonal-edge projection, choose right corner:
            closest_point = position + U;
            found_on = TrianglePointOn_Vertex;
        } else { // Point is above the triangle and to the right, above it's diagonal-edge projection:
            // Project the 2D vector from the top corner to the point onto a down-right diagonal normalized vector;
            // t = ([u, v] - [0, 1]) dot [0.707, -0.707]
            // t = [u - 0, v - 1] dot [0.707, -0.707]
            // t = u * 0.707 + (v - 1) * -0.707
            // t = u * 0.707 + -(v - 1) * 0.707
            // t = u * 0.707 + (1 - v) * 0.707
            // t = 0.707 * (u + 1 - v)
            // Then, starting from the top-left corner [0, 1] go towards the bottom right corner [1, 0] by t:
            // [u`, v`] = [0, 1] + t[1, -1];
            // u` = 0 + t * +1
            // v` = 1 + t * -1
            // u` = t
            // v` = 1 - t
            u = 0.70710678118f * (u + 1 - v);
            v = 1 - u;
            closest_point = position + U * u + V * v;
            found_on = TrianglePointOn_Edge;




//            // Scale it to be on the right or top edge of the square around the triangle:
//            if (v < u) { // Shallow slope, scale to be on the right edge:
//
//
//            }
            // Rotate the point in 2D an 1/8th of a turn counter-clockwise around the top-left corner of the triangle.
            // Then, take the x component of the rotated point (it's projection onto the rotated X axis)
            // That gives how far the projection of the point onto the diagonal edge is away from the top-left corner.
            // That then needs to be normalised
        }

        f32 distance_squared = (closest_point - point).squaredLength();
        if (distance_squared < closest_distance_so_far_squared) {
            closest_distance_so_far_squared = distance_squared;
            closest_point_so_far = closest_point;
        } else
            found_on = TrianglePointOn_None;

        return found_on;
    }
};


const vec3 CUBE_VERTEX_POSITIONS[CUBE_VERTEX_COUNT] = {
        {-1, -1, -1},
        {1, -1, -1},
        {1, 1, -1},
        {-1, 1, -1},
        {-1, -1, 1},
        {1, -1, 1},
        {1, 1, 1},
        {-1, 1, 1}
};

const TriangleVertexIndices CUBE_VERTEX_POSITION_INDICES[CUBE_TRIANGLE_COUNT] = {
        {0, 1, 2},
        {1, 5, 6},
        {5, 4, 7},
        {4, 0, 3},
        {3, 2, 6},
        {1, 0, 4},
        {0, 2, 3},
        {1, 6, 2},
        {5, 7, 6},
        {4, 3, 7},
        {3, 6, 7},
        {1, 4, 5}
};

const vec3 CUBE_VERTEX_NORMALS[CUBE_NORMAL_COUNT] = {
        {0, 0, -1},
        {1, 0, 0},
        {0, 0, 1},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0}
};

const TriangleVertexIndices CUBE_VERTEX_NORMAL_INDICES[CUBE_TRIANGLE_COUNT] = {
        {0, 0, 0},
        {1, 1, 1},
        {2, 2, 2},
        {3, 3, 3},
        {4, 4, 4},
        {5, 5, 5},
        {0, 0, 0},
        {1, 1, 1},
        {2, 2, 2},
        {3, 3, 3},
        {4, 4, 4},
        {5, 5, 5}
};

const vec2 CUBE_VERTEX_UVS[CUBE_UV_COUNT] = {
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
};

const TriangleVertexIndices CUBE_VERTEX_UV_INDICES[CUBE_TRIANGLE_COUNT] = {
        {0, 1, 2},
        {0, 1, 2},
        {0, 1, 2},
        {0, 1, 2},
        {0, 1, 2},
        {0, 1, 2},
        {0, 2, 3},
        {0, 2, 3},
        {0, 2, 3},
        {0, 2, 3},
        {0, 2, 3},
        {0, 2, 3}
};

struct Mesh {
    AABB aabb;
    RTree rtree;
    Triangle *triangles;

    vec3 *vertex_positions{nullptr};
    vec3 *vertex_normals{nullptr};
    vec2 *vertex_uvs{nullptr};

    TriangleVertexIndices *vertex_position_indices{nullptr};
    TriangleVertexIndices *vertex_normal_indices{nullptr};
    TriangleVertexIndices *vertex_uvs_indices{nullptr};

    EdgeVertexIndices *edge_vertex_indices{nullptr};

    u32 triangle_count{0};
    u32 vertex_count{0};
    u32 edge_count{0};
    u32 normals_count{0};
    u32 uvs_count{0};

    Mesh() = default;

    Mesh(u32 triangle_count,
         u32 vertex_count,
         u32 normals_count,
         u32 uvs_count,
         u32 edge_count,

         vec3 *vertex_positions,
         vec3 *vertex_normals,
         vec2 *vertex_uvs,

         TriangleVertexIndices *vertex_position_indices,
         TriangleVertexIndices *vertex_normal_indices,
         TriangleVertexIndices *vertex_uvs_indices,

         EdgeVertexIndices *edge_vertex_indices,
         AABB aabb
         ) :
            triangle_count{triangle_count},
            vertex_count{vertex_count},
            normals_count{normals_count},
            uvs_count{uvs_count},
            edge_count{edge_count},

            vertex_positions{vertex_positions},
            vertex_normals{vertex_normals},
            vertex_uvs{vertex_uvs},

            vertex_position_indices{vertex_position_indices},
            vertex_normal_indices{vertex_normal_indices},
            vertex_uvs_indices{vertex_uvs_indices},

            edge_vertex_indices{edge_vertex_indices},
            aabb{aabb}
         {}

    static Mesh CreateCube() {
        return {
                CUBE_TRIANGLE_COUNT,
                CUBE_VERTEX_COUNT,
                CUBE_NORMAL_COUNT,
                CUBE_UV_COUNT,
                0,

                (vec3*)CUBE_VERTEX_POSITIONS,
                (vec3*)CUBE_VERTEX_NORMALS,
                (vec2*)CUBE_VERTEX_UVS,

                (TriangleVertexIndices*)CUBE_VERTEX_POSITION_INDICES,
                (TriangleVertexIndices*)CUBE_VERTEX_NORMAL_INDICES,
                (TriangleVertexIndices*)CUBE_VERTEX_UV_INDICES,
                nullptr,

                {-1 , +1}
        };
    }
};

struct MeshQuery : RTreeQuery {
    TrianglePointOn findClosestPointOnMesh(Mesh &mesh, const vec3 &point, f32 max_distance, vec3 &closest_point) {
        RTree &rtree = mesh.rtree;
        RTreeNode *nodes = rtree.nodes;

        TrianglePointOn point_on = TrianglePointOn_None;
        if (collectOverlappedNodes(point, max_distance, nodes)) {
            f32 closest_distance_squared = max_distance * max_distance;
            for (u32 result_index = 0; result_index < node_count; result_index++) {
                const RTreeNode &node = nodes[node_ids[result_index]];
                for (u16 triangle_index_offset = 0; triangle_index_offset < node.child_count; triangle_index_offset++) {
                    Triangle &triangle = mesh.triangles[node.first_child_id + triangle_index_offset];
                    TrianglePointOn on = triangle.findClosestPoint(point, closest_distance_squared, closest_point);
                    if (on) point_on = on;
                }
            }
        }

        return point_on;
    }
};