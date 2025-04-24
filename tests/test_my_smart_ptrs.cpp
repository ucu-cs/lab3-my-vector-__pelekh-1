#include <gtest/gtest.h>
#include "my_unique_ptr.h"
#include "my_shared_ptr.h"
#include "my_weak_ptr.h"

TEST(SmartPtr, UniqueOwnership) {
    my_unique_ptr<int> up(new int(5));
    EXPECT_TRUE(up);
    my_unique_ptr<int> up2(std::move(up));
    EXPECT_FALSE(up);
    EXPECT_EQ(*up2, 5);
}

TEST(SmartPtr, SharedCount) {
    my_shared_ptr<int> sp1(new int(10));
    {
        my_shared_ptr<int> sp2 = sp1;
        EXPECT_EQ(sp1.use_count(), 2);
    }
    EXPECT_EQ(sp1.use_count(), 1);
}

TEST(SmartPtr, WeakLock) {
    my_shared_ptr<int> sp(new int(7));
    my_weak_ptr<int>  wp = sp;
    EXPECT_EQ(sp.use_count(), 1);
    auto sp2 = wp.lock();
    EXPECT_EQ(sp.use_count(), 2);
}
