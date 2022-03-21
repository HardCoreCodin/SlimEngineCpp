#pragma once

#include "./scene/scene.h"
//#include "./renderer/mesh_shaders.h"


namespace window {
    u16 width{DEFAULT_WIDTH};
    u16 height{DEFAULT_HEIGHT};
    char* title{(char*)""};
    u32 *content{nullptr};

    union RGBA2u32 {
        u32 value;
        RGBA rgba;

        RGBA2u32() : value{0} {}
    };

    void display(const Canvas &canvas) {
        PixelQuad *src_pixel = canvas.pixels;
        u32 *trg_value = content;
        vec3 color;
        RGBA2u32 background_pixel, trg_pixel;
        background_pixel.rgba.R = (u8)(canvas.background.color.r * canvas.background.color.r * FLOAT_TO_COLOR_COMPONENT);
        background_pixel.rgba.G = (u8)(canvas.background.color.g * canvas.background.color.g * FLOAT_TO_COLOR_COMPONENT);
        background_pixel.rgba.B = (u8)(canvas.background.color.b * canvas.background.color.b * FLOAT_TO_COLOR_COMPONENT);
        background_pixel.rgba.A = (u8)(canvas.background.opacity * canvas.background.opacity * FLOAT_TO_COLOR_COMPONENT);
        if (canvas.antialias) {
            for (u16 y = 0; y < canvas.dimensions.height; y++) {
                for (u16 x = 0; x < canvas.dimensions.width; x++, src_pixel++, trg_value++) {
                    if (src_pixel->TL.opacity != 0.0f ||
                        src_pixel->TR.opacity != 0.0f ||
                        src_pixel->BL.opacity != 0.0f ||
                        src_pixel->BR.opacity != 0.0f) {
                        color = src_pixel->TL.color * (src_pixel->TL.opacity * 0.25f);
                        color = src_pixel->TR.color.scaleAdd(src_pixel->TR.opacity * 0.25f, color);
                        color = src_pixel->BL.color.scaleAdd(src_pixel->BL.opacity * 0.25f, color);
                        color = src_pixel->BR.color.scaleAdd(src_pixel->BR.opacity * 0.25f, color);
                        trg_pixel.rgba.R = (u8)(color.r > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.r)));
                        trg_pixel.rgba.G = (u8)(color.g > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.g)));
                        trg_pixel.rgba.B = (u8)(color.b > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.b)));
                        trg_pixel.rgba.A = (u8)(clampedValue(src_pixel->TL.opacity) * FLOAT_TO_COLOR_COMPONENT);
                    } else trg_pixel = background_pixel;
                    *trg_value = trg_pixel.value;
                }
            }
        } else {
            for (u16 y = 0; y < canvas.dimensions.height; y++) {
                for (u16 x = 0; x < canvas.dimensions.width; x++, src_pixel++, trg_value++) {
                    if (src_pixel->TL.depth == INFINITY)
                        trg_pixel = background_pixel;
                    else {
                        color = src_pixel->TL.color * src_pixel->TL.opacity;
                        trg_pixel.rgba.R = (u8)(color.r > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.r)));
                        trg_pixel.rgba.G = (u8)(color.g > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.g)));
                        trg_pixel.rgba.B = (u8)(color.b > 1.0f ? MAX_COLOR_VALUE : (FLOAT_TO_COLOR_COMPONENT * sqrt(color.b)));
                        trg_pixel.rgba.A = (u8)(clampedValue(src_pixel->TL.opacity) * FLOAT_TO_COLOR_COMPONENT);
                    }
                    *trg_value = trg_pixel.value;
                }
            }
        }
    }
}


struct MemoryInitializer {
    MemoryInitializer() {
        u64 memory_size = sizeof(Selection) + settings::memory::extra;
        memory_size += settings::scene::geometries * sizeof(Geometry);
//        memory_size += settings::scene::textures   * sizeof(Texture);
        memory_size += settings::scene::meshes     * sizeof(Mesh);
        memory_size += settings::scene::curves     * sizeof(Curve);
        memory_size += settings::scene::boxes      * sizeof(Box);
        memory_size += settings::scene::grids      * sizeof(Grid);
        memory_size += settings::scene::cameras    * sizeof(Camera);
//        memory_size += settings::scene::materials  * sizeof(Material);
//        memory_size += settings::scene::lights     * sizeof(Light);
        memory_size += settings::hud::line_count   * sizeof(HUDLine);

//        if (settings::scene::textures && settings::scene::texture_files) {
//            for (u32 i = 0; i < settings::scene::textures; i++)
//                memory_size += Texture::getMemorySize(settings::scene::texture_files[i].char_ptr);
//        }

        if (settings::scene::meshes && settings::scene::mesh_files) {
            Mesh mesh;
            for (u32 i = 0; i < settings::scene::meshes; i++) {
                memory_size += Mesh::getMemorySize(settings::scene::mesh_files[i].char_ptr, mesh) + sizeof(AABB);
//                if (mesh.triangle_count > settings::scene::max_triangle_count) settings::scene::max_triangle_count = mesh.triangle_count;
//                if (mesh.vertex_count > settings::scene::max_vertex_count) settings::scene::max_vertex_count = mesh.vertex_count;
//                if (mesh.normals_count > settings::scene::max_normal_count) settings::scene::max_normal_count = mesh.normals_count;
            }
        }

//        memory_size += settings::scene::max_vertex_count * (sizeof(vec3) + sizeof(vec4) + 1);
//        memory_size += settings::scene::max_normal_count * sizeof(vec3);
        memory_size += FRAME_BUFFER_MEMORY_SIZE;

        memory::capacity = memory_size;
        memory::address = (u8*)os::getMemory(memory_size);
        memory::initialized = bool(memory::address);
    }
};


struct SlimEngine {
    MemoryInitializer _memory_initializer;
    Scene scene;
    Viewport viewport{scene.cameras};
//    Rasterizer rasterizer{scene};

    bool is_running{true};

    virtual void OnWindowRedraw() {};
    virtual void OnWindowResize(u16 width, u16 height) {};
    virtual void OnKeyChanged(  u8 key, bool pressed) {};
    virtual void OnMouseButtonUp(  mouse::Button &mouse_button) {};
    virtual void OnMouseButtonDown(mouse::Button &mouse_button) {};
    virtual void OnMouseButtOnDoubleClicked(mouse::Button &mouse_button) {};
    virtual void OnMouseWheelScrolled(f32 amount) {};
    virtual void OnMousePositionSet(i32 x, i32 y) {};
    virtual void OnMouseMovementSet(i32 x, i32 y) {};
    virtual void OnMouseRawMovementSet(i32 x, i32 y) {};

    void* allocateMemory(u64 size) {
        void *new_memory = memory::allocate(size);
        if (new_memory) return new_memory;

        is_running = false;

        return nullptr;
    }

    void resize(u16 width, u16 height) {
        window::width = width;
        window::height = height;
        viewport.canvas.dimensions.update(width, height, width);
        viewport.updateProjectionMatrix();
        viewport.updateProjectionPlane();

        OnWindowResize(width, height);

        update();
    }

    void update() {
        Canvas &canvas = viewport.canvas;
        HUD &hud = viewport.hud;

        canvas.clear();

        time::update_timer.beginFrame();
        OnWindowRedraw();
        time::update_timer.endFrame();

        if (hud.settings.show)
            hud.draw(canvas);

        window::display(canvas);
        mouse::resetChanges();
    }
};

SlimEngine* createEngine();

#include "./platforms/win32.h"