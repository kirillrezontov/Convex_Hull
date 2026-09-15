//
// Created by kirillr on 12.09.2026.
//

#ifndef CONVEX_HULL_SET_H
#define CONVEX_HULL_SET_H
#include "vector.h"
#include "algorythm.h"
#include <cmath>

template <typename T>
struct set_traits {
    static bool equal(const T& a, const T& b) {
        return a == b;
    }
    static constexpr int64_t lookup_hnum = 1;
    static size_t hash(const T& data) {
        auto buf = (char*)&data;
        size_t hval = 1469598103934665603ULL;
        for (int64_t i = 0; i < sizeof(T); ++i) {
            hval ^= buf[i];
            hval *= 1099511628211ULL;
        }
        return hval;
    }
    static int64_t lookup(const T& data, size_t* out) {
        out[0] = hash(data);
        return 1;
    }
};

template<>
struct set_traits<double> {
    static constexpr double epsilon = 0.0000001;
    static constexpr double step = 2*epsilon;
    static bool equal(const double& a, const double& b) {
        return fabs(a-b)<epsilon;
    }
    static constexpr int64_t lookup_hnum = 3;
    static size_t hash(const double& data) {
        int64_t e_data = floor(data/step+0.5);
        auto buf = (char*)&e_data; size_t hval = 1469598103934665603ULL;
        for (int64_t i = 0; i < sizeof(double); ++i) {
            hval ^= buf[i];
            hval *= 1099511628211ULL;
        }
        return hval;
    }
    static int64_t lookup(const double& data, size_t* out) {
        out[0] = hash(data-epsilon);
        out[1] = hash(data);
        out[2] = hash(data+epsilon);
        return 3;
    }
};

template <typename T>
class set {
    protected:
    int64_t _size, _capacity;
    vector<vector<T>> buckets;
    void rehash(int64_t new_cap) {
        if (new_cap <= _capacity) { return; }
        auto old = move(buckets);
        int64_t cap = 1; while (cap < new_cap) cap <<= 1;
        buckets = vector<vector<T>>(cap);
        for (int64_t i = 0; i < old.size(); ++i) {
            for (const auto& e : old[i]) {
                int64_t h = set_traits<T>::hash(e);
                buckets[h%cap].push_back(e);
            }
        }
        _capacity = new_cap;
    }
    public:
    class iterator {
        friend class set;
    protected:
        vector<T>::iterator ptr; vector<vector<T>>::iterator vptr;
        set& ownr;
        iterator(T* p,vector<T>* vp, set& owner):ptr(p), vptr(vp), ownr(owner) {}
    public:
        iterator(const iterator& other): ptr(other.ptr), vptr(other.vptr), ownr(other.ownr) {}
        iterator& operator=(const iterator& other) {
            if (ownr != other.ownr) throw BadIndex(ownr._size, ownr._size);
            ptr = other.ptr; vptr = other.vptr;
        }
        bool operator<(const iterator& other) const {
            return vptr<other.vptr || (vptr == other.vptr && ptr < other.ptr);
        }
        iterator operator++() {
            if (ptr == ownr.iend) throw BadIndex(ownr._size, ownr._size);
            iterator iter = *this; ++ptr;
            while (ptr != ownr.iend && ptr == vptr->end()) {
                vptr++; ptr=vptr->begin();
            }
            return iter;
        }
        iterator& operator++(int) {
            if (ptr == ownr.iend) throw BadIndex(ownr._size, ownr._size);
            ++ptr;
            while (ptr != ownr.iend && ptr == vptr->end()) {
                vptr++; ptr=vptr->begin();
            }
            return *this;
        }
        iterator operator--() {
            iterator iter = *this;
            while (ptr != ownr.ibegin && ptr == vptr->begin()) {
                vptr--; ptr=vptr->end();
            }
            if (ptr == ownr.ibegin) throw BadIndex(-1, ownr._size);
            ptr--; return iter;
        }
        iterator& operator--(int) {
            while (ptr != ownr.ibegin && ptr == vptr->begin()) {
                vptr--; ptr=vptr->end();
            }
            if (ptr == ownr.ibegin) throw BadIndex(-1, ownr._size);
            ptr--; return *this;
        }
        bool operator==(const iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const iterator& other) const {
            return ptr != other.ptr;
        }
        bool operator==(const vector<T>::iterator& other) const {
            return ptr == other;
        }
        bool operator!=(const vector<T>::iterator& other) const {
            return ptr != other;
        }
    };
    class const_iterator {
        friend class set;
    protected:
        vector<T>::const_iterator ptr; vector<vector<T>>::const_iterator vptr;
        set& ownr;
        const_iterator(const T* p, const vector<T>* vp, set& owner):ptr(p), vptr(vp), ownr(owner) {}
        const_iterator& operator=(const iterator& other) {
            if (ownr != other.ownr) throw BadIndex(ownr._size, ownr._size);
            ptr = other.ptr; vptr = other.vptr;
        }
    public:
        const_iterator(const const_iterator& other): ptr(other.ptr), vptr(other.vptr), ownr(other.ownr) {}
        const_iterator& operator=(const const_iterator& other) {
            if (ownr != other.ownr) throw BadIndex(ownr._size, ownr._size);
            ptr = other.ptr; vptr = other.vptr;
        }
        bool operator<(const const_iterator& other) const {
            return vptr<other.vptr || (vptr == other.vptr && ptr < other.ptr);
        }
        const_iterator operator++() {
            if (ptr == ownr.cend) throw BadIndex(ownr._size, ownr._size);
            const_iterator iter = *this; ++ptr;
            while (ptr != ownr.cend && ptr == vptr->end()) {
                vptr++; ptr=vptr->begin();
            }
            return iter;
        }
        const_iterator& operator++(int) {
            if (ptr == ownr.cend) throw BadIndex(ownr._size, ownr._size);
            ++ptr;
            while (ptr != ownr.cend && ptr == vptr->end()) {
                vptr++; ptr=vptr->begin();
            }
            return *this;
        }
        const_iterator operator--() {
            iterator iter = *this;
            while (ptr != ownr.cbegin && ptr == vptr->begin()) {
                vptr--; ptr=vptr->end();
            }
            if (ptr == ownr.cbegin) throw BadIndex(-1, ownr._size);
            ptr--; return iter;
        }
        const_iterator& operator--(int) {
            while (ptr != ownr.cbegin && ptr == vptr->begin()) {
                vptr--; ptr=vptr->end();
            }
            if (ptr == ownr.cbegin) throw BadIndex(-1, ownr._size);
            ptr--; return *this;
        }
        bool operator==(const const_iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const const_iterator& other) const {
            return ptr != other.ptr;
        }
        bool operator==(const vector<T>::const_iterator& other) const {
            return ptr == other;
        }
        bool operator!=(const vector<T>::const_iterator& other) const {
            return ptr != other;
        }
    };
protected:
    iterator ibegin;
    iterator iend;
    const_iterator cbegin;
    const_iterator cend;
public:
    const_iterator begin() const {return cbegin;}
    const_iterator end() const {return cend;}
    iterator begin() {return ibegin();}
    iterator end() {return iend;}
    int64_t size() const { return _size; }
    int64_t bucket_capacity() const { return _capacity; }
    bool empty() const { return _size == 0; }
    set(int64_t bucket_cap = 16): _size(0), _capacity(0) {
        bucket_cap = bucket_cap < 8 ? 8 : bucket_cap;
        int64_t cap = 1; while (cap < bucket_cap) cap <<= 1;
        _capacity = cap;
        buckets = vector<vector<T>>(cap);
        ibegin = iterator{buckets.front().begin(), buckets.front(), *this};
        cbegin = const_iterator{buckets.front().begin(), buckets.front(), *this};
    }
    set(set const& other):  _size(other.size),_capacity(other._capacity), buckets(other.buckets),
        ibegin(other.ibegin), iend(other.iend), cbegin(other.cbegin), cend(other.cend) {}
    set(set&& other) noexcept : _size{other._size}, _capacity{other._capacity}, buckets{move(other.buckets)},
        ibegin(other.ibegin), iend(other.iend), cbegin(other.cbegin), cend(other.cend) {
        other._size = 0;
    }
    set& operator = (set const& other) {
        if (this == &other) { return *this; }
        buckets = other.buckets;
        _capacity = other._capacity;
        _size = other._size;
        ibegin = other.ibegin;
        iend = other.iend;
        cbegin = other.cbegin;
        cend = other.cend;
        return *this;
    }
    set& operator = (set&& other) noexcept {
        if (this == &other) { return *this; }
        buckets = move(other.buckets);
        _capacity = other._capacity;
        _size = other._size;
        ibegin = other.ibegin;
        iend = other.iend;
        cbegin = other.cbegin;
        cend = other.cend;
        return *this;
    }
    iterator find(T const& data) {
        int64_t hvals[set_traits<T>::lookup_hnum];
        set_traits<T>::lookup(data, hvals);
        for (int64_t i = 0; i < set_traits<T>::lookup_hnum; ++i) {
            for (int64_t j = 0; j < buckets[hvals[i]%_capacity].size(); ++j) {
                if (set_traits<T>::equal(buckets[hvals[i]%_capacity][j], data)) {
                    return iterator(&buckets[hvals[i]%_capacity][j], buckets.back().end(),
                        &buckets[hvals[i]%_capacity], buckets.end());
                }
            }
        }
        return end();
    }
    // НЕ ЗАБУДЬ НАПИСАТЬ ФАЙНДЫ ТАК ЧТОБЫ ЭТА ДРИСНЯ ХОТЯ БЫ СКОМПИЛИРОВАЛАСЬ
    const_iterator find(T const& data) const {
        int64_t hvals[set_traits<T>::lookup_hnum];
        set_traits<T>::lookup(data, hvals);
        for (int64_t i = 0; i < set_traits<T>::lookup_hnum; ++i) {
            for (int64_t j = 0; j < buckets[hvals[i]%_capacity].size(); ++j) {
                if (set_traits<T>::equal(buckets[hvals[i]%_capacity][j], data)) {
                    return const_iterator(&buckets[hvals[i]%_capacity][j], buckets.back().end(),
                        &buckets[hvals[i]%_capacity], buckets.end());
                }
            }
        }
        return end();
    }
    bool contains(T const& data) const {
        int64_t hvals[set_traits<T>::lookup_hnum];
        set_traits<T>::lookup(data, hvals);
        for (int64_t i = 0; i < set_traits<T>::lookup_hnum; ++i) {
            for (int64_t j = 0; j < buckets[hvals[i]%_capacity].size(); ++j) {
                if (set_traits<T>::equal(buckets[hvals[i]%_capacity][j], data)) {
                    return true;
                }
            }
        }
        return false;
    }
    bool insert(T const& data) {
        if (contains(data)) { return false; }
        if (2*_size >= _capacity) {rehash(_capacity * 2); }
        int64_t hval = set_traits<T>::hash(data);
        buckets[hval%_capacity].push_back(data);
        iterator iter = {buckets[hval%_capacity].end()-1, buckets[hval%_capacity], *this};
        if ( _size == 0 ) {
            ibegin = iter;
            iend = iter;
            cbegin = iter;
            cend = iter;
        }
        else if (iter < ibegin) {
            ibegin = iter;
            cbegin = iter;
        }
        else if (iend < iter) {
            iend = iter;
            cend = iter;
        }
        _size++;
        return true;
    }
    bool insert(T&& data) {
        if (contains(data)) { return false; }
        if (2*_size >= _capacity) {rehash(_capacity * 2); }
        int64_t hval = set_traits<T>::hash(data);
        buckets[hval%_capacity].push_back(move(data));
        iterator iter = {buckets[hval%_capacity].end()-1, buckets[hval%_capacity], *this};
        if ( _size == 0 ) {
            ibegin = iter;
            iend = iter;
            cbegin = iter;
            cend = iter;
        }
        else if (iter < ibegin) {
            ibegin = iter;
            cbegin = iter;
        }
        else if (iend < iter) {
            iend = iter;
            cend = iter;
        }
        _size++;
        return true;
    }
    //НЕ ЗАБУДЬ НАПИСАТЬ ХУЙНЮ ЧТОБЫ ПРИ УДАЛЕНИИ ИТЕРАТОРЫ СДВИГАЛИСЬ
    bool remove(T const& data) {
        auto iter = find(data);
        if (iter != end()) {
            iter.vp->remove(iter.p);
            _size--;
            return true;
        }
        return false;
    }
};

#endif //CONVEX_HULL_SET_H
