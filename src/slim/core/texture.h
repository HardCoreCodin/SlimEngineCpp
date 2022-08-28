#pragma once

#include "./base.h"

struct TexelQuadComponent {
    u8 TL, TR, BL, BR;
};

struct TexelQuad {
    TexelQuadComponent R, G, B;
};

struct TextureMip {
    u16 width, height;
    TexelQuad *texel_quads;

    INLINE_XPU Pixel sample(f32 u, f32 v) const {
        if (u > 1) u -= (f32)((u32)u);
        if (v > 1) v -= (f32)((u32)v);

        const f32 U = u * (f32)width  + 0.5f;
        const f32 V = v * (f32)height + 0.5f;
        const u32 x = (u32)U;
        const u32 y = (u32)V;
        const f32 r = U - (f32)x;
        const f32 b = V - (f32)y;
        const f32 l = 1 - r;
        const f32 t = 1 - b;
        const f32 tl = t * l * COLOR_COMPONENT_TO_FLOAT;
        const f32 tr = t * r * COLOR_COMPONENT_TO_FLOAT;
        const f32 bl = b * l * COLOR_COMPONENT_TO_FLOAT;
        const f32 br = b * r * COLOR_COMPONENT_TO_FLOAT;

        const TexelQuad texel_quad = texel_quads[y * (width + 1) + x];
        return {
                fast_mul_add((f32)texel_quad.R.BR, br, fast_mul_add((f32)texel_quad.R.BL, bl, fast_mul_add((f32)texel_quad.R.TR, tr, (f32)texel_quad.R.TL * tl))),
                fast_mul_add((f32)texel_quad.G.BR, br, fast_mul_add((f32)texel_quad.G.BL, bl, fast_mul_add((f32)texel_quad.G.TR, tr, (f32)texel_quad.G.TL * tl))),
                fast_mul_add((f32)texel_quad.B.BR, br, fast_mul_add((f32)texel_quad.B.BL, bl, fast_mul_add((f32)texel_quad.B.TR, tr, (f32)texel_quad.B.TL * tl))),
                1.0f
        };
    }
};

struct Texture {
    u16 width, height;
    u8 mip_count;
    bool wrap, mipmap;
    TextureMip *mips;

    XPU static u8 GetMipLevel(f32 texel_area, u8 mip_count) {
        u8 mip_level = 0;
        while (texel_area > 1 && ++mip_level < mip_count) texel_area *= 0.25f;
        if (mip_level >= mip_count)
            mip_level = mip_count - 1;

        return mip_level;
    }

    XPU static u8 GetMipLevel(u16 width, u16 height, u8 mip_count, f32 uv_area) {
        return GetMipLevel(uv_area * (f32)(width * height), mip_count);
    }

    XPU static u8 GetMipLevel(const Texture &texture, f32 uv_area) {
        return GetMipLevel(uv_area * (f32)(texture.width * texture.height), texture.mip_count);
    }

    INLINE_XPU Pixel sample(f32 u, f32 v, f32 uv_area) const {
        return mips[mipmap ? GetMipLevel(uv_area * (f32)(width * height), mip_count) : 0].sample(u, v);
    }
};