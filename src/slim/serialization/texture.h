#pragma once

#include "../core/string.h"
#include "../core/texture.h"


u32 getSizeInBytes(const Texture &texture) {
    u16 mip_width  = texture.width;
    u16 mip_height = texture.height;

    u32 memory_size = 0;

    do {
        memory_size += sizeof(TextureMip);
        memory_size += (mip_width + 1) * (mip_height + 1) * sizeof(TexelQuad);

        mip_width /= 2;
        mip_height /= 2;
    } while (texture.mipmap && mip_width > 2 && mip_height > 2);

    return memory_size;
}

bool allocateMemory(Texture &texture, memory::MonotonicAllocator *memory_allocator) {
    if (getSizeInBytes(texture) > (memory_allocator->capacity - memory_allocator->occupied)) return false;
    texture.mips = (TextureMip*)memory_allocator->allocate(sizeof(TextureMip) * texture.mip_count);
    TextureMip *texture_mip = texture.mips;
    u16 mip_width  = texture.width;
    u16 mip_height = texture.height;

    do {
        texture_mip->texel_quads = (TexelQuad * )memory_allocator->allocate(sizeof(TexelQuad ) * (mip_height + 1) * (mip_width + 1));
        mip_width /= 2;
        mip_height /= 2;
        texture_mip++;
    } while (texture.mipmap && mip_width > 2 && mip_height > 2);

    return true;
}

void writeHeader(const Texture &texture, void *file) {
    os::writeToFile((void*)&texture.width,  sizeof(u16),  file);
    os::writeToFile((void*)&texture.height, sizeof(u16),  file);
    os::writeToFile((void*)&texture.mipmap, sizeof(bool), file);
    os::writeToFile((void*)&texture.wrap,   sizeof(bool), file);
    os::writeToFile((void*)&texture.mip_count, sizeof(u8), file);
}
void readHeader(Texture &texture, void *file) {
    os::readFromFile(&texture.width,  sizeof(u16),  file);
    os::readFromFile(&texture.height, sizeof(u16),  file);
    os::readFromFile(&texture.mipmap, sizeof(bool), file);
    os::readFromFile(&texture.wrap,   sizeof(bool), file);
    os::readFromFile(&texture.mip_count, sizeof(u8), file);
}

bool saveHeader(const Texture &texture, char *file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeHeader(texture, file);
    os::closeFile(file);
    return true;
}

bool loadHeader(Texture &texture, char *file_path) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;
    readHeader(texture, file);
    os::closeFile(file);
    return true;
}

void readContent(Texture &texture, void *file) {
    TextureMip *texture_mip = texture.mips;
    for (u8 mip_index = 0; mip_index < texture.mip_count; mip_index++, texture_mip++) {
        os::readFromFile(&texture_mip->width,  sizeof(u16), file);
        os::readFromFile(&texture_mip->height, sizeof(u16), file);
        os::readFromFile(texture_mip->texel_quads, sizeof(TexelQuad) * (texture_mip->width + 1) * (texture_mip->height + 1), file);
    }
}
void writeContent(const Texture &texture, void *file) {
    TextureMip *texture_mip = texture.mips;
    for (u8 mip_index = 0; mip_index < texture.mip_count; mip_index++, texture_mip++) {
        os::writeToFile(&texture_mip->width,  sizeof(u16), file);
        os::writeToFile(&texture_mip->height, sizeof(u16), file);
        os::writeToFile(texture_mip->texel_quads, sizeof(TexelQuad) * (texture_mip->width + 1) * (texture_mip->height + 1), file);
    }
}

bool saveContent(const Texture &texture, char *file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeContent(texture, file);
    os::closeFile(file);
    return true;
}

bool loadContent(Texture &texture, char *file_path) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;
    readContent(texture, file);
    os::closeFile(file);
    return true;
}

bool save(const Texture &texture, char* file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeHeader(texture, file);
    writeContent(texture, file);
    os::closeFile(file);
    return true;
}

bool load(Texture &texture, char *file_path, memory::MonotonicAllocator *memory_allocator = nullptr) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;

    if (memory_allocator) {
        new(&texture) Texture{};
        readHeader(texture, file);
        if (!allocateMemory(texture, memory_allocator)) return false;
    } else if (!texture.mips) return false;
    readContent(texture, file);
    os::closeFile(file);
    return true;
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