#include "./slim/platforms/win32_bitmap.h"
#include "./slim/serialization/texture.h"


struct PixelQuad {
    Pixel TL, TR, BL, BR;

    Color getAverageColor() const {
        return {
                0.25f * (TL.color.r + TR.color.r + BL.color.r + BR.color.r),
                0.25f * (TL.color.g + TR.color.g + BL.color.g + BR.color.g),
                0.25f * (TL.color.b + TR.color.b + BL.color.b + BR.color.b)
        };
    }
};

struct TextureMipLoader {
    u32 width, height;
    Pixel *texels;
    PixelQuad *texel_quads;

    void init(u32 Width, u32 Height) {
        width = Width;
        height = Height;
        texels = new Pixel[width * height];
        texel_quads = new PixelQuad[(width + 1) * (height + 1)];
    }

    void load(bool wrap) {
        PixelQuad *TL, *TR, *BL, *BR;
        bool L, R, T, B;
        const u32 last_y = height - 1;
        const u32 last_x = width - 1;
        const u32 l = 0;
        const u32 r = width;
        const u32 stride = width + 1;
        Pixel *texel = texels;
        PixelQuad *top_texel_quad_line = texel_quads;
        PixelQuad *bottom_texel_quad_line = top_texel_quad_line + height * stride;
        PixelQuad *current_line = top_texel_quad_line, *next_line = top_texel_quad_line + stride;
        for (u32 y = 0; y < height; y++, current_line += stride, next_line += stride) {
            T = (y == 0);
            B = (y == last_y);

            for (u32 x = 0; x < width; x++, texel++) {
                TL = current_line + x;
                TR = current_line + x + 1;
                BL = next_line + x;
                BR = next_line + x + 1;

                L = (x == 0);
                R = (x == last_x);

                TL->BR = TR->BL = BL->TR = BR->TL = *texel;
                if (     L) TL->BL = BL->TL = *texel;
                else if (R) TR->BR = BR->TR = *texel;
                if (wrap) {
                    if (     L) current_line[r].BR = next_line[r].TR = *texel;
                    else if (R) current_line[l].BL = next_line[l].TL = *texel;
                    if (T) {
                        bottom_texel_quad_line[x].BR = bottom_texel_quad_line[x + 1].BL = *texel;
                        if (     L) bottom_texel_quad_line[r].BR = *texel;
                        else if (R) bottom_texel_quad_line[l].BL = *texel;
                    } else if (B) {
                        top_texel_quad_line[x].TR = top_texel_quad_line[x + 1].TL = *texel;
                        if (     L) top_texel_quad_line[r].TR = *texel;
                        else if (R) top_texel_quad_line[l].TL = *texel;
                    }
                } else {
                    if (T) {
                        TL->TR = TR->TL = *texel;

                        if (     L) TL->TL = *texel;
                        else if (R) TR->TR = *texel;

                    } else if (B) {
                        BL->BR = BR->BL = *texel;

                        if (     L) BL->BL = *texel;
                        else if (R) BR->BR = *texel;
                    }
                }
            }
        }
    }
};

void loadMips(Texture &texture, TextureMipLoader *mips) {
    TextureMipLoader *current_mip = mips;
    TextureMipLoader *next_mip = current_mip + 1;

    u32 mip_width  = texture.width;
    u32 mip_height = texture.height;

    PixelQuad *colors_quad;
    u32 stride, offset, start;

    while (mip_width > 4 && mip_height > 4) {
        start = mip_width + 1;
        stride = start * 2;

        mip_width  /= 2;
        mip_height /= 2;

        next_mip->init(mip_width, mip_height);

        for (u32 y = 0; y < mip_height; y++) {
            offset = start + 1;

            for (u32 x = 0; x < mip_width; x++) {
                colors_quad = current_mip->texel_quads + offset;
                next_mip->texels[mip_width * y + x].color = colors_quad->getAverageColor();
                offset += 2;
            }

            start += stride;
        }

        next_mip->load(texture.flags.wrap);

        current_mip++;
        next_mip++;
    }
}

int main(int argc, char *argv[]) {
    Texture texture;

    char* bitmap_file_path = argv[1];
    char* texture_file_path = argv[2];
    for (u8 i = 3; i < (u8)argc; i++) {
        if (     argv[i][0] == '-' && argv[i][1] == 'f') texture.flags.flip = true;
        else if (argv[i][0] == '-' && argv[i][1] == 'c') texture.flags.channel = true;
        else if (argv[i][0] == '-' && argv[i][1] == 'l') texture.flags.linear = true;
        else if (argv[i][0] == '-' && argv[i][1] == 't') texture.flags.tile = true;
        else if (argv[i][0] == '-' && argv[i][1] == 'm') texture.flags.mipmap = true;
        else if (argv[i][0] == '-' && argv[i][1] == 'w') texture.flags.wrap = true;
        else return 0;
    }

    u8* components = loadBitmap(bitmap_file_path, texture);

    u32 mip_width  = texture.width;
    u32 mip_height = texture.height;
    texture.mip_count = 1;
    if (texture.flags.mipmap)
        while (mip_width > 4 && mip_height > 4) {
            mip_width /= 2;
            mip_height /= 2;
            texture.mip_count++;
        }

    auto *mips = new TextureMipLoader[texture.mip_count];
    mips->init(texture.width, texture.height);
    componentsToPixels(components, texture, mips->texels);
    mips->load(texture.flags.wrap);
    if (texture.flags.mipmap) loadMips(texture, mips);

    texture.mips = new TextureMip[texture.mip_count];
    TextureMip *mip = texture.mips;
    TextureMipLoader *loader_mip = mips;
    for (u16 i = 0; i < texture.mip_count; i++, mip++, loader_mip++) {
        mip->width  = loader_mip->width;
        mip->height = loader_mip->height;
        mip->texel_quads = new TexelQuad[(mip->width + 1) * (mip->height + 1)];

        TexelQuad *texel_quad = mip->texel_quads;
        PixelQuad *loader_texel_quad = loader_mip->texel_quads;
        u32 texel_quads_count = (u32)(mip->width + 1) * (u32)(mip->height + 1);
        for (u32 t = 0; t < texel_quads_count; t++, texel_quad++, loader_texel_quad++) {
            texel_quad->R.TL = (u8)(loader_texel_quad->TL.color.r * FLOAT_TO_COLOR_COMPONENT);
            texel_quad->G.TL = (u8)(loader_texel_quad->TL.color.g * FLOAT_TO_COLOR_COMPONENT);
            texel_quad->B.TL = (u8)(loader_texel_quad->TL.color.b * FLOAT_TO_COLOR_COMPONENT);

            texel_quad->R.TR = (u8)(loader_texel_quad->TR.color.r * FLOAT_TO_COLOR_COMPONENT);
            texel_quad->G.TR = (u8)(loader_texel_quad->TR.color.g * FLOAT_TO_COLOR_COMPONENT);
            texel_quad->B.TR = (u8)(loader_texel_quad->TR.color.b * FLOAT_TO_COLOR_COMPONENT);

            texel_quad->R.BL = (u8)(loader_texel_quad->BL.color.r * FLOAT_TO_COLOR_COMPONENT);
            texel_quad->G.BL = (u8)(loader_texel_quad->BL.color.g * FLOAT_TO_COLOR_COMPONENT);
            texel_quad->B.BL = (u8)(loader_texel_quad->BL.color.b * FLOAT_TO_COLOR_COMPONENT);

            texel_quad->R.BR = (u8)(loader_texel_quad->BR.color.r * FLOAT_TO_COLOR_COMPONENT);
            texel_quad->G.BR = (u8)(loader_texel_quad->BR.color.g * FLOAT_TO_COLOR_COMPONENT);
            texel_quad->B.BR = (u8)(loader_texel_quad->BR.color.b * FLOAT_TO_COLOR_COMPONENT);

        }
    }

    save(texture, texture_file_path);

    return 0;
}