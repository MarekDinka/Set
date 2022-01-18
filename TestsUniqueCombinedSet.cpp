#include <iostream>
#include "gtest/gtest.h"


using namespace ::testing;

#include "UniqueCombinedSet.h"

TEST(UniqueCombinedSetTest, intDoubleTest) {
    UniqueCombinedSet<int, double> set;
    int a = 1;
    set.add(a);
    ASSERT_EQ(1, set.getSize());
    ASSERT_EQ(1, set.getIterator().getData<int>());

    try {
        set.getIterator().getData<double>();
    } catch (WrongTypeException &e) {
        std::string a = "Wrong type!";
        ASSERT_EQ(a, e.what());
    }
}

TEST(UniqueCombinedSetTest, EmptyTest) {
    UniqueCombinedSet<int, double> set;
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

TEST(UniqueCombinedSetTest, MultipleValuesTest) {
    UniqueCombinedSet<int, char *> set;
    int b = 1;
    set.add(b);
    char *a = new char[4] { "aab" };
    set.add(a);
    int c = 2;
    set.add(c);
    ASSERT_EQ(3, set.getSize());
    for (auto iter = set.getIterator(); iter.finished(); ++iter) {
        try {
            ASSERT_TRUE(iter.getData<int>() == 1 || iter.getData<int>() == 2);
        } catch (WrongTypeException &e) {
            ASSERT_EQ(0, strcmp(a, iter.getData<char *>()));
        }
    }
    delete [] a;
}

TEST(UniqueCombinedSetTest, ResizeTest) {
    UniqueCombinedSet<int> set(2);
    int a = 1, b = 2, c = 22;
    set.add(a);
    set.add(b);
    set.add(c);
    ASSERT_EQ(3, set.getSize());
    ASSERT_EQ(4, set.getCapacity());
    for (auto iter = set.getIterator(); iter.finished(); ++iter) {
        ASSERT_TRUE(iter.getData<int>() == 1 || iter.getData<int>() == 2 || iter.getData<int>() == 22);
    }
}

TEST(UniqueCombinedSetTest, ContainsTest) {
    UniqueCombinedSet<int, char, char*, bool> set;
    int one = 1, two = 2, three = 3, four = 4, five = 5, six = 6;
    char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e', f = 'f';
    char *text1 =  new char[] {"abcd"};
    char *text2 = new char[] {"ef"};
    char *text3 = new char[] {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};
    bool fal = false;
    set.addMultiple(one, a, two, b, three, c, four, d, five, e, six, f, text1, fal, text2, text3);
    ASSERT_EQ(16, set.getSize());
    ASSERT_TRUE(set.contains(two));
    ASSERT_TRUE(set.contains(a));
    ASSERT_TRUE(set.contains(five));
    ASSERT_TRUE(set.contains(text1));
    ASSERT_TRUE(set.contains(d));
    ASSERT_TRUE(set.contains(a));
    ASSERT_TRUE(set.contains(text2));
    ASSERT_TRUE(set.contains(six));
    ASSERT_TRUE(set.contains(text3));
    ASSERT_TRUE(set.contains(fal));

    char *text4 = new char[] {"abcd"};
    int onee = 1;
    char aa = 'a';
    ASSERT_FALSE(set.contains(text4));
    ASSERT_FALSE(set.contains(onee));
    ASSERT_FALSE(set.contains(aa));

    a = 'b';
    aa = 'b';
    ASSERT_TRUE(set.contains(a));
    ASSERT_FALSE(set.contains(aa));
}

TEST(UniqueCombinedSetTest, CustomClassTest) {
    class A {
    public:
        int key = 0;
        A(int a) : key(a) {}
    };
    UniqueCombinedSet<A, A*> set;
    A a(1);
    A b(2);
    A c(1);
    set.add(a);
    set.add(b);
    set.add(c);
    set.add(a);
    ASSERT_EQ(3, set.getSize());
    A *aa = new A(1);
    A *bb = new A(2);
    A *cc = new A(1);
    set.add(aa);
    set.add(bb);
    set.add(cc);
    ASSERT_EQ(6, set.getSize());
    ASSERT_TRUE(set.contains(a));
    ASSERT_TRUE(set.contains(b));
    ASSERT_TRUE(set.contains(c));
    ASSERT_TRUE(set.contains(aa));
    ASSERT_TRUE(set.contains(bb));
    ASSERT_TRUE(set.contains(cc));
}

TEST(UniqueCombinedSetTest, RemoveTest) {
    UniqueCombinedSet<int, char> set;
    int a = 5;
    int b = 256;
    char c = 'a';
    char d = 'b';
    set.add(a);
    set.add(c);
    set.add(b);
    set.add(d);
    ASSERT_EQ(4, set.getSize());
    set.remove(a);
    ASSERT_EQ(3, set.getSize());
    ASSERT_FALSE(set.contains(a));
    set.remove(c);
    ASSERT_EQ(2, set.getSize());
    ASSERT_FALSE(set.contains(c));
    set.remove(b);
    ASSERT_EQ(1, set.getSize());
    ASSERT_FALSE(set.contains(b));

    try {
        set.remove(a);
    } catch(ValueNotFoundException &e) {
        std::string msg = "Value not found!";
        ASSERT_EQ(msg, e.what());
    }
    set.remove(d);
    ASSERT_EQ(0, set.getSize());
    ASSERT_FALSE(set.contains(d));
}

TEST(UniqueCombinedSetTest, AddMultipleTest) {
    UniqueCombinedSet<int, char> set;
    int a = 1, b = 2;
    char c = 'a', d = 'b';
    set.addMultiple(a, b, c, d);
    ASSERT_EQ(4, set.getSize());
    ASSERT_TRUE(set.contains(a));
    ASSERT_TRUE(set.contains(b));
    ASSERT_TRUE(set.contains(c));
    ASSERT_TRUE(set.contains(d));
    set.addMultiple(a);
    ASSERT_EQ(4, set.getSize());
};

TEST(UniqueCombinedSetTest, TypeNotAcceptedTest) {
    UniqueCombinedSet<int, char, double> set;
    bool fal = false;
    try {
        set.add<bool>(fal);
        ASSERT_TRUE(false);
    } catch (TypeNotAcceptedException &e) {
        std::string a = "Type not accepted by set!";
        ASSERT_EQ(a, e.what());
    }
}

TEST(UniqueCombinedSetTest, UnionTest) {
    UniqueCombinedSet<int, char> set1;
    UniqueCombinedSet<int, char> set2;
    int one = 1, two = 2, three = 3, four = 4;
    char a = 'a', b = 'b', c = 'c';

    set1.addMultiple(one, three, a, c);
    set2.addMultiple(two, one, four, one, b, c, c, a);
    UniqueCombinedSet<int, char> set3 = set1.setUnion(set2);
    ASSERT_EQ(7, set3.getSize());
    ASSERT_TRUE(set3.contains(one));
    ASSERT_TRUE(set3.contains(two));
    ASSERT_TRUE(set3.contains(three));
    ASSERT_TRUE(set3.contains(four));
    ASSERT_TRUE(set3.contains(a));
    ASSERT_TRUE(set3.contains(b));
    ASSERT_TRUE(set3.contains(c));
    for (auto iter = set3.getIterator(); iter.finished(); ++iter) {
        try {
            ASSERT_TRUE(iter.getData<int>() < 5 && iter.getData<int>() > 0);
        } catch (WrongTypeException &e) {
            ASSERT_TRUE(iter.getData<char>() >= 'a' && iter.getData<char>() <= 'c');
        }
    }
}

TEST(UniqueCombinedSetTest, IntersectionTest) {
    UniqueCombinedSet<int, char> set1;
    UniqueCombinedSet<int, char> set2;
    int one = 1, two = 2, three = 3, four = 4;
    char a = 'a', b = 'b', c = 'c';

    set1.addMultiple(one, three, a, c);
    set2.addMultiple(two, one, four, one, b, c, c, a);
    UniqueCombinedSet<int, char> set3 = set1.setIntersection(set2);
    ASSERT_EQ(3, set3.getSize());
    ASSERT_TRUE(set3.contains(one));
    ASSERT_TRUE(set3.contains(a));
    ASSERT_TRUE(set3.contains(c));
    for (auto iter = set3.getIterator(); iter.finished(); ++iter) {
        try {
            ASSERT_TRUE(iter.getData<int>() == 1);
        } catch (WrongTypeException &e) {
            ASSERT_TRUE(iter.getData<char>() == 'a' || iter.getData<char>() == 'c');
        }
    }
}

TEST(UniqueCombinedSetTest, SubSetTest) {
    UniqueCombinedSet<int, char> set1;
    UniqueCombinedSet<int, char> set2;
    int one = 1, two = 2, three = 3, four = 4;
    char a = 'a', b = 'b', c = 'c';

    set1.addMultiple(one, two, three, a, b, c);
    set2.addMultiple(one, two, a, b);

    ASSERT_TRUE(set1 > set2);
    ASSERT_TRUE(set1 >= set2);
    ASSERT_FALSE(set1 < set2);
    ASSERT_FALSE(set1 <= set2);
    ASSERT_FALSE(set1 == set2);

    set2.addMultiple(three, c);

    ASSERT_FALSE(set1 > set2);
    ASSERT_TRUE(set1 >= set2);
    ASSERT_FALSE(set1 < set2);
    ASSERT_TRUE(set1 <= set2);
    ASSERT_TRUE(set1 == set2);

    set1.remove(one);

    ASSERT_FALSE(set1 > set2);
    ASSERT_FALSE(set1 >= set2);
    ASSERT_TRUE(set1 < set2);
    ASSERT_TRUE(set1 <= set2);
    ASSERT_FALSE(set1 == set2);
}

TEST(UniqueCombinedSetTest, clearTest) {
    UniqueCombinedSet<int, char> set;
    int one = 1, two = 2, three = 3, four = 4;
    char a = 'a', b = 'b', c = 'c';

    set.addMultiple(one, two, three, a, b, c);
    set.clear();
    ASSERT_EQ(0, set.getSize());
    ASSERT_FALSE(set.contains(one));
    ASSERT_FALSE(set.contains(two));
    ASSERT_FALSE(set.contains(three));
    ASSERT_FALSE(set.contains(a));
    ASSERT_FALSE(set.contains(b));
    ASSERT_FALSE(set.contains(c));
    for (auto iter = set.getIterator(); iter.finished(); ++iter) {
        ASSERT_TRUE(false);
    }
}