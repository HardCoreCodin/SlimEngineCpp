#pragma once

#include "../core/string.h"

template <typename T>
u32 getSizeInBytes(const Image<T> &image) {
    return sizeof(T) * image.size * (image.flags.channel ? (image.flags.alpha ? 4 : 3) : 1);
}

template <typename T>
bool allocateMemory(Image<T> &image, memory::MonotonicAllocator *memory_allocator) {
    u32 size = getSizeInBytes(image);
    if (size > (memory_allocator->capacity - memory_allocator->occupied)) return false;
    image.content = (T*)memory_allocator->allocate(size);
    return true;
}

template <typename T>
void readContent(Image<T> &image, void *file) {
    os::readFromFile((void*)image.content, getSizeInBytes(image), file);
}

template <typename T>
void writeContent(const Image<T> &image, void *file) {
    os::writeToFile((void*)image.content, getSizeInBytes(image), file);
}

template <typename T>
u32 getTotalMemoryForImages(String *image_files, u32 image_count) {
    u32 memory_size{0};
    for (u32 i = 0; i < image_count; i++) {
        Image<T> image;
        loadHeader(image, image_files[i].char_ptr);
        memory_size += getSizeInBytes(image);
    }
    return memory_size;
}

template <typename T>
struct ImagePack {
    ImagePack(u8 count, Image<T> *images, char **files, char* adjacent_file, u64 memory_base = Terabytes(3)) {
        char string_buffer[200];
        u32 memory_size{0};
        Image<T> *image = images;
        for (u32 i = 0; i < count; i++, image++) {
            String string = String::getFilePath(files[i], string_buffer, adjacent_file);
            loadHeader(*image, string.char_ptr);
            memory_size += getSizeInBytes(*image);
        }
        memory::MonotonicAllocator memory_allocator{memory_size, memory_base};

        image = images;
        for (u32 i = 0; i < count; i++, image++) {
            String string = String::getFilePath(files[i], string_buffer, adjacent_file);
            load(*image, string.char_ptr, &memory_allocator);
        }
    }
};