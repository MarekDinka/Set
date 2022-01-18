This project consists of 5 different implementations of set, they all have similar methods, but different ways of dealing
with data.
=========================================================================================================================
Unordered:
    all unordered implementations of set have shared public methods:
        Set(size_t startingCapacity = 10) -> constructor, capacity will be increased if enough elements are added
        Iterator getIterator() -> iterator -> typical for-loop is:
            for(auto iter = set.getIterator(); iter.finished(); ++iter)
            public methods are:
                Iterator(Set<type> set) -> finds first and last element in unordered hashing table and initializes iterator
                void operator++() -> increments to next element
                type getData() -> returns data contained in current element
                bool finished() -> false if iterator is past last element, otherwise true
        void addMultiple(type value, types ... values) -> add multiple values to set
        void add(type value) -> add hashable value to set
        void add(type value, int key) -> add value with a key
        void remove(type value) -> remove value from set
        void remove(type value, int key)
        void clear() -> remove all elements
        bool contains(type value) -> true if such value is in set
        bool contains(type value, int key)
        size_t getSize() -> returns size of set(number of elements in set)
        size_t getCapacity() -> returns capacity of set
        type *getList() -> returns allocated list of elements
        Set<type> setUnion(Set<type> otherSet) -> returns a new set which will be a union of the two
        Set<type> setIntersection(Set<type> otherSet) -> returns a new set which will be a intersection of the two
        operators:
            == -> equality
            <, > -> subset
            <=, >= -> subset or equal
-------------------------------------------------------------------------------------------------------------------------
    Set:
        Ordinary set that uses hashing table with buckets for data storage and requires a key to be provided if lacks hash function
        for requested type.
        types with hashing function are: all integral and floating types, char *, std::string.
-------------------------------------------------------------------------------------------------------------------------
    UniqueSet:
        Instead of using key for hashing it uses pointer to variable that was added, which means that int a = 1; and
        int b = 1; are two different elements, this implementation is useful mainly for objects
-------------------------------------------------------------------------------------------------------------------------
    CombinedSet:
        This implementation is similar to Set but it allows you to store different types in one set, it is important to note
        here that different types are not equal even if they have the same key
-------------------------------------------------------------------------------------------------------------------------
    UniqueCombinedSet:
        Basically a combination of CombinedSet and UniqueSet
=========================================================================================================================
Ordered:
    OrderedSet:
        The last implementation is a ordered set which, uses a binary search tree to store all values and quickly access them,
        it also uses the same hashing as Set
        public methods are:
            Basically everything unordered sets have
            type min()
            type max()
            type *getSortedList()
            type getItem(size_t index) -> returns item would be on such index in a sorted list without creating one
            size_t getIndex(type value) -> returns index where such item would be in a sorted list
            size_t getIndex(type value, int key)