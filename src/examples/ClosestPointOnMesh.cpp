#include "../slim/scene/selection.h"
#include "../slim/draw/selection.h"
#include "../slim/draw/hud.h"
#include "../slim/draw/triangle.h"
#include "../slim/draw/curve.h"
#include "../slim/draw/grid.h"
#include "../slim/draw/mesh.h"
#include "../slim/draw/rtree.h"
#include "../slim/app.h"

#include "ClosestPointOnMesh.h"


struct ClosestPointOnMeshApp : SlimApp {
    // Viewport:
    Camera camera{
            {0, 10, -15},
            {-25*DEG_TO_RAD,0, 0}
    }, *cameras{&camera};
    Canvas canvas;
    Viewport viewport{canvas, &camera};
    bool antialias = false;

    bool draw_rtree = false;
    bool draw_query = false;
    bool draw_query_aabbs = false;
    bool draw_query_triangles = false;

    // HUD:
    HUDLine QueryLine{(char*)"Query : ", (char*)"On",(char*)"Off", &draw_query, true};
    HUDLine AABBsLine{(char*)"AABBs : ", (char*)"On", (char*)"Off", &draw_query_aabbs, true};
    HUDLine TriesLine{(char*)"Tries : ", (char*)"On", (char*)"Off", &draw_query_triangles, true};
    HUDLine RTreeLine{(char*)"RTree : ", (char*)"On", (char*)"Off", &draw_rtree, true};
    HUDSettings hud_settings{4};
    HUD hud{hud_settings, &QueryLine};

    // Scene:
    Grid grid{11,
              11}, *grids{&grid};
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
    Geometry sphere_geo{{{4, 4, 2}}, GeometryType_Curve, BrightYellow};
    Geometry mesh1{{{+8, 5, 0} }, GeometryType_Mesh, BrightBlue};
    Geometry mesh2{{{-8, 5, 0} }, GeometryType_Mesh, BrightGrey}, *geometries{&grid1};

    Transform *mesh_transform = &mesh1.transform;

    char strings[3][100] = {};
    String mesh_files[3] = {
            String::getFilePath((char*)"dog.mesh" ,strings[1],(char*)__FILE__),
            String::getFilePath((char*)"suzanne.mesh",strings[0],(char*)__FILE__),
            String::getFilePath((char*)"cube.mesh" ,strings[2],(char*)__FILE__)
    };
    Mesh meshes[3], *mesh = meshes;

    SceneCounts counts{1, 4, 1, 0, 1, 3 };
    Scene scene{counts,nullptr, cameras, geometries, grids, nullptr,curves,
                meshes, mesh_files, nullptr};
    Selection selection;

    MeshQuery mesh_query{mesh};

    // Drawing:
    f32 opacity = 0.5f;

    u16 min_depth = 0;
    u16 max_depth = 0;
    f32 world_max_distance = 1;
    f32 max_distance = 1;
    vec3 closest_point;
    TrianglePointOn point_on = TrianglePointOn_None;


    void drawMeshRTreeQuery() {
        const Camera &cam = *viewport.camera;
        vec3 pos, v1, v2, v3;
        Edge edge;
        for (u32 result_index = 0; result_index < mesh_query.result_count; result_index++) {
            RTreeQuery::Result &result = mesh_query.results[result_index];
            for (u32 triangle_index = result.start; triangle_index < result.end; triangle_index++) {
                Triangle &triangle = mesh->triangles[triangle_index];
                v1 = cam.internPos(mesh_transform->externPos(triangle.position));
                v2 = cam.internPos(mesh_transform->externPos(triangle.position + triangle.V));
                v3 = cam.internPos(mesh_transform->externPos(triangle.position + triangle.U));
                viewport.projectPoint(v1);
                viewport.projectPoint(v2);
                viewport.projectPoint(v3);
                viewport.canvas.fillTriangle(v1, v2, v3, BrightMagenta, opacity);
            }
        }
    }

    void OnRender() override {
        canvas.clear();
        drawGrid(grid, grid1.transform, viewport, grid1.color, opacity);
        drawMesh(*mesh, mesh1.transform, false, viewport, mesh1.color, opacity);
        drawMesh(*mesh, mesh2.transform, false, viewport, mesh2.color, opacity);
        drawCurve(sphere, sphere_geo.transform, viewport, sphere_geo.color, 0.2f);
        drawCurve(sphere, sphere_center_transform, viewport, Cyan, 0.2f);

        RTree &rtree = mesh->rtree;
        if (draw_rtree) drawRTree(rtree, *mesh_transform, viewport, min_depth, max_depth);
        if (mesh_query.result_count) {
            if (draw_query_aabbs) drawRTreeQuery(&mesh_query, rtree.nodes, *mesh_transform, viewport, BrightGreen, opacity);
            if (draw_query_triangles) drawMeshRTreeQuery();
        }
        if (draw_query && point_on) {
            Edge edge;
            edge.from = camera.internPos(sphere_center_transform.position);
            edge.to = camera.internPos(closest_point_transform.position);
            drawEdge(edge, viewport, Cyan, opacity, 3);
            drawCurve(sphere, closest_point_transform, viewport, point_on == TrianglePointOn_Vertex ? Red : White, opacity);

            if (point_on != TrianglePointOn_Vertex) {
                Triangle &triangle = mesh->triangles[mesh_query.closest_triangle_index];
                vec3 v1 = camera.internPos(mesh_transform->externPos(triangle.position));
                vec3 v2 = camera.internPos(mesh_transform->externPos(triangle.position + triangle.V));
                vec3 v3 = camera.internPos(mesh_transform->externPos(triangle.position + triangle.U));
                if (point_on == TrianglePointOn_Edge) {
                    f32 u = mesh_query.closest_uv.u;
                    f32 v = mesh_query.closest_uv.v;
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
        }

        if (controls::is_pressed::alt) drawSelection(selection, viewport, scene);
        if (hud.enabled) drawHUD(hud, canvas);

        canvas.drawToWindow();
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
            u16 depth = meshes[scene.geometries[1].id].rtree.height;
            if (key == controls::key_map::tab) hud.enabled = !hud.enabled;
            else if (key == '1') draw_query = !draw_query;
            else if (key == '2') draw_query_aabbs = !draw_query_aabbs;
            else if (key == '3') draw_query_triangles = !draw_query_triangles;
            else if (key == '4') draw_rtree = !draw_rtree;
            else if (key == '0') {
                if (max_depth < depth) max_depth++;
                if (min_depth < depth && controls::is_pressed::shift) min_depth++;
            }
            else if (key == '9') {
                if (max_depth > 0) max_depth--;
                if (min_depth > 0 && controls::is_pressed::shift) min_depth--;
            }
            else if (key == '8') { if (min_depth < depth) min_depth++; }
            else if (key == '7') { if (min_depth > 0) min_depth--; }
            else if (key == 'M') {
                u32 old_mesh_id = scene.geometries[1].id;
                u32 new_mesh_id = (old_mesh_id + 1) % 3;
                scene.geometries[1].id = new_mesh_id;
                scene.geometries[2].id = new_mesh_id;
                mesh = mesh_query.mesh = &meshes[new_mesh_id];
            }
        }
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
                mesh_transform = &selection.geometry->transform;
        }
        sphere_center_transform.position = sphere_geo.transform.position;

        vec3 &scale = mesh_transform->scale;
        max_distance = world_max_distance / ((scale.x == scale.y && scale.x == scale.z) ? scale.x : (scale.length()));
        vec3 point = mesh_transform->internPos(sphere_geo.transform.position);
        point_on = mesh_query.findClosestPointOnMesh(point, max_distance, closest_point);
        if (point_on) closest_point_transform.position = mesh_transform->externPos(closest_point);
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
                world_max_distance += mouse::wheel_scroll_amount * 0.01f;
                if (world_max_distance < 0.1f) world_max_distance = 0.1f;
                if (world_max_distance > 5.0f) world_max_distance = 5.0;
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
    return new ClosestPointOnMeshApp();
}