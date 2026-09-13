//
// Created by kirillr on 11.09.2026.
//

#ifndef PRACTICUM5_ALGORYTHM_H
#define PRACTICUM5_ALGORYTHM_H
#include <ctime>
#include <cstdlib>
#include "vector.h"


static bool seeded = []{srand(time(nullptr));return true;}();

template <typename T>
struct less{
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

template <typename T>
void swap(T& a, T& b) {
    T c = a; a = b; b = c;
}

template <typename T, typename Compare>
T* partition(T* begin, T* end, Compare comp) {
    T pivot = *(begin + std::rand()%(end - begin));
    T* left = begin, * right = end-1;
    while (true) {
        while (comp(*left, pivot)) {
            left++;
        }
        while (comp(pivot, *right)) {
            right--;
        }
        if (left >= right) {
            return right;
        }
        swap(*left, *right);
        left++;
        right--;
    }
    return nullptr;
}

template <typename T, typename Compare = less<T>>
void sort(T* begin, T* end, Compare comp = Compare()) {
    if (end - begin <= 1) {return;}
    T* p = partition(begin, end, comp);
    sort(begin, p+1, comp);
    sort(p+1, end, comp);
}
template <typename T>
void reverse(T* begin, T* end) {
    end--;
    while (begin < end) {
        swap(*begin, *end);
        begin++; end--;
    }
}

inline string to_string(size_t i) {
    string res;
    while (i>9) {
        res+=(char)((i%10)+'0'); i/=10;
    }
    res+=(char)(i+'0');
    reverse(res.begin(), res.end());
    return res;
}

#endif