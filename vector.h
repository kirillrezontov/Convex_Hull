#ifndef VECTOR_H
#define VECTOR_H

#include <cstdlib>
#include <cstring>
#include <iostream>


class BadAlloc {
    const size_t size;
    const char* _func;
    public:
    BadAlloc(size_t size, const char* func):size(size), _func(func) {}
    void what() const { std::cerr << "BadAlloc: " << size << ' ' << _func << std::endl; }
};
class BadIndex {
    const size_t _i;
    const size_t _size;
    public:
    BadIndex(size_t i, size_t size) : _i(i), _size(size) {}
    void what() const { std::cerr << "BadIndex: " << _i << ' ' << _size << std::endl; }
};
class BadFile {
    const char* _filename;
    const char* _func;
    public:
    BadFile(const char* filename, const char* func):_filename(filename), _func(func) {}
    void what() const { std::cerr << "BadFile: " << _filename << ' ' << _func << std::endl; }
};

template <typename T> struct remove_reference {using type = T;};
template <typename T> struct remove_reference<T&> {using type = T;};
template <typename T> struct remove_reference<T&&> {using type = T;};


template <typename T>
typename remove_reference<T>::type&& move(T&& t) {
    return static_cast<typename remove_reference<T>::type&&>(t);
}

template <typename T>
class vector {
protected:
    T* _arr;
    size_t _size;
    size_t _capacity;
    void destroy_arr(T* p, size_t n){for (size_t i = 0; i < n; i++){p[i].~T();}};
public:
    using iterator = T*;
    using const_iterator = const T*;
    const_iterator begin() const { return _arr; }
    const_iterator end() const { return _arr+_size; }
    iterator begin(){return _arr;}
    iterator end(){return _arr+_size;}

    vector():_arr(nullptr), _size(0), _capacity(0) {}

    vector(const vector& v): _arr(nullptr), _size(0), _capacity(0) {
        reserve(v._capacity);
        for (size_t i=0; i<v._size; i++) {
            new (_arr+i) T(v._arr[i]);
            _size++;
        }
    }

    vector(vector&& v) noexcept :_arr(v._arr), _size(v._size), _capacity(v._capacity) {
        v._arr = nullptr; v._size = 0; v._capacity = 0;
    }

    explicit vector(size_t size): _arr(nullptr), _size(0), _capacity(0)  {
        reserve(size);
        for (size_t i=0; i<size; i++) {
            new (_arr+i) T();
            _size++;
        }
    }

    ~vector() {
        destroy_arr(_arr, _size);
        free(_arr);
    }

    size_t capacity() const { return _capacity; }

    size_t size() const { return _size; }

    bool empty() const { return _size == 0; }

    void reserve(size_t capacity) {
        if (_capacity >= capacity) { return; }
        size_t new_capacity = (_capacity)?_capacity:8;
        while (capacity > new_capacity) { new_capacity<<=1; }
        T* tmp = (T*)malloc(new_capacity * sizeof(T));
        if (!tmp) throw BadAlloc(capacity, __func__);
        for (size_t i=0; i<_size; i++) {
            new(tmp+i) T(move(_arr[i]));
            _arr[i].~T();
        }
        free(_arr);
        _arr = tmp;
        _capacity = new_capacity;
    }

    void resize(size_t size) {
        if (size < _size) { destroy_arr(_arr+size, _size-size); _size = size; }
        else if (size > _size) {
            reserve(size);
            for (size_t i = _size; i<size; i++) { new (_arr+i) T(); };
            _size = size;
        }
    }

    T& operator[](size_t i) {
        if (i >= _size)
            throw BadIndex(i, _size);
        return _arr[i];
    }

    const T& operator[](size_t i) const {
        if (i >= _size)
            throw BadIndex(i, _size);
        return _arr[i];
    }

    T& front() const { return _arr[0]; }

    T& back() const { return _arr[_size-1]; }

    void push_back(const T& x) {
        if (_size == _capacity) { reserve((_capacity)?_capacity*2:8); }
        new (_arr+_size) T(x);
        _size++;
    }

    void push_back(T&& x) {
        if (_size == _capacity) { reserve((_capacity)?_capacity*2:8); }
        new (_arr+_size) T(move(x));
        _size++;
    }

    void pop_back() {
        if (_size == 0) { return; }
        _arr[_size-1].~T();
        _size--;
    }

    void remove(iterator& pos) {
        if (pos >= _arr+_size || pos < _arr) return;
        pos->~T();
        for (iterator i = pos; i < _arr+_size-1; ) {*i = move(*(i+1));}
        _size--;
    }

    void remove(size_t pos) {
        if (pos >= _size || pos < 0) return;
        _arr[pos].~T();
        for (iterator i = _arr+pos; i < _arr+_size-1; ) {*i = move(*(i+1));}
        _size--;
    }

    vector& operator=(const vector& v) {
        if (this == &v) { return *this; }
        clear();
        reserve(v._size);
        for (size_t i=0; i<v._size; i++) {
            new(_arr+i) T(v._arr[i]);
            _size++;
        }
        return *this;
    }

    vector& operator=(vector&& v) noexcept {
        if (this == &v) { return *this; }
        clear();
        free(_arr);
        _arr = v._arr;
        _size = v.size();
        _capacity = v.capacity();
        v._arr = nullptr; v._size = 0; v._capacity = 0;
        return *this;
    }

    void clear() {
        destroy_arr(_arr, _size);
        _size = 0;
    }
};

class string : public vector<char>{
public:
    size_t size() const { return _size-1; }
    size_t len() const { return _size-1; }
    string(size_t size): vector<char>(size+1) {}
    string(const char* str) {
        size_t len = strlen(str);
        resize(len+1);
        strcpy(_arr, str);
    }
    string(const string& str) = default;
    string(string&& str) noexcept :vector<char>(move(str)) {}
    string():vector<char>(1) {
        _arr[0]='\0';
    }
    string& operator=(const char* str) {
        size_t len = strlen(str);
        resize(len+1);
        strcpy(_arr, str);
        return *this;
    }
    string& operator=(const string&) = default;
    string& operator=(string&&) noexcept = default;
    friend string operator+(const string& a, const string& b) {
        string res(a); res += b; return res;
    }
    friend string operator+(const string& a, const char* b) {
        string res(a); res += b; return res;
    }
    friend string operator+(const char* a, const string& b) {
        string res(a); res += b; return res;
    }
    string& operator+=(const string& str) {
        size_t len = str._size, size = _size-1;
        resize(size + len);
        strcpy(_arr+size, str._arr);
        return *this;
    }
    string& operator+=(string&& str) {
        size_t len = str._size, size = _size-1;
        resize(size + len);
        strcpy(_arr+size, str._arr);
        return *this;
    }
    string& operator+=(const char* str) {
        size_t len = strlen(str), size = _size-1;
        resize(size + len);
        strcpy(_arr+size, str);
        return *this;
    }
    string& operator+=(const char symbol) {
        back() = symbol;
        push_back('\0');
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const string& str) {
        for (size_t i=0; i<str._size-1; i++) {
            os << str[i];
        }
        return os;
    }
    const char* c_str() const { return _arr; }
};


#endif
