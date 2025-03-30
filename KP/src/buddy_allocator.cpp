
#include "buddy_allocator.hpp"

BuddyAllocator::BuddyAllocator(void* const memory, const size_t size) {
    if (!memory || size == 0) {
        throw std::invalid_argument("Invalid memory or size");
    }

    this->size = size;
    this->block_size = 32;
    this->total_blocks = this->size / this->block_size;

    // Размещаем битовую карту в начале памяти
    size_t bitmap_size = (this->total_blocks + 7) / 8;
    this->bitmap = static_cast<uint8_t*>(memory);
    std::memset(this->bitmap, 0, bitmap_size); // Все блоки свободны

    // Остальную часть памяти используем для аллокации
    this->memory = static_cast<uint8_t*>(memory) + bitmap_size;
}

BuddyAllocator::~BuddyAllocator() {
    if (memory) {
        std::memset(memory, 0, size);
    }
}

void* BuddyAllocator::allocate(const size_t request_size) {
    if (request_size == 0 || request_size > size) {
        return nullptr;
    }

    size_t blocks_needed = (request_size + block_size - 1) / block_size;
    size_t free_blocks = 0;

    for (size_t i = 0; i < total_blocks; ++i) {
        if (!(bitmap[i / 8] & (1 << (i % 8)))) { // Проверяем, свободен ли блок
            ++free_blocks;
            if (free_blocks == blocks_needed) {
                size_t start_block = i - blocks_needed + 1;

                // Помечаем блоки как занятые
                for (size_t j = start_block; j <= i; ++j) {
                    bitmap[j / 8] |= (1 << (j % 8));
                }

                return static_cast<uint8_t*>(memory) + start_block * block_size;
            }
        } else {
            free_blocks = 0;
        }
    }

    return nullptr; // Недостаточно свободных блоков
}

void BuddyAllocator::deallocate(void* const ptr) {
    if (!ptr) {
        return;
    }

    size_t offset = static_cast<uint8_t*>(ptr) - static_cast<uint8_t*>(memory);
    if (offset % block_size != 0) {
        return;
    }

    size_t block_index = offset / block_size;

    // Сбрасываем бит, соответствующий блоку
    bitmap[block_index / 8] &= ~(1 << (block_index % 8));
}

size_t BuddyAllocator::getLargestFreeBlock() const {
    size_t max_free_blocks = 0;
    size_t current_free_blocks = 0;

    for (size_t i = 0; i < total_blocks; ++i) {
        if (!(bitmap[i / 8] & (1 << (i % 8)))) { // Если блок свободен
            ++current_free_blocks;
            if (current_free_blocks > max_free_blocks) {
                max_free_blocks = current_free_blocks;
            }
        } else {
            current_free_blocks = 0;
        }
    }

    return max_free_blocks * block_size;
}

size_t BuddyAllocator::getTotalFreeMemory() const {
    size_t free_memory = 0;

    for (size_t i = 0; i < total_blocks; ++i) {
        if (!(bitmap[i / 8] & (1 << (i % 8)))) { // Если блок свободен
            free_memory += block_size;
        }
    }

    return free_memory;
}