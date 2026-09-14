#ifndef SOLUTION_H
#define SOLUTION_H
#include "vector.h"
#include "algorythm.h"
#include "cmath"
struct point {
    double x, y;
    static constexpr double epsilon = 0.0000001;
    bool operator==(const point & other) const {
        return fabs(x-other.x) < epsilon && fabs(y - other.y) < epsilon;
    }
};

template <>
struct less<point> {
    bool operator()(const point& a, const point& b) const {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    }
};

inline point vec(const point& p1, const point& p2) {
    return {p2.x - p1.x,p2.y - p1.y};
}

inline double vector_product(const point& a, const point& b) {
    return a.x * b.y - a.y * b.x;
}

inline double scalar_product_sq(const point& a, const point& b) {
    double res = a.x * b.x + a.y * b.y;
    return res*res*((res<0)?-1:1);
}

inline double mod(const point& a) {
    return a.x*a.x + a.y*a.y;
}

class Solution {
protected:
    vector<point> points;
    vector<point> hull;
public:
    explicit Solution(const char* filename);
    virtual void Solve() = 0;

    void PrintResult(const char* filename) const;

    const vector<point>& GetPoints() const;
    const vector<point>& GetHull() const;
};

class NaiveSolution : public Solution {
    public:
    explicit NaiveSolution(const char* filename);
    void Solve() override;
};

class OptimalSolution : public Solution {
public:
    explicit OptimalSolution(const char* filename);
    void Solve() override;
};

#endif