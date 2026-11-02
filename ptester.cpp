//
// Created by kirillr on 26.10.2026.
//

#include "ptester.h"
#include <fstream>
#include <iomanip>

auto test_path = "test/test_";
auto test_count = "test/test_count.txt";

constexpr int INT_MAX_2 = INT_MAX>>1;

std::ostream& operator<<(std::ostream& os, const solution_result& s) {
    os << s.test_number << "; " << (s.success?"AC; ":"WA; ") << s.points_number << "; " << s.work_actions << "; " << s.work_time;
    return os;
}

ptest::ptest(int64_t test_number) : test_number(test_number) {}

ptest::ptest(int64_t test_number, int64_t n, double d, double r) : test_number(test_number) {
    while (points.size() < n) {
        double rad = r*pow(1.0*mct::thread_local_rand(0, INT_MAX)/INT_MAX, d);
        double angle = 1.0*mct::thread_local_rand(0, INT_MAX-1)/INT_MAX_2*M_PI;
        points.insert(point(rad*cos(angle),rad*sin(angle)));
    }
}

ptest::ptest(ptest && other) noexcept : test_number(other.test_number), points(mct::move(other.points)) {
    other.test_number = 0;
}

std::ostream& operator<<(std::ostream& os, const ptest& t) {
    os << t.points.size() << '\n';
    for (const auto& p : t.points) {
        os << p << '\n';
    }
    return os;
}

std::istream& operator>>(std::istream& is, ptest& t) {
    int64_t p_size;
    is >> p_size; t.points.clear();
    point p = {0, 0};
    for (int64_t i = 0; i < p_size; i++) {
        is >> p; t.points.insert(p);
    }
    return is;
}

solution_result ptest::check(solution& s) {
    solution_result result;
    auto start = std::chrono::steady_clock::now();
    s.solve();
    auto end = std::chrono::steady_clock::now();
    mct::vector hull(s.get_hull());
    result.work_time = std::chrono::duration_cast<std::chrono::seconds>(end-start).count();
    result.work_actions = s.get_actions();
    result.points_number = points.size();
    result.test_number = test_number;
    auto hull_size = hull.size();
    if (hull_size < 3 && points.size() >= 3) return result;
    for (int64_t i = 0; i < hull_size; ++i) {
        point a = hull[i], b = hull[(i+1)%hull_size], c = hull[(i+2)%hull_size];
        if ((to_vec(a,b) & to_vec(b,c)) < -point::epsilon) {
            return result;
        }
    }
    for (int64_t i = 0; i < hull_size; ++i) {
        if (!points.contains(hull[i])) { return result; }
        auto u = to_vec(hull[i], hull[(i+1)%hull_size]);
        for (const auto& p : points) {
            if (p == hull[(i+1)%hull_size]) { continue; }
            auto v = to_vec(hull[i], p);
            if ((u & v) < -point::epsilon) { return result; }
        }
    }
    result.success = true;
    return result;
}

ptester::ptester() {
    int64_t t_size; std::cin >> t_size;
    for (int64_t i = 0; i < t_size; ++i) {
        int64_t t_number; std::cin >> t_number;
        tests.push_back(ptest(t_number));
        std::ifstream ifs((mct::string(test_path) + mct::to_string(t_number) + ".txt").c_str());
        ifs >> tests[i];
        ifs.close();
    }
}

ptester::ptester(int64_t test_number, int64_t n1, int64_t n2, double d, double r) : optimal_results(test_number), naive_results(test_number){
    std::ifstream ifs(test_count);
    if (!ifs.good()) throw mct::bad_file(test_count, __func__);
    int64_t t_num; ifs >> t_num; ifs.close(); tests.reserve(test_number);
    for (int64_t i = 0; i < test_number; ++i) {
        tests.emplace_back(i+t_num, n1+i*(n2-n1)/(test_number-1), d, r);
        std::ofstream ofs((mct::string(test_path) + mct::to_string(i+t_num) + ".txt").c_str());
        ofs << std::fixed << std::setprecision(17) << tests[i];
        ofs.close();
    }
    t_num+=test_number;
    std::ofstream ofs(test_count);
    ofs << t_num;
}

void ptester::write_naive_results(const char *filename) {
    std::ofstream ofs(filename);
    for (const auto& r: naive_results) {
        ofs << r << '\n';
    }
    ofs.close();
}

void ptester::write_optimal_results(const char *filename) {
    std::ofstream ofs(filename);
    for (const auto& r: optimal_results) {
        ofs << r << '\n';
    }
}

auto thread_count = mct::max(1u, std::thread::hardware_concurrency());

void ptester::run_tests() {
    auto run_test = [](ptest* t, solution_result* optimal_res, solution_result* naive_res) {
        auto path = mct::string(test_path) + mct::to_string(t->test_number) + ".txt";
        optimal_solution os(path.c_str());
        naive_solution ns(path.c_str());
        *optimal_res = t->check(os);
        *naive_res = t->check(ns);
    };
    for (int64_t i = 0; i < (tests.size()+thread_count-1)/thread_count; ++i) {
        mct::vector<std::jthread> threads;
        for (u_int j = 0; j < thread_count && i*thread_count + j < tests.size(); ++j) {
            threads.emplace_back(run_test, &tests[i*thread_count+j], &optimal_results[i*thread_count+j], &naive_results[i*thread_count+j]);
        }
    }
}
