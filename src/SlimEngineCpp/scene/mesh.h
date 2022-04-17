#pragma once

#include "../core/types.h"


struct EdgeVertexIndices {
    u32 from, to;
};

union TriangleVertexIndices {
    u32 ids[3];
    struct {
        u32 v1, v2, v3;
    };
};

struct Mesh {
    u32 triangle_count{0};
    u32 vertex_count{0};
    u32 normals_count{0};
    u32 uvs_count{0};
    u32 edge_count{0};

    vec3 *vertex_positions{nullptr};
    vec3 *vertex_normals{nullptr};
    vec2 *vertex_uvs{nullptr};

    TriangleVertexIndices *vertex_position_indices{nullptr};
    TriangleVertexIndices *vertex_normal_indices{nullptr};
    TriangleVertexIndices *vertex_uvs_indices{nullptr};

    EdgeVertexIndices *edge_vertex_indices{nullptr};
    AABB aabb;

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

    static u32 getMemorySize(char *file_path, Mesh &mesh) {
        void *file = os::openFileForReading(file_path);

        os::readFromFile(&mesh.aabb,           sizeof(AABB), file);
        os::readFromFile(&mesh.vertex_count,   sizeof(u32),  file);
        os::readFromFile(&mesh.triangle_count, sizeof(u32),  file);
        os::readFromFile(&mesh.edge_count,     sizeof(u32),  file);
        os::readFromFile(&mesh.uvs_count,      sizeof(u32),  file);
        os::readFromFile(&mesh.normals_count,  sizeof(u32),  file);

        u32 memory_size = sizeof(vec3) * mesh.vertex_count;
        memory_size += sizeof(TriangleVertexIndices) * mesh.triangle_count;
        memory_size += sizeof(EdgeVertexIndices) * mesh.edge_count;

        if (mesh.uvs_count) {
            memory_size += sizeof(vec2) * mesh.uvs_count;
            memory_size += sizeof(TriangleVertexIndices) * mesh.triangle_count;
        }
        if (mesh.normals_count) {
            memory_size += sizeof(vec3) * mesh.normals_count;
            memory_size += sizeof(TriangleVertexIndices) * mesh.triangle_count;
        }

        os::closeFile(file);

        return memory_size;
    }

    void load(char *file_path, memory::MonotonicAllocator *memory_allocator) {
        void *file = os::openFileForReading(file_path);

        vertex_normals          = null;
        vertex_normal_indices   = null;
        vertex_uvs              = null;
        vertex_uvs_indices      = null;

        os::readFromFile(&aabb,           sizeof(AABB), file);
        os::readFromFile(&vertex_count,   sizeof(u32),  file);
        os::readFromFile(&triangle_count, sizeof(u32),  file);
        os::readFromFile(&edge_count,     sizeof(u32),  file);
        os::readFromFile(&uvs_count,      sizeof(u32),  file);
        os::readFromFile(&normals_count,  sizeof(u32),  file);

        vertex_positions        = (vec3*                 )memory_allocator->allocate(sizeof(vec3)                  * vertex_count);
        vertex_position_indices = (TriangleVertexIndices*)memory_allocator->allocate(sizeof(TriangleVertexIndices) * triangle_count);
        edge_vertex_indices     = (EdgeVertexIndices*    )memory_allocator->allocate(sizeof(EdgeVertexIndices)     * edge_count);

        os::readFromFile(vertex_positions,             sizeof(vec3)                  * vertex_count,   file);
        os::readFromFile(vertex_position_indices,      sizeof(TriangleVertexIndices) * triangle_count, file);
        os::readFromFile(edge_vertex_indices,          sizeof(EdgeVertexIndices)     * edge_count,     file);
        if (uvs_count) {
            vertex_uvs         = (vec2*                 )memory_allocator->allocate(sizeof(vec2)                  * uvs_count);
            vertex_uvs_indices = (TriangleVertexIndices*)memory_allocator->allocate(sizeof(TriangleVertexIndices) * triangle_count);
            os::readFromFile(vertex_uvs,               sizeof(vec2)                  * uvs_count,      file);
            os::readFromFile(vertex_uvs_indices,       sizeof(TriangleVertexIndices) * triangle_count, file);
        }
        if (normals_count) {
            vertex_normals          = (vec3*                 )memory_allocator->allocate(sizeof(vec3)                  * normals_count);
            vertex_normal_indices   = (TriangleVertexIndices*)memory_allocator->allocate(sizeof(TriangleVertexIndices) * triangle_count);
            os::readFromFile(vertex_normals,                sizeof(vec3)                  * normals_count,  file);
            os::readFromFile(vertex_normal_indices,         sizeof(TriangleVertexIndices) * triangle_count, file);
        }

        os::closeFile(file);
    }

    void save(char* file_path) const {
        void *file = os::openFileForWriting(file_path);

        os::writeToFile((void*)&aabb,           sizeof(AABB), file);
        os::writeToFile((void*)&vertex_count,   sizeof(u32),  file);
        os::writeToFile((void*)&triangle_count, sizeof(u32),  file);
        os::writeToFile((void*)&edge_count,     sizeof(u32),  file);
        os::writeToFile((void*)&uvs_count,      sizeof(u32),  file);
        os::writeToFile((void*)&normals_count,  sizeof(u32),  file);
        os::writeToFile((void*)vertex_positions,        sizeof(vec3)                  * vertex_count,   file);
        os::writeToFile((void*)vertex_position_indices, sizeof(TriangleVertexIndices) * triangle_count, file);
        os::writeToFile((void*)edge_vertex_indices,     sizeof(EdgeVertexIndices)     * edge_count,     file);
        if (uvs_count) {
            os::writeToFile(vertex_uvs,          sizeof(vec2)                  * uvs_count,      file);
            os::writeToFile(vertex_uvs_indices,  sizeof(TriangleVertexIndices) * triangle_count, file);
        }
        if (normals_count) {
            os::writeToFile(vertex_normals,        sizeof(vec3)                  * normals_count,  file);
            os::writeToFile(vertex_normal_indices, sizeof(TriangleVertexIndices) * triangle_count, file);
        }

        os::closeFile(file);
    }

    static u32 getTotalMemoryForMeshes(String *mesh_files, u32 mesh_count) {
        u32 memory_size{0};
        Mesh mesh;
        for (u32 i = 0; i < mesh_count; i++) memory_size += Mesh::getMemorySize(mesh_files[i].char_ptr, mesh);
        return memory_size;
    }
};

struct CubeMesh : Mesh {
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

    CubeMesh() : Mesh{
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