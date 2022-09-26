#pragma once

#include "./canvas.h"
#include "../core/texture.h"

void drawTextureMip(const TextureMip &texture_mip, const Canvas &canvas, const RectI draw_bounds, bool cropped = true, f32 opacity = 1.0f) {
    Color texel_color;
    i32 draw_width = draw_bounds.right - draw_bounds.left+1;
    i32 draw_height = draw_bounds.bottom - draw_bounds.top+1;
    if (cropped) {
        if (draw_width > texture_mip.width) draw_width = texture_mip.width;
        if (draw_height > texture_mip.height) draw_height = texture_mip.height;
        i32 remainder_x = 1 + texture_mip.width - draw_width;
        TexelQuad *texel_quad = texture_mip.texel_quads;
        i32 Y = draw_bounds.top;
        for (i32 y = 0; y < draw_height; y++, Y++) {
            i32 X = draw_bounds.left;
            for (i32 x = 0; x < draw_width; x++, X++, texel_quad++) {
                texel_color.r = (float)texel_quad->R.BR * COLOR_COMPONENT_TO_FLOAT;
                texel_color.g = (float)texel_quad->G.BR * COLOR_COMPONENT_TO_FLOAT;
                texel_color.b = (float)texel_quad->B.BR * COLOR_COMPONENT_TO_FLOAT;
                canvas.setPixel(X, Y, texel_color, opacity);
            }
            texel_quad += remainder_x;
        }
    } else {
        float u_step = 1.0f / (float)draw_width;
        float v_step = 1.0f / (float)draw_height;
        float v = v_step * 0.5f;
        i32 Y = draw_bounds.top;
        for (i32 y = 0; y < draw_height; y++, Y++, v += v_step) {
            i32 X = draw_bounds.left;
            float u = u_step * 0.5f;
            for (i32 x = 0; x < draw_width; x++, X++, u += u_step) {
                texel_color = texture_mip.sample(u, v).color;
                canvas.setPixel(X, Y, texel_color, opacity);
            }
        }
    }
}

void drawTexture(const Texture &texture, const Canvas &canvas, const RectI draw_bounds, bool cropped = true, f32 opacity = 1.0f) {
    if (draw_bounds.right < 0 ||
        draw_bounds.bottom < 0 ||
        draw_bounds.left >= canvas.dimensions.width ||
        draw_bounds.top >= canvas.dimensions.height)
        return;

    u8 mip_level = 0;
    if (!cropped) {
        i32 draw_width = draw_bounds.right - draw_bounds.left+1;
        i32 draw_height = draw_bounds.bottom - draw_bounds.top+1;
        float texel_area = (float)(texture.width * texture.height) / (float)(draw_width * draw_height);
        mip_level = Texture::GetMipLevel(texel_area, texture.mip_count);
    }
    drawTextureMip(texture.mips[mip_level], canvas, draw_bounds, cropped, opacity);
}