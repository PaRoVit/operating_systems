#include <gtest/gtest.h>
#include <sort.hpp>

TEST(SortTest, HandlesTwoThreadsPerformance) {
    int threadsAmount1 = 1;
    auto start1 = std::chrono::high_resolution_clock::now();
    run(threadsAmount1);
    auto end1 = std::chrono::high_resolution_clock::now();
    double execution_time1 = std::chrono::duration<double, std::milli>(end1 - start1).count();

    int threadsAmount2 = 2;
    auto start2 = std::chrono::high_resolution_clock::now();
    run(threadsAmount2);
    auto end2 = std::chrono::high_resolution_clock::now();
    double execution_time2 = std::chrono::duration<double, std::milli>(end2 - start2).count();

    EXPECT_LE(execution_time2, execution_time1);  
}

TEST(SortTest, CorrectlySortedArray1) {
    int threadsAmount = 8;

    for (int thread = 1; thread <= threadsAmount; thread++) {
        std::vector<int> sortedArray = run(thread);
        EXPECT_TRUE(std::is_sorted(sortedArray.begin(), sortedArray.end()));
    }
}

