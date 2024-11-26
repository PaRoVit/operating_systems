#include <gtest/gtest.h>
extern "C" {
    #include "implementations.hpp"
}

TEST(Link1, TestSquare1) {
    float result = Square(20.2, 20.5);
    EXPECT_FLOAT_EQ(result, 414.1);
}

TEST(Link1, TestSquare2) {
    float result = Square(-2.3, 20.5);
    EXPECT_FLOAT_EQ(result, -1);
}

TEST(Link1, TestSquare3) {
    float result = Square(0, 20.5);
    EXPECT_FLOAT_EQ(result, -1);
}

TEST(Link1, TestE1) {
    float result = E(0);
    EXPECT_FLOAT_EQ(result, -1);
}

TEST(Link1, TestE2) {
    float result = E(20);
    EXPECT_NEAR(result, 2.6533, 0.001);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}