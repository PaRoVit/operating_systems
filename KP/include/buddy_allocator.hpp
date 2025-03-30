#pragma once

#include <iostream>
#include <cstring>
#include <cstddef>
#include <cstdint>

class BuddyAllocator {
private:
    void* memory;           // Указатель на начало доступной памяти
    size_t size;            // Общий размер памяти
    uint8_t* bitmap;        // Указатель на битовую карту
    size_t block_size;      // Размер одного блока
    size_t total_blocks;    // Общее количество блоков

public:
    BuddyAllocator(void* const memory, const size_t size);

    ~BuddyAllocator();

    void* allocate(const size_t request_size);
    void deallocate(void* const ptr);
    
    size_t getLargestFreeBlock() const;
    size_t getTotalFreeMemory() const;
};