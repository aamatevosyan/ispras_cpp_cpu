//
// Created by armen on 1/23/21.
//

#ifndef ISPRAS_CPP_CPU_MSTACK_H
#define ISPRAS_CPP_CPU_MSTACK_H

#define MAX_SIZE 4096

#include <stdexcept>
#include <sstream>
#include <functional>
#include "custom_exceptions.hpp"

using namespace std;

constexpr unsigned long HASH = 5381;

template<class T>
class mstack {
    T *arr;
    int top;
    int capacity;
    unsigned long lastHash;
    function<unsigned long(T)> _hash;

public:
    mstack(function<unsigned long(T)> _hash = nullptr, int size = MAX_SIZE);    // constructor

    void push(T);

    T pop();

    T peek();

    int size();

    bool isEmpty();

    bool isFull();

    string dump();

    unsigned long getHash();

    bool hashChanged();

    void reset();

    ~mstack() {
        delete[] arr;
    }
};

template<class T>
mstack<T>::mstack(function<unsigned long(T)> _hash, int size) {
    arr = new T[size + 2]; // using dummy objects for bounds
    capacity = size;
    top = 0;
    this->_hash = _hash;
    lastHash = getHash();
}

template<class T>
void mstack<T>::push(T x) {
    if (isFull())
        throw stack_exception("Stack OverFlow");

    arr[++top] = x;
}

template<class T>
T mstack<T>::pop() {
    peek();
    return arr[top--];
}

template<class T>
T mstack<T>::peek() {
    if (isEmpty())
        throw stack_exception("Stack UnderFlow");

    return arr[top];
}

template<class T>
int mstack<T>::size() {
    return top;
}

template<class T>
bool mstack<T>::isEmpty() {
    return top == 0;
}

template<class T>
bool mstack<T>::isFull() {
    return top == capacity;
}

template<class T>
string mstack<T>::dump() {
    void *addr = this;
    ostringstream oss;
    oss << "Stack [" << addr << "]\n";

    if (size() > 0) {

        oss << "{\n";

        oss << "\tdata [" << size() << "] = " << arr << "\n";
        oss << "\t{\n";

        for (int i = 1; i <= size(); i++)
            oss << "\t\t[" << i - 1 << "] = " << arr[i] << "\n";

        oss << "\t}\n";

        oss << "}\n";
    } else {
        oss << "Empty stack.\n";
    }
    return oss.str();
}

template<typename T>
unsigned long mstack<T>::getHash() {
    if (_hash == nullptr) {
        return 0;
    }

    unsigned long res = HASH;

    for (int i = 0; i <= capacity + 1; i++)
        res = ((res << 5) + res) + _hash(arr[i]);

    return res;
}

template<typename T>
bool mstack<T>::hashChanged() {
    unsigned long currentHash = getHash();
    unsigned long tmp = lastHash;

    lastHash = currentHash;
    return currentHash != tmp;
}

template<class T>
void mstack<T>::reset() {
    top = 0;
}

#endif //ISPRAS_CPP_CPU_MSTACK_H
