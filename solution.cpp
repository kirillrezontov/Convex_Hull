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

Solution::Solution(const char* filename) {
    std::ifstream ifs(filename);
    if (!ifs.good()) { throw mct::bad_file(filename, __func__); }
    point p{0,0};
    while (ifs >> p) {
        points.push_back(p);
    }
    ifs.close();
}

void Solution::PrintResult(const char* filename) const {
    std::ofstream ofs(filename, std::ios::app);
    if (!ofs.good()) { throw mct::bad_file(filename, __func__); }
    for (auto p : hull) {
        ofs << p << std::endl;
    }
    ofs.close();
}

inline const mct::vector<point>& Solution::GetPoints() const {
    return points;
}

inline const mct::vector<point>& Solution::GetHull() const {
    return hull;
}

NaiveSolution::NaiveSolution(const char *filename):
    Solution(filename) {}

void NaiveSolution::Solve() {
    point u {DBL_MAX, DBL_MAX}, v {0,0};
    for (const auto& i : points) {
        mct::mins(u, i);
    }
    hull.push_back(u); u = {0, 1};
    while (true) {
        double cosmin = LDBL_MAX, coscur; int hnext = -1;
        for (int i = 0; i < points.size(); ++i) {
            if (points[i] == hull.back()) continue;
            v = to_vec(hull.back(), points[i]);
            coscur = sq_sc_prod(u, v)/sq_mod(v);
            if (mct::mins(cosmin, coscur)) hnext = i;
        }
        u = to_vec(points[hnext], hull.back());
        hull.push_back(points[hnext]);
        if (hull.front() == hull.back()) {
            hull.pop_back();
            break;
        }
    }
}

OptimalSolution::OptimalSolution(const char* filename):
    Solution(filename) {}

void OptimalSolution::Solve() {
    auto psorted(points);
    mct::sort(psorted.begin(), psorted.end());
    hull.push_back(psorted[0]), hull.push_back(psorted[1]);
    for (int i = 2; i < points.size(); ++i) {
        while (hull.size() > 1 && (to_vec(*(hull.end()-1), *(hull.end()-2)) &
            to_vec(*(hull.end()-1), psorted[i])) > 0) {
            hull.pop_back();
        }
        hull.push_back(psorted[i]);
    }
    for (int64_t i = points.size()-1; i >= 0; --i) {
        while (hull.size() > 1 && (to_vec(*(hull.end()-1), *(hull.end()-2)) &
            to_vec(*(hull.end()-1), psorted[i])) > 0) {
            hull.pop_back();
            }
        hull.push_back(psorted[i]);
    }
    hull.pop_back();
}
