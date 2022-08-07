#pragma once

#include "./mesh.h"

struct RTreePartitionSide {
    AABB *aabbs;
    f32 *surface_areas;
};

struct RTreePartitionAxis {
    RTreePartitionSide left, right;
    u32 left_node_count, *sorted_leaf_ids;
    f32 smallest_surface_area;

    void partitionBVHNodeIDs(u8 axis, RTreeNode *nodes, i32 *stack, u32 N) {
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
                    i32 pivot = (i32)sorted_leaf_ids[end];
                    i32 t;
                    bool do_swap;

                    for (i32 i = start; i < end; i++) {
                        switch (axis) {
                            case 0: do_swap = nodes[sorted_leaf_ids[i]].aabb.max.x < nodes[pivot].aabb.max.x; break;
                            case 1: do_swap = nodes[sorted_leaf_ids[i]].aabb.max.y < nodes[pivot].aabb.max.y; break;
                            case 2: do_swap = nodes[sorted_leaf_ids[i]].aabb.max.z < nodes[pivot].aabb.max.z; break;
                        }
                        if (do_swap) {
                            t = (i32)sorted_leaf_ids[i];
                            sorted_leaf_ids[i] = sorted_leaf_ids[pIndex];
                            sorted_leaf_ids[pIndex] = t;
                            pIndex++;
                        }
                    }
                    t = (i32)sorted_leaf_ids[end];
                    sorted_leaf_ids[end] = sorted_leaf_ids[pIndex];
                    sorted_leaf_ids[pIndex] = t;
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

        AABB L = nodes[sorted_leaf_ids[left_index]].aabb;
        AABB R = nodes[sorted_leaf_ids[right_index]].aabb;

        left.aabbs[left_index]   = L;
        right.aabbs[right_index] = R;

        for (left_index = 0; left_index < N; left_index++, right_index--) {
            if (left_index) {
                L += nodes[sorted_leaf_ids[left_index ]].aabb;
                R += nodes[sorted_leaf_ids[right_index]].aabb;

                left.aabbs[left_index] = L;
                right.aabbs[right_index] = R;
            }

            left.surface_areas[  left_index] = L.area();
            right.surface_areas[right_index] = R.area();
        }

        u32 last_index = right_index = N - 1;
        left_node_count = left_index = next_index = 1;
        smallest_surface_area = INFINITY;

        for (current_index = 0; current_index < last_index; current_index++, next_index++, left_index++, right_index--) {
            current_surface_area = (
                    left.surface_areas[current_index] * (f32)left_index +
                    right.surface_areas[  next_index] * (f32)right_index
            );
            if (smallest_surface_area > current_surface_area) {
                smallest_surface_area = current_surface_area;
                left_node_count = next_index;
            }
        }
    }
};

struct RTreeBuildIteration {
    u32 start, end, node_id, depth;
};

constexpr f32 EPS = 0.0001f;
constexpr i32 MAX_TRIANGLES_PER_MESH_RTREE_NODE = 4;

struct RTreeBuilder {
    RTreeNode *leaf_nodes;
    RTreePartitionAxis partition_axis[3];
    RTreeBuildIteration *build_iterations;
    u32 *leaf_ids;
    i32 *sort_stack;

    static u32 getSizeInBytes(u32 max_leaf_count) {
        u32 memory_size = sizeof(u32) + sizeof(i32) + 2 * (sizeof(AABB) + sizeof(f32));
        memory_size *= 3;
        memory_size += sizeof(RTreeBuildIteration) + sizeof(RTreeNode) + sizeof(u32);
        memory_size *= max_leaf_count;

        return memory_size;
    }

    RTreeBuilder(Mesh *meshes, u32 mesh_count, memory::MonotonicAllocator *memory_allocator) {
        u32 max_leaf_node_count = 0;
        if (mesh_count)
            for (u32 m = 0; m < mesh_count; m++)
                if (meshes[m].triangle_count > max_leaf_node_count)
                    max_leaf_node_count = meshes[m].triangle_count;

        build_iterations = (RTreeBuildIteration*)memory_allocator->allocate(sizeof(RTreeBuildIteration) * max_leaf_node_count);
        leaf_nodes       = (RTreeNode*          )memory_allocator->allocate(sizeof(RTreeNode)      * max_leaf_node_count);
        leaf_ids         = (u32*                )memory_allocator->allocate(sizeof(u32)            * max_leaf_node_count);
        sort_stack       = (i32*                )memory_allocator->allocate(sizeof(i32)            * max_leaf_node_count);

        for (u8 i = 0; i < 3; i++) {
            partition_axis[i].sorted_leaf_ids     = (u32* )memory_allocator->allocate(sizeof(u32)  * max_leaf_node_count);
            partition_axis[i].left.aabbs          = (AABB*)memory_allocator->allocate(sizeof(AABB) * max_leaf_node_count);
            partition_axis[i].right.aabbs         = (AABB*)memory_allocator->allocate(sizeof(AABB) * max_leaf_node_count);
            partition_axis[i].left.surface_areas  = (f32* )memory_allocator->allocate(sizeof(f32)  * max_leaf_node_count);
            partition_axis[i].right.surface_areas = (f32* )memory_allocator->allocate(sizeof(f32)  * max_leaf_node_count);
        }
    }

    u32 splitNode(RTreeNode &node, u32 start, u32 end, RTree &rtree) {
        u32 N = end - start;
        u32 *ids = leaf_ids + start;

        node.first_child_id = rtree.node_count;
        RTreeNode &left_node  = rtree.nodes[rtree.node_count++];
        RTreeNode &right_node = rtree.nodes[rtree.node_count++];
        left_node = RTreeNode{};
        right_node = RTreeNode{};

        f32 smallest_surface_area = INFINITY;
        u8 chosen_axis = 0;

        for (u8 axis = 0; axis < 3; axis++) {
            RTreePartitionAxis &pa = partition_axis[axis];
            for (u32 i = 0; i < N; i++) pa.sorted_leaf_ids[i] = ids[i];

            // Partition the BVH's primitive-ids for the current partition axis:
            pa.partitionBVHNodeIDs(axis, leaf_nodes, sort_stack, N);

            // Choose the current partition axis if it's smallest surface area is smallest so far:
            if (pa.smallest_surface_area < smallest_surface_area) {
                smallest_surface_area = pa.smallest_surface_area;
                chosen_axis = axis;
            }
        }

        RTreePartitionAxis &chosen_partition_axis = partition_axis[chosen_axis];
        left_node.aabb  = chosen_partition_axis.left.aabbs[chosen_partition_axis.left_node_count-1];
        right_node.aabb = chosen_partition_axis.right.aabbs[chosen_partition_axis.left_node_count];

        for (u32 i = 0; i < N; i++) ids[i] = chosen_partition_axis.sorted_leaf_ids[i];

        return start + chosen_partition_axis.left_node_count;
    }

    void build(RTree &rtree, u32 N, u16 max_leaf_size) {
        rtree.height = 1;
        rtree.node_count = 1;

        RTreeNode &root = rtree.nodes[0];
        root = RTreeNode{};

        if (N <= max_leaf_size) {
            root.child_count = (u16)N;
            root.aabb.min = INFINITY;
            root.aabb.max = -INFINITY;

            RTreeNode *builder_node = leaf_nodes;
            for (u32 i = 0; i < N; i++, builder_node++) {
                rtree.leaf_ids[i] = builder_node->first_child_id;
                root.aabb += builder_node->aabb;
            }

            return;
        }

        u32 middle = splitNode(root, 0, N, rtree);
        RTreeBuildIteration *stack = build_iterations;
        u32 *leaf_id;

        RTreeBuildIteration left{0, middle, 1, 1};
        RTreeBuildIteration right{middle, N, 2, 1};

        stack[0] = left;
        stack[1] = right;

        i32 stack_size = 1;
        u32 leaf_count = 0;;

        while (stack_size >= 0) {
            left = stack[stack_size];
            RTreeNode &node = rtree.nodes[left.node_id];
            N = left.end - left.start;
            if (N <= max_leaf_size) {
                node.depth = left.depth;
                node.child_count = (u16)N;
                node.first_child_id = leaf_count;

                leaf_id = leaf_ids + left.start;
                for (u32 i = 0; i < N; i++, leaf_id++)
                    rtree.leaf_ids[leaf_count + i] = leaf_nodes[*leaf_id].first_child_id;
                leaf_count += N;
                stack_size--;
            } else {
                middle = splitNode(node, left.start, left.end, rtree);
                left.depth++;
                right.depth = left.depth;
                right.end = left.end;
                right.start = left.end = middle;
                left.node_id  = node.first_child_id;
                right.node_id = node.first_child_id + 1;
                rtree.nodes[rtree.node_count - 1].depth = left.depth;
                rtree.nodes[rtree.node_count - 2].depth = right.depth;
                stack[  stack_size] = left;
                stack[++stack_size] = right;
                if (left.depth > rtree.height) rtree.height = left.depth;
            }
        }

        RTreeNode &left_node = rtree.nodes[1];
        RTreeNode &right_node = rtree.nodes[2];
        left_node.depth = right_node.depth = 1;
        root.aabb = left_node.aabb + right_node.aabb;
    }

    void buildMesh(Mesh &mesh) {
        for (u32 i = 0; i < mesh.triangle_count; i++) {
            TriangleVertexIndices &indices = mesh.vertex_position_indices[i];
            const vec3 &v1 = mesh.vertex_positions[indices.ids[0]];
            const vec3 &v2 = mesh.vertex_positions[indices.ids[1]];
            const vec3 &v3 = mesh.vertex_positions[indices.ids[2]];
            RTreeNode &leaf_node = leaf_nodes[i];
            vec3 &min = leaf_node.aabb.min;
            vec3 &max = leaf_node.aabb.max;

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

            leaf_node.first_child_id = leaf_ids[i] = i;
        }

        build(mesh.rtree, mesh.triangle_count, MAX_TRIANGLES_PER_MESH_RTREE_NODE);

        for (u32 i = 0; i < mesh.triangle_count; i++) leaf_ids[i] = mesh.rtree.leaf_ids[i];
        for (u32 i = 0; i < mesh.triangle_count; i++) {
            u32 builder_leaf_id = leaf_ids[i];
            TriangleVertexIndices &indices = mesh.vertex_position_indices[builder_leaf_id];
            Triangle &triangle = mesh.triangles[i];
            mesh.rtree.leaf_ids[builder_leaf_id] = i;
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