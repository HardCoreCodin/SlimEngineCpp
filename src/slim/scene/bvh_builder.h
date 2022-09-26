#pragma once

#include "./mesh.h"

struct BVHPartitionSide {
    AABB *aabbs;
    f32 *surface_areas;
};

struct BVHPartition {
    BVHPartitionSide left, right;
    u32 left_node_count, *sorted_node_ids;
    f32 surface_area;

    void partition(u8 axis, BVHNode *nodes, i32 *stack, u32 N) {
        u32 current_index, next_index, left_index, right_index;
        f32 current_surface_area;
        left_index = 0;
        right_index = N - 1;


        // Sort nodes by axis:
        {
            i32 start = 0;
            i32 end = (i32) N - 1;
            i32 top = -1;

            // push initial values of start and end to stack
            stack[++top] = start;
            stack[++top] = end;

            // Keep popping from stack while is not empty
            while (top >= 0) {
                // Pop h and l
                end = stack[top--];
                start = stack[top--];

                // Partition nodes by axis:
                // Set pivot element at its correct position in sorted array
                i32 pIndex = start;
                {
                    i32 pivot = (i32)sorted_node_ids[end];
                    i32 t;
                    bool do_swap;

                    for (i32 i = start; i < end; i++) {
                        switch (axis) {
                            case 0: do_swap = nodes[sorted_node_ids[i]].aabb.max.x < nodes[pivot].aabb.max.x; break;
                            case 1: do_swap = nodes[sorted_node_ids[i]].aabb.max.y < nodes[pivot].aabb.max.y; break;
                            case 2: do_swap = nodes[sorted_node_ids[i]].aabb.max.z < nodes[pivot].aabb.max.z; break;
                        }
                        if (do_swap) {
                            t = (i32)sorted_node_ids[i];
                            sorted_node_ids[i] = sorted_node_ids[pIndex];
                            sorted_node_ids[pIndex] = t;
                            pIndex++;
                        }
                    }
                    t = (i32)sorted_node_ids[end];
                    sorted_node_ids[end] = sorted_node_ids[pIndex];
                    sorted_node_ids[pIndex] = t;
                }

                // If there are elements on left side of pivot,
                // then push left side to stack
                if (pIndex - 1 > start) {
                    stack[++top] = start;
                    stack[++top] = pIndex - 1;
                }

                // If there are elements on right side of pivot,
                // then push right side to stack
                if (pIndex + 1 < end) {
                    stack[++top] = pIndex + 1;
                    stack[++top] = end;
                }
            }
        }

        AABB L = nodes[sorted_node_ids[left_index]].aabb;
        AABB R = nodes[sorted_node_ids[right_index]].aabb;

        left.aabbs[left_index]   = L;
        right.aabbs[right_index] = R;

        for (left_index = 0; left_index < N; left_index++, right_index--) {
            if (left_index) {
                L += nodes[sorted_node_ids[left_index ]].aabb;
                R += nodes[sorted_node_ids[right_index]].aabb;

                left.aabbs[left_index] = L;
                right.aabbs[right_index] = R;
            }

            left.surface_areas[  left_index] = L.area();
            right.surface_areas[right_index] = R.area();
        }

        u32 last_index = right_index = N - 1;
        left_node_count = left_index = next_index = 1;
        surface_area = INFINITY;

        for (current_index = 0; current_index < last_index; current_index++, next_index++, left_index++, right_index--) {
            current_surface_area = (
                    left.surface_areas[current_index] * (f32)left_index +
                    right.surface_areas[  next_index] * (f32)right_index
            );
            if (surface_area > current_surface_area) {
                surface_area = current_surface_area;
                left_node_count = next_index;
            }
        }
    }
};

struct BVHBuildIteration {
    u32 start, end, node_id;
    u8 depth;
};

constexpr f32 EPS = 0.0001f;
constexpr i32 MAX_TRIANGLES_PER_MESH_RTREE_NODE = 4;

struct BVHBuilder {
    BVHNode *nodes;
    BVHPartition partitions[3];
    BVHBuildIteration *iterations;
    u32 *node_ids, *leaf_ids;
    i32 *sort_stack;

    static u32 getSizeInBytes(u32 max_leaf_count) {
        u32 memory_size = sizeof(u32) + sizeof(i32) + 2 * (sizeof(AABB) + sizeof(f32));
        memory_size *= 3;
        memory_size += sizeof(BVHBuildIteration) + sizeof(BVHNode) + sizeof(u32) * 2;
        memory_size *= max_leaf_count;

        return memory_size;
    }

    BVHBuilder(Mesh *meshes, u32 mesh_count, memory::MonotonicAllocator *memory_allocator) {
        u32 max_leaf_node_count = 0;
        if (mesh_count)
            for (u32 m = 0; m < mesh_count; m++)
                if (meshes[m].triangle_count > max_leaf_node_count)
                    max_leaf_node_count = meshes[m].triangle_count;

        iterations = (BVHBuildIteration*)memory_allocator->allocate(sizeof(BVHBuildIteration) * max_leaf_node_count);
        nodes      = (BVHNode*          )memory_allocator->allocate(sizeof(BVHNode)           * max_leaf_node_count);
        node_ids   = (u32*              )memory_allocator->allocate(sizeof(u32)                 * max_leaf_node_count);
        leaf_ids   = (u32*              )memory_allocator->allocate(sizeof(u32)                 * max_leaf_node_count);
        sort_stack = (i32*              )memory_allocator->allocate(sizeof(i32)                 * max_leaf_node_count);

        for (u8 i = 0; i < 3; i++) {
            partitions[i].sorted_node_ids     = (u32* )memory_allocator->allocate(sizeof(u32)  * max_leaf_node_count);
            partitions[i].left.aabbs          = (AABB*)memory_allocator->allocate(sizeof(AABB) * max_leaf_node_count);
            partitions[i].right.aabbs         = (AABB*)memory_allocator->allocate(sizeof(AABB) * max_leaf_node_count);
            partitions[i].left.surface_areas  = (f32* )memory_allocator->allocate(sizeof(f32)  * max_leaf_node_count);
            partitions[i].right.surface_areas = (f32* )memory_allocator->allocate(sizeof(f32)  * max_leaf_node_count);
        }
    }

    u32 splitNode(BVHNode &node, u32 start, u32 end, BVH &bvh) {
        u32 N = end - start;
        u32 *ids = node_ids + start;

        node.first_index = bvh.node_count;
        BVHNode &left_node  = bvh.nodes[bvh.node_count++];
        BVHNode &right_node = bvh.nodes[bvh.node_count++];
        left_node = BVHNode{};
        right_node = BVHNode{};

        f32 smallest_surface_area = INFINITY;
        u8 chosen_axis = 0;

        for (u8 axis = 0; axis < 3; axis++) {
            BVHPartition &pa = partitions[axis];
            for (u32 i = 0; i < N; i++) pa.sorted_node_ids[i] = ids[i];

            // Partition the nodes for the current partition axis:
            pa.partition(axis, nodes, sort_stack, N);

            // Choose the current partition axis if it's smallest surface area is smallest so far:
            if (pa.surface_area < smallest_surface_area) {
                smallest_surface_area = pa.surface_area;
                chosen_axis = axis;
            }
        }

        BVHPartition &chosen_partition_axis = partitions[chosen_axis];
        left_node.aabb  = chosen_partition_axis.left.aabbs[chosen_partition_axis.left_node_count-1];
        right_node.aabb = chosen_partition_axis.right.aabbs[chosen_partition_axis.left_node_count];

        for (u32 i = 0; i < N; i++) ids[i] = chosen_partition_axis.sorted_node_ids[i];

        return start + chosen_partition_axis.left_node_count;
    }

    void build(BVH &bvh, u32 N, u16 max_leaf_size) {
        bvh.height = 1;
        bvh.node_count = 1;

        BVHNode &root = bvh.nodes[0];
        root = BVHNode{};

        if (N <= max_leaf_size) {
            root.leaf_count = (u16)N;
            root.aabb.min = INFINITY;
            root.aabb.max = -INFINITY;

            BVHNode *builder_node = nodes;
            for (u32 i = 0; i < N; i++, builder_node++) {
                leaf_ids[i] = builder_node->first_index;
                root.aabb += builder_node->aabb;
            }

            return;
        }

        u32 middle = splitNode(root, 0, N, bvh);
        BVHBuildIteration *stack = iterations;
        u32 *node_id;

        BVHBuildIteration left{0, middle, 1, 1};
        BVHBuildIteration right{middle, N, 2, 1};

        stack[0] = left;
        stack[1] = right;

        i32 stack_size = 1;
        u32 leaf_count = 0;;

        while (stack_size >= 0) {
            left = stack[stack_size];
            BVHNode &node = bvh.nodes[left.node_id];
            N = left.end - left.start;
            if (N <= max_leaf_size) {
                node.depth = left.depth;
                node.leaf_count = (u16)N;
                node.first_index = leaf_count;

                node_id = node_ids + left.start;
                for (u32 i = 0; i < N; i++, node_id++)
                    leaf_ids[leaf_count + i] = nodes[*node_id].first_index;
                leaf_count += N;
                stack_size--;
            } else {
                middle = splitNode(node, left.start, left.end, bvh);
                left.depth++;
                right.depth = left.depth;
                right.end = left.end;
                right.start = left.end = middle;
                left.node_id  = node.first_index;
                right.node_id = node.first_index + 1;
                bvh.nodes[bvh.node_count - 1].depth = left.depth;
                bvh.nodes[bvh.node_count - 2].depth = right.depth;
                stack[  stack_size] = left;
                stack[++stack_size] = right;
                if (left.depth > bvh.height) bvh.height = left.depth;
            }
        }

        BVHNode &left_node = bvh.nodes[1];
        BVHNode &right_node = bvh.nodes[2];
        left_node.depth = right_node.depth = 1;
        root.aabb = left_node.aabb + right_node.aabb;
    }

    void buildMesh(Mesh &mesh) {
        for (u32 i = 0; i < mesh.triangle_count; i++) {
            TriangleVertexIndices &indices = mesh.vertex_position_indices[i];
            const vec3 &v1 = mesh.vertex_positions[indices.ids[0]];
            const vec3 &v2 = mesh.vertex_positions[indices.ids[1]];
            const vec3 &v3 = mesh.vertex_positions[indices.ids[2]];
            BVHNode &node = nodes[i];
            vec3 &min = node.aabb.min;
            vec3 &max = node.aabb.max;

            min = minimum(minimum(v1, v2), v3);
            max = maximum(maximum(v1, v2), v3);

            f32 diff = max.x - min.x;
            if (diff < 0) diff = -diff;
            if (diff < EPS) {
                min.x -= EPS;
                max.x += EPS;
            }

            diff = max.y - min.y;
            if (diff < 0) diff = -diff;
            if (diff < EPS) {
                min.y -= EPS;
                max.y += EPS;
            }

            diff = max.z - min.z;
            if (diff < 0) diff = -diff;
            if (diff < EPS) {
                min.z -= EPS;
                max.z += EPS;
            }

            node.first_index = node_ids[i] = i;
        }

        build(mesh.bvh, mesh.triangle_count, MAX_TRIANGLES_PER_MESH_RTREE_NODE);

        for (u32 i = 0; i < mesh.triangle_count; i++) {
            Triangle &triangle = mesh.triangles[i];
            TriangleVertexIndices &indices = mesh.vertex_position_indices[leaf_ids[i]];
            const vec3 &v1 = mesh.vertex_positions[indices.ids[0]];
            const vec3 &v2 = mesh.vertex_positions[indices.ids[1]];
            const vec3 &v3 = mesh.vertex_positions[indices.ids[2]];

            triangle.U = v3 - v1;
            triangle.V = v2 - v1;
            triangle.normal = triangle.U.cross(triangle.V).normalized();
            triangle.position = v1;
            triangle.local_to_tangent.X = triangle.U;
            triangle.local_to_tangent.Y = triangle.V;
            triangle.local_to_tangent.Z = triangle.normal;
            triangle.local_to_tangent = triangle.local_to_tangent.inverted();
        }
    }
};