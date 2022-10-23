#pragma once

#include "../core/string.h"
#include "../core/texture.h"


u32 getSizeInBytes(const Texture &texture) {
    u32 mip_width  = texture.width;
    u32 mip_height = texture.height;
    u32 memory_size = 0;

    do {
        memory_size += sizeof(TextureMip);
        memory_size += (mip_width + 1) * (mip_height + 1) * sizeof(TexelQuad);

        mip_width /= 2;
        mip_height /= 2;
    } while (texture.flags.mipmap && mip_width > 2 && mip_height > 2);

    return memory_size;
}

bool allocateMemory(Texture &texture, memory::MonotonicAllocator *memory_allocator) {
    u32 size = getSizeInBytes(texture);
    if (size > (memory_allocator->capacity - memory_allocator->occupied)) return false;
    texture.mips = (TextureMip*)memory_allocator->allocate(sizeof(TextureMip) * texture.mip_count);
    TextureMip *texture_mip = texture.mips;
    u32 mip_width  = texture.width;
    u32 mip_height = texture.height;

    do {
        texture_mip->texel_quads = (TexelQuad*)memory_allocator->allocate(sizeof(TexelQuad) * (mip_height + 1) * (mip_width + 1));
        mip_width /= 2;
        mip_height /= 2;
        texture_mip++;
    } while (texture.flags.mipmap && mip_width > 2 && mip_height > 2);

    return true;
}

void readContent(Texture &texture, void *file) {
    TextureMip *texture_mip = texture.mips;
    for (u8 mip_index = 0; mip_index < texture.mip_count; mip_index++, texture_mip++) {
        os::readFromFile(&texture_mip->width,  sizeof(u32), file);
        os::readFromFile(&texture_mip->height, sizeof(u32), file);
        os::readFromFile(texture_mip->texel_quads, sizeof(TexelQuad) * (texture_mip->width + 1) * (texture_mip->height + 1), file);
    }
}
void writeContent(const Texture &texture, void *file) {
    TextureMip *texture_mip = texture.mips;
    for (u8 mip_index = 0; mip_index < texture.mip_count; mip_index++, texture_mip++) {
        os::writeToFile(&texture_mip->width,  sizeof(u32), file);
        os::writeToFile(&texture_mip->height, sizeof(u32), file);
        os::writeToFile(texture_mip->texel_quads, sizeof(TexelQuad) * (texture_mip->width + 1) * (texture_mip->height + 1), file);
    }
}

u32 getTotalMemoryForTextures(String *texture_files, u32 texture_count) {
    u32 memory_size{0};
    for (u32 i = 0; i < texture_count; i++) {
        Texture texture;
        loadHeader(texture, texture_files[i].char_ptr);
        memory_size += getSizeInBytes(texture);
    }
    return memory_size;
}

struct TexturePack {
    TexturePack(u8 count, Texture *textures, char **files, char* adjacent_file, u64 memory_base = Terabytes(3)) {
        char string_buffer[200];
        u32 memory_size{0};
        Texture *texture = textures;
        for (u32 i = 0; i < count; i++, texture++) {
            String string = String::getFilePath(files[i], string_buffer, adjacent_file);
            loadHeader(*texture, string.char_ptr);
            memory_size += getSizeInBytes(*texture);
        }
        memory::MonotonicAllocator memory_allocator{memory_size, memory_base};

        texture = textures;
        for (u32 i = 0; i < count; i++, texture++) {
            String string = String::getFilePath(files[i], string_buffer, adjacent_file);
            load(*texture, string.char_ptr, &memory_allocator);
        }
    }
};