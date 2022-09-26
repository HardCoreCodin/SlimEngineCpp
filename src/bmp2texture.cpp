#ifdef COMPILER_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "./slim/platforms/win32_base.h"
#include "./slim/core/texture.h"

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
    void *filePtr;  //our file pointer
    BITMAPFILEHEADER bitmapFileHeader;  //our bitmap file header
    unsigned char *bitmapImage;  //store image data

    //open file in read binary mode
    filePtr = win32_openFileForReading(filename);
    if (filePtr == nullptr)
        return nullptr;

    //read the bitmap file header
    win32_readFromFile(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), filePtr);

    //verify that this is a .BMP file by checking bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        win32_closeFile(filePtr);
        return nullptr;
    }

    //read the bitmap info header
    win32_readFromFile(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), filePtr);

    //move file pointer to the beginning of bitmap data
    SetFilePointer(filePtr, (LONG)bitmapFileHeader.bfOffBits, nullptr, FILE_BEGIN);

    u32 bytes_per_pixel = bitmapInfoHeader->biBitCount / 8;
    if (bitmapInfoHeader->biSizeImage == 0) {
        bitmapInfoHeader->biSizeImage = bytes_per_pixel;
        bitmapInfoHeader->biSizeImage *= bitmapInfoHeader->biWidth;
        bitmapInfoHeader->biSizeImage *= bitmapInfoHeader->biHeight < 0 ? -bitmapInfoHeader->biHeight : bitmapInfoHeader->biHeight;
    }
    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //verify memory allocation
    if (!bitmapImage) {
        free(bitmapImage);
        win32_closeFile(filePtr);
        return nullptr;
    }

    //read in the bitmap image data
    win32_readFromFile(bitmapImage, bitmapInfoHeader->biSizeImage, filePtr);

    //close file and return bitmap image data
    win32_closeFile(filePtr);

    if (bitmapInfoHeader->biHeight > 0) {
        unsigned char* flipped_bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);
        u32 stride = bitmapInfoHeader->biWidth * bytes_per_pixel;
        u32 trg_offset = (bitmapInfoHeader->biHeight - 1) * stride;
        u32 src_offset = 0;
        for (i32 y = 0; y < bitmapInfoHeader->biHeight; y++) {
            for (i32 x = 0; x < bitmapInfoHeader->biWidth; x++) {
                flipped_bitmapImage[trg_offset++] = bitmapImage[src_offset++];
                flipped_bitmapImage[trg_offset++] = bitmapImage[src_offset++];
                flipped_bitmapImage[trg_offset++] = bitmapImage[src_offset++];
            }
            trg_offset -= stride;
            trg_offset -= stride;
        }
        free(bitmapImage);
        return flipped_bitmapImage;
    }

    return bitmapImage;
}

struct TexelQuadLoader {
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
    u16 width, height;
    Pixel *texels;
    TexelQuadLoader *texel_quads;

    void init(u16 Width, u16 Height) {
        width = Width;
        height = Height;
        texels = new Pixel[width * height];
        texel_quads = new TexelQuadLoader[(width + 1) * (height + 1)];
    }

    void load(bool wrap) {
        TexelQuadLoader *TL, *TR, *BL, *BR;
        bool L, R, T, B;
        const u16 last_y = height - 1;
        const u16 last_x = width - 1;
        const u32 l = 0;
        const u32 r = width;
        const u16 stride = width + 1;
        Pixel *texel = texels;
        TexelQuadLoader *top_texel_quad_line = texel_quads;
        TexelQuadLoader *bottom_texel_quad_line = top_texel_quad_line + height * stride;
        TexelQuadLoader *current_line = top_texel_quad_line, *next_line = top_texel_quad_line + stride;
        for (u16 y = 0; y < height; y++, current_line += stride, next_line += stride) {
            T = (y == 0);
            B = (y == last_y);

            for (u16 x = 0; x < width; x++, texel++) {
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

struct TextureLoader {
    u16 width, height;
    float gamma;
    bool wrap, mipmap;
    u8 mip_count = 1;
    TextureMipLoader *mips;

    TextureLoader(u16 width, u16 height, float gamma = 2.2, bool wrap = true, bool mipmap = true) :
            width{width}, height{height}, gamma{gamma}, wrap{wrap}, mipmap{mipmap} {}

    void load(u8 *texel_components, u8 bit_count) {
        u16 mip_width  = width;
        u16 mip_height = height;
        mip_count = 1;
        if (mipmap)
            while (mip_width > 4 && mip_height > 4) {
                mip_width /= 2;
                mip_height /= 2;
                mip_count++;
            }

        mips = new TextureMipLoader[mip_count];
        mips->init(width, height);

        Pixel *texel = mips->texels;
        u8 *texel_component = texel_components;
        u16 x, y;
        for (y = 0; y < height; y++)
            for (x = 0; x < width; x++, texel++) {
                texel->color.r = powf((f32)(*(texel_component++)) * COLOR_COMPONENT_TO_FLOAT, gamma);
                texel->color.g = powf((f32)(*(texel_component++)) * COLOR_COMPONENT_TO_FLOAT, gamma);
                texel->color.b = powf((f32)(*(texel_component++)) * COLOR_COMPONENT_TO_FLOAT, gamma);
                if (bit_count == 32) texel_component++;
            }

        mips->load(wrap);

        if (!mipmap)
            return;

        TextureMipLoader *current_mip = mips;
        TextureMipLoader *next_mip = current_mip + 1;

        mip_width  = width;
        mip_height = height;

        TexelQuadLoader *colors_quad;
        u32 stride, offset, start;

        while (mip_width > 4 && mip_height > 4) {
            start = mip_width + 1;
            stride = start * 2;

            mip_width  /= 2;
            mip_height /= 2;

            next_mip->init(mip_width, mip_height);

            for (y = 0; y < mip_height; y++) {
                offset = start + 1;

                for (x = 0; x < mip_width; x++) {
                    colors_quad = current_mip->texel_quads + offset;
                    texel = next_mip->texels + (mip_width * y + x);
                    texel->color = colors_quad->getAverageColor();
                    offset += 2;
                }

                start += stride;
            }

            next_mip->load(wrap);

            current_mip++;
            next_mip++;
        }
    }
};

void loadTexture(Texture *texture, u16 width, u16 height, float gamma, bool wrap, bool mipmap, u8 *texel_components, u8 bit_count) {
    TextureLoader texture_loader{width, height, gamma, wrap, mipmap};
    texture_loader.load(texel_components, bit_count);

    texture->wrap = wrap;
    texture->mipmap = mipmap;
    texture->width = width;
    texture->height = height;
    texture->mip_count = texture_loader.mip_count;
    texture->mips = new TextureMip[texture->mip_count];

    TextureMip *mip = texture->mips;
    TextureMipLoader *loader_mip = texture_loader.mips;
    for (u8 i = 0; i < texture->mip_count; i++, mip++, loader_mip++) {
        mip->width  = loader_mip->width;
        mip->height = loader_mip->height;
        mip->texel_quads = new TexelQuad[(mip->width + 1) * (mip->height + 1)];

        TexelQuad *texel_quad = mip->texel_quads;
        TexelQuadLoader *loader_texel_quad = loader_mip->texel_quads;
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
}

int bmp2texture(char* bmp_file_path, char* texture_file_path, bool wrap = true, float gamma = 2.2f, bool mipmap = true) {
    BITMAPINFOHEADER bitmapInfoHeader;
    u8* texel_components = LoadBitmapFile(bmp_file_path, &bitmapInfoHeader);

    Texture texture{};
    loadTexture(&texture, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight, gamma, wrap, mipmap, texel_components, bitmapInfoHeader.biBitCount);

    void* file = win32_openFileForWriting(texture_file_path);

    win32_writeToFile(&texture.width,  sizeof(u16), file);
    win32_writeToFile(&texture.height,  sizeof(u16), file);
    win32_writeToFile(&texture.mipmap,  sizeof(bool), file);
    win32_writeToFile(&texture.wrap,  sizeof(bool), file);
    win32_writeToFile(&texture.mip_count,  sizeof(u8), file);

    TextureMip *texture_mip = texture.mips;
    for (u8 mip_index = 0; mip_index < texture.mip_count; mip_index++, texture_mip++) {
        win32_writeToFile(&texture_mip->width,  sizeof(u16), file);
        win32_writeToFile(&texture_mip->height,  sizeof(u16), file);
        win32_writeToFile(texture_mip->texel_quads,  sizeof(TexelQuad) * ((texture_mip->width + 1) * (texture_mip->height + 1)), file);
    }

    win32_closeFile(file);

    return 0;
}
//int EndsWith(const char *str, const char *suffix) {
//    if (!str || !suffix)
//        return 0;
//    size_t lenstr = strlen(str);
//    size_t lensuffix = strlen(suffix);
//    if (lensuffix >  lenstr)
//        return 0;
//    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
//}

int main(int argc, char *argv[]) {
    // Error if less than 2 arguments were provided
//    bool valid_input = argc >= 3 && (EndsWith(argv[1], ".bmp") && (EndsWith(argv[2], ".texture")));
//    if (!valid_input) {
//        printf("Exactly 2 file paths need to be provided: A '.bmp' file (input) then a '.text' file (output)");
//        return 1;
//    }

    char* src_file_path = argv[1];
    char* trg_file_path = argv[2];
    bool mipmap = false;
    bool wrap = false;
    float gamma = 2.2f;
    for (u8 i = 3; i < (u8)argc; i++) {
        if (     argv[i][0] == '-' && argv[i][1] == 'm') mipmap = true;
        else if (argv[i][0] == '-' && argv[i][1] == 'w') wrap = true;
        else if (argv[i][0] == '-' && argv[i][1] == 'l') gamma = 1.0f;
        else return 0;
//        else {
//            printf("Unknown argument: %s", argv[i]);
//            valid_input = false;
//            break;
//        }
    }
    return bmp2texture(src_file_path, trg_file_path, wrap, gamma, mipmap);
}