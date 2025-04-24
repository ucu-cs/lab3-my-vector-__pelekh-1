#include <gtest/gtest.h>
#include <numeric>
#include "my_vector.h"

TEST(MyVector, FundamentalTypes) {
    my_vector<int> v = {1,2,3};
    v.push_back(4);
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v.back(), 4);
}

TEST(MyVector, NestedVector) {
    my_vector<my_vector<int>> vv(2, my_vector<int>{1,2});
    vv[1].push_back(3);
    EXPECT_EQ(vv.at(1).size(), 3);
}

TEST(MyVector, Algorithms) {
    my_vector<int> v(5, 1);
    std::iota(v.begin(), v.end(), 1);
    my_vector<int> dst(5);
    std::copy(v.begin(), v.end(), dst.begin());
    EXPECT_EQ(dst[4], 5);
}
