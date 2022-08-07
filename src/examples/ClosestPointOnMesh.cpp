#include "../slim/scene/selection.h"
#include "../slim/draw/selection.h"
#include "../slim/draw/hud.h"
#include "../slim/draw/curve.h"
#include "../slim/draw/grid.h"
#include "../slim/draw/mesh.h"
#include "../slim/draw/rtree.h"
#include "../slim/app.h"
// Or using the single-header file:
//#include "../slim.h"

struct ClosestPointOnMeshApp : SlimApp {
    // Viewport:
    Camera camera{
        {0, 10, -15},
        {-25*DEG_TO_RAD,0, 0}
    }, *cameras{&camera};
    Canvas canvas;
    Viewport viewport{canvas, &camera};
    bool antialias = false;

    // HUD:
    HUDLine AA{(char*)"AA : ",
               (char*)"On",
               (char*)"Off",
               &antialias,
               true};
    HUDSettings hud_settings{1};
    HUD hud{hud_settings, &AA};

    // Scene:
    Grid grid{11,
              11}, *grids{&grid};
    Curve sphere{ CurveType_Sphere,  30}, *curves{&sphere};

    Transform grid_transform{
          {0, 0, 0},
        {0, 45 * DEG_TO_RAD, 0},
        {5, 1, 5}
    };

    Geometry grid1{grid_transform,GeometryType_Grid, Green};
    Geometry sphere_geo{{{4, 4, 2}}, GeometryType_Curve, Magenta};
    Geometry mesh1{{{+8, 5, 0} }, GeometryType_Mesh, Blue};
    Geometry mesh2{{{-8, 5, 0} }, GeometryType_Mesh, Cyan}, *geometries{&grid1};

    char strings[2][100] = {};
    String mesh_files[2] = {
        String::getFilePath((char*)"suzanne.mesh",strings[0],(char*)__FILE__),
        String::getFilePath((char*)"dog.mesh" ,strings[1],(char*)__FILE__)
    };
    Mesh meshes[2];

    SceneCounts counts{1, 3, 1, 0, 1, 2 };
    Scene scene{counts,nullptr, cameras, geometries, grids,nullptr,curves,
                meshes, mesh_files};
    Selection selection;

    // Drawing:
    f32 opacity = 0.5f;

    u16 min_depth = 0;
    u16 max_depth = 0;
    vec3 point;
    f32 max_distance = 1;

    bool draw_rtree = false;
    bool draw_rtree_query_result = false;
    bool draw_rtree_query_aabbs = false;
    bool draw_rtree_query_triangles = false;

    void OnRender() override {
        canvas.clear();

        drawGrid(grid, grid1.transform, viewport, grid1.color, opacity);

        Mesh &mesh{meshes[scene.geometries[1].id]};
        drawMesh(mesh, mesh1.transform, false, viewport, mesh1.color, opacity);
        drawMesh(mesh, mesh2.transform, false, viewport, mesh2.color, opacity);

        if (draw_rtree) {
            drawRTree(mesh.rtree, mesh1.transform, viewport, min_depth, max_depth);
            drawRTree(mesh.rtree, mesh2.transform, viewport, min_depth, max_depth);
        }

        point = mesh1.transform.internPos(sphere_geo.transform.position);

        vec3 &scale = mesh1.transform.scale;
        f32 local_space_max_distance = max_distance / ((scale.x == scale.y && scale.x == scale.z) ? scale.x : (scale.length()));
        if (mesh.rtree.collectOverlappedNodes(point, local_space_max_distance)) {
            if (draw_rtree_query_aabbs)
                drawRTreeQueryResult(mesh.rtree.query_result, mesh.rtree.nodes,
                                     mesh1.transform, viewport, Red);

            Edge edge;
            if (draw_rtree_query_triangles)
                for (u32 result_index = 0; result_index < mesh.rtree.query_result.node_count; result_index++) {
                    const RTreeNode &node = mesh.rtree.nodes[mesh.rtree.query_result.node_ids[result_index]];
                    for (u16 i = 0; i < node.child_count; i++) {
                        Triangle &triangle = mesh.triangles[node.first_child_id + i];
                        vec3 position = camera.internPos(mesh1.transform.externPos(triangle.position));
                        edge.from = position;
                        edge.to =  camera.internPos(mesh1.transform.externPos(triangle.position + triangle.U));
                        edge.to.z -= 0.01;
                        drawEdge(edge, viewport, Green, 1, 1);

                        edge.from = camera.internPos(mesh1.transform.externPos(triangle.position + triangle.V));
                        edge.from.z -= 0.01;
                        drawEdge(edge, viewport, Green, 1, 1);

                        edge.to = position;
                        drawEdge(edge, viewport, Green, 1, 1);
                    }
                }
        }
        drawCurve(sphere, sphere_geo.transform, viewport, sphere_geo.color);

        if (controls::is_pressed::alt) drawSelection(selection, viewport, scene);
        if (hud.enabled)
            drawHUD(hud, canvas);

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
            else if (key == '1') draw_rtree = !draw_rtree;
            else if (key == '2') draw_rtree_query_result = !draw_rtree_query_result;
            else if (key == '3') draw_rtree_query_aabbs = !draw_rtree_query_aabbs;
            else if (key == '4') draw_rtree_query_triangles = !draw_rtree_query_triangles;
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
                u32 new_mesh_id = (old_mesh_id + 1) % 2;
                scene.geometries[1].id = new_mesh_id;
                scene.geometries[2].id = new_mesh_id;
            }
        }
    }

    void OnUpdate(f32 delta_time) override {
        if (!mouse::is_captured) selection.manipulate(viewport, scene);
        if (!controls::is_pressed::alt &&
            !controls::is_pressed::ctrl &&
            !controls::is_pressed::shift)
            viewport.updateNavigation(delta_time);
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
                max_distance += mouse::wheel_scroll_amount * 0.01f;
                if (max_distance < 0.1f) max_distance = 0.1f;
                if (max_distance > 5.0f) max_distance = 5.0;
                sphere_geo.transform.scale = max_distance;
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