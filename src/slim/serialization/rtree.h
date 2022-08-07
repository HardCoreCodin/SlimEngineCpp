#pragma once

#include "../core/string.h"
#include "../scene/rtree.h"

u32 getSizeInBytes(const RTree &rtree) {
    return sizeof(RTreeNode) * rtree.node_count +
            sizeof(u32) * (rtree.leaf_ids_count + rtree.height + rtree.node_count);
}

bool allocateMemory(RTree &rtree, memory::MonotonicAllocator *memory_allocator) {
    if (getSizeInBytes(rtree) > (memory_allocator->capacity - memory_allocator->occupied)) return false;

    rtree.nodes           = (RTreeNode*)memory_allocator->allocate(sizeof(RTreeNode) * rtree.node_count);
    rtree.leaf_ids              = (u32*)memory_allocator->allocate(sizeof(u32)       * rtree.leaf_ids_count);
    rtree.query_result.node_ids = (u32*)memory_allocator->allocate(sizeof(u32)       * rtree.node_count);
    rtree.query_result.stack    = (u32*)memory_allocator->allocate(sizeof(u32)       * rtree.height);

    return true;
}

void writeHeader(const RTree &rtree, void *file) {
    os::writeToFile((void*)&rtree.node_count,     sizeof(u32),  file);
    os::writeToFile((void*)&rtree.leaf_ids_count, sizeof(u32),  file);
    os::writeToFile((void*)&rtree.height,         sizeof(u32),  file);
}
void readHeader(const RTree &rtree, void *file) {
    os::readFromFile((void*)&rtree.node_count,     sizeof(u32),  file);
    os::readFromFile((void*)&rtree.leaf_ids_count, sizeof(u32),  file);
    os::readFromFile((void*)&rtree.height,         sizeof(u32),  file);
}

bool saveHeader(const RTree &rtree, char *file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeHeader(rtree, file);
    os::closeFile(file);
    return true;
}

bool loadHeader(RTree &rtree, char *file_path) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;
    readHeader(rtree, file);
    os::closeFile(file);
    return true;
}

void readContent(RTree &rtree, void *file) {
    os::readFromFile(rtree.nodes,    rtree.node_count * sizeof(RTreeNode), file);
    os::readFromFile(rtree.leaf_ids, rtree.leaf_ids_count * sizeof(u32), file);
}
void writeContent(const RTree &rtree, void *file) {
    os::writeToFile(rtree.nodes,    rtree.node_count * sizeof(RTreeNode), file);
    os::writeToFile(rtree.leaf_ids, rtree.leaf_ids_count * sizeof(u32), file);
}

bool saveContent(const RTree &rtree, char *file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeContent(rtree, file);
    os::closeFile(file);
    return true;
}

bool loadContent(RTree &rtree, char *file_path) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;
    readContent(rtree, file);
    os::closeFile(file);
    return true;
}

bool save(const RTree &rtree, char* file_path) {
    void *file = os::openFileForWriting(file_path);
    if (!file) return false;
    writeHeader(rtree, file);
    writeContent(rtree, file);
    os::closeFile(file);
    return true;
}

bool load(RTree &rtree, char *file_path, memory::MonotonicAllocator *memory_allocator = nullptr) {
    void *file = os::openFileForReading(file_path);
    if (!file) return false;

    if (memory_allocator) {
        rtree = RTree{};
        readHeader(rtree, file);
        if (!allocateMemory(rtree, memory_allocator)) return false;
    } else if (!rtree.nodes) return false;
    readContent(rtree, file);
    os::closeFile(file);
    return true;
}