#include <iostream>
#include <chrono>

#include "buddy_allocator.hpp"
#include "list_allocator.hpp"

int main() {
    using namespace std::chrono;
    {
        steady_clock::time_point buddy_allocator_init_start = steady_clock::now();
        BuddyAllocator buddy_allocator(4096);
        steady_clock::time_point buddy_allocator_init_end = steady_clock::now();
        std::cerr << "Buddy allocator initialization with one page of memory :"
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(
                          buddy_allocator_init_end - buddy_allocator_init_start).count()
                  << " ns" << std::endl;


        steady_clock::time_point list_allocator_init_start = steady_clock::now();
        ListAllocator list_allocator(4096);
        steady_clock::time_point list_allocator_init_end = steady_clock::now();
        std::cerr << "List allocator initialization with one page of memory :"
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(
                          list_allocator_init_end - list_allocator_init_start).count()
                  << " ns" << std::endl;

        std::cerr << "\n";

    }
    std::cerr << "First test: Allocate 10 char[256] arrays, free 5 of them, allocate 10 char[128] arrays:\n";
    {
        ListAllocator allocator(4096);
        std::vector<char *> pointers(1000, 0);
        steady_clock::time_point test1_start = steady_clock::now();
        for (int i = 0; i < 10; ++i) {
            pointers[i] = (char *) allocator.allocate(256);
        }
        for (int i = 5; i < 10; ++i) {
            allocator.deallocate(pointers[i]);
        }
        for (int i = 5; i < 15; ++i) {
            pointers[i] = (char *) allocator.allocate(128);
        }
        steady_clock::time_point test1_end = steady_clock::now();
        std::cerr << "List allocator first test:"
                  << std::chrono::duration_cast<std::chrono::microseconds>(test1_end - test1_start).count()
                  << " microseconds" << std::endl;
        allocator.PrintStatus(std::cerr);
        for (int i = 0; i < 15; ++i) {
            allocator.deallocate(pointers[i]);
        }
    }
    {
        BuddyAllocator allocator(8192);
        std::vector<char *> pointers(1000, 0);
        steady_clock::time_point test1_start = steady_clock::now();
        for (int i = 0; i < 10; ++i) {
            pointers[i] = (char *) allocator.allocate(256);
        }
        for (int i = 5; i < 10; ++i) {
            allocator.deallocate(pointers[i]);
        }
        for (int i = 5; i < 15; ++i) {
            pointers[i] = (char *) allocator.allocate(128);
        }
        steady_clock::time_point test1_end = steady_clock::now();
        std::cout << "Buddy allocator first test:"
                  << std::chrono::duration_cast<std::chrono::microseconds>(test1_end - test1_start).count()
                  << " microseconds" << std::endl;
        allocator.PrintStatus(std::cerr);
        for (int i = 0; i < 15; ++i) {
            allocator.deallocate(pointers[i]);
        }
    }
    std::cerr << "Second test: Allocate and free 75 20 bytes arrays:\n";
    {
        ListAllocator allocator(16000);
        std::vector<char *> pointers(750, 0);
        steady_clock::time_point alloc_start = steady_clock::now();
        for (int i = 0; i < 750; ++i) {
            pointers[i] = (char *) allocator.allocate(20);
        }
        steady_clock::time_point alloc_end = steady_clock::now();
        for (int i = 0; i < 750; ++i) {
            allocator.deallocate(pointers[i]);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cerr << "List allocator second test:\n"
                << "Allocation :" << duration_cast<std::chrono::microseconds>(alloc_end - alloc_start).count() << " microseconds" << "\n"
                << "Deallocation :" << duration_cast<std::chrono::microseconds>(test_end - alloc_end).count() << " microseconds" << "\n";
    }
    {
        BuddyAllocator allocator(16000);
        std::vector<char *> pointers(75, 0);
        steady_clock::time_point alloc_start = steady_clock::now();
        for (int i = 0; i < 75; ++i) {
            pointers[i] = (char *) allocator.allocate(20);
        }
        steady_clock::time_point alloc_end = steady_clock::now();
        for (int i = 0; i < 75; ++i) {
            allocator.deallocate(pointers[i]);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cerr << "Buddy allocator second test:\n"
                  << "Allocation :" << duration_cast<std::chrono::microseconds>(alloc_end - alloc_start).count() << " microseconds" << "\n"
                  << "Deallocation :" << duration_cast<std::chrono::microseconds>(test_end - alloc_end).count() << " microseconds" << "\n";
    }
    std::cerr << "Third test: Allocate 50 20 bytes arrays, deallocate every second, allocate 25 12 bytes :\n";
    {
        ListAllocator allocator(16000);
        std::vector<char *> pointers(750, 0);
        steady_clock::time_point test_start = steady_clock::now();
        for (int i = 0; i < 500; ++i) {
            pointers[i] = (char *) allocator.allocate(20);
        }
        for (int i = 0; i < 250; ++i) {
            allocator.deallocate(pointers[i * 2]);
        }
        for (int i = 500; i < 750; ++i) {
            pointers[i] = (char*) allocator.allocate(12);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cerr << "List allocator third test:"
                  << std::chrono::duration_cast<std::chrono::microseconds>(test_end - test_start).count()
                  << " microseconds" << std::endl;
        allocator.PrintStatus(std::cerr);
        for (int i = 0; i < 250; ++i) {
            allocator.deallocate(pointers[i * 2 + 1]);
        }
        for (int i = 500; i < 750; ++i) {
            allocator.deallocate(pointers[i]);
        }
    }
    {
        BuddyAllocator allocator(16000);
        std::vector<char *> pointers(75, 0);
        steady_clock::time_point test_start = steady_clock::now();
        for (int i = 0; i < 50; ++i) {
            pointers[i] = (char *) allocator.allocate(20);
        }
        for (int i = 0; i < 25; ++i) {
            allocator.deallocate(pointers[i * 2]);
        }
        for (int i = 500; i < 75; ++i) {
            pointers[i] = (char*) allocator.allocate(12);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cerr << "Buddy allocator third test:"
                  << std::chrono::duration_cast<std::chrono::microseconds>(test_end - test_start).count()
                  << " microseconds" << std::endl;
        allocator.PrintStatus(std::cerr);
        for (int i = 0; i < 25; ++i) {
            allocator.deallocate(pointers[i * 2 + 1]);
        }
        for (int i = 500; i < 75; ++i) {
            allocator.deallocate(pointers[i]);
        }
    }
    std::cerr << "Fourth test: Allocate and free 150 20 bytes arrays:\n";
    {
        ListAllocator allocator(32000);
        std::vector<char *> pointers(1500, 0);
        steady_clock::time_point alloc_start = steady_clock::now();
        for (int i = 0; i < 1500; ++i) {
            pointers[i] = (char *) allocator.allocate(20);
        }
        steady_clock::time_point alloc_end = steady_clock::now();
        for (int i = 0; i < 1500; ++i) {
            allocator.deallocate(pointers[i]);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cerr << "List allocator fourth test:\n"
                  << "Allocation :" << duration_cast<std::chrono::microseconds>(alloc_end - alloc_start).count() << " microseconds" << "\n"
                  << "Deallocation :" << duration_cast<std::chrono::microseconds>(test_end - alloc_end).count() << " microseconds" << "\n";
    }
    {
        BuddyAllocator allocator(32000);
        std::vector<char *> pointers(150, 0);
        steady_clock::time_point alloc_start = steady_clock::now();
        for (int i = 0; i < 150; ++i) {
            pointers[i] = (char *) allocator.allocate(20);
        }
        steady_clock::time_point alloc_end = steady_clock::now();
        for (int i = 0; i < 150; ++i) {
            allocator.deallocate(pointers[i]);
        }
        steady_clock::time_point test_end = steady_clock::now();
        std::cerr << "Buddy allocator fourth test:\n"
                  << "Allocation :" << duration_cast<std::chrono::microseconds>(alloc_end - alloc_start).count() << " microseconds" << "\n"
                  << "Deallocation :" << duration_cast<std::chrono::microseconds>(test_end - alloc_end).count() << " microseconds" << "\n";
    }

    return 0;
}