//
// Created by kirillr on 12.09.2026.
//

#ifndef CONVEX_HULL_TESTER_H
#define CONVEX_HULL_TESTER_H
#include "solution.h"
#include "set.h"
#include <chrono>

struct result {
    uint64_t time; bool success;
};

template<>
struct set_traits<point> {
    static constexpr double epsilon = point::epsilon;
    static constexpr double step = 2*epsilon;
    static bool equal(const point& a, const point& b) {
        return fabs(a.x-b.x)<epsilon && fabs(a.y-b.y)<epsilon;
    }
    static constexpr size_t lookup_hnum = 9;
    static int64_t hash(const point& data) {
        int64_t e_data[2] {(int64_t)floor(data.x/step+0.5), (int64_t)floor(data.y/step+0.5)};
        auto buf = (char*)&e_data; size_t hval = 1469598103934665603ULL;
        for (size_t i = 0; i < sizeof(size_t)*2; ++i) {
            hval ^= buf[i];
            hval *= 1099511628211ULL;
        }
        return (int64_t)(hval%INT64_MAX);
    }
    static size_t lookup(const point& data, int64_t* out) {
        int k = 0;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                out[k++] = hash(point{data.x+i*step, data.y+j*step});
            }
        }
        return 9;
    }
};

class Tester {
    bool rerun;
    int start, end;
    class test {
    protected:
        friend class vector<test>;
        set<point> points;
        test() = default;
    public:
        test(int n, double radius, double distribution);
        test(int n);
        test(test&& other) noexcept;
        test(const char* filename);
        void FillFile(const char* filename) const;
        result check(Solution& solution) const;
    };
    vector<test> tests;
    vector<result> optimal_results, naive_results;
public:
    Tester(int n, int m, double radius, double distribution);
    Tester();
    Tester(int start, int end);
    void RunTests();
    void PrintResults(const char* filename) const;
    void PrintResults() const;
};

#endif //CONVEX_HULL_TESTER_H
