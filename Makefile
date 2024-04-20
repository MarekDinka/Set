default: all

all:
	g++ -o TestsCombinedSet.cpp TestsOrderedSet.cpp TestsSet.cpp TestsUniqueCombinedSet.cpp TestsUniqueSet.cpp -lgtest -Wall -Wno-sign-compare && ./main
