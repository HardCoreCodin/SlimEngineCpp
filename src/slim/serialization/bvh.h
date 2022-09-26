#pragma once

#include "../core/string.h"
#include "../scene/bvh.h"

u32 getSizeInBytes(const BVH &bvh) {
    return sizeof(BVHNode) * bvh.node_count;
}

bool allocateMemory(BVH &bvh, memory::MonotonicAllocator *memory_allocator) {
    if (getSizeInBytes(bvh) > (memory_allocator->capacity - memory_allocator->occupied)) return false;

    bvh.nodes = (BVHNode*)memory_allocator->allocate(sizeof(BVHNode) * bvh.node_count);

    return true;
}

void writeHeader(const BVH &bvh, void *file) {
    os::writeToFile((void*)&bvh.node_count,     sizeof(u32),  file);
    os::writeToFile((void*)&bvh.height,         sizeof(u32),  file);
}
void readHeader(const BVH &bvh, void *file) {
    os::readFromFile((void*)&bvh.node_count,     sizeof(u32),  file);
    os::readFromFile((void*)&bvh.height,         sizeof(u32),  file);
}

bool saveHeader(const BVH &bvh, char *file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeHeader(bvh, file);
    os::closeFile(file);
    return true;
}

bool loadHeader(BVH &bvh, char *file_path) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;
    readHeader(bvh, file);
    os::closeFile(file);
    return true;
}

void readContent(BVH &bvh, void *file) {
    os::readFromFile(bvh.nodes,    bvh.node_count * sizeof(BVHNode), file);
}
void writeContent(const BVH &bvh, void *file) {
    os::writeToFile(bvh.nodes,    bvh.node_count * sizeof(BVHNode), file);
}

bool saveContent(const BVH &bvh, char *file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeContent(bvh, file);
    os::closeFile(file);
    return true;
}

bool loadContent(BVH &bvh, char *file_path) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;
    readContent(bvh, file);
    os::closeFile(file);
    return true;
}

bool save(const BVH &bvh, char* file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeHeader(bvh, file);
    writeContent(bvh, file);
    os::closeFile(file);
    return true;
}

bool load(BVH &bvh, char *file_path, memory::MonotonicAllocator *memory_allocator = nullptr) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;

    if (memory_allocator) {
        bvh = BVH{};
        readHeader(bvh, file);
        if (!allocateMemory(bvh, memory_allocator)) return false;
    } else if (!bvh.nodes) return false;
    readContent(bvh, file);
    os::closeFile(file);
    return true;
}