#ifndef SOLUTION_H
#define SOLUTION_H
#include "vector.h"
#define ld long double

struct point {
    ld x, y;
};

inline point p_min(const point& p1, const point& p2) {
    return {
        p2.x - p1.x,
        p2.y - p1.y
    };
}


inline ld vector_product(const point& a, const point& b) {
    return a.x * b.y - a.y * b.x;
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


class OptimalSolution : public Solution {
public:
    explicit OptimalSolution(const char* filename);
    void Solve() override;
};

#endif