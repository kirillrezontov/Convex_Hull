//
// Created by kirillr on 12.09.2026.
//

#include "tester.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#define PI 3.14159265359
#define tfile "test/test_"
#define tcount "test/test_count.txt"

Tester::Tester() {
    using namespace std;
    cout << "Number of tests: ";
    int n; cin >> n;
    for (int i = 0; i < n; ++i) {
        cout << "Number of points: ";
        int m; cin >> m;
        if (m<=0) continue;
        tests.push_back(test(m));
    }
}

Tester::Tester(int n, int m, double radius, double distribution) {
    if (n<=0 || m <= 2) return;
    for (int i = 0; i < n; ++i) {
        std::cout << "\rGenerating tests " << 100*i/n << "%...";
        std::cout.flush();
        tests.push_back(test(m, radius, distribution));
    }
    std::cout << "\rGenerating tests 100%\n";
    std::cout.flush();
}

void Tester::RunTests(){
    if (tests.empty()) return;
    int passed = 0;
    std::ifstream tc(tcount);
    if (!tc.is_open()) throw BadFile(tcount, __func__);
    size_t test_count = 0;
    tc >> test_count;
    tc.close();
    for (const auto &test : tests) {
        string test_name = tfile + to_string(test_count)+".txt";
        std::cout << "\rRunning tests " << 100*passed/tests.size() << "%...";
        std::cout.flush();
        test.FillFile(test_name.c_str()); test_count++;
        std::ofstream tcr(tcount);
        if (!tcr.is_open()) throw BadFile(tcount, __func__);
        tcr << test_count; tcr.close();
        OptimalSolution Os(test_name.c_str());
        NaiveSolution Ns(test_name.c_str());
        optimal_results.push_back(test.check(Os));
        naive_results.push_back(test.check(Ns));
        passed++;
    }
    std::cout << "\rRunning tests " << 100*passed/tests.size() << "%";
    std::cout.flush();
}

void Tester::PrintResults() const {
    using namespace std;
    cout << "Optimal solution results: \t";
    size_t ot=0,nt=0,os=0,ns=0;
    for (int i = 0; i < optimal_results.size(); ++i) {
        cout << i << ' ' << (optimal_results[i].success?"A ": "F ")<< optimal_results[i].time << "us \t";
        ot+=optimal_results[i].time; os+=optimal_results[i].success;
    }
    cout << "Optimal solution:\npassed " << os << '/' << optimal_results.size() << ", avg time " << ot/optimal_results.size() << " us\n" ;
    cout << endl;
    cout << "Naive solution results: \t";
    for (int i = 0; i < naive_results.size(); ++i) {
        cout << i << ' ' << (naive_results[i].success?"A ": "F ")<< naive_results[i].time << "us \t";
        nt+=naive_results[i].time; ns+=naive_results[i].success;
    }
    cout << "Naive solution:\npassed " << ns << '/' << naive_results.size() << ", avg time " << nt/naive_results.size() << " us\n" ;
    cout << endl;
}

void Tester::PrintResults(const char* filename) const {
    std::ofstream cout(filename);
    if (!cout.is_open()) throw BadFile(filename, __func__);
    cout << "Optimal solution results: \t";
    size_t ot=0,nt=0,os=0,ns=0;
    for (int i = 0; i < optimal_results.size(); ++i) {
        cout << i << ' ' << (optimal_results[i].success?"A ": "F ")<< optimal_results[i].time << "us \t";
        ot+=optimal_results[i].time; os+=optimal_results[i].success;
    }
    cout << "\nOptimal solution:\npassed " << os << '/' << optimal_results.size() << ", avg time " << ot/optimal_results.size() << " us\n" ;
    cout << std::endl;
    cout << "Naive solution results: \t";
    for (int i = 0; i < naive_results.size(); ++i) {
        cout << i << ' ' << (naive_results[i].success?"A ": "F ")<< naive_results[i].time << "us \t";
        nt+=naive_results[i].time; ns+=naive_results[i].success;
    }
    cout << "\nNaive solution:\npassed " << ns << '/' << naive_results.size() << ", avg time " << nt/naive_results.size() << " us\n" ;
    cout << std::endl;
}

Tester::test::test(int n) {
    for (int i = 0; i < n; ++i) {
        double x, y; std::cin >> x >> y;
        points.insert(point{x,y});
    }
}

Tester::test::test(int n, double radius, double distribution) {
    for (int i = 0; i < n; ++i) {
        double r = radius*pow(1.*rand()/RAND_MAX, distribution);
        double angle = double(rand()) / RAND_MAX * 2 * PI;
        points.insert(point{r*cos(angle), r*sin(angle)});
    }
}

result Tester::test::check(Solution& solution) const {
    result res {0,false};
    auto start = std::chrono::steady_clock::now();
    solution.Solve();
    auto end = std::chrono::steady_clock::now();
    res.time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    auto hull=solution.GetHull();
    size_t h = hull.size();
    if (h < 3 && points.size() >= 3) { return res; }
    for (size_t i = 0; i < h; ++i) {
        point a = hull[i], b = hull[(i+1)%h], c = hull[(i+2)%h];
        if (vector_product(vec(a,b), vec(b,c)) < point::epsilon) {
            return res;
        }
    }
    for (auto i : hull) {
        bool found = false;
        for (const auto& p : points) {
            if (i == p) {
                found = true;
                break;
            }
        }
    }
    for (int i = 0; i < hull.size(); ++i) {
        point u = vec(hull[i], hull[(i+1)%hull.size()]);
        for (const auto& p: points) {
            point v = vec(hull[i], p);
            if (p == hull[(i+1)%hull.size()]) { continue; }
            if (vector_product(u, v) < 0) { return res; }
        }
    }
    res.success = true;
    return res;
}

void Tester::test::FillFile(const char* filename) const {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) throw BadFile(filename, __func__);
    ofs << std::setprecision(17);
    for (auto p: points) {
        ofs << p.x << ' ' << p.y << std::endl;
    }
    ofs.close();
}

