#pragma once

#include <iostream>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <type_traits>
#include "Exceptions.h"

template<class type> class OrderedSet {

    class Node {
        type data;
        size_t key, size;
        Node *left = nullptr, *right = nullptr;
    public:
        Node(type data, size_t key) : data(data), key(key), size(1) {};
        bool operator==(Node &node) {return key == node.key;};
        bool operator<(Node &node) {return key < node.key;};
        void operator++() {size++;};
        void operator--() {size--;};
        type getData() {return data;};
        size_t getKey() {return key;};
        size_t getSize() {return size;};
        Node *getLeft() {return left;};
        Node *getRight() {return right;};
        void setLeft(Node *node) {left=node;};
        void setRight(Node *node) {right=node;};
        void setSize(size_t s) {size=s;};
    };
    Node *root;
public:
    explicit OrderedSet() : root(nullptr) {};

    class Iterator {
        friend OrderedSet<type>;

        size_t i;
        OrderedSet<type> set;
        Node **list;
        size_t size;

    public:
        explicit Iterator(OrderedSet<type> set) : i(0), set(set), size(set.getSize()) {
            list = new Node*[set.getSize()];
            rek(set.root);
            i = 0;
        }
        ~Iterator() {
            delete list;
        }

        void rek(Node *node) {
            if (node != nullptr) {
                rek(node->getLeft());
                list[i] = node;
                i++;
                rek(node->getRight());
            }
        };

        void operator++() {
            if (i < 0 || i >= size) {
                throw IndexOutOfRangeException();
            }
            i++;
        };

        type getData() {return list[i]->getData();}
        bool finished() {return i < size;};

    private:
        Node* getNode() {return list[i];}
    };

    friend Iterator;
    Iterator getIterator() {
        return Iterator(*this);
    }

    OrderedSet<type> setUnion(OrderedSet<type> otherSet) {
        OrderedSet<type> newSet {otherSet};
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            newSet.add(iter.getData());
        }
        return newSet;
    };

    OrderedSet<type> setIntersection(OrderedSet<type> otherSet) {
        OrderedSet<type> newSet;
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            if (otherSet.contains(iter.getData())) {
                newSet.add(iter.getData());
            }
        }
        return newSet;
    };

    bool operator==(OrderedSet<type> otherSet) {
        if (getSize() != otherSet.getSize()) {
            return false;
        }
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            if (!otherSet.contains(iter.getData())) {
                return false;
            }
        }
        return true;
    };
    bool operator<(OrderedSet<type> otherSet) {
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            if (!otherSet.contains(iter.getData())) {
                return false;
            }
        }
        if (getSize() >= otherSet.getSize()) {
            return false;
        }
        return true;
    };
    bool operator>(OrderedSet<type> otherSet) {return otherSet < *this;};
    bool operator<=(OrderedSet<type> otherSet) {return *this < otherSet || *this == otherSet;};
    bool operator>=(OrderedSet<type> otherSet) {return *this > otherSet || *this == otherSet;};

    void clear() {
        Node *n = nullptr;
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            delete n;
            n = iter.getNode();
        }
        delete n;
        root = nullptr;
    };

    template<typename... types>
    void addMultiple(type value, types... values) {add(value); addMultiple(values...);};
    void add(type value) { add(value, hash(value)); };
    void add(type value, int key) {
        if (!contains(value, key)) {
            Node *newNode = new Node(value, key);
            addToList(newNode);
        }
    };

    void remove(type value) { remove(value, hash(value)); };
    void remove(type value, int key) {  // REFACTOR: this looks too complicated
        if (!contains(value, key)) {
            throw ValueNotFoundException();
        }
        Node *node = root, *n = nullptr, *nodeToRemove = new Node(value, key);
        while (node != nullptr) {
            --(*node);
            if (*node == *nodeToRemove) {
                Node* newSubtree;
                if (node->getLeft() == nullptr && node->getRight() == nullptr) {
                    newSubtree = nullptr;
                } else if (node->getLeft() == nullptr) {
                    newSubtree = node->getRight();
                } else if (node->getRight() == nullptr) {
                    newSubtree = node->getLeft();
                } else {
                    if (node->getLeft()->getSize() < node->getRight()->getSize()) {
                        getMax(node->getLeft(), node->getRight()->getSize())->setRight(node->getRight());;
                        newSubtree = node->getLeft();
                    } else {
                        getMin(node->getRight(), node->getRight()->getSize())->setLeft(node->getLeft());
                        newSubtree = node->getRight();
                    }
                }
                if (n == nullptr) {
                    root = newSubtree;
                } else if (n->getLeft() == node) {
                    n->setLeft(newSubtree);
                } else {
                    n->setRight(newSubtree);
                }
                delete node;
                delete nodeToRemove;
                return;
            }
            n = node;
            if (*nodeToRemove < *node) {
                node = node->getLeft();
            } else {
                node = node->getRight();
            }
        }
        delete nodeToRemove;
        throw ValueNotFoundException();
    };

    bool contains(type value) { return contains(value, hash(value)); };
    bool contains(type value, int key) {
        Node *node = root, *nodeToFind = new Node(value, key);
        while (node != nullptr) {
            if (*node == *nodeToFind) {
                delete nodeToFind;
                return true;
            }
            if (*nodeToFind < *node) {
                node = node->getLeft();
            } else {
                node = node->getRight();
            }
        }
        delete nodeToFind;
        return false;
    };

    size_t getSize() {return (root == nullptr) ? 0 : root->getSize();}
    type min() {
        if (root == nullptr) {
            throw EmptySetException();
        }
        Node *node = root;
        while (node->getLeft() != nullptr) {
            node = node->getLeft();
        }
        return node->getData();
    };
    type max() {
        if (root == nullptr) {
            throw EmptySetException();
        }
        Node *node = root;
        while (node->getRight() != nullptr) {
            node = node->getRight();
        }
        return node->getData();
    };
    type *getSortedList() {
        type *listToReturn = new type[getSize()];
        int j = 0;
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            listToReturn[j] = iter.getData();
            j++;
        }
        return listToReturn;
    };

    type getItem(size_t index) {
        if (root == nullptr) {
            throw EmptySetException();
        }
        int skipped = 0;
        if (root->getLeft() != nullptr) {
            skipped = root->getLeft()->getSize();
        }
        return getItemRek(skipped, root, index);
    };
    size_t getIndex(type value) {return getIndex(value, hash(value));}
    size_t getIndex(type value, int key) {
        Node *nodeToFind = new Node(value, key);
        size_t i = getIndexRek(0, root, nodeToFind);
        delete nodeToFind;
        return i;
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

    void addToList(Node *nodeToAdd) {
        if (root == nullptr) {
            root = nodeToAdd;
            return;
        }
        Node *node = root;
        while (node != nodeToAdd) {
            ++(*node);
            if (*nodeToAdd < *node) {
                if (node->getLeft() == nullptr) {
                    node->setLeft(nodeToAdd);
                    return;
                }
                node = node->getLeft();
            } else {
                if (node->getRight() == nullptr) {
                    node->setRight(nodeToAdd);
                    return;
                }
                node = node->getRight();
            }
        }
        throw SomethingWentBadException(); // this should not happen
    };

    Node *getMin(Node* node, size_t sizeToAdd) {
        while(node->getLeft() != nullptr) {
            node->setSize(node->getSize()+sizeToAdd);
            node = node->getLeft();
        }
        return node;
    }

    Node *getMax(Node* node, size_t sizeToAdd) {
        while(node->getRight() != nullptr) {
            node->setSize(node->getSize()+sizeToAdd);
            node = node->getRight();
        }
        return node;
    }

    type getItemRek(size_t i, Node *node, size_t index) {
        if (i == index) {
            return node->getData();
        }
        if (node->getLeft() != nullptr && i > index) {
            int skipped = 0;
            if (node->getLeft()->getRight() != nullptr) {
                skipped = node->getLeft()->getRight()->getSize();
            }
            return getItemRek(i-1-skipped, node->getLeft(), index);
        }
        if (node->getRight() != nullptr) {
            int skipped = 0;
            if (node->getRight()->getLeft() != nullptr) {
                skipped = node->getRight()->getLeft()->getSize();
            }
            return getItemRek(i+1+skipped, node->getRight(), index);
        }
        throw IndexOutOfRangeException();
    }

    size_t getIndexRek(size_t i, Node *node, Node *nodeToFind) {
        if (node == nullptr) {
            throw EmptySetException();
        }
        int skipped;
        if (*node == *nodeToFind) {
            skipped = 0;
            if (node->getLeft() != nullptr) {
                skipped = node->getLeft()->getSize();
            }
            return i+skipped;
        }
        if (*nodeToFind < *node) {
            return getIndexRek(i, node->getLeft(), nodeToFind);
        }
        skipped = 0;
        if (node->getLeft() != nullptr) {
            skipped = node->getLeft()->getSize();
        }
        return getIndexRek(i+skipped+1, node->getRight(), nodeToFind);
    }
};