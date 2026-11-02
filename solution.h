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

class solution {
protected:
    mct::vector<point> points;
    mct::vector<point> hull;
    int64_t actions{};
public:
    friend std::ostream& operator<<(std::ostream& os, const solution& s);
    friend std::istream& operator>>(std::istream& is, solution& s);
    explicit solution(const char* filename);
    explicit solution();
    virtual void solve() = 0;

    void print_result(const char* filename) const;
    void print_result() const;
    [[nodiscard]] const mct::vector<point>& get_points() const;
    [[nodiscard]] const mct::vector<point>& get_hull() const;
    [[nodiscard]] int64_t get_actions() const;
};

class naive_solution : public solution {
    public:
    explicit naive_solution(const char* filename);
    void solve() override;
};

class optimal_solution : public solution {
public:
    explicit optimal_solution(const char* filename);
    void solve() override;
};

#endif