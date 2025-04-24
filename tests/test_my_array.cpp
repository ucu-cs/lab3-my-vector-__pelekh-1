#include <gtest/gtest.h>
#include "my_array.h"
#include <complex>

TEST(MyArray, Basic) {
    my_array<int, 3> arr = {1,2,3};
    EXPECT_EQ(arr.front(), 1);
    EXPECT_EQ(arr.back(), 3);
}

TEST(MyArray, Nested) {
    my_array< my_array<int,2>, 2 > big = { {1,2}, {3,4} };
    EXPECT_EQ(big[1][0], 3);
}

TEST(MyArray, Algorithms) {
    my_array<int,4> a = {4,3,2,1};
    std::ranges::sort(a);
    EXPECT_TRUE(std::ranges::is_sorted(a));
}
