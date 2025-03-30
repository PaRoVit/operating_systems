#include "list_allocator.hpp"


ListAllocator::ListAllocator(void* memory, size_t size) {
    if (!memory || size < sizeof(ListAllocator)) {
        throw std::invalid_argument("Invalid memory or size");
    }

    memory_start = static_cast<char*>(memory) + sizeof(ListAllocator);
    total_size = size - sizeof(ListAllocator);

    free_list = reinterpret_cast<Block*>(memory_start);
    free_list->size = total_size - sizeof(Block);
    free_list->next = nullptr;
    free_list->is_free = true;
}

ListAllocator::~ListAllocator() {
    if (memory_start) {
        std::memset(memory_start, 0, total_size);
    }
}

void* ListAllocator::allocate(size_t size) {
    if (size == 0) {
        return nullptr;
    }

    size = (size + MIN_BLOCK_SIZE - 1) / MIN_BLOCK_SIZE * MIN_BLOCK_SIZE;

    Block* best = nullptr;
    Block* prev_best = nullptr;
    Block* current = free_list;
    Block* prev = nullptr;

    while (current) {
        if (current->is_free && current->size >= size) {
            if (!best || current->size < best->size) {
                best = current;
                prev_best = prev;
            }
        }
        prev = current;
        current = current->next;
    }

    if (best) {
        size_t remain_size = best->size - size;
        if (remain_size >= sizeof(Block) + MIN_BLOCK_SIZE) {
            Block* new_block = reinterpret_cast<Block*>(
                reinterpret_cast<char*>(best) + sizeof(Block) + size);
            new_block->size = remain_size - sizeof(Block);
            new_block->is_free = true;
            new_block->next = best->next;
            best->next = new_block;
            best->size = size;
        }

        best->is_free = false;

        if (prev_best == nullptr) {
            free_list = best->next;
        } else {
            prev_best->next = best->next;
        }

        return reinterpret_cast<void*>(reinterpret_cast<char*>(best) + sizeof(Block));
    }

    return nullptr;
}

void ListAllocator::deallocate(void* ptr) {
    if (!ptr) {
        return;
    }

    Block* head = reinterpret_cast<Block*>(
        reinterpret_cast<char*>(ptr) - sizeof(Block));

    head->is_free = true;
    head->next = free_list;
    free_list = head;

    Block* current = free_list;
    while (current && current->next) {
        if (reinterpret_cast<char*>(current) + sizeof(Block) + current->size ==
            reinterpret_cast<char*>(current->next)) {
            current->size += current->next->size + sizeof(Block);
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

size_t ListAllocator::getLargestFreeBlock() const {
    size_t max_free_size = 0;

    Block* current = free_list;
    while (current) {
        if (current->is_free && current->size > max_free_size) {
            max_free_size = current->size;
        }
        current = current->next;
    }

    return max_free_size;
}

size_t ListAllocator::getTotalFreeMemory() const {
    size_t total_free_size = 0;

    Block* current = free_list;
    while (current) {
        if (current->is_free) {
            total_free_size += current->size;
        }
        current = current->next;
    }

    return total_free_size;
}
