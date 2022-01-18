#pragma once

#include <iostream>
#include <cstring>
#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <tuple>
#include <cassert>
#include "Exceptions.h"


template<class... types> class CombinedSet {
    const float RESIZE_AT = 0.75;
    const int MULTIPLY_SIZE_BY = 2;

    class Type {
        size_t index;
    public:
        Type(size_t i) : index(i) {};
        size_t getTypeId() {return index;};
    };

    template<typename T> class Node {
        T data;
        size_t key;
    public:
        Node(T data, size_t key) : data(data), key(key) {};
        bool operator==(Node &node) {return key == node.key;};
        T getData() {return data;};
        size_t getKey() {return key;};
    };

    class Wrap {
        void *node = nullptr;
        Type *type = nullptr;
        Wrap *next = nullptr;
    public:
        Wrap(void* n, Type *t) : node(n), type(t) {};
        ~Wrap() {
            const size_t i = type->getTypeId();
            deleteThem<typeArraySize>(i);
        }

        bool operator==(Wrap &other) {
            if (type->getTypeId() != other.getType()->getTypeId()) {
                return false;
            }
            const size_t i = type->getTypeId();
            return equal<typeArraySize>(i, other);
        }
        void *getNode() {return node;};
        Type *getType() {return type;};
        Wrap *getNext() {return next;};
        template<typename T>
        T getData() {
            if (!checkType<T, typeArraySize>(type->getTypeId())) {
                throw WrongTypeException();
            }
            Node<T> *n = static_cast<Node<T>*>(node);
            return n->getData();
        }
        size_t getKey() {return getNodeKey<typeArraySize>(type->getTypeId());};

        void setNext(Wrap *n) {(next == nullptr) ? next=n : throw OccupiedSpaceException();};


    private:
        template<size_t I, std::enable_if_t<0<I, bool> = true>
        size_t getNodeKey(size_t i) {
            if (I > typeArraySize || I-1 < i) {
                throw TypeNotAcceptedException();
            }
            if (I-1 > i) {
                return getNodeKey<I-1>(i);;
            }
            using t = typename std::tuple_element<I-1, typeArray>::type;
            Node<t> *n = static_cast<Node<t>*>(node);
            return n->getKey();
        }

        template<size_t I, std::enable_if_t<0==I, bool> = true>
        size_t getNodeKey(size_t i) {return -1;} // <- this function should never fire, only required for compilation

        template<typename type, size_t I, std::enable_if_t<0<I, bool> = true>
        bool checkType(size_t i) {
            if (I > typeArraySize || I-1 < i) {
                throw TypeNotAcceptedException();
            }
            if (I-1 > i) {
                return checkType<type, I-1>(i);
            }
            using t = typename std::tuple_element<I-1, typeArray>::type;
            return typeid(type) == typeid(t);
        }

        template<typename type, size_t I, std::enable_if_t<0==I, bool> = true>
        bool checkType(size_t i) {return false;}

        template<size_t I, std::enable_if_t<0<I, bool> = true>
        void deleteThem(size_t i) {
            if (I > typeArraySize || I-1 < i) {
                throw TypeNotAcceptedException();
            }
            if (I-1 > i) {
                deleteThem<I-1>(i);
                return;
            }
            using t = typename std::tuple_element<I-1, typeArray>::type;
            Node<t> *n = static_cast<Node<t>*>(node);
            delete n;
            delete type;
        }

        template<size_t I, std::enable_if_t<0==I, bool> = true>
        void deleteThem(size_t i) {}

        template<size_t I, std::enable_if_t<0<I, bool> = true>
        bool equal(size_t i, Wrap &other) {
            if (I > typeArraySize || I-1 < i) {
                throw TypeNotAcceptedException();
            }
            if (I-1 > i) {
                return equal<I-1>(i, other);
            }
            using t = typename std::tuple_element<I-1, typeArray>::type;
            Node<t> *n1 = static_cast<Node<t>*>(node), *n2 = static_cast<Node<t>*>(other.getNode());
            return *n1 == *n2;
        }

        template<size_t I, std::enable_if_t<0==I, bool> = true>
        bool equal(size_t i, Wrap &other) {
            return false;
        }
    };

    using typeArray = std::tuple<types...>;
    static constexpr size_t typeArraySize = sizeof...(types);
    size_t capacity = 0;
    size_t size = 0;
    Wrap **list;
public:
    explicit CombinedSet(size_t startingCapacity = 10) {
        list = new Wrap*[startingCapacity] {nullptr};
        capacity = startingCapacity;
    };

    class Iterator {
        friend CombinedSet<types...>;

        Wrap *wrapper;
        size_t i;
        CombinedSet<types...> set;
        Wrap *last;
        bool end = false;
    public:
        explicit Iterator(CombinedSet<types...> set) : wrapper(nullptr), i(0), set(set) {
            Wrap* w = nullptr;
            for (int j = 0; j < set.capacity; j++) {
                if (set.list[j] != nullptr) {
                    w = set.list[j];
                    if (wrapper == nullptr) {
                        i = j;
                        wrapper = set.list[j];
                    }
                }
            }
            if (w != nullptr) {
                while (w->getNext() != nullptr) {
                    w = w->getNext();
                }
            } else {
                end = true;
            }
            last = w;
        }

        void operator++() {
            if (i >= (int) set.capacity || i < 0) {
                throw IndexOutOfRangeException();
            }
            if (wrapper == nullptr) {
                i++;
                if (i >= set.capacity) {
                    return;
                }
                wrapper = set.list[i];
            } else {
                wrapper = wrapper->getNext();
            }
            if (wrapper == nullptr) {
                ++(*this);
            }
        };

        template<typename type>
        type getData() {return (wrapper != nullptr) ? wrapper->template getData<type>() : throw EmptySetException();}

        bool finished() {
            if (end) {
                return false;
            }
            if (wrapper == last) {
                end = true;
            }
            return true;
        };

    private:
        Wrap* getWrapper() {return wrapper;}
    };

    friend Iterator;
    Iterator getIterator() {
        return Iterator(*this);
    }

    CombinedSet<types...> setUnion(CombinedSet<types...> otherSet) {
        CombinedSet<types...> newSet {otherSet};
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            newSet.CopyNodeValueAndAddIt<newSet.typeArraySize>(iter.getWrapper()->getType()->getTypeId(), iter.getWrapper()->getNode());
        }
        return newSet;
    };

    CombinedSet<types...> setIntersection(CombinedSet<types...> otherSet) {
        CombinedSet<types...> newSet;
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            if (otherSet.containsWrapper(iter.getWrapper())) {
                newSet.CopyNodeValueAndAddIt<newSet.typeArraySize>(iter.getWrapper()->getType()->getTypeId(), iter.getWrapper()->getNode());
            }
        }
        return newSet;
    };

    bool operator==(CombinedSet<types...> otherSet) {
        if (size != otherSet.getSize()) {
            return false;
        }
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            if (!otherSet.containsWrapper(iter.getWrapper())) {
                return false;
            }
        }
        return true;
    };
    bool operator<(CombinedSet<types...> otherSet) {
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            if (!otherSet.containsWrapper(iter.getWrapper())) {
                return false;
            }
        }
        if (size >= otherSet.getSize()) {
            return false;
        }
        return true;
    };
    bool operator>(CombinedSet<types...> otherSet) {return otherSet < *this;};
    bool operator<=(CombinedSet<types...> otherSet) {return *this < otherSet || *this == otherSet;};
    bool operator>=(CombinedSet<types...> otherSet) {return *this > otherSet || *this == otherSet;};

    void clear() {
        Wrap *w = nullptr;
        for (auto iter = getIterator(); iter.finished(); ++iter) {
            delete w;
            w = iter.getWrapper();
        }
        delete w;
        for (int i = 0; i < capacity; i++) {
            list[i] = nullptr;
        }
        size = 0;
    };

    template<typename type,typename... otherTypes>
    void addMultiple(type value, otherTypes... values) {add(value); addMultiple(values...);};
    template<typename type>
    void add(type value) { add<type>(value, hash(value)); };
    template<typename type>
    void add(type value, int key) {
        Type *newType = new Type(getTypeId<type>());
        Node<type> *newNode = new Node<type>(value, key);
        Wrap *wrapper = new Wrap(newNode, newType);
        addToList(wrapper, key%capacity);
    };

    template<typename type>
    void remove(type value) { remove(value, hash(value)); };
    template<typename type>
    void remove(type value, int key) {
        Type *t = new Type(getTypeId<type>());
        Node<type> *n = new Node<type>(value, key);
        Wrap *wrapper = list[key%capacity], *w = nullptr, *wrapperToRemove = new Wrap(n, t);
        while (wrapper != nullptr) {
            if (*wrapper == *wrapperToRemove) {
                if (w == nullptr) {
                    list[key%capacity] = nullptr;
                } else {
                    w->setNext(wrapper->getNext());
                }
                delete wrapper;
                delete wrapperToRemove;
                size--;
                return;
            }
            w = wrapper;
            wrapper = wrapper->getNext();
        }
        delete wrapperToRemove;
        throw ValueNotFoundException();
    }

    template<typename type>
    bool contains(type value) { return contains(value, hash(value)); };
    template<typename type>
    bool contains(type value, int key) {
        Type *t = new Type(getTypeId<type>());
        Node<type> *n = new Node<type>(value, key);
        Wrap *wrapper = list[key%capacity], *wrapperToFind = new Wrap(n, t);
        while (wrapper != nullptr) {
            if (*wrapper == *wrapperToFind) {
                delete wrapperToFind;
                return true;
            }
            wrapper = wrapper->getNext();
        }
        delete wrapper;
        return false;
    };

    size_t getSize() {return size;}
    size_t getCapacity() {return capacity;}

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

    template<typename typeToFind, size_t I = 0, std::enable_if_t<I < typeArraySize, bool> = true>
    size_t getTypeId() {
        using t = typename std::tuple_element<I, typeArray>::type;
        if (typeid(typeToFind) == typeid(t)) {
            return I;
        }
        return getTypeId<typeToFind, I+1>();
    };

    template<typename typeToFind, size_t I = 0, std::enable_if_t<typeArraySize <= I, bool> = true>
    size_t getTypeId() {
        throw TypeNotAcceptedException();
    };

    void addToList(Wrap *wrapperToAdd, const size_t &position) {
        Wrap *wrapper = list[position];
        if (wrapper == nullptr) {
            list[position] = wrapperToAdd;
            size++;
            return;
        }
        if (*wrapper == *wrapperToAdd) {
            delete wrapperToAdd;
            return;
        }
        while (wrapper->getNext() != nullptr) {
            wrapper = wrapper->getNext();
            if (*wrapper == *wrapperToAdd) {
                delete wrapperToAdd;
                return;
            }
        }
        wrapper->setNext(wrapperToAdd);
        size++;
        if (size/capacity >= RESIZE_AT) {
            resize(capacity*MULTIPLY_SIZE_BY);
        }
    };

    void resize(const int &newCapacity) {
        Wrap **oldList = list;
        list = new Wrap* [newCapacity] {nullptr};
        size_t oldCapacity = capacity;
        capacity = newCapacity;
        size = 0;
        Wrap *wrapper = nullptr, *w = nullptr;
        for (int i = 0; i < oldCapacity; i++) {
            wrapper = oldList[i];
            while (wrapper != nullptr) {
//                wrapper->template getINode<typeArraySize, resizeFunctor>(wrapper->getType()->getTypeId()); -> add<t>(node->getData(), node->getKey());
                CopyNodeValueAndAddIt<typeArraySize>(wrapper->getType()->getTypeId(), wrapper->getNode());
                w = wrapper;
                wrapper = wrapper->getNext();
                delete w;
            }
        }
    };

    bool containsWrapper(Wrap *wrapperToFind) {
        Wrap *wrapper = list[wrapperToFind->getKey()%capacity];
        while (wrapper != nullptr) {
            if (*wrapper == *wrapperToFind) {
                return true;
            }
            wrapper = wrapper->getNext();
        }
        return false;
    }

    template<size_t I, std::enable_if_t<0<I, bool> = true>
    void CopyNodeValueAndAddIt(size_t i, void *node) {
        if (I > typeArraySize || I-1 < i) {
            throw TypeNotAcceptedException();
        }
        if (I-1 > i) {
            return CopyNodeValueAndAddIt<I-1>(i, node);
        }
        using t = typename std::tuple_element<I-1, typeArray>::type;
        Node<t> *n = static_cast<Node<t>*>(node);
        add<t>(n->getData(), n->getKey());
    };

    template<size_t I, std::enable_if_t<0==I, bool> = true>
    void CopyNodeValueAndAddIt(size_t i, void *node) {};

//    template<typename T>
//    class resizeFunctor {
//        bool operator()(Node<T> *node) {
//            add<T>(node->getData(), node->getKey());
//            return true;
//        }
//    };
};