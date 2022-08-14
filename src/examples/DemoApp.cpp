#include "../slim.h"
#include "./ClosestPointOnMesh.hpp"


struct DemoApp : SlimApp {
    // Viewport:
    Camera camera{
            {0, 10, -15},
            {-25*DEG_TO_RAD,0, 0}
    }, *cameras{&camera};
    Canvas canvas;
    Viewport viewport{canvas, &camera};
    bool antialias = false;

    bool draw_rtree = false;
    bool draw_query_result = false;
    bool draw_query_aabbs = false;
    bool draw_query_triangles = false;
    bool adaptive = true;
    bool multi = true;

    u8 min_depth = 0;
    u8 max_depth = 0;
    f32 world_max_distance = 0.3f;

    time::Timer query_timer;

    // HUD:
    HUDLine QueryLine{(char*)"Show Query     : ", (char*)"On",(char*)"Off", &draw_query_result, true, Yellow, DarkYellow} ;
    HUDLine AABBsLine{(char*)"Show AABBs     : ", (char*)"On",(char*)"Off", &draw_query_aabbs, true, Magenta, DarkMagenta};
    HUDLine TriesLine{(char*)"Show Triangles : ", (char*)"On",(char*)"Off", &draw_query_triangles, true, Green, DarkGreen};
    HUDLine RTreeLine{(char*)"Show RTree     : ", (char*)"On",(char*)"Off", &draw_rtree, true, Cyan, DarkCyan};
    HUDLine MultiLine{(char*)"Cross Mesh     : ", (char*)"On",(char*)"Off", &multi, true, BrightBlue, Blue};
    HUDLine AdaptLine{(char*)"Adaptive Mode  : ", (char*)"On",(char*)"Off", &adaptive, true, Green, Red};
    HUDLine TimerLine{(char*)"Micro Seconds  : "};
    HUDSettings hud_settings{7};
    HUD hud{hud_settings, &QueryLine, White};

    // Scene:
    Grid grid{11, 11}, *grids{&grid};
    Curve sphere{ CurveType_Sphere,  30}, *curves{&sphere};

    Transform closest_point_transform{
            {0, 0, 0},
            {0, 0, 0},
            {0.1f, 0.1f, 0.1f}
    };
    Transform sphere_center_transform{
            {0, 0, 0},
            {0, 0, 0},
            {0.1f, 0.1f, 0.1f}
    };
    Transform grid_transform{
            {0, 0, 0},
            {0, 45 * DEG_TO_RAD, 0},
            {5, 1, 5}
    };

    Geometry grid1{grid_transform,GeometryType_Grid, BrightGrey};
    Geometry sphere_geo{{{4, 4, 2}, {}, {world_max_distance}}, GeometryType_Curve, BrightYellow};
    Geometry mesh1{{{+8, 5, 0} }, GeometryType_Mesh, BrightBlue};
    Geometry mesh2{{{-8, 5, 0} }, GeometryType_Mesh, BrightGrey}, *geometries{&grid1};

    char strings[3][100] = {};
    String mesh_files[3] = {
        String::getFilePath((char*)"dog.mesh" ,strings[1],(char*)__FILE__),
        String::getFilePath((char*)"suzanne.mesh",strings[0],(char*)__FILE__),
        String::getFilePath((char*)"cube.mesh" ,strings[2],(char*)__FILE__)
    };
    Mesh dog, monkey, cube, *meshes = &dog;

    SceneCounts counts{1, 4, 1, 0, 1, 3 };
    Scene scene{counts,nullptr, cameras, geometries, grids, nullptr,curves,
                meshes, mesh_files, nullptr};
    Selection selection;

    Geometry *query_geo = &mesh1;
    ClosestPointOnMesh query{&dog};
    ClosestPointOnTriangle closest_point_on_triangle;

    // Drawing:
    f32 opacity = 0.5f;

    void doQuery(Mesh &mesh, Transform &transform) {
        query_timer.beginFrame();

        if (draw_query_triangles || draw_query_aabbs)
            for (u32 node_index = 0; node_index < mesh.rtree.node_count; node_index++)
                mesh.rtree.nodes[node_index].flags = 0;

        vec3 &scale = transform.scale;
        f32 max_distance = world_max_distance / ((scale.x == scale.y && scale.x == scale.z) ? scale.x : (scale.length()));

        query.mesh = &mesh;
        query.mesh_transform = &transform;
        if (multi) {
            Geometry &other_geo = query_geo == &mesh1 ? mesh2 : mesh1;
            Mesh &other_mesh = meshes[other_geo.id];
            query.search_origin_transform = &other_geo.transform;
            query.find(other_mesh.vertex_positions, other_mesh.vertex_count, max_distance, adaptive);
        } else {
            query.search_origin_transform = nullptr;
            query.find(sphere_geo.transform.position, max_distance, closest_point_on_triangle, adaptive);
        }

        query_timer.endFrame();
        TimerLine.value = (i32)query_timer.microseconds;
    }

    void OnUpdate(f32 delta_time) override {
        if (!mouse::is_captured) selection.manipulate(viewport, scene);
        if (!controls::is_pressed::alt &&
            !controls::is_pressed::ctrl &&
            !controls::is_pressed::shift)
            viewport.updateNavigation(delta_time);

        if (selection.changed) {
            selection.changed = false;
            if (selection.geometry && selection.geo_type == GeometryType_Mesh)
                query_geo = selection.geometry;
        }
        sphere_center_transform.position = sphere_geo.transform.position;

        if (!query.stack) query.allocate(dog.vertex_count);

        doQuery(meshes[query_geo->id], query_geo->transform);
    }

    void OnRender() override {
        canvas.clear();
        drawGrid(grid, grid1.transform, viewport, grid1.color, opacity);
        drawMesh(meshes[mesh1.id], mesh1.transform, false, viewport, mesh1.color, opacity);
        drawMesh(meshes[mesh2.id], mesh2.transform, false, viewport, mesh2.color, opacity);
        drawCurve(sphere, sphere_geo.transform, viewport, sphere_geo.color, 0.2f);
        drawCurve(sphere, sphere_center_transform, viewport, Cyan, 0.2f);

        Geometry &other_geo = query_geo == &mesh1 ? mesh2 : mesh1;
        Mesh &other_mesh = meshes[other_geo.id];
        bool large_mesh = other_mesh.vertex_count > 8;
        if (multi && !large_mesh) {
            vec3 sphere_position = sphere_geo.transform.position;

            for (u32 vertex_index = 0; vertex_index < other_mesh.vertex_count; vertex_index++) {
                vec3 v = other_mesh.vertex_positions[vertex_index];
                sphere_geo.transform.position = sphere_center_transform.position = other_geo.transform.externPos(v);
                drawCurve(sphere, sphere_geo.transform, viewport, sphere_geo.color, 0.2f);
                drawCurve(sphere, sphere_center_transform, viewport, Cyan, 0.2f);

                sphere_geo.transform.position = sphere_position;
            }
        }

        if (draw_rtree) drawRTree(query.mesh->rtree, *query.mesh_transform, viewport, min_depth, max_depth);
        if (draw_query_aabbs) drawQueryAABBs();
        if (draw_query_triangles) drawQueryTriangles();
        if (draw_query_result) {
            if (multi) {
                if (large_mesh) {
                    for (u32 vertex_index = 0; vertex_index < other_mesh.vertex_count; vertex_index++)
                        drawBoundsOfEdgeToClosestPoint(query.results[vertex_index]);
                } else {
                    for (u32 vertex_index = 0; vertex_index < other_mesh.vertex_count; vertex_index++)
                        drawEdgeToClosestPoint(query.results[vertex_index]);
                }
            } else
                drawQueryResult(closest_point_on_triangle);
        }

        if (controls::is_pressed::alt) drawSelection(selection, viewport, scene);
        if (hud.enabled) drawHUD(hud, canvas);

        canvas.drawToWindow();
    }

    void drawBoundsOfEdgeToClosestPoint(ClosestPointOnTriangle &query_result, i32 size = 3) {
        static Edge edge;
        static RectI rect;

        if (!query_result.on) return;

        edge.from = camera.internPos(query_result.search_origin);
        edge.to   = camera.internPos(query_result.closest_point);
        viewport.projectEdge(edge);
        rect.left = (i32)edge.from.x - size;
        rect.right = (i32)edge.from.x + size;
        rect.top = (i32)edge.from.y - size;
        rect.bottom = (i32)edge.from.y + size;
        viewport.canvas.fillRect(rect, Cyan);
        rect.left = (i32)edge.to.x - size;
        rect.right = (i32)edge.to.x + size;
        rect.top = (i32)edge.to.y - size;
        rect.bottom = (i32)edge.to.y + size;
        viewport.canvas.fillRect(rect, Red);
    }

    void drawEdgeToClosestPoint(ClosestPointOnTriangle &query_result) {
        static Edge edge;

        if (!query_result.on) return;

        closest_point_transform.position = query_result.closest_point;
        edge.from = camera.internPos(query_result.search_origin);
        edge.to   = camera.internPos(query_result.closest_point);

        ColorID color = query_result.on == TrianglePointOn_Vertex ? Red : White;
        drawEdge(edge, viewport, Cyan, opacity, 3);
        drawCurve(sphere, closest_point_transform, viewport, color, opacity);
    }

    void drawQueryResult(ClosestPointOnTriangle &query_result) {
        static Edge edge;
        static vec3 v1, v2, v3;

        if (!query_result.on) return;

        drawEdgeToClosestPoint(query_result);

        Triangle &triangle = query.mesh->triangles[query_result.triangle_index];
        v1 = camera.internPos(query.mesh_transform->externPos(triangle.position));
        v2 = camera.internPos(query.mesh_transform->externPos(triangle.position + triangle.V));
        v3 = camera.internPos(query.mesh_transform->externPos(triangle.position + triangle.U));

        if (query_result.on == TrianglePointOn_Edge) {
            f32 u = query_result.uv.u;
            f32 v = query_result.uv.v;
            if (u == 0 || v == 0) {
                edge.from = v1;
                edge.to = u == 0 ? v2 : v3;
            } else {
                edge.from = v2;
                edge.to = v3;
            }
            drawEdge(edge, viewport, Red, 1, 3);
        } else {
            viewport.projectPoint(v1);
            viewport.projectPoint(v2);
            viewport.projectPoint(v3);
            canvas.fillTriangle(v1, v2, v3, Red, 0.5f);
        }
    }

    void drawQueryTriangles() const {
        static vec3 pos, v1, v2, v3;
        static Edge edge;

        Mesh &mesh = meshes[query_geo->id];
        Transform &transform = query_geo->transform;
        for (u32 node_index = 0; node_index < mesh.rtree.node_count; node_index++) {
            RTreeNode &node = mesh.rtree.nodes[node_index];
            if (node.flags == BROAD_PHASE_INCLUDED) {
                u32 start = node.first_index;
                u32 end = start + node.leaf_count;
                for (u32 triangle_index = start; triangle_index < end; triangle_index++) {
                    Triangle &triangle = mesh.triangles[triangle_index];
                    v1 = camera.internPos(transform.externPos(triangle.position));
                    v2 = camera.internPos(transform.externPos(triangle.position + triangle.V));
                    v3 = camera.internPos(transform.externPos(triangle.position + triangle.U));
                    viewport.projectPoint(v1);
                    viewport.projectPoint(v2);
                    viewport.projectPoint(v3);
                    viewport.canvas.fillTriangle(v1, v2, v3, BrightMagenta, opacity);
                }
            }
        }
    }

    void drawQueryAABBs() const {
        static Box box;
        static Transform box_transform;

        Mesh &mesh = meshes[query_geo->id];
        Transform &transform = query_geo->transform;
        for (u32 node_index = 0; node_index < mesh.rtree.node_count; node_index++) {
            RTreeNode &node = mesh.rtree.nodes[node_index];
            if (node.flags == BROAD_PHASE_INCLUDED) {
                u32 start = node.first_index;
                u32 end = start + node.leaf_count;
                for (u32 triangle_index = start; triangle_index < end; triangle_index++) {
                    box_transform = transform;
                    box_transform.scale *= (node.aabb.max - node.aabb.min) * 0.5f;
                    box_transform.position = transform.externPos((node.aabb.min + node.aabb.max) * 0.5f);

                    drawBox(box, box_transform, viewport, BrightGreen, opacity);
                }
            }
        }
    }

    void OnKeyChanged(u8 key, bool is_pressed) override {
        Move &move = viewport.navigation.move;
        Turn &turn = viewport.navigation.turn;
        if (key == 'X') turn.left     = is_pressed;
        if (key == 'C') turn.right    = is_pressed;
        if (key == 'R') move.up       = is_pressed;
        if (key == 'F') move.down     = is_pressed;
        if (key == 'W') move.forward  = is_pressed;
        if (key == 'S') move.backward = is_pressed;
        if (key == 'A') move.left     = is_pressed;
        if (key == 'D') move.right    = is_pressed;
        if (!is_pressed) {
            u8 depth = meshes[scene.geometries[1].id].rtree.height;
            if (key == controls::key_map::tab) hud.enabled = !hud.enabled;
            else if (key == 'M') query_geo->id = (query_geo->id + 1) % 3;
            else if (key == 'Q') draw_query_result = !draw_query_result;
            else if (key == 'C') multi = !multi;
            else if (key == 'B') draw_query_aabbs = !draw_query_aabbs;
            else if (key == 'G') draw_query_triangles = !draw_query_triangles;
            else if (key == 'T') draw_rtree = !draw_rtree;
            else if (key == 'V') adaptive = !adaptive;
            else if (key == '3') { if (min_depth > 0) min_depth--; }
            else if (key == '4') { if (min_depth < depth) min_depth++; }
            else if (key == '1') {
                if (max_depth > 0) max_depth--;
                if (min_depth > 0 && controls::is_pressed::shift) min_depth--;
            }
            else if (key == '2') {
                if (max_depth < depth) max_depth++;
                if (min_depth < depth && controls::is_pressed::shift) min_depth++;
            }
        }
    }

    void OnWindowResize(u16 width, u16 height) override {
        viewport.updateDimensions(width, height);
        canvas.dimensions.update(width, height);
    }

    void OnMouseButtonDown(mouse::Button &mouse_button) override {
        mouse::pos_raw_diff_x = mouse::pos_raw_diff_y = 0;
    }

    void OnMouseWheelScrolled(f32 amount) override {
        if (controls::is_pressed::ctrl && selection.geometry) {
            if (selection.geometry == &sphere_geo) {
                world_max_distance += mouse::wheel_scroll_amount * 0.001f;
                if (world_max_distance < 0.3f) world_max_distance = 0.3f;
                if (world_max_distance > 3.0f) world_max_distance = 3.0;
                sphere_geo.transform.scale = world_max_distance;
            } else {
                f32 scale = selection.geometry->transform.scale.x;
                f32 new_scale = scale + mouse::wheel_scroll_amount * 0.01f;
                if (new_scale < 1) new_scale = 1;
                selection.geometry->transform.scale *= new_scale / scale;
            }
        }
    }

    void OnMouseButtonDoubleClicked(mouse::Button &mouse_button) override {
        if (&mouse_button == &mouse::left_button) {
            mouse::is_captured = !mouse::is_captured;
            os::setCursorVisibility(!mouse::is_captured);
            os::setWindowCapture(    mouse::is_captured);
            OnMouseButtonDown(mouse_button);
        }
    }
};

SlimApp* createApp() {
    return new DemoApp();
}