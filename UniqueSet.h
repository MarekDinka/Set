#pragma once

#include <iostream>
#include <typeinfo>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <type_traits>
#include "Exceptions.h"

template<typename type> class UniqueSet {
    const float RESIZE_AT = 0.75;
    const int MULTIPLY_SIZE_BY = 2;

    class Node {
        type* data;
        size_t key;
        Node *next = nullptr;
    public:
        Node(type* data, size_t key) : data(data), key(key) {};
        bool operator==(Node &node) {return key == node.key;};
        type getData() {return *data;};
        type* getDataPointer() {return data;}
        size_t getKey() {return key;};
        Node *getNext() {return next;};
        void setNext(Node *node) {(next == nullptr) ? next=node : throw OccupiedSpaceException();};
    };
    size_t capacity = 0;
    size_t size = 0;
    Node **list; // list -> bucket
public:
    explicit UniqueSet(size_t startingCapacity = 10) {
        list = new Node* [startingCapacity] {nullptr};
        capacity = startingCapacity;
    };

    class Iterator {
        friend UniqueSet<type>;

        Node *node;
        size_t i;
        UniqueSet<type> set;
        Node *last;
        bool end = false;
    public:
        explicit Iterator(UniqueSet<type> set) : node(nullptr), i(0), set(set) {
            Node* n = nullptr;
            for (int j = 0; j < set.capacity; j++) {
                if (set.list[j] != nullptr) {
                    n = set.list[j];
                    if (node == nullptr) {
                        i = j;
                        node = set.list[j];
                    }
                }
            }
            while (n->getNext() != nullptr) {
                n = n->getNext();
            }
            last = n;
        }

        void operator++() {
            if (i >= (int) set.capacity || i < 0) {
                throw IndexOutOfRangeException();
            }
            if (node == nullptr) {
                i++;
                if (i >= set.capacity) {
                    return;
                }
                node = set.list[i];
            } else {
                node = node->getNext();
            }
            if (node == nullptr) {
                ++(*this);
            }
        };

        type getData() {return node->getData();}
        bool finished() {
            if (end) {
                return false;
            }
            if (node == last) {
                end = true;
            }
            return true;
        };

    private:
        Node* getNode() {return node;}
    };

    friend Iterator;
    Iterator getIterator() {
        return Iterator(*this);
    }

    UniqueSet<type> setUnion(UniqueSet<type> otherSet) {
        UniqueSet<type> newSet {otherSet};
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            type *tmp = iter.getNode()->getDataPointer();
            newSet.add(*tmp);
        }
        return newSet;
    };

    UniqueSet<type> setIntersection(UniqueSet<type> otherSet) {
        UniqueSet<type> newSet;
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            type *tmp = iter.getNode()->getDataPointer();
            if (otherSet.contains(*tmp)) {
                newSet.add(*tmp);
            }
        }
        return newSet;
    };

    bool operator==(UniqueSet<type> otherSet) {
        if (size != otherSet.getSize()) {
            return false;
        }
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            type *tmp = iter.getNode()->getDataPointer();
            if (!otherSet.contains(*tmp)) {
                return false;
            }
        }
        return true;
    };
    bool operator<(UniqueSet<type> otherSet) {
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            type *tmp = iter.getNode()->getDataPointer();
            if (!otherSet.contains(*tmp)) {
                return false;
            }
        }
        if (size >= otherSet.getSize()) {
            return false;
        }
        return true;
    };
    bool operator>(UniqueSet<type> otherSet) {return otherSet < *this;};
    bool operator<=(UniqueSet<type> otherSet) {return *this < otherSet || *this == otherSet;};
    bool operator>=(UniqueSet<type> otherSet) {return *this > otherSet || *this == otherSet;};

    void clear() {
        Node *n = nullptr;
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            delete n;
            n = iter.getNode();
        }
        delete n;
        for (int i = 0; i < capacity; i++) {
            list[i] = nullptr;
        }
        size = 0;
    };

    template<typename... types>
    void addMultiple(type &value, types&... values) {add(*(&value)); addMultiple(values...);};
    void add(type &value) {
        size_t key = hash(value);
        type *val = &value;
        Node *newNode = new Node(val, key);
        addToList(newNode, key%capacity);
    };

    void remove(type &value) {
        size_t key = hash(value);
        type *val = &value;
        Node *node = list[key%capacity], *n = nullptr, *nodeToRemove = new Node(val, key);
        while (node != nullptr) {
            if (*node == *nodeToRemove) {
                if (n == nullptr) {
                    list[key%capacity] = nullptr;
                } else {
                    n->setNext(node->getNext());
                }
                delete node;
                delete nodeToRemove;
                size--;
                return;
            }
            n = node;
            node = node->getNext();
        }
        delete nodeToRemove;
        throw ValueNotFoundException();
    }

    bool contains(type &value) {
        size_t key = hash(value);
        type *val = &value;
        Node *node = list[key%capacity], *nodeToFind = new Node(val, key);
        while (node != nullptr) {
            if (*node == *nodeToFind) {
                delete nodeToFind;
                return true;
            }
            node = node->getNext();
        }
        delete nodeToFind;
        return false;
    };

    size_t getSize() {return size;}
    size_t getCapacity() {return capacity;}
    type *getList() {
        type *listToReturn = new type[size];
        int i = 0;
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            listToReturn[i] = *iter.getNode()->getDataPointer();
            i++;
        }
        return listToReturn;
    };

private:

    void addMultiple() {};

    size_t hash(type &value) { return reinterpret_cast<size_t>(&value);}; //pointer -> size_t

    void addToList(Node *nodeToAdd, const size_t &position) {
        Node *node = list[position];
        if (node == nullptr) {
            list[position] = nodeToAdd;
            size++;
            return;
        }
        if (*node == *nodeToAdd) {
            delete nodeToAdd;
            return;
        }
        while (node->getNext() != nullptr) {
            node = node->getNext();
            if (*node == *nodeToAdd) {
                delete nodeToAdd;
                return;
            }
        }
        node->setNext(nodeToAdd);
        size++;
        if (size/capacity >= RESIZE_AT) {
            resize(capacity*MULTIPLY_SIZE_BY);
        }
    };

    void resize(const int &newCapacity) {
        Node **oldList = list;
        list = new Node* [newCapacity] {nullptr};
        size_t oldCapacity = capacity;
        capacity = newCapacity;
        size = 0;
        Node *node = nullptr, *n = nullptr;
        for (int i = 0; i < oldCapacity; i++) {
            node = oldList[i];
            while (node != nullptr) {
                type* temp = node->getDataPointer();
                add(*temp);
                n = node;
                node = node->getNext();
                delete n;
            }
        }
    };
};