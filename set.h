//
// Created by kirillr on 12.09.2026.
//

#ifndef CONVEX_HULL_SET_H
#define CONVEX_HULL_SET_H
#include "vector.h"
#include <cmath>
#include <filesystem>

template <typename T>
struct set_traits {
    static bool equal(const T& a, const T& b) {
        return a == b;
    }
    static constexpr size_t lookup_hnum = 1;
    static size_t hash(const T& data) {
        auto buf = (char*)&data;
        size_t hval = 1469598103934665603ULL;
        for (size_t i = 0; i < sizeof(T); ++i) {
            hval ^= buf[i];
            hval *= 1099511628211ULL;
        }
        return hval;
    }
    static size_t lookup(const T& data, size_t* out) {
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
    static constexpr size_t lookup_hnum = 3;
    static size_t hash(const double& data) {
        size_t e_data = floor(data/step+0.5);
        auto buf = (char*)&e_data; size_t hval = 1469598103934665603ULL;
        for (size_t i = 0; i < sizeof(double); ++i) {
            hval ^= buf[i];
            hval *= 1099511628211ULL;
        }
        return hval;
    }
    static size_t lookup(const double& data, size_t* out) {
        out[0] = hash(data-epsilon);
        out[1] = hash(data);
        out[2] = hash(data+epsilon);
        return 3;
    }
};

template <typename T>
class set {
    protected:
    size_t _size, _capacity;
    vector<vector<T>> buckets;
    void rehash(size_t new_cap) {
        if (new_cap <= _capacity) { return; }
        auto old = move(buckets);
        size_t cap = 1; while (cap < new_cap) cap <<= 1;
        buckets = vector<vector<T>>(cap);
        for (size_t i = 0; i < old.size(); ++i) {
            for (auto e : old[i]) {
                size_t h = set_traits<T>::hash(e);
                buckets[h%cap].push_back(e);
            }
        }
        _capacity = new_cap;
    }
    public:
    class iterator {
        friend class set;
        protected:
        T* p, *pend;
        vector<T>* vp, *vpend;
        iterator(T* pos, T* p_end, vector<T>* vpos, vector<T> *vp_end):p(pos), pend(p_end), vp(vpos), vpend(vp_end){}
        public:
        T& operator*() const {return *p;}
        T* operator->() const {return p;}
        iterator operator++() {
            auto i = *this;
            ++p; if (p!=pend) {
                if (p == vp->end()) {
                    while (vp!=vpend && vp->empty()) {
                        ++vp;
                    }
                    if (vp == vpend) p = pend;
                    else p = vp->begin();
                }
            }
            return i;
        }
        iterator& operator++(int) {
            ++p; if (p!=pend) {
                if (p == vp->end()) {
                    while (vp!=vpend && vp->empty()) {
                        ++vp;
                    }
                    if (vp == vpend) p = pend;
                    else p = vp->begin();
                }
            }
            return *this;
        }
        iterator operator--() {
            auto i = *this;
            if (p==vp->begin()) {
                --vp; p=vp->end()-1;
            }
            return i;
        }
        iterator& operator--(int) {
            if (p==vp->begin()) {
                --vp; p=vp->end()-1;
            }
            return *this;
        }
        bool operator==(const T* ptr) const {
            return p == ptr;
        }
        bool operator!=(const T* ptr) const {
            return p != ptr;
        }
        bool operator!=(const iterator iter) const {
            return p != iter.p;
        }
    };
    class const_iterator {
        friend class set;
    protected:
        const T* p, * pend;
        const vector<T>* vp, *vpend;
        const_iterator(const T* pos,const T* p_end,const vector<T>* vpos, const vector<T>* vp_end):p(pos), pend(p_end), vp(vpos), vpend(vp_end){}
    public:
        const T& operator*() const {return *p;}
        const T* operator->() const {return p;}
        const_iterator operator++() {
            auto i = *this;
            ++p; if (p!=pend) {
                if (p == vp->end()) {
                    while (vp!=vpend && vp->empty()) {
                        ++vp;
                    }
                    if (vp == vpend) p = pend;
                    else p = vp->begin();
                }
            }
            return i;
        }
        const_iterator& operator++(int) {
            ++p; if (p!=pend) {
                if (p == vp->end()) {
                    while (vp!=vpend && vp->empty()) {
                        ++vp;
                    }
                    if (vp == vpend) p = pend;
                    else p = vp->begin();
                }
            }
            return *this;
        }
        const_iterator operator--() {
            auto i = *this;
            if (p==vp->begin()) {
                --vp; p=vp->end()-1;
            }
            return i;
        }
        const_iterator& operator--(int) {
            if (p==vp->begin()) {
                --vp; p=vp->end()-1;
            }
            return *this;
        }
        bool operator==(const T* ptr) const {
            return p == ptr;
        }
        bool operator!=(const T* ptr) const {
            return p != ptr;
        }
        bool operator!=(const const_iterator iter) const {
            return p != iter.p;
        }
    };

    iterator begin() {return {buckets.front().begin(), buckets.back().end(), buckets.begin(), buckets.end()};}
    iterator end() {return {buckets.back().end(), buckets.back().end(), buckets.end()-1, buckets.end()};}
    const_iterator begin() const { return {buckets.front().begin(), buckets.back().end(), buckets.begin(), buckets.end()};}
    const_iterator end() const {return {buckets.back().end(), buckets.back().end(), buckets.end()-1, buckets.end()};}
    size_t size() const { return _size; }
    size_t bucket_capacity() const { return _capacity; }
    bool empty() const { return _size == 0; }
    set(size_t bucket_cap = 16): _size(0), _capacity(0) {
        bucket_cap = bucket_cap < 8 ? 8 : bucket_cap;
        size_t cap = 1; while (cap < bucket_cap) cap <<= 1;
        _capacity = cap;
        buckets = vector<vector<T>>(cap);
    }
    set(set const& other) {
        buckets(other.buckets);
        _capacity = other._capacity;
        _size = other._size;
    }
    set(set&& other) noexcept : _size{other._size}, _capacity{other._capacity}, buckets{move(other.buckets)} {
        other._size = 0;
    }
    set& operator = (set const& other) {
        if (this == &other) { return *this; }
        buckets = other.buckets;
        _capacity = other._capacity;
        _size = other._size;
        return *this;
    }
    set& operator = (set&& other) noexcept {
        if (this == &other) { return *this; }
        buckets = move(other.buckets);
        _capacity = other._capacity;
        _size = other._size;
        return *this;
    }
    iterator find(T const& data) {
        size_t hvals[set_traits<T>::lookup_hnum];
        set_traits<T>::lookup(data, hvals);
        for (size_t i = 0; i < set_traits<T>::lookup_hnum; ++i) {
            for (size_t j = 0; j < buckets[hvals[i]%_capacity].size(); ++j) {
                if (set_traits<T>::equal(buckets[hvals[i]%_capacity][j], data)) {
                    return iterator(&buckets[hvals[i]%_capacity][j], buckets.back().end(),
                        &buckets[hvals[i]%_capacity], buckets.end());
                }
            }
        }
        return end();
    }
    const_iterator find(T const& data) const {
        size_t hvals[set_traits<T>::lookup_hnum];
        set_traits<T>::lookup(data, hvals);
        for (size_t i = 0; i < set_traits<T>::lookup_hnum; ++i) {
            for (size_t j = 0; j < buckets[hvals[i]%_capacity].size(); ++j) {
                if (set_traits<T>::equal(buckets[hvals[i]%_capacity][j], data)) {
                    return const_iterator(&buckets[hvals[i]%_capacity][j], buckets.back().end(),
                        &buckets[hvals[i]%_capacity], buckets.end());
                }
            }
        }
        return end();
    }
    bool contains(T const& data) const {
        size_t hvals[set_traits<T>::lookup_hnum];
        set_traits<T>::lookup(data, hvals);
        for (size_t i = 0; i < set_traits<T>::lookup_hnum; ++i) {
            for (size_t j = 0; j < buckets[hvals[i]%_capacity].size(); ++j) {
                if (set_traits<T>::equal(buckets[hvals[i]%_capacity][j], data)) {
                    return true;
                }
            }
        }
        return false;
    }
    bool insert(T const& data) {
        if (contains(data)) { return false; }
        if (_size >= _capacity) {rehash(_capacity * 2); }
        size_t hval = set_traits<T>::hash(data);
        buckets[hval%_capacity].push_back(data);
        _size++;
        return true;
    }
    bool insert(T&& data) {
        if (contains(data)) { return false; }
        if (_size >= _capacity) {rehash(_capacity * 2); }
        size_t hval = set_traits<T>::hash(data);
        buckets[hval%_capacity].push_back(move(data));
        _size++;
        return true;
    }
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
