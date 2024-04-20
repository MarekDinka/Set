#include <iostream>
#include "gtest/gtest.h"


using namespace ::testing;

#include "Set.h"

TEST(SetTest, charTest) {
    Set<char *> set;
    char *a = new char[3] {"aa"}; // REFACTOR: useless allocation, not dealocated
    set.add(a);
    ASSERT_EQ(1, set.getSize());
    char **list = set.getList();
    size_t size = set.getSize();
    ASSERT_EQ(1, size);
    ASSERT_EQ('a', *list[0]);
}

TEST(SetTest, stringTest) {
    Set<std::string> set;
    std::string a = "aa";
    set.add(a);
    ASSERT_EQ(1, set.getSize());
    std::string *list = set.getList();
    size_t size = set.getSize();
    ASSERT_EQ(1, size);
    ASSERT_EQ("aa", list[0]);
}

TEST(SetTest, intTest) {
    Set<int> set;
    int a = 5;
    set.add(a);
    set.add(-1);
    ASSERT_EQ(2, set.getSize());
    int *list = set.getList();
    ASSERT_EQ(a, list[0]);
}

TEST(SetTest, longIntTest) {
    Set<long int> set;
    long int a = 5;
    set.add(a);
    ASSERT_EQ(1, set.getSize());
    long int *list = set.getList();
    ASSERT_EQ(a, list[0]);
}

TEST(SetTest, doubleTest) {
    Set<double> set;
    double a = 2.00000000001;
    set.add(a);
    ASSERT_EQ(1, set.getSize());
    double *list = set.getList();
    ASSERT_EQ(a, list[0]);
}

TEST(SetTest, BoolTest) {
    Set<bool> set;
    bool a = false;
    set.add(a);
    ASSERT_EQ(1, set.getSize());
}



TEST(SetTest, CustomClassTest) {
    class A {
    public:
        int key;
        A(int key) : key(key) {};
    };

    A *a = new A(5);
    A *b = new A(4);
    Set<A*> set;
    set.add(a, a->key);
    set.add(b, b->key);
    ASSERT_EQ(2, set.getSize());
    ASSERT_EQ(b, set.getList()[0]);
}

TEST(SetTest, resizeTest) {
    Set<int> set(2);
    for (int i = 0; i < 10; i++) {
        set.add(i);
    }
    ASSERT_EQ(16, set.getCapacity());
    ASSERT_EQ(10, set.getSize());
    int *list = set.getList();
    for (int i = 0; i < set.getSize(); i++) {
        ASSERT_EQ(i, list[i]);
    }
}

TEST(SetTest, testUnique) {
    Set<int> set;
    for (int i = 0; i < 5; i++) {
        set.add(i);
    }
    for (int i = 0; i < 5; i++) {
        set.add(i);
    }
    ASSERT_EQ(5, set.getSize());
}

TEST(SetTest, uniqueCustomClass) {
    class A {
    public:
        int key;
        A(int key) : key(key) {};
    };

    A *a = new A(5);
    A *b = new A(4);
    A *c = new A(5);
    Set<A*> set;
    set.add(a, a->key);
    set.add(b, b->key);
    set.add(c, c->key);
    set.add(a, a->key);
    ASSERT_EQ(2, set.getSize());
}

TEST(SetTest, containsInt) {
    Set<int> set;
    int a = 6;
    set.add(a);
    ASSERT_TRUE(set.contains(a));
}

TEST(SetTest, containsCustomClass) {
    class A {
    public:
        int key;
        A(int key) : key(key) {};
    };

    A *a = new A(5);
    A *b = new A(4);
    A c(6);
    Set<A*> set;
    set.add(a, a->key);
    set.add(b, b->key);
    ASSERT_TRUE(set.contains(a, a->key));
    ASSERT_TRUE(set.contains(b, b->key));
}

TEST(SetTest, notAllocatedCustomClass) {
    class A {
    public:
        int key;
        A(int key) : key(key) {};
    };

    Set<A> set;
    A a(2);
    set.add(a, a.key);
    ASSERT_TRUE(set.contains(a, a.key));
}

TEST(SetTest, emptySetContains) {
    int a = 2;
    Set<int> set;
    ASSERT_FALSE(set.contains(a));
}

TEST(SetTest, emptySetSize) {
    Set<int> set;
    ASSERT_EQ(0, set.getSize());
}

TEST(SetTest, RemoveFromEmpty) {
    Set<int> set;
    try {
        set.remove(4);
    } catch (ValueNotFoundException &e) {
        ASSERT_EQ("Value not found!", e.what());
    }
}

TEST(SetTest, RemoveInt) {
    Set<int> set;
    set.add(6);
    ASSERT_EQ(1, set.getSize());
    set.remove(6);
    ASSERT_EQ(0, set.getSize());
}

TEST(SetTest, RemoveCustomClass) {
    class A {
    public:
        int key;
        A(int key) : key(key) {};
    };

    Set<A*> set;
    A *a = new A(2);
    A *b = new A(3);
    set.add(a, a->key);
    set.add(b, b->key);
    set.remove(a, a->key);
    ASSERT_EQ(b, set.getList()[0]);
}

TEST(SetTest, RemoveCustomClassWithAnotherCustomClass) {
    class A {
    public:
        int key;
        A(int key) : key(key) {};
    };

    Set<A*> set;
    A *a = new A(2);
    A *b = new A(2);
    set.add(a, a->key);
    set.add(b, b->key);
    set.remove(b, b->key);
    ASSERT_EQ(0, set.getSize());
}

TEST(SetTest, bucketTest) {
    Set<int> set(4);
    set.add(1);
    set.add(5);

    ASSERT_TRUE(set.contains(1));
    ASSERT_TRUE(set.contains(5));
    ASSERT_EQ(2, set.getSize());

    set.clear();
    ASSERT_FALSE(set.contains(1));
    ASSERT_FALSE(set.contains(5));
}

TEST(SetTest, ClearTest) {
    Set<int> set;

    set.add(1);
    set.add(2);
    set.clear();
    ASSERT_EQ(0, set.getSize());
    ASSERT_FALSE(set.contains(1));
    ASSERT_FALSE(set.contains(2));
    set.add(1);
    ASSERT_TRUE(set.contains(1));
}

TEST(SetTest, UnionTest) {
    Set<int> set1;
    Set<int> set2;

    for (int i = 0; i < 10; i++) {
        if (i < 5) {
            set1.add(i);
        } else if (i == 5) {
            set1.add(i);
            set2.add(i);
        } else {
            set2.add(i);
        }
    }

    Set<int> set3 = set1.setUnion(set2);
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(set3.contains(i));
    }
    ASSERT_FALSE(set3.contains(10));
}

TEST(SetTest, IntersectionTest) {
    Set<int> set1;
    Set<int> set2;

    for (int i = 0; i < 10; i++) {
        if (i < 5) {
            set1.add(i);
        } else if (i == 5) {
            set1.add(i);
            set2.add(i);
        } else {
            set2.add(i);
        }
    }

    Set<int> set3 = set1.setIntersection(set2);
    for (int i = 0; i < 10; i++) {
        if (i == 5) {i++;}
        ASSERT_FALSE(set3.contains(i));
    }
    ASSERT_TRUE(set3.contains(5));
}

TEST(SetTest, SubSetTest) {
    Set<int> set1;
    Set<int> set2;

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