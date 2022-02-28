#pragma once

#include "./mesh.h"

static const vec3 CUBE_VERTEX_POSITIONS[CUBE_VERTEX_COUNT] = {
        {-1, -1, -1},
        {1, -1, -1},
        {1, 1, -1},
        {-1, 1, -1},
        {-1, -1, 1},
        {1, -1, 1},
        {1, 1, 1},
        {-1, 1, 1}
};

static const TriangleVertexIndices CUBE_VERTEX_POSITION_INDICES[CUBE_TRIANGLE_COUNT] = {
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

static const vec3 CUBE_VERTEX_NORMALS[CUBE_NORMAL_COUNT] = {
        {0, 0, -1},
        {1, 0, 0},
        {0, 0, 1},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0}
};
static const TriangleVertexIndices CUBE_VERTEX_NORMAL_INDICES[CUBE_TRIANGLE_COUNT] = {
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

static const vec2 CUBE_VERTEX_UVS[CUBE_UV_COUNT] = {
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
};
static const TriangleVertexIndices CUBE_VERTEX_UV_INDICES[CUBE_TRIANGLE_COUNT] = {
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

struct Cube : Mesh {
    Cube() : Mesh{
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
        } {}
};