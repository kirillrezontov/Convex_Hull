//
// Created by kirillr on 11.09.2026.
//

#include "solution.h"
#include <fstream>
#include "sort.h"

Solution::Solution(const char* filename) {
    std::ifstream ifs(filename);
    if (!ifs.good()) { throw BadFile(); }
    ifs >> std::ws; point p;
    while (!ifs.eof()) {
        ifs >> p.x >> p.y;
        points.push_back(p);
        ifs >> std::ws;
    }
    ifs.close();
}

void Solution::PrintResult(const char* filename) const {
    std::ofstream ofs(filename);
    if (!ofs.good()) { throw BadFile(); }
    for (auto p : hull) {
        ofs << "(" << p.x << ", " << p.y << ")" << std::endl;
    }
    ofs.close();
}

const vector<point>& Solution::GetPoints() const {
    return points;
}

const vector<point>& Solution::GetHull() const {
    return hull;
}

OptimalSolution::OptimalSolution(const char* filename):
    Solution(filename) {}

template <>
struct less<point> {
    bool operator()(const point& a, const point& b) const {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    }
};

void OptimalSolution::Solve() {
    auto psorted(points);
    sort(psorted.begin(), psorted.end(), less<point>());
    hull.push_back(psorted[0]), hull.push_back(psorted[1]);
    for (size_t i = 2; i < points.size(); ++i) {
        while (hull.size() > 1 && vector_product(p_min(*(hull.end()-1), *(hull.end()-2)),
            p_min(*(hull.end()-1), psorted[i])) >= 0) {
            hull.pop_back();
        }
        hull.push_back(psorted[i]);
    }
    for (size_t i = points.size()-1; i > 0; --i) {
        while (hull.size() > 1 && vector_product(p_min(*(hull.end()-1), *(hull.end()-2)),
            p_min(*(hull.end()-1), psorted[i])) >= 0) {
            hull.pop_back();
            }
        hull.push_back(psorted[i]);
    }
}
