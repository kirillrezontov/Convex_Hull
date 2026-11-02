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
                out[k++] = hash(x+(point(i,j)*epsilon));
            }
        }
    }
};

struct solution_result {
    bool success;
    int64_t test_number;
    int64_t points_number;
    int64_t work_time;
    int64_t work_actions;
    solution_result() : success(false), test_number(0), points_number(0), work_time(0), work_actions(0) {}
    friend std::ostream& operator<<(std::ostream& os, const solution_result& t);
};

class ptest {
    friend class ptester;
    mct::unordered_set<point> points;
    int64_t test_number;
    public:
    ptest() = delete;
    ptest(int64_t test_number);
    ptest(int64_t test_number, int64_t n, double d, double r);
    ptest(const ptest& other) = delete;
    ptest(ptest&& other) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const ptest& t);
    friend std::istream& operator>>(std::istream& is, ptest& t);
    solution_result check(solution& s);
};

class ptester {
    mct::vector<ptest> tests;
    mct::vector<solution_result> naive_results;
    mct::vector<solution_result> optimal_results;
    public:
    ptester();
    ptester(int64_t test_number, int64_t n1, int64_t n2, double d, double r);
    ptester(const ptester&) = delete;
    void run_tests();
    void write_naive_results(const char* filename);
    void write_optimal_results(const char* filename);
};


#endif //CONVEX_HULL_PTESTER_H
