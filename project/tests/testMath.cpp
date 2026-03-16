#include <gtest/gtest.h>
#include "math.h"

TEST(MathTest, Add) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(-1, 1), 0);
    EXPECT_EQ(add(0, 0), 0);
}

TEST(MathTest, Sub) {
    EXPECT_EQ(sub(5, 3), 2);
    EXPECT_EQ(sub(3, 5), -2);
    EXPECT_EQ(sub(10, 10), 0);
}