//
// Created by kirillr on 12.09.2026.
//

#ifndef CONVEX_HULL_SET_H
#define CONVEX_HULL_SET_H
#include "vector.h"

const size_t FNV_32_PRIME = 0x01000193;

template <typename T>
size_t fnv_hash(const T* data) {
    char* buf = (char*)data;
    size_t hval = 0x811c9dc5;
    int size = sizeof(T);
    while (size) {
        hval *= FNV_32_PRIME;
        hval ^= (unsigned int)*buf++;
    }
    return hval;
}

template<typename T>
struct default_hash{
    size_t operator()(const T* data) const {
        return fnv_hash(data);
    }
};


template <typename T, typename hash_f>
class set {
    protected:
    size_t _size;
    hash_f _hash;
    vector<vector<T>> nodes;
    public:
    set(hash_f hash = default_hash<T>{}) : _size(0), _hash(hash) {}
    size_t size() const { return _size; }
    bool insert(const T& data) {
    }
};

#endif //CONVEX_HULL_SET_H
