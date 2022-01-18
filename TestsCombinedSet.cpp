#include <iostream>
#include "gtest/gtest.h"


using namespace ::testing;

#include "CombinedSet.h"

TEST(CombinedSetTest, intDoubleTest) {
    CombinedSet<int, double> set;
    set.add(1);
    ASSERT_EQ(1, set.getSize());
    ASSERT_EQ(1, set.getIterator().getData<int>());

    try {
        set.getIterator().getData<double>();
    } catch (WrongTypeException &e) {
        std::string a = "Wrong type!";
        ASSERT_EQ(a, e.what());
    }
}

TEST(CombinedSetTest, EmptyTest) {
    CombinedSet<int, double> set;
    ASSERT_EQ(0, set.getSize());
    for (auto iter = set.getIterator(); iter.finished(); ++iter) {
        ASSERT_TRUE(false);
    }
    try {
        set.getIterator().getData<int>();
    } catch (EmptySetException &e) {
        std::string a = "Set is empty!";
        ASSERT_EQ(a, e.what());
    }
}

TEST(CombinedSetTest, MultipleValuesTest) {
    CombinedSet<int, char *> set;
    set.add(1);
    char a[] = "aab";
    set.add(a);
    set.add(2);
    ASSERT_EQ(3, set.getSize());
    for (auto iter = set.getIterator(); iter.finished(); ++iter) {
        try {
            ASSERT_TRUE(iter.getData<int>() == 1 || iter.getData<int>() == 2);
        } catch (WrongTypeException &e) {
            ASSERT_EQ(0, strcmp(a, iter.getData<char *>()));
        }
    }
}

TEST(CombinedSetTest, ResizeTest) {
    CombinedSet<int> set(2);
    set.add(1);
    set.add(2);
    set.add(22);
    ASSERT_EQ(3, set.getSize());
    ASSERT_EQ(4, set.getCapacity());
    for (auto iter = set.getIterator(); iter.finished(); ++iter) {
        ASSERT_TRUE(iter.getData<int>() == 1 || iter.getData<int>() == 2 || iter.getData<int>() == 22);
    }
}

TEST(CombinedSetTest, ContainsTest) {
    CombinedSet<int, char, char*, bool> set;
    for (int i = 0; i < 10; i++) {
        set.add(i);
    }
    for (char i = 'a'; i < 'h'; i++) {
        set.add(i);
    }
    char a[] = "abcd";
    char b[] = "ef";
    char c[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    set.add(a);
    set.add(b);
    set.add(c);
    set.add(false);
    ASSERT_EQ(21, set.getSize());
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(set.contains(i));
    }
    ASSERT_FALSE(set.contains(10));
    for (char i = 'a'; i < 'h'; i++) {
        ASSERT_TRUE(set.contains(i));
    }
    ASSERT_FALSE(set.contains('h'));
    ASSERT_TRUE(set.contains(a));
    ASSERT_TRUE(set.contains(b));
    ASSERT_TRUE(set.contains(c));
    char d[] = "aa";
    ASSERT_FALSE(set.contains(d));
    ASSERT_TRUE(set.contains(false));
    ASSERT_FALSE(set.contains(true));
}

TEST(CombinedSetTest, CustomClassTest) {
    class A {
    public:
        int key = 0;
        A(int a) : key(a) {}
    };
    CombinedSet<A, A*> set;
    A a(1);
    A b(2);
    A c(1);
    set.add(a, a.key);
    set.add(b, b.key);
    set.add(c, c.key);
    ASSERT_EQ(2, set.getSize());
    A *aa = new A(1);
    A *bb = new A(2);
    A *cc = new A(1);
    set.add(aa, aa->key);
    set.add(bb, bb->key);
    set.add(cc, cc->key);
    ASSERT_EQ(4, set.getSize());
    ASSERT_TRUE(set.contains(a, a.key));
    ASSERT_TRUE(set.contains(b, b.key));
    ASSERT_TRUE(set.contains(c, c.key));
    ASSERT_TRUE(set.contains(aa, aa->key));
    ASSERT_TRUE(set.contains(bb, bb->key));
    ASSERT_TRUE(set.contains(cc, cc->key));
}

TEST(CombinedSetTest, RemoveTest) {
    CombinedSet<int, char> set;
    set.add(5);
    set.add('a');
    set.add(256);
    set.add('b');
    ASSERT_EQ(4, set.getSize());
    set.remove(5);
    ASSERT_EQ(3, set.getSize());
    ASSERT_FALSE(set.contains(5));
    set.remove('b');
    ASSERT_EQ(2, set.getSize());
    ASSERT_FALSE(set.contains('b'));
    set.remove(256);
    ASSERT_EQ(1, set.getSize());
    ASSERT_FALSE(set.contains(256));

    try {
        set.remove(1);
    } catch(ValueNotFoundException &e) {
        std::string a = "Value not found!";
        ASSERT_EQ(a, e.what());
    }
    set.remove('a');
    ASSERT_EQ(0, set.getSize());
    ASSERT_FALSE(set.contains('a'));
}

TEST(CombinedSetTest, AddMultipleTest) {
    CombinedSet<int, char> set;
    set.addMultiple(1, 2, 'a', 'b');
    ASSERT_EQ(4, set.getSize());
    ASSERT_TRUE(set.contains(1));
    ASSERT_TRUE(set.contains(2));
    ASSERT_TRUE(set.contains('a'));
    ASSERT_TRUE(set.contains('b'));
    set.addMultiple(1);
    ASSERT_EQ(4, set.getSize());
};

TEST(CombinedSetTest, TypeNotAcceptedTest) {
    CombinedSet<int, char, double> set;
    try {
        set.add<bool>(false);
        ASSERT_TRUE(false);
    } catch (TypeNotAcceptedException &e) {
        std::string a = "Type not accepted by set!";
        ASSERT_EQ(a, e.what());
    }
}

TEST(CombinedSetTest, UnionTest) {
    CombinedSet<int, char> set1;
    CombinedSet<int, char> set2;

    set1.addMultiple(1, 3, 'a', 'c');
    set2.addMultiple(2, 1, 4, 'b', 'c', 'a');
    CombinedSet<int, char> set3 = set1.setUnion(set2);
    ASSERT_EQ(7, set3.getSize());
    for (int i = 1; i < 5; i++) {
        ASSERT_TRUE(set3.contains(i));
    }
    ASSERT_TRUE(set3.contains('a'));
    ASSERT_TRUE(set3.contains('b'));
    ASSERT_TRUE(set3.contains('c'));
    for (auto iter = set3.getIterator(); iter.finished(); ++iter) {
        try {
            ASSERT_TRUE(iter.getData<int>() < 5 && iter.getData<int>() > 0);
        } catch (WrongTypeException &e) {
            ASSERT_TRUE(iter.getData<char>() >= 'a' && iter.getData<char>() <= 'c');
        }
    }
}

TEST(CombinedSetTest, IntersectionTest) {
    CombinedSet<int, char> set1;
    CombinedSet<int, char> set2;

    set1.addMultiple(1, 3, 'a', 'c');
    set2.addMultiple(2, 1, 4, 'b', 'c', 'a');
    CombinedSet<int, char> set3 = set1.setIntersection(set2);
    ASSERT_EQ(3, set3.getSize());
    ASSERT_TRUE(set3.contains(1));
    ASSERT_TRUE(set3.contains('a'));
    ASSERT_TRUE(set3.contains('c'));
    for (auto iter = set3.getIterator(); iter.finished(); ++iter) {
        try {
            ASSERT_TRUE(iter.getData<int>() == 1);
        } catch (WrongTypeException &e) {
            ASSERT_TRUE(iter.getData<char>() == 'a' || iter.getData<char>() == 'c');
        }
    }
}

TEST(CombinedSetTest, SubSetTest) {
    CombinedSet<int, char> set1;
    CombinedSet<int, char> set2;

    set1.addMultiple(1, 2, 3, 'a', 'b', 'c');
    set2.addMultiple(1, 2, 'a', 'b');

    ASSERT_TRUE(set1 > set2);
    ASSERT_TRUE(set1 >= set2);
    ASSERT_FALSE(set1 < set2);
    ASSERT_FALSE(set1 <= set2);
    ASSERT_FALSE(set1 == set2);

    set2.addMultiple(3, 'c');

    ASSERT_FALSE(set1 > set2);
    ASSERT_TRUE(set1 >= set2);
    ASSERT_FALSE(set1 < set2);
    ASSERT_TRUE(set1 <= set2);
    ASSERT_TRUE(set1 == set2);

    set1.remove(1);

    ASSERT_FALSE(set1 > set2);
    ASSERT_FALSE(set1 >= set2);
    ASSERT_TRUE(set1 < set2);
    ASSERT_TRUE(set1 <= set2);
    ASSERT_FALSE(set1 == set2);
}

TEST(CombinedSetTest, clearTest) {
    CombinedSet<int, char> set;
    set.addMultiple(1, 2, 3, 'a', 'b', 'c');
    set.clear();
    ASSERT_EQ(0, set.getSize());
    for (int i = 1; i < 4; i++) {
        ASSERT_FALSE(set.contains(i));
    }
    for (char i = 'a'; i < 'd'; i++) {
        ASSERT_FALSE(set.contains(i));
    }
    for (auto iter = set.getIterator(); iter.finished(); ++iter) {
        ASSERT_TRUE(false);
    }
}