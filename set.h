//
// Created by kirillr on 12.09.2026.
//

#ifndef CONVEX_HULL_SET_H
#define CONVEX_HULL_SET_H
#include "vector.h"
#include "algorythm.h"
#include <cmath>
#include <cstdint>

template <typename T>
struct set_traits {
    static bool equal(const T& a, const T& b) {
        return a == b;
    }
    static constexpr int64_t lookup_hnum = 1;
    static int64_t hash(const T& data) {
        auto buf = (char*)&data;
        size_t hval = 1469598103934665603ULL;
        for (int64_t i = 0; i < sizeof(T); ++i) {
            hval ^= buf[i];
            hval *= 1099511628211ULL;
        }
        return (int64_t)(hval%INT64_MAX);
    }
    static int64_t lookup(const T& data, int64_t* out) {
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
    static int64_t hash(const double& data) {
        int64_t e_data = floor(data/step+0.5);
        auto buf = (char*)&e_data; size_t hval = 1469598103934665603ULL;
        for (int64_t i = 0; i < sizeof(double); ++i) {
            hval ^= buf[i];
            hval *= 1099511628211ULL;
        }
        return (int64_t)(hval%INT64_MAX);
    }
    static int64_t lookup(const double& data, int64_t* out) {
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

    int64_t cap_count(int64_t cap) {
        if (cap < 8) { return 8; }
        int64_t new_cap = 1;
        while (new_cap < cap) { new_cap<<=1; }
        return new_cap;
    }

    void rehash(int64_t new_cap) {
        if (new_cap <= _capacity) { return; }
        auto old = move(buckets);
        int64_t cap = cap_count(new_cap);
        buckets = vector<vector<T>>(cap);
        for (int64_t i = 0; i < old.size(); ++i) {
            for (auto& e : old[i]) {
                int64_t h = set_traits<T>::hash(e);
                buckets[h%cap].push_back(move(e));
            }
        }
        _capacity = new_cap;
        for (int64_t i = 0; i < _capacity; i++) {
            if (buckets[i].size()) {
                ibegin = iterator{buckets[i].begin(), &buckets[i], *this};
                cbegin = const_iterator{buckets[i].begin(), &buckets[i], *this};
                break;
            }
        }
        for (int64_t i = _capacity-1; i >= 0; i--) {
            if (buckets[i].size()) {
                iend = iterator{buckets[i].end(), &buckets[i], *this};
                cend = const_iterator{buckets[i].end(), &buckets[i], *this};
                break;
            }
        }
    }
    public:
    bool operator!=(const set<T>& other) const {
        return buckets != other.buckets;
    }
    bool operator==(const set<T>& other) const {
        return buckets == other.buckets;
    }
    class iterator {
        friend class set;
    protected:
        vector<T>::iterator ptr; vector<vector<T>>::iterator vptr;
        set& ownr;
        iterator(T* p,vector<T>* vp, set& owner):ptr(p), vptr(vp), ownr(owner) {}
    public:
        iterator(const iterator& other) noexcept : ptr(other.ptr), vptr(other.vptr), ownr(other.ownr) {}
        iterator& operator=(const iterator& other) {
            if (ownr != other.ownr) throw BadIndex(ownr._size, ownr._size);
            ptr = other.ptr; vptr = other.vptr;
            return *this;
        }
        bool operator<(const iterator& other) const {
            return vptr<other.vptr || (vptr == other.vptr && ptr < other.ptr);
        }
        T& operator*() { return *ptr; }
        T* operator->() { return ptr; }
        iterator operator++(int) {
            if (!(*this < ownr.iend)) throw BadIndex(ownr._size, ownr._size);
            iterator iter = *this; ++ptr;
            while (ptr != ownr.iend.ptr && ptr == vptr->end()) {
                vptr++; ptr=vptr->begin();
            }
            return iter;
        }
        iterator& operator++() {
            if (!(*this < ownr.iend)) throw BadIndex(ownr._size, ownr._size);
            ++ptr;
            while (ptr != ownr.iend.ptr && ptr == vptr->end()) {
                vptr++; ptr=vptr->begin();
            }
            return *this;
        }
        iterator operator--(int) {
            iterator iter = *this;
            if (ownr.iend < *this) throw BadIndex(ownr._size + (ptr - ownr.iend.ptr), ownr._size);
            while (ptr != ownr.ibegin.ptr && ptr == vptr->begin()) {
                vptr--; ptr=vptr->end();
            }
            if (ptr == ownr.ibegin.ptr) throw BadIndex(-1, ownr._size);
            ptr--; return iter;
        }
        iterator& operator--() {
            if (ownr.iend < *this) throw BadIndex(ownr._size + (ptr - ownr.iend.ptr), ownr._size);
            while (ptr != ownr.ibegin.ptr && ptr == vptr->begin()) {
                vptr--; ptr=vptr->end();
            }
            if (ptr == ownr.ibegin.ptr) throw BadIndex(-1, ownr._size);
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
        const set& ownr;
        const_iterator(const T* p, const vector<T>* vp, const set& owner):ptr(p), vptr(vp), ownr(owner) {}
        const_iterator& operator=(const iterator& other) {
            if (ownr != other.ownr) throw BadIndex(ownr._size, ownr._size);
            ptr = other.ptr; vptr = other.vptr;
            return *this;
        }
    public:
        const_iterator(const const_iterator& other) noexcept : ptr(other.ptr), vptr(other.vptr), ownr(other.ownr) {}
        const_iterator& operator=(const const_iterator& other) {
            if (ownr != other.ownr) throw BadIndex(ownr._size, ownr._size);
            ptr = other.ptr; vptr = other.vptr;
            return *this;
        }
        bool operator<(const const_iterator& other) const {
            return vptr<other.vptr || (vptr == other.vptr && ptr < other.ptr);
        }
        const T& operator*() const { return *ptr; }
        const T* operator->() const { return ptr; }
        const_iterator operator++(int) {
            if (!(*this < ownr.cend)) throw BadIndex(ownr._size, ownr._size);
            const_iterator iter = *this; ++ptr;
            while (ptr != ownr.cend.ptr && ptr == vptr->end()) {
                vptr++; ptr=vptr->begin();
            }
            return iter;
        }
        const_iterator& operator++() {
            if (!(*this < ownr.cend)) throw BadIndex(ownr._size, ownr._size);
            ++ptr;
            while (ptr != ownr.cend.ptr && ptr == vptr->end()) {
                vptr++; ptr=vptr->begin();
            }
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator iter = *this;
            if (ownr.cend < *this) throw BadIndex(ownr._size + (ptr - ownr.cend.ptr), ownr._size);
            while (ptr != ownr.cbegin.ptr && ptr == vptr->begin()) {
                vptr--; ptr=vptr->end();
            }
            if (ptr == ownr.cbegin.ptr) throw BadIndex(-1, ownr._size);
            ptr--; return iter;
        }
        const_iterator& operator--() {
            if (ownr.cend < *this) throw BadIndex(ownr._size + (ptr - ownr.cend.ptr), ownr._size);
            while (ptr != ownr.cbegin.ptr && ptr == vptr->begin()) {
                vptr--; ptr=vptr->end();
            }
            if (ptr == ownr.cbegin.ptr) throw BadIndex(-1, ownr._size);
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
    iterator begin() {return ibegin;}
    iterator end() {return iend;}
    int64_t size() const { return _size; }
    int64_t bucket_capacity() const { return _capacity; }

    bool empty() const { return _size == 0; }

    set(int64_t bucket_cap = 16): _size(0), _capacity(cap_count(bucket_cap)), buckets(bucket_cap),
        ibegin(buckets.begin()->begin(), buckets.begin(), *this),
        iend(buckets.begin()->end(), buckets.begin(), *this),
        cbegin(buckets.begin()->begin(), buckets.begin(), *this),
        cend(buckets.begin()->end(), buckets.begin(), *this) {}

    set(set const& other):  _size(other._size),_capacity(other._capacity), buckets(other.buckets),
        ibegin(buckets.begin()->begin(), buckets.begin(), *this),
        iend(buckets.begin()->end(), buckets.begin(), *this),
        cbegin(buckets.begin()->begin(), buckets.begin(), *this),
        cend(buckets.begin()->end(), buckets.begin(), *this)
    {
        for (int64_t i = 0; i < _capacity; i++) {
            if (buckets[i].size()) {
                ibegin = iterator{buckets[i].begin(), &buckets[i], *this};
                cbegin = const_iterator{buckets[i].begin(), &buckets[i], *this};
                break;
            }
        }
        for (int64_t i = _capacity-1; i >= 0; i--) {
            if (buckets[i].size()) {
                iend = iterator{buckets[i].end(), &buckets[i], *this};
                cend = const_iterator{buckets[i].end(), &buckets[i], *this};
                break;
            }
        }
    }
    set(set&& other) noexcept : _size{other._size}, _capacity{other._capacity}, buckets{move(other.buckets)},
        ibegin{other.ibegin.ptr, other.ibegin.vptr, *this}, iend{other.iend.ptr, other.iend.vptr, *this},
        cbegin{other.cbegin.ptr, other.cbegin.vptr, *this}, cend{other.iend.ptr, other.iend.vptr, *this}
    {
        other._size = 0;
    }

    set& operator = (set const& other) {
        if (this == &other) { return *this; }
        buckets = other.buckets;
        _capacity = other._capacity;
        _size = other._size;
        if (_size == 0) {
            ibegin = iterator{buckets.begin()->begin(), buckets.begin(), *this};
            iend = iterator{buckets.begin()->end(), buckets.begin(), *this};
            cbegin = const_iterator{buckets.begin()->begin(), buckets.begin(), *this};
            cend = const_iterator{buckets.begin()->end(), buckets.begin(), *this};
        }
        for (int64_t i = 0; i < _capacity; i++) {
            if (buckets[i].size()) {
                ibegin = iterator{buckets[i].begin(), &buckets[i], *this};
                cbegin = const_iterator{buckets[i].begin(), &buckets[i], *this};
                break;
            }
        }
        for (int64_t i = _capacity-1; i >= 0; i--) {
            if (buckets[i].size()) {
                iend = iterator{buckets[i].end(), &buckets[i], *this};
                cend = const_iterator{buckets[i].end(), &buckets[i], *this};
                break;
            }
        }
        return *this;
    }

    set& operator = (set&& other) noexcept {
        if (this == &other) { return *this; }
        buckets = move(other.buckets);
        _capacity = other._capacity;
        _size = other._size;
        ibegin=iterator{other.ibegin.ptr, other.ibegin.vptr, *this}; iend=iterator{other.iend.ptr, other.iend.vptr, *this};
        cbegin=const_iterator{other.cbegin.ptr, other.cbegin.vptr, *this}; cend=const_iterator{other.iend.ptr, other.iend.vptr, *this};
        other._size = 0;
        return *this;
    }

    iterator find(T const& data) {
        int64_t hvals[set_traits<T>::lookup_hnum];
        set_traits<T>::lookup(data, hvals);
        for (int64_t i = 0; i < set_traits<T>::lookup_hnum; ++i) {
            for (int64_t j = 0; j < buckets[hvals[i]%_capacity].size(); ++j) {
                if (set_traits<T>::equal(buckets[hvals[i]%_capacity][j], data)) {
                    return iterator(&buckets[hvals[i]%_capacity][j], &buckets[hvals[i]%_capacity], *this);
                }
            }
        }
        return end();
    }

    const_iterator find(T const& data) const {
        int64_t hvals[set_traits<T>::lookup_hnum];
        set_traits<T>::lookup(data, hvals);
        for (int64_t i = 0; i < set_traits<T>::lookup_hnum; ++i) {
            for (int64_t j = 0; j < buckets[hvals[i]%_capacity].size(); ++j) {
                if (set_traits<T>::equal(buckets[hvals[i]%_capacity][j], data)) {
                    return const_iterator(&buckets[hvals[i]%_capacity][j], &buckets[hvals[i]%_capacity], *this);
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
        iterator iter = {buckets[hval%_capacity].end()-1, &buckets[hval%_capacity], *this};
        if ( !_size ) {
            ibegin = iter;
            cbegin = iter;
            iend = iterator{iter.ptr+1, iter.vptr, iter.ownr};
            cend = const_iterator{iter.ptr+1, iter.vptr, iter.ownr};
        }
        else if (iter < ibegin) {
            ibegin = iter;
            cbegin = iter;
        }
        else if (!(iter < iend)) {
            iend = iterator{iter.ptr+1, iter.vptr, iter.ownr};
            cend = const_iterator{iter.ptr+1, iter.vptr, iter.ownr};
        }
        _size++;
        return true;
    }

    bool insert(T&& data) {
        if (contains(data)) { return false; }
        if (2*_size >= _capacity) {rehash(_capacity * 2); }
        int64_t hval = set_traits<T>::hash(data);
        buckets[hval%_capacity].push_back(move(data));
        iterator iter = {buckets[hval%_capacity].end()-1, &buckets[hval%_capacity], *this};
        if ( !_size ) {
            ibegin = iter;
            cbegin = iter;
            iend = iterator{iter.ptr+1, iter.vptr, iter.ownr};
            cend = const_iterator{iter.ptr+1, iter.vptr, iter.ownr};
        }
        else if (iter < ibegin) {
            ibegin = iter;
            cbegin = iter;
        }
        else if (!(iter < iend)) {
            iend = iterator{iter.ptr+1, iter.vptr, iter.ownr};
            cend = const_iterator{iter.ptr+1, iter.vptr, iter.ownr};
        }
        _size++;
        return true;
    }

    bool remove(T const& data) {
        auto iter = find(data);
        if (_size == 1) {
            iter.vptr->remove(iter.ptr);
            ibegin = iterator{buckets.begin()->begin(), buckets.begin(), *this};
            iend = iterator{buckets.begin()->end(), buckets.begin(), *this};
            cbegin = const_iterator{buckets.begin()->begin(), buckets.begin(), *this};
            cend = const_iterator{buckets.begin()->end(), buckets.begin(), *this};
            _size--;
            return true;
        }
        if (iter != iend) {
            if (ibegin.vptr == iter.vptr) {
                if (iter.vptr->size() == 1) {
                    do {
                        ibegin.vptr++;
                        cbegin.vptr++;
                    }while (cbegin.vptr->empty());
                    ibegin.ptr = ibegin.vptr->begin();
                    cbegin.ptr = cbegin.vptr->begin();
                }
            }
            if (iter.vptr == iend.vptr) {
                if (iter.vptr->size() == 1) {
                    do {
                        iend.vptr--;
                        cend.vptr--;
                    }while (cend.vptr->empty());
                    iend.ptr = iend.vptr->end();
                    cend.ptr = cend.vptr->end();
                }
                else {
                    cend.ptr--; iend.ptr--;
                }
            }
            iter.vptr->remove(iter.ptr);
            _size--;
            return true;
        }
        return false;
    }
};

#endif //CONVEX_HULL_SET_H
