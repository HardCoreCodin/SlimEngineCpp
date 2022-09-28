#pragma once

#ifdef COMPILER_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "./win32_base.h"

void componentsToPixels(u8 *components, ImageHeader &header, Pixel *pixels) {
    Pixel* pixel = pixels;
    u8 *component = components;
    u32 count = header.width * header.height;
    for (u32 i = 0; i < count; i++, pixel++) {
        pixel->color.b = powf((f32)(*(component++)) * COLOR_COMPONENT_TO_FLOAT, header.gamma);
        pixel->color.g = powf((f32)(*(component++)) * COLOR_COMPONENT_TO_FLOAT, header.gamma);
        pixel->color.r = powf((f32)(*(component++)) * COLOR_COMPONENT_TO_FLOAT, header.gamma);
        if (header.depth == 32)
            pixel->opacity = (f32)(*(component++)) * COLOR_COMPONENT_TO_FLOAT;
    }
}

void flipImage(const u8 *components, ImageHeader &header, u8 *flipped) {
    u32 bytes_per_pixel = header.depth / 8;
    u32 stride = bytes_per_pixel * header.width;
    u32 trg_offset = stride * (header.height - 1);
    u32 src_offset = 0;
    for (u32 y = 0; y < header.height; y++) {
        for (u32 x = 0; x < header.width; x++) {
            flipped[trg_offset++] = components[src_offset++];
            flipped[trg_offset++] = components[src_offset++];
            flipped[trg_offset++] = components[src_offset++];
            if (bytes_per_pixel == 4)
                flipped[trg_offset++] = components[src_offset++];
        }
        trg_offset -= stride;
        trg_offset -= stride;
    }
}

u8* loadBitmap(char *filename, ImageHeader &header, bool flip = false) {
    void *file = os::openFileForReading(filename);
    if (!file) return nullptr;

    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;
    os::readFromFile(&file_header, sizeof(BITMAPFILEHEADER), file);
    os::readFromFile(&info_header, sizeof(BITMAPINFOHEADER), file);
    if (file_header.bfType != 0x4D42) {
        os::closeFile(file);
        return nullptr;
    }

    bool flipped = info_header.biHeight > 0;
    header.depth = info_header.biBitCount;
    header.width = info_header.biWidth;
    header.height = flipped ? info_header.biHeight : -info_header.biHeight;
    u32 bytes_per_pixel = header.depth / 8;
    u32 size_in_bytes = bytes_per_pixel * header.width * header.height;
    u8 *components = new u8[size_in_bytes];

    SetFilePointer(file, (LONG)file_header.bfOffBits, nullptr, FILE_BEGIN);
    os::readFromFile(components, size_in_bytes, file);
    os::closeFile(file);

    if (flipped && flip) {
        u8 *flipped_components = new u8[size_in_bytes];
        flipImage(components, header, flipped_components);
        for (u32 i = 0; i < size_in_bytes; i++) components[i] = flipped_components[i];
        delete[] flipped_components;
    }

    return components;
}

