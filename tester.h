//
// Created by kirillr on 12.09.2026.
//

#ifndef CONVEX_HULL_TESTER_H
#define CONVEX_HULL_TESTER_H
#include "solution.h"
#include <chrono>

struct result {
    uint64_t time; bool success;
};

class Tester {
    class test {
        vector<point> points;
    public:
        test(int n, double radius, double distribution);
        test(int n);
        void FillFile(const char* filename) const;
        result check(Solution& solution) const;
    };
    vector<test> tests;
    vector<result> optimal_results, naive_results;
public:
    Tester(int n, int m, double radius = 10, double distribution = 0);
    Tester();
    void RunTests();
    void PrintResults(const char* filename) const;
    void PrintResults() const;
};

#endif //CONVEX_HULL_TESTER_H
