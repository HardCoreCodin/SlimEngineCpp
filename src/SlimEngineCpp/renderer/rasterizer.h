#pragma once

#include "../math/vec2.h"
#include "../math/mat4.h"
#include "../scene/scene.h"
#include "../viewport/viewport.h"


//struct Rasterizer {
//    const Scene &scene;
//
//    mat4 model_to_world_inverted_transposed, model_to_world, world_to_clip;
//    vec4 *clip_space_vertex_positions;
//    vec3 *world_space_vertex_positions, *world_space_vertex_normals;
//    u8 *vertex_flags;
//
//    static Cube cube;
//
//    explicit Rasterizer(const Scene &scene) : scene{scene} {
//        vertex_flags = (u8*)memory::allocate(settings::scene::max_vertex_count);
//        clip_space_vertex_positions  = (vec4*)memory::allocate(sizeof(vec4) * settings::scene::max_vertex_count);
//        world_space_vertex_positions = (vec3*)memory::allocate(sizeof(vec3) * settings::scene::max_vertex_count);
//        world_space_vertex_normals = (vec3*)memory::allocate(sizeof(vec3) * settings::scene::max_normal_count);
//    };
//
//    void rasterize(const Viewport &viewport) {
//        const f32 pz = viewport.frustum.projection_matrix.W.z;
//        const f32 n = viewport.frustum.near_clipping_plane_distance;
//        const Dimensions &dim = viewport.dimensions;
//        static vec3 world_space_vertex_positions[6], normals[6];
//        static vec4 positions[6];
//        static vec2 uvs[6];
//        static Shaded shaded;
//
//        mat4 world_to_view{viewport.camera->getInverseMatrix()};
//        world_to_clip = world_to_view * viewport.frustum.projection_matrix;
//
//        bool last_UV_taken;
//        f32 dot, t, one_minus_t, one_over_ABC, ABC, ABy, ABx, ACy, ACx, Cdx, Bdx, Cdy, Bdy,
//            A, B, B_start, C, C_start, pixel_depth;
//        u32 face_index, vertex_index, face_count, vertex_count, clipped_index,
//            v1_index, out1_index, in1_index, first_x, last_x,
//            v2_index, out2_index, in2_index, first_y, last_y,
//            v3_index;
//        u8 v1_flags, new_v2num, out1_num, in1_num,
//           v2_flags, new_v1num, out2_num, in2_num,
//           v3_flags;
//
//        vec2 pixel_min, pixel_max, uv1, uv2, uv3, lastUV, screen_transform;
//        vec3 normal, attr_in, attr_out, new_v1, new_v2, pos1, pos2, pos3, norm1, norm2, norm3, ABCw, ABCp;
//        vec4 v1, v2, v3, *clipped, in1, in2, out1, out2, *position;
//
//        vec2 last_pixel_coord{dim.f_width - 1, dim.f_height - 1};
//        f32 width = dim.f_width, height = dim.f_height;
//        i32 stride = dim.width;
//        if ( viewport.canvas.antialias) {
//            screen_transform.x = dim.f_width;
//            screen_transform.y = dim.f_height;
//            stride <<= 1;
//            width *= 2;
//            height *= 2;
//            last_pixel_coord = {width - 1, height - 1};
//        } else {
//            screen_transform.x = dim.h_width;
//            screen_transform.y = dim.h_height;
//        }
//
//        PixelShader pixel_shader;
//        shaded.viewing_origin = viewport.camera->position;
//
//        bool mesh_has_normals, mesh_has_uvs, clipping_produced_an_extra_face;
//        TriangleVertexIndices position_indices, normal_indices, uvs_indices;
//        Pixel *pixel;
//        PixelQuad *pixel_quad;
//        Mesh *mesh;
//        Geometry *geometry = scene.geometries;
//        for (u32 geometry_id = 0; geometry_id < settings::scene::geometries; geometry_id++, geometry++) {
//            if (geometry->type == GeometryType_Box)
//                mesh = &cube;
//            else if (geometry->type == GeometryType_Mesh)
//                mesh = scene.meshes + geometry->id;
//            else
//                continue;
//
//            shaded.geometry = geometry;
//            shaded.material = scene.materials + geometry->material_id;
//            vertex_count = mesh->vertex_count;
//
//            // Prepare a matrix for converting from model space to clip space:
//            model_to_world = geometry->transform.getMatrix();
//            model_to_world_inverted_transposed = model_to_world.inverted().transposed();
//
//            // Execute mesh shader and skip this geometry if it got culled:
//            if (!shaded.material->mesh_shader(*mesh, *this))
//                continue;
//
//            // Cull the vertices and skip this geometry if it's entirely outside the view frustum:
//
//
//            // Check vertex positions against the frustum:
//            // ----------------------------------------------------
//            // For each vertex, check where it is in relation to the view frustum,
//            // and collect the results into a flags array (single number bit-pattern per-vertex).
//            // While doing so, keep track of which side(s) of the frustum are shared by all vertices.
//            // When done, first analyze the results before continuing to phase 2.
//            // Early bail-out if any of these conditions are met:
//            // A. The entire mesh is outside the frustum - early return with the CULL flag.
//            // B. The entire mesh is inside the frustum - early return with the INSIDE flag.
//            // C. No geometric clipping is needed - early return with the INSIDE flag.
//            bool needs_clipping = false;
//            bool has_inside = false;
//
//            u8 shared_directions = IS_OUT;
//            u8        directions = IS_OUT;
//
//            u8 *flags = vertex_flags;
//            position = clip_space_vertex_positions;
//            for (vertex_index = 0; vertex_index < vertex_count; vertex_index++, position++, flags++) {
//                *flags = CULL;
//
//                if (position->z < 0) {
//                    // Af at lease one vertex is outside the view frustum behind the near clipping plane,
//                    // the geometry needs to be checked for clipping
//                    needs_clipping = true;
//                    *flags = IS_NEAR;
//                    continue;
//                } else directions = position->z > position->w ? IS_FAR : 0;
//
//                if (     position->x >  position->w) directions |= IS_RIGHT;
//                else if (position->x < -position->w) directions |= IS_LEFT;
//
//                if (     position->y >  position->w) directions |= IS_ABOVE;
//                else if (position->y < -position->w) directions |= IS_BELOW;
//
//                if (directions) {
//                    // This vertex is outside of the view frustum.
//                    *flags = directions;
//                    // Note: This flag 'may' get removed from this vertex before the perspective-devide
//                    // (so it won't be skipped, essentially bringing it back) if it's still needed for culling/clipping.
//
//                    // Intersect the shared directions so-far, against this current out-direction:
//                    shared_directions &= directions;
//                    // Note: This will end-up beign zero if either:
//                    // A. All vertices are inside the frustum - no need for face clipping.
//                    // B. All vertices are outside the frustum in at least one direction shared by all.
//                    //   (All vertices are above and/or all vertices on the left and/or all vertices behind, etc.)
//                } else {
//                    has_inside = true;
//                    *flags = IS_NDC;
//                }
//            }
//
//            if (!has_inside && shared_directions)
//                // All vertices are completely outside, and all share at least one out-region.
//                // The entire mesh is completely outside the frustum and does not intersect it in any way.
//                // It can be safely culled altogether.
//                continue;
//
//            // The mesh intersects the frustum in some way
//
//            pixel_shader = shaded.material->pixel_shader;
//
//            mesh_has_uvs     = mesh->uvs_count     != 0;
//            mesh_has_normals = mesh->normals_count != 0;
//            face_count = mesh->triangle_count;
//
//            // Check its faces as well and check for clipping cases:
//            for (face_index = 0; face_index < face_count; face_index++) {
//                // Fetch the index and out-direction flags of each of the face's vertices:
//                position_indices = mesh->vertex_position_indices[face_index];
//
//                v1_index = position_indices.v1;
//                v2_index = position_indices.v2;
//                v3_index = position_indices.v3;
//
//                v1_flags = vertex_flags[v1_index] & IS_OUT;
//                v2_flags = vertex_flags[v2_index] & IS_OUT;
//                v3_flags = vertex_flags[v3_index] & IS_OUT;
//
//                positions[0] = clip_space_vertex_positions[v1_index];
//                positions[1] = clip_space_vertex_positions[v2_index];
//                positions[2] = clip_space_vertex_positions[v3_index];
//
//                world_space_vertex_positions[0] = world_space_vertex_positions[v1_index];
//                world_space_vertex_positions[1] = world_space_vertex_positions[v2_index];
//                world_space_vertex_positions[2] = world_space_vertex_positions[v3_index];
//
//                if (mesh_has_normals) {
//                    normal_indices = mesh->vertex_normal_indices[face_index];
//                    for (u8 i = 0; i < 3; i++) normals[i] = world_space_vertex_normals[normal_indices.ids[i]];
//                }
//
//                if (mesh_has_uvs) {
//                    uvs_indices = mesh->vertex_uvs_indices[face_index];
//                    for (u8 i = 0; i < 3; i++) uvs[i] = mesh->vertex_uvs[uvs_indices.ids[i]];
//                }
//
//                if ( viewport.frustum.cull_back_faces) {
//                    // Check face orientation "early" (before the perspective divide)
//                    // Compute a normal vector of the face from these 2 direction vectors:
//                    pos1 = positions[0].v3;
//                    pos2 = positions[1].v3;
//                    pos3 = positions[2].v3;
//                    normal = (pos3 - pos1) ^ (pos2 - pos1);
//
//                    // Dot the vector from the face to the origin with the normal:
//                    dot = normal.z*(pz - pos1.z) - normal.y*pos1.y - normal.x*pos1.x;
//                    if (dot < 0.0001f) {
//                        // if the angle is 90 the face is at grazing angle to the camera.
//                        // if the angle is greater than 90 degrees the face faces away from the camera.
//                        continue;
//                    }
//                }
//
//                clipping_produced_an_extra_face = false;
//                if (needs_clipping) {
//                    if ((v1_flags | v2_flags) | v3_flags) {
//                        // One or more vertices are outside - check edges for intersections:
//                        if ((v1_flags & v2_flags) & v3_flags) {
//                            // All vertices share one or more out-direction(s).
//                            // The face is fully outside the frustum, and does not intersect it.
//                            continue;
//                            // Note: This includes the cases where "all" 3 vertices cross the near clipping plane.
//                            // Below there are checks for when "any" of the vertices cross it (1 or 2, but not 3).
//                        }
//
//                        // One or more vertices are outside, and no out-direction is shared across them.
//                        // The face is visible in the view frustum in some way.
//                        vertex_flags[v1_index] |= IS_NDC;
//                        vertex_flags[v2_index] |= IS_NDC;
//                        vertex_flags[v3_index] |= IS_NDC;
//
//                        // Check if any vertex crosses the near clipping plane:
//                        if (v1_flags & IS_NEAR ||
//                            v2_flags & IS_NEAR ||
//                            v3_flags & IS_NEAR) {
//                            // There is at least one vertex behind the near clipping plane.
//                            // The face needs to be clipped
//                            // Clipping is done only against the near clipping plane, so there are only 2 possible cases:
//                            // 1: One vertex is inside the frustum and the other two are outside beyond the near clipping plane.
//                            // 2: Two vertices are inside the frustum and the third is outside beyond the near clipping plane.
//
//                            // Figure out which case applies to this current face, and which vertices are in/out:
//                            in2_num   = out2_num = 0;
//                            in2_index = out2_index = -1;
//                            if (v1_flags & IS_NEAR) {
//                                out1_index = v1_index;
//                                out1_num = 1;
//                                if (v2_flags & IS_NEAR) {
//                                    out2_index = v2_index;
//                                    out2_num = 2;
//                                    in1_index = v3_index;
//                                    in1_num = 3;
//                                } else {
//                                    in1_index = v2_index;
//                                    in1_num = 2;
//                                    if (v3_flags & IS_NEAR) {
//                                        out2_index = v3_index;
//                                        out2_num = 3;
//                                    } else {
//                                        in2_index = v3_index;
//                                        in2_num = 3;
//                                    }
//                                }
//                            } else {
//                                in1_index = v1_index;
//                                in1_num = 1;
//                                if (v2_flags & IS_NEAR) {
//                                    out1_index = v2_index;
//                                    out1_num = 2;
//                                    if (v3_flags & IS_NEAR) {
//                                        out2_index = v3_index;
//                                        out2_num = 3;
//                                    } else {
//                                        in2_index = v3_index;
//                                        in2_num = 3;
//                                    }
//                                } else {
//                                    in2_index = v2_index;
//                                    in2_num = 2;
//                                    out1_index = v3_index;
//                                    out1_num = 3;
//                                }
//                            }
//                            in1  = clip_space_vertex_positions[in1_index];
//                            out1 = clip_space_vertex_positions[out1_index];
//
//                            // Compute and store the (relative)amount by which the FIRST outside
//                            // vertex would need to be moved 'inwards' towards the FIRST inside vertex:
//                            t = out1.z / (out1.z - in1.z);
//                            one_minus_t = 1 - t;
//                            // Note:
//                            // Clip space is set up such that a depth of 0 is where the near clipping plane is.
//                            // So 'out1z' would be (negative)distance of the vertex from the near clipping plane,
//                            // representing the 'amount' by which the 'outside' vertex would needs to be 'pushed' forward
//                            // to land it on the near clipping plane. The denominator here is the 'full' depth-distance
//                            // between the 2 vertices - the sum of distances of the 2 vertices from to the clipping plane.
//                            // The ratio of the former from the latter is thus the (positive)interpolation amount 't' (0->1).
//                            // Since 'out1z' would be negative here, 'in1z' is negated as well to get the full (negative)sum.
//                            // Since both the numerator and the denominator here would be negative, the result is positive.
//                            // The interpolation value 't' is with respect to the 'outside' vertex, and so would be multiplied
//                            // by any attribute-value of the 'outside' vertex. The complement of that (1 - t) would be multiplied
//                            // by any attribute-value of the 'inside' vertex, and the sum would be the interpolated value.
//                            // *The same logic applies for the second interpolation in either of it's 2 cases below.
//
//                            // Compute the index of the "unshared" position-value(s) of the 'clipped' vertex of this face:
//                            clipped_index = out1_num - 1;
//                            clipped = positions + clipped_index;
//
//                            // Compute the new clip-space coordinates of the clipped-vertex:
//                            new_v1.z = fast_mul_add(out1.z, one_minus_t, t*in1.z);
//                            clipped->x = new_v1.x = fast_mul_add(out1.x, one_minus_t, t*in1.x);
//                            clipped->y = new_v1.y = fast_mul_add(out1.y, one_minus_t, t*in1.y);
//                            clipped->z = 0;
//                            clipped->w = n;
//                            // Note:
//                            // The 'Z' coordinate of this new vertex position in clip-space is set to '0' since it has
//                            // now been moved onto the clipping plane itself. Similarly, the 'W' coordinate is set to
//                            // what the "original" Z-depth value this vertex "would have had", had it been on the
//                            // near clipping plane in view-space in the first place.
//                            // *The same logic applies for the second interpolation in either of it's 2 cases below.
//
//                            attr_in  = world_space_vertex_positions[position_indices.ids[in1_num - 1]];
//                            attr_out = world_space_vertex_positions[position_indices.ids[out1_num - 1]];
//                            world_space_vertex_positions[clipped_index] = attr_out.scaleAdd(one_minus_t, attr_in * t);
//
//                            if (mesh_has_normals) {
//                                attr_in  = world_space_vertex_normals[normal_indices.ids[in1_num - 1]];
//                                attr_out = world_space_vertex_normals[normal_indices.ids[out1_num - 1]];
//                                normals[clipped_index] = attr_out.scaleAdd(one_minus_t, attr_in * t).normalized();
//                            }
//
//                            if (mesh_has_uvs) {
//                                attr_in.v2  = mesh->vertex_uvs[uvs_indices.ids[in1_num - 1]];
//                                attr_out.v2 = mesh->vertex_uvs[uvs_indices.ids[out1_num - 1]];
//                                uvs[clipped_index] = attr_out.v2.scaleAdd(one_minus_t, attr_in.v2 * t);
//                            }
//
//                            if (out2_num) {
//                                // One vertex is inside the frustum, and the other two are outside beyond the near clipping plane.
//                                // The triangle just needs to get smaller by moving the 2 outside-vertices back to the near clipping plane.
//
//                                // Compute and store the (relative)amount by which the SECOND outside
//                                // vertex needs to be moved inwards towards the FIRST inside vertex:
//                                out2 = clip_space_vertex_positions[out2_index];
//                                t = out2.z / (out2.z - in1.z);
//                                one_minus_t = 1 - t;
//
//                                // Compute the index of the "unshared" position-value(s) of the 'clipped' vertex of this face:
//                                clipped_index = out2_num - 1;
//                                clipped = positions + clipped_index;
//
//                                // Compute the new clip-space coordinates of the clipped-vertex:
//                                clipped->x = fast_mul_add(out2.x, one_minus_t, t*in1.x);
//                                clipped->y = fast_mul_add(out2.y, one_minus_t, t*in1.y);
//                                clipped->z = 0;
//                                clipped->w = n;
//
//                                attr_in  = world_space_vertex_positions[position_indices.ids[in1_num - 1]];
//                                attr_out = world_space_vertex_positions[position_indices.ids[out2_num - 1]];
//                                world_space_vertex_positions[clipped_index] = attr_out.scaleAdd(one_minus_t, attr_in * t);
//
//                                if (mesh_has_normals) {
//                                    attr_in  = world_space_vertex_normals[normal_indices.ids[in1_num - 1]];
//                                    attr_out = world_space_vertex_normals[normal_indices.ids[out2_num - 1]];
//                                    normals[clipped_index] = attr_out.scaleAdd(one_minus_t, attr_in * t).normalized();
//                                }
//
//                                if (mesh_has_uvs) {
//                                    attr_in.v2  = mesh->vertex_uvs[uvs_indices.ids[in1_num - 1]];
//                                    attr_out.v2 = mesh->vertex_uvs[uvs_indices.ids[out2_num - 1]];
//                                    uvs[clipped_index] = attr_out.v2.scaleAdd(one_minus_t, attr_in.v2 * t);
//                                }
//                            } else {
//                                // Two vertices are inside the frustum, and the third one is behind the near clipping plane.
//                                // Clipping forms a quad which needs to be split into 2 triangles.
//                                // The first one is formed from the original one, by moving the vertex that is behind the
//                                // clipping plane right up-to the near clipping plane itself (exactly as in the first case).
//                                // The second triangle is a new triangle that needs to be created, from the 2 vertices that
//                                // are inside, plus a new vertex that would need to be interpolated by moving the same vertex
//                                // that is outside up-to the near clipping plane but towards the other vertex that is inside.
//
//                                // Compute and store the (relative)amount by which the FIRST outside vertex
//                                // needs to be moved inwards towards the SECOND inside vertex:
//                                in2 = clip_space_vertex_positions[in2_index];
//                                t = out1.z / (out1.z - in2.z);
//                                one_minus_t = 1 - t;
//
//                                new_v2 = out1.v3.scaleAdd(one_minus_t, in2.v3 * t);
//
//                                // Determine orientation:
//                                // Compute 2 direction vectors forming a plane for the face:
//                                // Compute a normal vector of the face from these 2 direction vectors:
//                                normal = (new_v1 - in2.v3) ^ (new_v2 - in2.v3);
//
//                                // Dot the vector from the face to the origin with the normal:
//                                dot = normal.z*(pz - in2.z) - normal.y*in2.y - normal.x*in2.x;
//                                if (dot > 0) {
//                                    // if the angle is greater than 90 degrees the face is facing the camera
//                                    new_v1num = 2;
//                                    new_v2num = 1;
//                                } else {
//                                    // if the angle is 90 the face is at grazing angle to the camera.
//                                    // if the angle is greater than 90 degrees the face faces away from the camera.
//                                    new_v1num = 1;
//                                    new_v2num = 2;
//                                }
//
//                                // Since this vertex belongs to an 'extra' new face, the index is offset to that index-space
//                                clipped_index = 3 + new_v2num;
//                                positions[3] = in2;
//                                positions[3 + new_v1num] = *clipped;
//                                clipped = positions + clipped_index;
//
//                                // Compute the new clip-space coordinates of the clipped-vertex:
//                                clipped->x = new_v2.x;
//                                clipped->y = new_v2.y;
//                                clipped->z = 0;
//                                clipped->w = n;
//
//                                attr_in  = world_space_vertex_positions[position_indices.ids[in2_num - 1]];
//                                attr_out = world_space_vertex_positions[position_indices.ids[out1_num - 1]];
//                                world_space_vertex_positions[clipped_index] = attr_out.scaleAdd(one_minus_t, attr_in * t);
//                                world_space_vertex_positions[3] = attr_in;
//                                world_space_vertex_positions[3 + new_v1num] = world_space_vertex_positions[out1_num - 1];
//
//                                if (mesh_has_normals) {
//                                    attr_in  = world_space_vertex_normals[normal_indices.ids[in2_num - 1]];
//                                    attr_out = world_space_vertex_normals[normal_indices.ids[out1_num - 1]];
//                                    normals[clipped_index] = attr_out.scaleAdd(one_minus_t, attr_in * t).normalized();
//                                    normals[3] = attr_in;
//                                    normals[3 + new_v1num] = normals[out1_num - 1];
//                                }
//
//                                if (mesh_has_uvs) {
//                                    attr_in.v2  = mesh->vertex_uvs[uvs_indices.ids[in2_num - 1]];
//                                    attr_out.v2 = mesh->vertex_uvs[uvs_indices.ids[out1_num - 1]];
//                                    uvs[clipped_index] = attr_out.v2.scaleAdd(one_minus_t, attr_in.v2 * t);
//                                    uvs[3] = attr_in.v2;
//                                    uvs[3 + new_v1num] = uvs[out1_num - 1];
//                                }
//
//                                clipping_produced_an_extra_face = true;
//                            }
//                        }
//                        // Even if no vertices are behind the view frustum the face is still visible.
//                        // It either intersects the frustum in direction(s) other than the near clipping plane,
//                        // or it may fully surround the whole view frustum.
//                        // No geometric clipping is needed, but the face can not be culled.
//                    } // else: No vertices are outside the frustum (the face is fully within it).
//                } // else: No vertex is behind the near clipping plane.
//                // Since geometric clipping is done only against the near clipping plane, there is nothing to clip.
//                // The other sides of the frustum would get raster-clipped later by clamping pixels outside the screen.
//
//                vertex_count = clipping_produced_an_extra_face ? 6 : 3;
//                position = positions;
//                for (vertex_index = 0; vertex_index < vertex_count; vertex_index++, position++) {
//                    // The perspective divide should finalize normalizing the depth values
//                    // into the 0 -> 1 space, by dividing clip-space 'z' by clip-space 'w'
//                    // (coordinates that came out of the multiplication by the projection matrix)
//                    // screen_z = clip_z / clip_w = Z[i] / W[i]
//                    // However: The rasterizer is going to then need to convert this value
//                    // into a spectrum of values that are all divided by 'clip_w'
//                    // in order to linearly interpolate it there (from vertex values to a pixel value).
//
//                    // Store reciprocals for use in rasterization:
//                    position->w = 1.0f / position->w;
//                    position->v3 = position->v3 * position->w;
//                    // Scale the normalized screen to the pixel size:
//                    // (from normalized size of -1->1 horizontally and vertically having a width and height of 2)
//                    position->x *= screen_transform.x;
//                    position->y *= -screen_transform.y;
//
//                    // Move the screen up and to the right appropriately,
//                    // such that it goes 0->width horizontally and 0->height vertically:
//                    position->x += screen_transform.x;
//                    position->y += screen_transform.y;
//
//                    // Scale the normalized screen to the pixel size:
//                    // (from normalized size of -1->1 horizontally and vertically having a width and height of 2)
//                    // Then, move the screen up and to the right appropriately,
//                    // such that it goes 0->width horizontally and 0->height vertically:
////                position->x = fast_mul_add(position->x, screen_transform.x, screen_transform.x);
////                position->x = fast_mul_add(position->y, -screen_transform.y, screen_transform.y);
//                }
//
//                vertex_count = clipping_produced_an_extra_face ? 2 : 1;
//                v1_index = 0;
//                v2_index = 1;
//                v3_index = 2;
//
//                for (vertex_index = 0; vertex_index < vertex_count; vertex_index++, v1_index += 3, v2_index += 3, v3_index += 3) {
//                    if (pixel_shader) {
//                        v1 = positions[v1_index];
//                        v2 = positions[v2_index];
//                        v3 = positions[v3_index];
//
//                        // Cull this triangle against the edges of the viewport:
//                        pixel_min = min(v1.v3.v2, min(v2.v3.v2, v3.v3.v2));
//                        if (pixel_min.x >= width ||
//                            pixel_min.y >= height)
//                            continue;
//
//                        pixel_max = max(v1.v3.v2, max(v2.v3.v2, v3.v3.v2));
//                        if (pixel_max.x < 0 ||
//                            pixel_max.y < 0)
//                            continue;
//
//                        // Clip the bounds of the triangle to the viewport:
//                        pixel_min = pixel_min.clamped();
//                        pixel_max = pixel_max.clamped(last_pixel_coord);
//
//                        // Compute area components:
//                        ABy = v2.y - v1.y;
//                        ABx = v2.x - v1.x;
//
//                        ACy = v3.y - v1.y;
//                        ACx = v3.x - v1.x;
//
//                        ABC = ACx*ABy - ACy*ABx;
//
//                        // Cull faces facing backwards:
//                        if (ABC <= 0)
//                            return;
//
//                        // Floor bounds coordinates down to their integral component:
//                        first_x = (u32)pixel_min.x;
//                        first_y = (u32)pixel_min.y;
//                        last_x  = (u32)pixel_max.x;
//                        last_y  = (u32)pixel_max.y;
//
//                        pixel_min.x = (f32)first_x;
//                        pixel_min.y = (f32)first_y;
//                        pixel_max.x = (f32)last_x;
//                        pixel_max.y = (f32)last_y;
//
//                        // Compute edge exclusions:
//                        // Drawing: Top-down
//                        // Origin: Top-left
//                        // Shadow rules: Top/Left
//                        // Winding: CW (Flipped vertically due to top-down drawing!)
////                    const bool exclude_edge_1 = ABy > 0;
////                    const bool exclude_edge_2 = v2.y > v3.y;
////                    const bool exclude_edge_3 = ACy < 0;
//
//                        // Compute weight constants:
//                        one_over_ABC = 1.0f / ABC;
//
//                        Cdx =  ABy * one_over_ABC;
//                        Bdx = -ACy * one_over_ABC;
//
//                        Cdy = -ABx * one_over_ABC;
//                        Bdy =  ACx * one_over_ABC;
//
//                        // Compute initial areal coordinates for the first pixel center:
//                        pixel_min += vec2{0.5f, 0.5f};
//                        C_start = Cdx*pixel_min.x + Cdy*pixel_min.y + (v1.y*v2.x - v1.x*v2.y) * one_over_ABC;
//                        B_start = Bdx*pixel_min.x + Bdy*pixel_min.y + (v3.y*v1.x - v3.x*v1.y) * one_over_ABC;
//
//                        pos1 = world_space_vertex_positions[v1_index];
//                        pos2 = world_space_vertex_positions[v2_index];
//                        pos3 = world_space_vertex_positions[v3_index];
//
//                        norm1 = normals[v1_index];
//                        norm2 = normals[v2_index];
//                        norm3 = normals[v3_index];
//
//                        uv1 = uvs[v1_index];
//                        uv2 = uvs[v2_index];
//                        uv3 = uvs[v3_index];
//
//                        // Scan the bounds:
//                        for (u32 y = first_y; y <= last_y; y++, C_start += Cdy, B_start += Bdy) {
//                            B = B_start;
//                            C = C_start;
//
//                            last_UV_taken = false;
//
//                            for (u32 x = first_x; x <= last_x; x++, B += Bdx, C += Cdx) {
//                                if (Bdx < 0 && B < 0 ||
//                                    Cdx < 0 && C < 0)
//                                    break;
//
//                                A = 1 - B - C;
//
//                                // Skip the pixel if it's outside:
//                                if (fminf(A, fminf(B, C)) < 0)
//                                    continue;
//
//                                // If the pixel is on a shadow-edge, skip it:
////                             if ((A == 0 && exclude_edge_1) ||
////                                 (B == 0 && exclude_edge_2) ||
////                                 (C == 0 && exclude_edge_3))
////                                 continue;
//
//                                // Cull and test pixel based on its depth:
//                                if ( viewport.canvas.antialias) {
//                                    pixel_quad = viewport.canvas.pixels + ( viewport.dimensions.stride * (y >> 1)) + (x >> 1);
//                                    pixel = &pixel_quad->quad[y & 1][x & 1];
//                                } else {
//                                    pixel_quad = viewport.canvas.pixels + ( viewport.dimensions.stride * y) + x;
//                                    pixel = &pixel_quad->TL;
//                                }
//                                pixel_depth = A*v1.z + B*v2.z + C*v3.z;
//                                if (pixel_depth < 0 ||
//                                    pixel_depth > 1 ||
//                                    pixel_depth > pixel->depth)
//                                    continue;
//
//                                pixel->depth = pixel_depth;
//
//                                ABCw = {A*v1.w, B*v2.w, C*v3.w};
//                                shaded.depth = 1.0 / (f64)(ABCw.x + ABCw.y + ABCw.z);
//                                ABCp = ABCw * (f32)shaded.depth;
//
//                                shaded.position = pos1.scaleAdd(ABCp.x, pos2.scaleAdd(ABCp.y, pos3 * ABCp.z));
//                                if (mesh_has_normals) shaded.normal = norm1.scaleAdd(ABCp.x, norm2.scaleAdd(ABCp.y, norm3 * ABCp.z)).normalized();
//                                if (mesh_has_uvs) {
//                                    shaded.UV = uv1.scaleAdd(ABCp.x, uv2.scaleAdd(ABCp.y, uv3 * ABCp.z));
//
//                                    if (last_UV_taken) {
//                                        shaded.dUV = shaded.UV - lastUV;
//                                    } else {
//                                        ABCp.y = B + Bdx;
//                                        ABCp.z = C + Cdx;
//                                        ABCp.x =  v1.w * (1 - ABCp.y - ABCp.z);
//                                        ABCp.y *= v2.w;
//                                        ABCp.z *= v3.w;
//                                        ABCp = ABCw / (ABCp.x + ABCp.y + ABCp.z);
//                                        shaded.dUV = uv1.scaleAdd(ABCp.x, uv2.scaleAdd(ABCp.y, uv3 * ABCp.z)) - shaded.UV;
//                                        last_UV_taken = true;
//                                    }
//
//                                    if (shaded.dUV.u < 0) shaded.dUV.u = -shaded.dUV.u;
//                                    if (shaded.dUV.v < 0) shaded.dUV.v = -shaded.dUV.v;
//                                    lastUV = shaded.UV;
//                                }
//                                shaded.coords.x = x;
//                                shaded.coords.y = y;
//                                shaded.color = 0.0f;
//                                shaded.opacity = 1;
//                                pixel_shader(shaded, scene);
//                                shaded.color.x *= FLOAT_TO_COLOR_COMPONENT;
//                                shaded.color.y *= FLOAT_TO_COLOR_COMPONENT;
//                                shaded.color.z *= FLOAT_TO_COLOR_COMPONENT;
//                                shaded.color.x *= shaded.color.x;
//                                shaded.color.y *= shaded.color.y;
//                                shaded.color.z *= shaded.color.z;
//                                viewport.canvas.setPixel((i32)x, (i32)y, shaded.color, shaded.opacity, pixel_depth);
//                            }
//                        }
//                    }
//                    if ( viewport.show_wire_frame || !pixel_shader) {
//                        vec3 color = Color(vertex_index ? Red : White);
//                        new_v1 = positions[v1_index].v3;
//                        new_v2 = positions[v2_index].v3;
//                        if (viewport.canvas.antialias) {
//                            new_v1.x *= 0.5f;
//                            new_v1.y *= 0.5f;
//                            new_v2.x *= 0.5f;
//                            new_v2.y *= 0.5f;
//                        }
//                        new_v1.z -= 0.001f;
//                        new_v2.z -= 0.001f;
//                        viewport.canvas.drawLine(new_v1.x, new_v1.y, new_v1.z,
//                                                 new_v2.x, new_v2.y, new_v2.z,
//                                                 color, 1, 0);
//                        new_v1 = positions[v2_index].v3;
//                        new_v2 = positions[v3_index].v3;
//                        if (viewport.canvas.antialias) {
//                            new_v1.x *= 0.5f;
//                            new_v1.y *= 0.5f;
//                            new_v2.x *= 0.5f;
//                            new_v2.y *= 0.5f;
//                        }
//                        new_v1.z -= 0.001f;
//                        new_v2.z -= 0.001f;
//                        viewport.canvas.drawLine(new_v1.x, new_v1.y, new_v1.z,
//                                                 new_v2.x, new_v2.y, new_v2.z,
//                                                 color, 1, 0);
//                        new_v1 = positions[v3_index].v3;
//                        new_v2 = positions[v1_index].v3;
//                        if ( viewport.canvas.antialias) {
//                            new_v1.x *= 0.5f;
//                            new_v1.y *= 0.5f;
//                            new_v2.x *= 0.5f;
//                            new_v2.y *= 0.5f;
//                        }
//                        new_v1.z -= 0.001f;
//                        new_v2.z -= 0.001f;
//                        viewport.canvas.drawLine(new_v1.x, new_v1.y, new_v1.z,
//                                                 new_v2.x, new_v2.y, new_v2.z,
//                                                 color, 1, 0);
//                    }
//                }
//            }
//        }
//    }
//};