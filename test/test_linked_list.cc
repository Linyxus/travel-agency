#include "LinkedList.hh"
#include "gtest/gtest.h"

namespace tagc {

TEST(LinkedListTest, IteratorWorks) {
    auto l = LinkedList<int>{};
    l.push(1); l.push(10); l.push(100);

    int s = 0;
    for (auto i : l) {
        s = s + i;
    }
    ASSERT_EQ(s, 111);

    for (auto it = l.begin(); it != l.end(); it++) {
        *it = *it * 2;
    }

    s = 0;
    for (auto i : l) {
        s = s + i;
    }
    ASSERT_EQ(s, 222);
}

TEST(LinkedListTest, AppendWorks) {
    auto l = LinkedList<int>{};
    l.append(1); l.append(10); l.append(100);

    auto it = l.cbegin();
    ASSERT_EQ(*it, 1);

    it++;
    ASSERT_EQ(*it, 10);

    it++;
    ASSERT_EQ(*it, 100);
}

TEST(LinkedListTest, CopyWorks) {
    auto l1 = LinkedList<int>{};
    l1.push(1); l1.push(10); l1.push(100);

    auto l2(l1);

    for (auto it = l1.begin(); it != l1.end(); it++) {
        *it = *it * 2;
    }

    int s = 0;
    for (auto i : l1) {
        s = s + i;
    }
    ASSERT_EQ(s, 222);

    s = 0;
    for (auto i : l2) {
        s = s + i;
    }
    ASSERT_EQ(s, 111);
}

TEST(LinkedListTest, MoveWorks) {
    auto l1 = LinkedList<int>{};
    l1.push(1); l1.push(10); l1.push(100);

    auto l2(std::move(l1));

    int s = 0;
    for (auto i : l2) {
        s = s + i;
    }
    ASSERT_EQ(s, 111);

    ASSERT_EQ(l1.begin(), l1.end());
}

TEST(LinkedListTest, LengthWorks) {
    auto l = LinkedList<int>{};
    ASSERT_EQ(l.length(), 0);

    l.push(1);
    ASSERT_EQ(l.length(), 1);

    l.push(1);
    ASSERT_EQ(l.length(), 2);
}

}
