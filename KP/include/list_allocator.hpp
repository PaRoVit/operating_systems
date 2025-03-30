#pragma once

#include <iostream>
#include <cstring>
#include <cstddef>

#define MIN_BLOCK_SIZE 32

class ListAllocator {
private:
    struct Block {
        size_t size;
        Block* next;
        bool is_free;
    };

    Block* free_list;       // Указатель на список свободных блоков
    void* memory_start;     // Указатель на начало выделенной памяти
    size_t total_size;      // Общий размер выделенной памяти

public:
    ListAllocator(void* memory, size_t size);

    ~ListAllocator();

    void* allocate(size_t size);
    void deallocate(void* ptr);

    size_t getLargestFreeBlock() const;
    size_t getTotalFreeMemory() const;
};
