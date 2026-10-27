//
// Created by kirillr on 26.10.2026.
//

#ifndef CONVEX_HULL_PTESTER_H
#define CONVEX_HULL_PTESTER_H
#include "solution.h"
#include <chrono>
#include <thread>
#include <random>
#include "/home/kirillr/code/containers/mct.h"

template <>
struct mct::unordered_set_traits<point> {
    static int64_t hash(const point& p) {
        const int64_t data[2] {static_cast<int64_t>(floor(p.x/step + 0.5)), static_cast<int64_t>(floor(p.y/step + 0.5))};
        return MurmurHash2(reinterpret_cast<const char*>(data), sizeof(int64_t)*2);
    }
    static constexpr int64_t lookup_num = 9;
    static constexpr double epsilon = 1.0/(1<<23);
    static constexpr double step = 2*epsilon;
    static bool equal(const point& x, const point& y) {
        return x == y;
    }
    static void lookup(const point& x, int64_t* out) {
        int k = 0;
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                out[k++] = hash(x+point(i,j)*epsilon);
            }
        }
    }
};

class ptest {
    mct::unordered_set<point> points;
    int64_t test_number;
    public:
    ptest() = delete;
    ptest(int64_t test_number);
    ptest(int64_t test_number, int64_t n, double d, double r);
    ptest(const ptest&) = delete;
    friend std::ostream& operator<<(std::ostream& os, const ptest& t);
    friend std::istream& operator>>(std::istream& is, ptest& t);
    void fill_file(const char* c)
};

class ptester {

};


#endif //CONVEX_HULL_PTESTER_H
