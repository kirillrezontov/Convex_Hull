//
// Created by kirillr on 11.09.2026.
//

#include "solution.h"
#include <fstream>
#include <cfloat>

inline bool point::operator==(const point & other) const {
    return fabs(x-other.x) < epsilon && fabs(y - other.y) < epsilon;
}
inline bool point::operator!=(const point & other) const {
    return fabs(x-other.x) > epsilon && fabs(y - other.y) > epsilon;
}
inline bool point::operator<(const point & other) const {
    return x < other.x || (fabs(x-other.x) < epsilon && y < other.y);
}
inline bool point::operator>(const point & other) const {
    return x > other.x || (fabs(x-other.x) < epsilon && y > other.y);
}
inline std::ostream& operator<<(std::ostream& os, const point& p) {
    return os << "(" << p.x << "," << p.y << ")";
}
inline std::istream& operator>>(std::istream& is, point& p) {
    return is >> p.x >> p.y;
}
inline point operator+(const point& a, const point& b) {
    return point{a.x + b.x, a.y + b.y};
}
inline point operator-(const point& a, const point& b) {
    return point{a.x - b.x, a.y - b.y};
}
inline double operator*(const point& a, const point& b) {
    return a.x * b.x + a.y * b.y;
}

inline point operator*(const point &a, const double &b) {
    return point{a.x*b, a.y*b};
}

inline point operator*(const double &a, const point &b) {
    return point{a*b.x, a*b.y};
}

inline point operator/(const point &a, const double &b) {
    return point{a.x/b, a.y/b};
}

inline double operator&(const point& a, const point& b) {
    return a.x * b.y - a.y * b.x;
}
inline double sq_sc_prod(const point& a, const point& b) {
    double s = a.x * b.x + a.y * b.y;
    return s*s*(s>=0?1:-1);
}
inline double sq_mod(const point &a) {
    return a.x * a.x + a.y * a.y;
}
inline point to_vec(const point& a, const point& b) {
    return b - a;
}

std::ostream& operator<<(std::ostream& os, const solution& s) {
    os << s.hull.size() << '\n';
    for (const auto& i : s.hull) {
        os << i << "\n";
    }
    return os;
}

std::istream& operator>>(std::istream& is, solution& s) {
    point p = {0, 0};
    int64_t p_size; is >> p_size;
    s.points.clear();s.points.resize(p_size);
    for (auto& i : s.points) {
        is >> i;
    }
    return is;
}

solution::solution(const char* filename) {
    std::ifstream ifs(filename);
    if (!ifs.good()) { throw mct::bad_file(filename, __func__); }
    ifs >> *this;
    ifs.close();
}

solution::solution() {
    std::cin >> *this;
}

void solution::print_result(const char* filename) const {
    std::ofstream ofs(filename, std::ios::app);
    if (!ofs.good()) { throw mct::bad_file(filename, __func__); }
    ofs << *this;
    ofs.close();
}

void solution::print_result() const {
    std::cout << *this;
}

[[nodiscard]] int64_t solution::get_actions() const {
    return actions;
}

[[nodiscard]] inline const mct::vector<point>& solution::get_points() const {
    return points;
}

[[nodiscard]] inline const mct::vector<point>& solution::get_hull() const {
    return hull;
}

naive_solution::naive_solution(const char *filename):
    solution(filename) {}

void naive_solution::solve() {
    point u {DBL_MAX, DBL_MAX}, v {0,0};
    for (const auto& i : points) {
        mct::mins(u, i); ++actions;
    }
    hull.push_back(u); u = {0, 1};
    while (true) {
        double cosmin = LDBL_MAX, coscur; int hnext = -1;
        for (int i = 0; i < points.size(); ++i) {
            ++actions;
            if (points[i] == hull.back()) continue;
            v = to_vec(hull.back(), points[i]);
            coscur = sq_sc_prod(u, v)/sq_mod(v);
            ++actions;
            if (mct::mins(cosmin, coscur)) {hnext = i; ++actions;}
        }
        u = to_vec(points[hnext], hull.back()); ++actions;
        hull.push_back(points[hnext]); ++actions;
        if (hull.front() == hull.back()) {
            hull.pop_back(); ++actions;
            break;
        }
    }
}

optimal_solution::optimal_solution(const char* filename):
    solution(filename) {}

void optimal_solution::solve() {
    auto psorted(points);
    actions += mct::sort(psorted.begin(), psorted.end());
    hull.push_back(psorted[0]), hull.push_back(psorted[1]); ++actions;
    for (int i = 2; i < points.size(); ++i) {
        while (hull.size() > 1 && (to_vec(*(hull.end()-1), *(hull.end()-2)) &
            to_vec(*(hull.end()-1), psorted[i])) > 0) {
            hull.pop_back(); ++actions;
        }
        hull.push_back(psorted[i]); ++actions;
    }
    for (int64_t i = points.size()-1; i >= 0; --i) {
        while (hull.size() > 1 && (to_vec(*(hull.end()-1), *(hull.end()-2)) &
            to_vec(*(hull.end()-1), psorted[i])) > 0) {
            hull.pop_back(); ++actions;
            }
        hull.push_back(psorted[i]); ++actions;
    }
    hull.pop_back();++actions;
}
