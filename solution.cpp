//
// Created by kirillr on 11.09.2026.
//

#include "solution.h"
#include <fstream>
#include <cfloat>

std::ostream& operator<<(std::ostream& os, const solution& s) {
    os << s.hull.size() << '\n';
    for (const auto& i : s.hull) {
        os << i << "\n";
    }
    return os;
}

std::istream& operator>>(std::istream& is, solution& s) {
    int64_t p_size; is >> p_size;
    s.points.clear(); s.points.resize(p_size);
    for (int64_t i = 0; i < p_size; ++i) {
        is >> s.points[i];
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

[[nodiscard]] const mct::vector<point>& solution::get_points() const {
    return points;
}

[[nodiscard]] const mct::vector<point>& solution::get_hull() const {
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
