#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include "buddy_allocator.hpp" // Подключите ваш класс BuddyAllocator
#include "list_allocator.hpp" // Подключите ваш класс ListAllocator

void logMetrics(const std::string& filename, int iteration, double fragmentation, double efficiency) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << iteration << " " << fragmentation << " " << efficiency << std::endl;
        file.close();
    }
}

int main() {
    const size_t MEMORY_SIZE = 1024 * 1024; // 1 MB памяти
    void* memory_buddy = std::malloc(MEMORY_SIZE);
    void* memory_list = std::malloc(MEMORY_SIZE);
    const size_t REQUEST_SIZE = 32;        // Размер запрашиваемой памяти

    BuddyAllocator buddyAllocator(memory_buddy, MEMORY_SIZE);
    ListAllocator listAllocator(memory_list, MEMORY_SIZE);

    std::vector<void*> buddyAllocations;
    std::vector<void*> listAllocations;

    auto testAllocator = [](auto& allocator, const std::string& metricsFile, size_t requestSize) {
        std::vector<void*> allocations;
        std::vector<std::pair<int, double>> allocationMetrics;
        std::vector<std::pair<int, double>> deallocationMetrics;

        auto start = std::chrono::high_resolution_clock::now();

        // Тест выделения памяти
        for (int i = 0; i < 3000; ++i) {
            try {
                void* ptr = allocator.allocate(requestSize);
                allocations.push_back(ptr);
            } catch (std::bad_alloc& e) {
                std::cerr << "Allocation failed at iteration " << i << ": " << e.what() << std::endl;
                break;
            }

            auto current = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = current - start;
            allocationMetrics.emplace_back(i, duration.count());

            size_t largestFreeBlock = allocator.getLargestFreeBlock();
            size_t totalFreeMemory = allocator.getTotalFreeMemory();
            double fragmentation = 1.0 - static_cast<double>(largestFreeBlock) / totalFreeMemory;
            double efficiency = static_cast<double>(requestSize) / (requestSize + sizeof(int));
            logMetrics(metricsFile, i, fragmentation, efficiency);
        }

        auto deallocStart = std::chrono::high_resolution_clock::now();

        // Тест освобождения памяти
        for (size_t i = 0; i < allocations.size(); ++i) {
            allocator.deallocate(allocations[i]);
            auto deallocEnd = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> deallocDuration = deallocEnd - deallocStart;
            deallocationMetrics.emplace_back(i, deallocDuration.count());
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> totalDuration = end - start;

        std::cout << "Время выполнения: " << totalDuration.count() << " секунд" << std::endl;

        return std::make_pair(allocationMetrics, deallocationMetrics);
    };

    // Тестируем BuddyAllocator
    auto [buddyAllocMetrics, buddyDeallocMetrics] = testAllocator(buddyAllocator, "buddy_metrics.txt", REQUEST_SIZE);

    // Тестируем ListAllocator
    auto [listAllocMetrics, listDeallocMetrics] = testAllocator(listAllocator, "list_metrics.txt", REQUEST_SIZE);

    // Сохранение результатов в файлы
    auto saveMetrics = [](const std::string& filename, const std::vector<std::pair<int, double>>& metrics) {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& [iteration, time] : metrics) {
                file << iteration << " " << time << "\n";
            }
            file.close();
        }
    };

    saveMetrics("buddy_alloc_output.txt", buddyAllocMetrics);
    saveMetrics("buddy_dealloc_output.txt", buddyDeallocMetrics);
    saveMetrics("list_alloc_output.txt", listAllocMetrics);
    saveMetrics("list_dealloc_output.txt", listDeallocMetrics);

    return 0;
}
