#include <iostream>
#include "gtest/gtest.h"


using namespace ::testing;

#include "UniqueSet.h"

TEST(UniqueSetTest, FunctionalityTest) {
    class A {
    public:
        int id = 0;
    };
    UniqueSet<A> set;
    A a;
    A b;
    set.add(a);
    a.id = 3;
    set.add(b);
    set.add(a);
    ASSERT_EQ(2, set.getSize());
    A* list = set.getList();
    ASSERT_TRUE(list[0].id == 3 || list[1].id == 3);
    A tmp[2];
    int i = 0;
    for (auto iter = set.getIterator(); iter.finished(); ++iter) {
        tmp[i] = iter.getData();
        i++;
    }
    ASSERT_TRUE(tmp[0].id == 3 || tmp[1].id == 3);
    if (list[0].id == 3) {
        list[0].id = 5;
    } else {
        list[1].id = 5;
    }
    list = set.getList();
    ASSERT_FALSE(list[0].id == 5 || list[1].id == 5);
}

TEST(UniqueSetTest, stringTest) {
    UniqueSet<char *> set;
    char *a = new char[3] {"aa"};
    set.add(a);
    ASSERT_EQ(1, set.getSize());
    char **list = set.getList();
    size_t size = set.getSize();
    ASSERT_EQ(1, size);
    ASSERT_EQ('a', *list[0]);
}

TEST(UniqueSetTest, newTest) {
    class A {
    public:
        int id;
        A(int id) : id(id) {};
    };
    A* a = new A(3);
    UniqueSet<A*> set;
    set.add(a);
    ASSERT_EQ(set.getList()[0]->id, 3);
    a->id = 5;
    ASSERT_EQ(set.getList()[0]->id, 5);
}

TEST(UniqueSetTest, iteratorTest) {
    UniqueSet<int> set;
    int a = 5;
    int b = 5;
    int c = 5;
    set.add(a);
    set.add(b);
    set.add(c);
    ASSERT_EQ(3, set.getSize());
    int tmp;
    for (auto iter = set.getIterator(); iter.finished(); ++iter) {
        tmp = iter.getData();
        ASSERT_TRUE(&tmp != &a && &tmp != &b && &tmp != &c);
    }
}
TEST(UniqueSetTest, doubleTest) {
    UniqueSet<double> set;
    double a = 2.00000000001;
    set.add(a);
    ASSERT_EQ(1, set.getSize());
    double *list = set.getList();
    ASSERT_EQ(a, list[0]);
    ASSERT_EQ(a, set.getIterator().getData());
}

TEST(UniqueSetTest, iteratorBugTest) {
    UniqueSet<int> set;
    int a = 1;
    int b = 2;
    set.add(a);
    set.add(b);
    for (auto iter = set.getIterator(); iter.finished(); ++iter) {
        ASSERT_TRUE(iter.getData() == a || iter.getData() == b);
    }
}

TEST(UniqueSetTest, AddMultipleTest) {
    UniqueSet<int> set;
    int a = 1;
    int b = 2;
    int c = 1;
    set.addMultiple(a, b, c);
    ASSERT_EQ(3, set.getSize());
    set.addMultiple(a, b, c);
    ASSERT_EQ(3, set.getSize());
}

TEST(UniqueSetTest, resizeTest) {
    UniqueSet<int> set(2);
    int a = 1, b = 2, c = 3, d = 1, e = 2, f = 3;
    set.addMultiple(a,b,c,d,e,f);
    ASSERT_EQ(8, set.getCapacity());
    ASSERT_EQ(6, set.getSize());
    ASSERT_TRUE(set.contains(a));
    ASSERT_TRUE(set.contains(b));
    ASSERT_TRUE(set.contains(c));
    ASSERT_TRUE(set.contains(d));
    ASSERT_TRUE(set.contains(e));
    ASSERT_TRUE(set.contains(f));
}

TEST(UniqueSetTest, testUnique) {
    UniqueSet<int> set;
    for (int i = 0; i < 5; i++) {
        set.add(i);
    }
    ASSERT_EQ(1, set.getSize());
}

TEST(Test, uniqueCustomClass) {
    class A {
    public:
        int key;
        A(int key) : key(key) {};
    };

    A *a = new A(5);
    A *b = new A(4);
    A *c = new A(5);
    UniqueSet<A*> set;
    set.add(a);
    set.add(b);
    set.add(c);
    set.add(a);
    ASSERT_EQ(3, set.getSize());
}

TEST(UniqueSetTest, containsInt) {
    UniqueSet<int> set;
    int a = 6;
    set.add(a);
    ASSERT_TRUE(set.contains(a));
}

TEST(UniqueSetTest, containsCustomClass) {
    class A {
    public:
        int key;
        A(int key) : key(key) {};
    };

    A *a = new A(5);
    A *b = new A(4);
    A c(6);
    UniqueSet<A*> set;
    set.add(a);
    set.add(b);
    ASSERT_TRUE(set.contains(a));
    ASSERT_TRUE(set.contains(b));
}

TEST(UniqueSetTest, notAllocatedCustomClass) {
    class A {
    public:
        int key;
        A(int key) : key(key) {};
    };

    UniqueSet<A> set;
    A a(2);
    A b(2);
    set.add(a);
    ASSERT_TRUE(set.contains(a));
    ASSERT_FALSE(set.contains(b));
}

TEST(UniqueSetTest, emptySetContains) {
    int a = 2;
    UniqueSet<int> set;
    ASSERT_FALSE(set.contains(a));
}

TEST(UniqueSetTest, emptySetSize) {
    UniqueSet<int> set;
    ASSERT_EQ(0, set.getSize());
}

TEST(UniqueSetTest, RemoveFromEmpty) {
    UniqueSet<int> set;
    int a = 4;
    try {
        set.remove(a);
    } catch (ValueNotFoundException &e) {
        std::string aa = "Value not found!";
        ASSERT_EQ(aa, e.what());
    }
}

TEST(UniqueSetTest, RemoveInt) {
    UniqueSet<int> set;
    int a = 6;
    set.add(a);
    ASSERT_EQ(1, set.getSize());
    ASSERT_TRUE(set.contains(a));
    set.remove(a);
    ASSERT_EQ(0, set.getSize());
    ASSERT_FALSE(set.contains(a));
}

TEST(UniqueSetTest, RemoveCustomClass) {
    class A {
    public:
        int key;
        A(int key) : key(key) {};
    };

    UniqueSet<A*> set;
    A *a = new A(2);
    A *b = new A(3);
    set.add(a);
    set.add(b);
    set.remove(a);
    ASSERT_EQ(b, set.getList()[0]);
}

TEST(UniqueSetTest, ClearTest) {
    UniqueSet<int> set;

    int a = 1;
    int b = 2;

    set.add(a);
    set.add(b);
    set.clear();
    ASSERT_EQ(0, set.getSize());
    ASSERT_FALSE(set.contains(a));
    ASSERT_FALSE(set.contains(b));
    set.add(a);
    ASSERT_TRUE(set.contains(a));
}


TEST(UniqueSetTest, ArrayTest) {
    UniqueSet<int> set;
    int a[10];

    for (int i = 0; i < 10; i++) {
        a[i] = i;
        set.add(a[i]);
    }
    ASSERT_EQ(10, set.getSize());
    int* aa= &a[0];
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(set.contains(a[i]));
    }
}

TEST(UniqueSetTest, UnionTest) {
    UniqueSet<int> set1;
    UniqueSet<int> set2;
    int a[10];
    for (int i = 0; i < 10; i++) {
        a[i] = i;
        if (i < 5) {
            set1.add(a[i]);
        } else if (i == 5) {
            set1.add(a[i]);
            set2.add(a[i]);
        } else {
            set2.add(a[i]);
        }
    }

    UniqueSet<int> set3 = set1.setUnion(set2);
    ASSERT_EQ(10, set3.getSize());
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(set3.contains(a[i]));
    }
}

TEST(UniqueSetTest, IntersectionTest) {
    UniqueSet<int> set1;
    UniqueSet<int> set2;
    int a[10];
    for (int i = 0; i < 10; i++) {
        a[i] = i;
        if (i < 5) {
            set1.add(a[i]);
        } else if (i == 5) {
            set1.add(a[i]);
            set2.add(a[i]);
        } else {
            set2.add(a[i]);
        }
    }

    UniqueSet<int> set3 = set1.setIntersection(set2);
    for (int i = 0; i < 10; i++) {
        if (i == 5) {i++;}
        ASSERT_FALSE(set3.contains(a[i]));
    }
    ASSERT_TRUE(set3.contains(a[5]));
}

TEST(UniqueSetTest, SubSetTest) {
    UniqueSet<int> set1;
    UniqueSet<int> set2;

    int one = 1;
    int two = 2;
    int drei = 3;

    set1.add(one);
    set1.add(two);
    set2.add(one);

    ASSERT_TRUE(set2 < set1);
    ASSERT_FALSE(set2 > set1);
    ASSERT_FALSE(set2 == set1);
    ASSERT_TRUE(set2 <= set1);
    ASSERT_FALSE(set2 >= set1);

    set2.add(two);

    ASSERT_FALSE(set2 < set1);
    ASSERT_FALSE(set2 > set1);
    ASSERT_TRUE(set2 == set1);
    ASSERT_TRUE(set2 <= set1);
    ASSERT_TRUE(set2 >= set1);

    set2.add(drei);
    set2.remove(two);

    ASSERT_FALSE(set2 < set1);
    ASSERT_FALSE(set2 > set1);
    ASSERT_FALSE(set2 == set1);
    ASSERT_FALSE(set2 <= set1);
    ASSERT_FALSE(set2 >= set1);

    set1.remove(two);

    ASSERT_FALSE(set2 < set1);
    ASSERT_TRUE(set2 > set1);
    ASSERT_FALSE(set2 == set1);
    ASSERT_FALSE(set2 <= set1);
    ASSERT_TRUE(set2 >= set1);
}

TEST(UniqueSetTest, addMultipleTest) {
    UniqueSet<int> set;
    int a = 5;
    int b = 5;
    int c = 5;
    int d = 5;
    set.addMultiple(a, b, c, d, a, b);
    ASSERT_EQ(4, set.getSize());
    for (auto iter = set.getIterator(); iter.finished(); ++iter) {
        ASSERT_EQ(5, iter.getData());
    }
    ASSERT_TRUE(set.contains(a));
    ASSERT_TRUE(set.contains(b));
    ASSERT_TRUE(set.contains(c));
    ASSERT_TRUE(set.contains(d));
}