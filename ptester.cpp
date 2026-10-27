//
// Created by kirillr on 26.10.2026.
//

#include "ptester.h"

int int_rand(const int & min, const int & max) {
    thread_local std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(min,max);
    return distribution(generator);
}

constexpr int INT_MAX_2 = INT_MAX>>1;

ptest::ptest(int64_t test_number) : test_number(test_number) {}

ptest::ptest(int64_t test_number, int64_t n, double d, double r) : test_number(test_number) {
    while (points.size() < n) {
        double angle = 1.0*int_rand(0, INT_MAX-1)/INT_MAX_2*M_PI;
        points.insert(point(r*cos(angle),r*sin(angle)));
    }
}

std::ostream& operator<<(std::ostream& os, const ptest& t) {
    os << t.points.size() << '\n';
    for (const auto& p : t.points) {
        os << p << '\n';
    }
    return os;
}

std::istream& operator>>(std::istream& is, ptest& t) {
    int64_t test_size;
    is >> test_size;
    t.points.clear();
    for (int64_t i = 0; i < test_size; i++) {
        point p; is >> p; t.points.insert(p);
    }
}

