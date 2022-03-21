#pragma once

#include "../core/types.h"
//
//union TexelQuad {
//    struct {
//        vec4 TL, TR, BL, BR;
//    };
//    vec4 quadrants[4];
//
//    TexelQuad(const TexelQuad &texel_quad) : TL{texel_quad.TL}, TR{texel_quad.TR}, BL{texel_quad.BL}, BR{texel_quad.BR}{}
//};
//
//struct TextureMip {
//    u16 width, height;
//    vec4 *texels;
//    TexelQuad *texel_quads;
//    TexelQuad **texel_quad_lines;
//
//    INLINE vec4 sample(const vec2 &UV, bool filter) const {
//        f32 u = UV.u;
//        f32 v = UV.v;
//        if (u > 1) u -= (f32)((u32)u);
//        if (v > 1) v -= (f32)((u32)v);
//
//        const f32 offset = filter ? 0.5f : -0.5f;
//
//        const f32 U = u * (f32)width  + offset;
//        const f32 V = v * (f32)height + offset;
//        const u32 x = (u32)U;
//        const u32 y = (u32)V;
//
//        if (!filter) return texels[width * y + x];
//
//        const f32 r = U - (f32)x;
//        const f32 b = V - (f32)y;
//        const f32 l = 1 - r;
//        const f32 t = 1 - b;
//        const f32 factors[4] = {
//                t * l, t * r,
//                b * l, b * r
//        };
//        const TexelQuad texel_quad{texel_quad_lines[y][x]};
//        vec4 texel{};
//        for (u8 i = 0; i < 4; i++) texel = texel_quad.quadrants[i].scaleAdd(factors[i], texel);
//
//        return texel;
//    }
//};
//
//struct Texture {
//    u16 width, height;
//    u8 mip_count;
//    bool wrap, mipmap, filter;
//    TextureMip *mips;
//
//    INLINE vec4 sample(const vec2 &UV, const vec2 &dUV) const {
//        u8 mip_level = 0;
//        if (mipmap) {
//            const f32 pixel_width  = dUV.u * (f32)width;
//            const f32 pixel_height = dUV.v * (f32)height;
//            f32 pixel_size = (f32)(pixel_width + pixel_height) * 0.5f;
//            const u8 last_mip = mip_count - 1;
//
//            while (pixel_size > 1 && mip_level < last_mip) {
//                pixel_size /= 2;
//                mip_level += 1;
//            }
//        }
//
//        return mips[mip_level].sample(UV, filter);
//    }
//
//    static u32 getMemorySize(char* file_path) {
//        void *file = os::openFileForReading(file_path);
//        static Texture texture;
//
//        os::readFromFile(&texture.width,  sizeof(u16),  file);
//        os::readFromFile(&texture.height, sizeof(u16),  file);
//        os::readFromFile(&texture.filter, sizeof(bool), file);
//        os::readFromFile(&texture.mipmap, sizeof(bool), file);
//
//        u16 mip_width  = texture.width;
//        u16 mip_height = texture.height;
//
//        u32 memory_size = 0;
//
//        do {
//            memory_size += sizeof(TextureMip);
//            memory_size += mip_width * mip_height * sizeof(vec4);
//            if (texture.filter) {
//                memory_size += (mip_width + 1) * (mip_height + 1) * sizeof(TexelQuad);
//                memory_size += (mip_height + 1) * sizeof(TexelQuad*);
//            }
//
//            mip_width /= 2;
//            mip_height /= 2;
//        } while (texture.mipmap && mip_width > 2 && mip_height > 2);
//
//        os::closeFile(file);
//
//        return memory_size;
//    }
//
//    void load(char* file_path) {
//        void *file = os::openFileForReading(file_path);
//        os::readFromFile(&width,  sizeof(u16),  file);
//        os::readFromFile(&height, sizeof(u16),  file);
//        os::readFromFile(&filter, sizeof(bool), file);
//        os::readFromFile(&mipmap, sizeof(bool), file);
//        os::readFromFile(&wrap,   sizeof(bool), file);
//        os::readFromFile(&mip_count, sizeof(u8), file);
//
//        mips = (TextureMip*)memory::allocate(sizeof(TextureMip) * mip_count);
//
//        u32 size, mip_height, mip_stride;
//        TextureMip *texture_mip = mips;
//        for (u8 mip_index = 0; mip_index < mip_count; mip_index++, texture_mip++) {
//            os::readFromFile(&texture_mip->width,  sizeof(u16), file);
//            os::readFromFile(&texture_mip->height, sizeof(u16), file);
//
//            size = sizeof(vec4) * texture_mip->width * texture_mip->height;
//            texture_mip->texels = (vec4*)memory::allocate(size);
//            os::readFromFile(texture_mip->texels, size, file);
//
//            if (filter) {
//                mip_height = texture_mip->height + 1;
//                mip_stride = texture_mip->width  + 1;
//
//                size = sizeof(TexelQuad) * mip_height * mip_stride;
//                texture_mip->texel_quads = (TexelQuad*)memory::allocate(size);
//                os::readFromFile(texture_mip->texel_quads, size, file);
//
//                size = sizeof(TexelQuad*) * mip_height;
//                texture_mip->texel_quad_lines = (TexelQuad**)memory::allocate(size);
//                TexelQuad *line_start = texture_mip->texel_quads;
//                TexelQuad **line = texture_mip->texel_quad_lines;
//                for (u16 y = 0; y < mip_height; y++, line++, line_start += mip_stride) *line = line_start;
//            }
//        }
//
//        os::closeFile(file);
//    }
//};