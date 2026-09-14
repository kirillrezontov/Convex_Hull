//
// Created by kirillr on 11.09.2026.
//

#include "solution.h"
#include <fstream>
#include <cfloat>

Solution::Solution(const char* filename) {
    std::ifstream ifs(filename);
    if (!ifs.good()) { throw BadFile(filename, __func__); }
    point p;
    while (ifs >> p.x >> p.y) {
        points.push_back(p);
    }
    ifs.close();
}

void Solution::PrintResult(const char* filename) const {
    std::ofstream ofs(filename, std::ios::app);
    if (!ofs.good()) { throw BadFile(filename, __func__); }
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

NaiveSolution::NaiveSolution(const char *filename):
    Solution(filename) {}

void NaiveSolution::Solve() {
    point u = {DBL_MAX, DBL_MAX}, v;
    for (int i = 0; i < points.size(); ++i) {
        if (!less<point>{}(u, points[i])) { u = points[i]; }
    }
    hull.push_back(u); u = {0, 1};
    while (true) {
        double cosmin = LDBL_MAX, coscur; int hnext = -1;
        for (int i = 0; i < points.size(); ++i) {
            if (points[i] == hull.back()) continue;
            v = vec(hull.back(), points[i]);
            coscur = scalar_product_sq(u, v)/mod(v);
            if (coscur < cosmin) { cosmin = coscur; hnext = i; };
        }
        u = vec(points[hnext], hull.back());
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
    sort(psorted.begin(), psorted.end());
    hull.push_back(psorted[0]), hull.push_back(psorted[1]);
    for (int i = 2; i < points.size(); ++i) {
        while (hull.size() > 1 && vector_product(vec(*(hull.end()-1), *(hull.end()-2)),
            vec(*(hull.end()-1), psorted[i])) > 0) {
            hull.pop_back();
        }
        hull.push_back(psorted[i]);
    }
    for (int i = points.size()-1; i >= 0; --i) {
        while (hull.size() > 1 && vector_product(vec(*(hull.end()-1), *(hull.end()-2)),
            vec(*(hull.end()-1), psorted[i])) > 0) {
            hull.pop_back();
            }
        hull.push_back(psorted[i]);
    }
    hull.pop_back();
}
