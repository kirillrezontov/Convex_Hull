#ifndef VECTOR_H
#define VECTOR_H

#include <cstdlib>
#include <cstring>


class BadAlloc{};
class BadIndex{};
class BadFile{};

template <class T>
class vector {
protected:
    T* _arr;
    size_t _size;
    size_t _capacity;
public:
    using iterator = T*;
    using const_iterator = const T*;
    const_iterator begin() const { return _arr; }
    const_iterator end() const { return _arr+_size; }
    iterator begin(){return _arr;}
    iterator end(){return _arr+_size;}
    vector() {
        _capacity = 8; _size = 0;
        _arr = (T*)malloc(_capacity * sizeof(T));
        if (!_arr) {
            throw BadAlloc();
        }
    }
    vector(const vector& v): _capacity(v._capacity), _size(v._size) {
        _arr = (T*)malloc(_capacity * sizeof(T));
        if (!_arr) {
            throw BadAlloc();
        }
        memcpy(_arr, v._arr, _size * sizeof(T));
    }
    vector(size_t size) {
        _size = size;
        _capacity = 1; int _cap = 8;
        while (size>0) {size>>=1; _capacity<<=1;}
        _capacity = (_capacity >= _cap) ? _capacity : _cap;
        _arr = (T*)calloc(sizeof(T), _capacity);
        if (!_arr) {
            throw BadAlloc();
        }
    }
    ~vector() {free(_arr);}
    size_t capacity() const { return _capacity; }
    size_t size() const { return _size; }
    bool empty() const { return _size == 0; }
    T& operator[](size_t i) {
        if (i >= _size)
            throw BadIndex();
        return _arr[i];
    }
    const T& operator[](size_t i) const {
        if (i >= _size)
            throw BadIndex();
        return _arr[i];
    }
    T& front() { return _arr[0]; }
    T& back() { return _arr[_size-1]; }
    bool push_back(const T& p) {
        if (_size == _capacity) {
            auto tmp = (T*)realloc(_arr, _capacity * sizeof(T) << 1);
            if (!tmp) {
                return false;
            }
            _capacity <<= 1;
            _arr = tmp;
        }
        _size++;
        _arr[_size-1] = p;
        return true;
    }
    bool pop_back() {
        if (_size == 0) {
            return false;
        }
        _size--;
        return true;
    }
    vector& operator=(const vector& v) {
        if (this == &v) { return *this; }
        auto tmp = (T*)malloc(v._capacity * sizeof(T));
        if (!tmp) {
            throw BadAlloc();
        }
        free(_arr);
        _arr = tmp;
        _size = v._size;
        _capacity = v._capacity;
        memcpy(_arr, v._arr, v._size * sizeof(T));
        return *this;
    }
};

#endif