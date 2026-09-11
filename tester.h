//
// Created by kirillr on 12.09.2026.
//

#ifndef CONVEX_HULL_TESTER_H
#define CONVEX_HULL_TESTER_H
#include "solution.h"
#include <chrono>

struct result {
    time_t time; bool success;
};

class Tester {
    class test {
        vector<point> points;
    public:
        test(int n, double radius, char distribution);
        test(int n);
        void FillFile(const char* filename);
        result check(Solution& solution);
    };
    vector<test> tests;
    vector<result> optimal_results, naive_results;
public:
    Tester(int n, int m, double radius = 10, char distribution = 0);
    Tester();
    void RunTests();
    void PrintResults(const char* filename);
    void PrintResults();
};

#endif //CONVEX_HULL_TESTER_H
