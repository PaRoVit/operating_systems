#include <gtest/gtest.h>
extern "C" {
    #include "implementations.hpp"
}

TEST(Link2, TestSquare1) {
    float result = Square(20.2, 20.5);
    EXPECT_FLOAT_EQ(result, 207.05);
}

TEST(Link2, TestSquare2) {
    float result = Square(-2.3, 20.5);
    EXPECT_FLOAT_EQ(result, -1);
}

TEST(Link2, TestSquare3) {
    float result = Square(0, 20.5);
    EXPECT_FLOAT_EQ(result, -1);
}

TEST(Link2, TestE1) {
    float result = E(0);
    EXPECT_FLOAT_EQ(result, -1);
}

TEST(Link2, TestE2) {
    float result = E(20);
    EXPECT_NEAR(result, 2.71828, 0.001);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}