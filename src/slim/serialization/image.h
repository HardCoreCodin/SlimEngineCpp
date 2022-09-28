#pragma once

#include "../core/string.h"

u32 getSizeInBytes(const Image &image) {
    return image.width * image.height * sizeof(Pixel);
}

bool allocateMemory(Image &image, memory::MonotonicAllocator *memory_allocator) {
    if (getSizeInBytes(image) > (memory_allocator->capacity - memory_allocator->occupied)) return false;
    image.pixels = (Pixel*)memory_allocator->allocate(sizeof(Pixel) * image.width * image.height);
    return true;
}

void writeHeader(const ImageHeader &image_header, void *file) {
    os::writeToFile((void*)&image_header.width,  sizeof(u32),  file);
    os::writeToFile((void*)&image_header.height, sizeof(u32),  file);
    os::writeToFile((void*)&image_header.depth,  sizeof(u32),  file);
    os::writeToFile((void*)&image_header.gamma,  sizeof(f32),  file);
}
void readHeader(ImageHeader &image_header, void *file) {
    os::readFromFile(&image_header.width,  sizeof(u32),  file);
    os::readFromFile(&image_header.height, sizeof(u32),  file);
    os::readFromFile(&image_header.depth,  sizeof(u32),  file);
    os::readFromFile(&image_header.gamma,  sizeof(f32),  file);
}

bool saveHeader(const Image &image, char *file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeHeader(image, file);
    os::closeFile(file);
    return true;
}

bool loadHeader(Image &image, char *file_path) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;
    readHeader(image, file);
    os::closeFile(file);
    return true;
}

void readContent(Image &image, void *file) {
    os::readFromFile((void*)image.pixels, sizeof(Pixel) * image.width * image.height, file);
}
void writeContent(const Image &image, void *file) {
    os::writeToFile((void*)image.pixels, sizeof(Pixel) * image.width * image.height, file);
}

bool saveContent(const Image &image, char *file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeContent(image, file);
    os::closeFile(file);
    return true;
}

bool loadContent(Image &image, char *file_path) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;
    readContent(image, file);
    os::closeFile(file);
    return true;
}

bool save(const Image &image, char* file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeHeader(image, file);
    writeContent(image, file);
    os::closeFile(file);
    return true;
}

bool load(Image &image, char *file_path, memory::MonotonicAllocator *memory_allocator = nullptr) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;

    if (memory_allocator) {
        new(&image) Image{};
        readHeader(image, file);
        if (!allocateMemory(image, memory_allocator)) return false;
    } else if (!image.pixels) return false;
    readContent(image, file);
    os::closeFile(file);
    return true;
}

u32 getTotalMemoryForImages(String *image_files, u32 image_count) {
    u32 memory_size{0};
    for (u32 i = 0; i < image_count; i++) {
        Image image;
        loadHeader(image, image_files[i].char_ptr);
        memory_size += getSizeInBytes(image);
    }
    return memory_size;
}

struct ImagePack {
    ImagePack(u8 count, Image *images, char **files, char* adjacent_file, u64 memory_base = Terabytes(3)) {
        char string_buffer[200];
        u32 memory_size{0};
        Image *image = images;
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