//
// Created by kirillr on 12.09.2026.
//

#ifndef CONVEX_HULL_TESTER_H
#define CONVEX_HULL_TESTER_H
#include "solution.h"
#include <chrono>
#include "/home/kirillr/code/containers/mct.h"

struct result {
    uint64_t time; bool success;
};

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

class tester {
    class test {
    protected:
        mct::unordered_set<point> points;
    public:
        test() = default;
        test(int n, double radius, double distribution);
        test(int n);
        test(test&& other) noexcept;
        test& operator=(test&& other) noexcept;
        void FillFile(const char* filename) const;
        result check(Solution& solution) const;
    };
    mct::vector<test> tests;
    mct::vector<result> optimal_results, naive_results;
    friend void fill_test(int i, int n, double radius, double distribution);
public:
    tester(int n, int m, double radius = 10, double distribution = 0);
    tester();
    void RunTests();
    void PrintResults(const char* filename) const;
    void PrintResults() const;
};

#endif //CONVEX_HULL_TESTER_H
