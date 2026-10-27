#ifndef SOLUTION_H
#define SOLUTION_H
#include "/home/kirillr/code/containers/mct.h"
#include "cmath"

struct point {
    double x, y;
    static constexpr double epsilon = 0.0000001;
    bool operator==(const point & other) const ;
    bool operator!=(const point & other) const ;
    bool operator<(const point & other) const ;
    bool operator>(const point & other) const ;
    friend std::ostream& operator<<(std::ostream& os, const point& p);
    friend std::istream& operator>>(std::istream& is, point& p);
    friend point operator+(const point& a, const point& b);
    friend point operator-(const point& a, const point& b);
    friend double operator*(const point& a, const point& b);
    friend point operator*(const point& a, const double& b);
    friend point operator*(const double& a, const point& b);
    friend point operator/(const point& a, const double& b);
    friend double operator&(const point& a, const point& b);
    friend double sq_sc_prod(const point& a, const point& b);
    friend double sq_mod(const point& a);
    friend point to_vec(const point& a, const point& b);
};

template <>
struct mct::less<point> {
    bool operator()(const point& a, const point& b) const {
        return a < b;
    }
};

class Solution {
protected:
    mct::vector<point> points;
    mct::vector<point> hull;
public:
    explicit Solution(const char* filename);
    virtual void Solve() = 0;

    void PrintResult(const char* filename) const;

    [[nodiscard]] const mct::vector<point>& GetPoints() const;
    [[nodiscard]] const mct::vector<point>& GetHull() const;
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