#pragma once

#include <iostream>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <type_traits>
#include "Exceptions.h"

template<class type> class Set {
    const float RESIZE_AT = 0.75;
    const int MULTIPLY_SIZE_BY = 2;

    class Node {
        type data;
        size_t key;
        Node *next = nullptr;
    public:
        Node(type data, size_t key) : data(data), key(key) {};
        bool operator==(Node &node) {return key == node.key;};
        type getData() {return data;};
        size_t getKey() {return key;};
        Node *getNext() {return next;};
        void setNext(Node *node) {(next == nullptr) ? next=node : throw OccupiedSpaceException();};
    };
    size_t capacity = 0;
    size_t size = 0;
    Node **list; // list -> bucket
public:
    explicit Set(size_t startingCapacity = 10) {
        list = new Node* [startingCapacity] {nullptr};
        capacity = startingCapacity;
    };

    class Iterator {
        friend Set<type>;

        Node *node;
        size_t i;
        Set<type> set;
        Node *last;
        bool end = false;
    public:
        explicit Iterator(Set<type> set) : node(nullptr), i(0), set(set) {
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
                if (i >= (int) set.capacity) {
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

    Set<type> setUnion(Set<type> otherSet) {
        Set<type> newSet {otherSet};
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            newSet.add(iter.getData());
        }
        return newSet;
    };

    Set<type> setIntersection(Set<type> otherSet) {
        Set<type> newSet;
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            if (otherSet.contains(iter.getData())) {
                newSet.add(iter.getData());
            }
        }
        return newSet;
    };

    bool operator==(Set<type> otherSet) {
        if (size != otherSet.getSize()) {
            return false;
        }
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            if (!otherSet.contains(iter.getData())) {
                return false;
            }
        }
        return true;
    };
    bool operator<(Set<type> otherSet) {
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            if (!otherSet.contains(iter.getData())) {
                return false;
            }
        }
        if (size >= otherSet.getSize()) {
            return false;
        }
        return true;
    };
    bool operator>(Set<type> otherSet) {return otherSet < *this;};
    bool operator<=(Set<type> otherSet) {return *this < otherSet || *this == otherSet;};
    bool operator>=(Set<type> otherSet) {return *this > otherSet || *this == otherSet;};

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
    void addMultiple(type value, types... values) {add(value); addMultiple(values...);};
    void add(type value) { add(value, hash(value)); };
    void add(type value, int key) {
        Node *newNode = new Node(value, key);
        addToList(newNode, key%capacity);
    };

    void remove(type value) { remove(value, hash(value)); };
    void remove(type value, int key) {
        Node *node = list[key%capacity], *n = nullptr, *nodeToRemove = new Node(value, key);
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

    bool contains(type value) { return contains(value, hash(value)); };
    bool contains(type value, int key) {
        Node *node = list[key%capacity], *nodeToFind = new Node(value, key);
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
        int j = 0;
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            listToReturn[j] = iter.getData();
            j++;
        }
        return listToReturn;
    };

private:

    void addMultiple() {};

    template <typename Integer,
            std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
    size_t hash(Integer &key) { return key; };
    template <typename Floating,
            std::enable_if_t<std::is_floating_point<Floating>::value, bool> = true>
    size_t hash(Floating &key) {
        size_t result = 0;
        memcpy(&result, &key, sizeof( double ));
        return result & 0xfffff000;
    };
    size_t hash(const char* key) {
        unsigned h = 0;
        while (*key) {
            h = h * 101 + (unsigned) *key++;
        }
        return h;
    };

    size_t hash(const std::string key) {
        unsigned h = 0;
        const char *a = key.c_str();
        while (*a) {
            h = h * 101 + (unsigned) *a++;
        }
        return h;
    };

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
                add(node->getData(), node->getKey());
                n = node;
                node = node->getNext();
                delete n;
            }
        }
    };
};