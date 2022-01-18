#include <iostream>
#include "gtest/gtest.h"

using namespace ::testing;

#include "OrderedSet.h"

TEST(OrderedSetTest, test) {
    OrderedSet<int> set;
    set.add(1);
    set.add(2);
    set.add(1);
    ASSERT_EQ(2, set.getSize());
    ASSERT_TRUE(set.contains(1));
    ASSERT_TRUE(set.contains(2));
}

TEST(OrderedSetTest, ClassTest) {
    class A {
    public:
        int key = 0;
        A(int key) : key(key) {};
    };

    A a(1);
    A b(2);
    A c(3);
    OrderedSet<A> set;

    set.add(a, a.key);
    ASSERT_EQ(1, set.getSize());
    set.add(b, b.key);
    ASSERT_EQ(2, set.getSize());
    set.add(c, c.key);
    ASSERT_EQ(3, set.getSize());

    ASSERT_TRUE(set.contains(a, a.key));
    ASSERT_EQ(3, set.getSize());
}

TEST(OrderedSetTest, CharPointerTest) {
    OrderedSet<char *> set;
    char a[] = "ab";
    char b[] = "ba";
    char c[] = "aa";
    char d[] = "aaa";
    char e[] = "aba";
    char f[] = "aaaa";
    set.addMultiple(a, b, c, d, e, f);
    ASSERT_EQ(6, set.getSize());
    char **list = set.getSortedList();
    ASSERT_EQ(0, strcmp(c, list[0]));
    ASSERT_EQ(0, strcmp(a, list[1]));
    ASSERT_EQ(0, strcmp(b, list[2]));
    ASSERT_EQ(0, strcmp(d, list[3]));
    ASSERT_EQ(0, strcmp(e, list[4]));
    ASSERT_EQ(0, strcmp(f, list[5]));
}

TEST(OrderedSetTest, longIntTest) {
    OrderedSet<long int> set;
    long int a = 2;
    set.add(a);
    ASSERT_EQ(1, set.getSize());
    ASSERT_TRUE(set.contains(a));
}

TEST(OrderedSetTest, FloatTest) {
    OrderedSet<float> set;
    set.add(2.5);
    set.add(2.55);
    set.add(2.555);

    ASSERT_EQ(3, set.getSize());
    ASSERT_TRUE(set.contains(2.55));
}

TEST(OrderedSetTest, removeTest) {
    OrderedSet<int> set;
    for (int i = 0; i < 4; i++) {
        set.add(i);
    }
    ASSERT_TRUE(set.contains(2));
    set.remove(2);
    ASSERT_FALSE(set.contains(2));
    ASSERT_EQ(3, set.getSize());
}

TEST(OrderedSetTest, balancedRemoveTest) {
    OrderedSet<int> set;
    int a[7] = {4, 2, 6, 1, 3, 5, 7};
    for (int i = 0; i < 7; i++) {
        set.add(a[i]);
    }

    ASSERT_EQ(7, set.getSize());
    set.remove(4);
    ASSERT_FALSE(set.contains(4));
    ASSERT_EQ(6, set.getSize());
}

TEST(OrderedSetTest, emptyRemoveTest) {
    OrderedSet<int> set;
    try {
        set.remove(2);
    } catch (ValueNotFoundException &e) {
        std::string a = "Value not found!";
        ASSERT_EQ(a, e.what());
    }
}

TEST(OrderedSetTest, removeLastTest) {
    OrderedSet<int> set;
    set.add(2);
    ASSERT_EQ(1, set.getSize());
    set.remove(2);
    ASSERT_EQ(0, set.getSize());
    ASSERT_FALSE(set.contains(2));
}

TEST(OrderedSetTest, ClearTest) {
    OrderedSet<int> set;
    int a[7] = {4, 2, 6, 1, 3, 5, 7};
    for (int i = 0; i < 7; i++) {
        set.add(a[i]);
    }

    set.clear();
    ASSERT_EQ(0, set.getSize());
    for (int i = 0; i < 7; i++) {
        ASSERT_FALSE(set.contains(a[i]));
    }
    try {
        set.remove(2);
    } catch (ValueNotFoundException &e) {
        std::string a = "Value not found!";
        ASSERT_EQ(a, e.what());
    }
}

TEST(OrderedSetTest, IteratorTest) {
    OrderedSet<int> set;
    int a[7] = {4, 2, 6, 1, 3, 5, 7};
    for (int i = 0; i < 7; i++) {
        set.add(a[i]);
    }
    int i = 1;
    for (auto iter = set.getIterator(); iter.finished(); ++iter, i++) {
        ASSERT_EQ(i, iter.getData());
    }
}

TEST(OrderedSetTest, sortedListTest) {
    OrderedSet<int> set;
    int a[13] = {9, 8, 5, 6, 7, 2, 3, 8, 6, 1, 2, 4, 10};

    for (int i = 0; i < 13; i++) {
        set.add(a[i]);
    }
    ASSERT_EQ(10, set.getSize());
    int *list = set.getSortedList();
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(list[i], i+1);
    }
}

TEST(OrderedSetTest, minMaxTest) {
    OrderedSet<int> set;
    int a[9] = {9, 5, 6, 7, 2, 3, 6, 1, 4};

    for (int i = 0; i < 9; i++) {
        set.add(a[i]);
    }
    ASSERT_EQ(1, set.min());
    ASSERT_EQ(9, set.max());
}



TEST(OrderedSetTest, ConditionsTest) {
    OrderedSet<int> set1;
    OrderedSet<int> set2;

    set1.add(1);
    set1.add(2);
    set2.add(1);

    ASSERT_TRUE(set2 < set1);
    ASSERT_FALSE(set2 > set1);
    ASSERT_FALSE(set2 == set1);
    ASSERT_TRUE(set2 <= set1);
    ASSERT_FALSE(set2 >= set1);

    set2.add(2);

    ASSERT_FALSE(set2 < set1);
    ASSERT_FALSE(set2 > set1);
    ASSERT_TRUE(set2 == set1);
    ASSERT_TRUE(set2 <= set1);
    ASSERT_TRUE(set2 >= set1);

    set2.add(3);
    set2.remove(2);

    ASSERT_FALSE(set2 < set1);
    ASSERT_FALSE(set2 > set1);
    ASSERT_FALSE(set2 == set1);
    ASSERT_FALSE(set2 <= set1);
    ASSERT_FALSE(set2 >= set1);

    set1.remove(2);

    ASSERT_FALSE(set2 < set1);
    ASSERT_TRUE(set2 > set1);
    ASSERT_FALSE(set2 == set1);
    ASSERT_FALSE(set2 <= set1);
    ASSERT_TRUE(set2 >= set1);
}

TEST(OrderedSetTest, unionTest) {
    OrderedSet<int> set1;
    OrderedSet<int> set2;
    int a1[6] = {4, 2, 6, 1, 3, 5};
    int a2[6] = {7, 6, 8, 10, 9, 1};

    for (int i = 0; i < 6; i++) {
        set1.add(a1[i]);
        set2.add(a2[i]);
    }
    OrderedSet<int> set3 = set1.setUnion(set2);
    for (int i = 1; i < 11; i++) {
        ASSERT_TRUE(set3.contains(i));
    }

}

TEST(OrderedSetTest, intersectionTest) {
    OrderedSet<int> set1;
    OrderedSet<int> set2;
    int a1[6] = {4, 2, 6, 1, 3, 5};
    int a2[6] = {2, 2, 8, 3, 9, 1};

    for (int i = 0; i < 6; i++) {
        set1.add(a1[i]);
        set2.add(a2[i]);
    }
    OrderedSet<int> set3 = set1.setIntersection(set2);
    for (int i = 1; i < 11; i++) {
        if (i < 4) {
            ASSERT_TRUE(set3.contains(i));
        } else {
            ASSERT_FALSE(set3.contains(i));
        }
    }
}

TEST(OrderedSetTest, getItemTest) {
    OrderedSet<int> set;
    int a[11] = {6, 1, 9, 7, 3, 2, 8, 5, 9, 4, 2};
    for (int i = 0; i < 11; i++) {
        set.add(a[i]);
    }
    for (int i = 0; i < set.getSize(); i++) {
        ASSERT_EQ(i+1, set.getItem(i));
    }
    set.clear();

    for (int i = 0; i < 11; i++) {
        set.add(i);
    }
    for (int i = 0; i < 11; i++) {
        ASSERT_EQ(i, set.getItem(i));
    }
}

TEST(OrderedSetTest, getIndexTest) {
    OrderedSet<int> set;
    int a[11] = {6, 1, 9, 7, 3, 2, 8, 5, 9, 4, 2};
    for (int i = 0; i < 11; i++) {
        set.add(a[i]);
    }
    for (int i = 1; i < 10; i++) {
        ASSERT_EQ(i-1, set.getIndex(i));
    }
    set.clear();

    for (int i = 0; i < 11; i++) {
        set.add(i);
    }
    for (int i = 0; i < 11; i++) {
        ASSERT_EQ(i, set.getIndex(i));
    }
    set.clear();

    for (int i = 10; i >= 0; i--) {
        set.add(i);
    }
    for (int i = 0; i < 11; i++) {
        ASSERT_EQ(i, set.getIndex(i));
    }
    set.clear();

    int b[6] = {6, 1, 5, 2, 4, 3};
    for (int i = 0; i < 6; i++) {
        set.add(b[i]);
    }
    for (int i = 0; i < 6; i++) {
        ASSERT_EQ(i, set.getIndex(i+1));
    }
}