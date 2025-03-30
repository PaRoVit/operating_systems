#include <gtest/gtest.h>
#include "list_allocator.hpp"
#include "buddy_allocator.hpp"
#include <iostream>
#include <sstream>


TEST(ListAllocatorTest, Memory) {
    ListAllocator allocator(1000);

    void* ptr1 = allocator.allocate(100);
    void* ptr2 = allocator.allocate(200);

    allocator.deallocate(ptr1);
    allocator.deallocate(ptr2);

    void* ptr3 = allocator.allocate(300);
    EXPECT_NE(ptr3, nullptr); 
}

TEST(ListAllocatorTest, PrintStatus) {
    ListAllocator allocator(1000);
    
    allocator.allocate(100);
    allocator.allocate(200);

    std::stringstream ss;
    allocator.PrintStatus(ss);

    std::string output = ss.str();
    EXPECT_NE(output.find("Occupied memory"), std::string::npos);
    EXPECT_NE(output.find("Free memory"), std::string::npos);
}




TEST(BuddyAllocatorTest, Memory) {
    BuddyAllocator allocator(1024);

    void* ptr1 = allocator.allocate(128);
    void* ptr2 = allocator.allocate(256);

    allocator.deallocate(ptr1);
    allocator.deallocate(ptr2);

    void* ptr3 = allocator.allocate(384); 
    EXPECT_NE(ptr3, nullptr);
}

TEST(BuddyAllocatorTest, PrintStatus) {
    BuddyAllocator allocator(1024);

    allocator.allocate(128);
    allocator.allocate(256);

    std::stringstream ss;
    allocator.PrintStatus(ss);

    std::string output = ss.str();
    EXPECT_NE(output.find("Occupied memory"), std::string::npos);
    EXPECT_NE(output.find("Free memory"), std::string::npos);
}
