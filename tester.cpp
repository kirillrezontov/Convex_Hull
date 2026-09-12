//
// Created by kirillr on 12.09.2026.
//

#include "tester.h"

#include <cmath>
#include <iostream>
#include <fstream>
#define PI 3.14159265359
#define tfile "/home/kirillr/Convex_Hull/test.txt"
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
    distribution = distribution>5 ? 5 : distribution;
    for (int i = 0; i < n; ++i) {
        tests.push_back(test(m, radius, distribution));
    }
}

void Tester::RunTests(){
    int passed = 0;
    for (const auto &test : tests) {
        std::cout << '\r' << 100*passed/tests.size() << "%";
        test.FillFile(tfile);
        OptimalSolution Os(tfile);
        NaiveSolution Ns(tfile);
        optimal_results.push_back(test.check(Os));
        naive_results.push_back(test.check(Ns));
        passed++;
    }
    std::cout << '\r' << 100*passed/tests.size() << "%";
}

void Tester::PrintResults() const {
    using namespace std;
    cout << "Optimal solution results: \t";
    for (int i = 0; i < optimal_results.size(); ++i) {
        cout << i << ' ' << (optimal_results[i].success?"A ": "F ")<< optimal_results[i].time << "us \t";
    }
    cout << endl;
    cout << "Naive solution results: \t";
    for (int i = 0; i < naive_results.size(); ++i) {
        cout << i << ' ' << (naive_results[i].success?"A ": "F ")<< naive_results[i].time << "us \t";
    }
    cout << endl;
}

void Tester::PrintResults(const char* filename) const {
    std::ofstream cout(filename);
    cout << "Optimal solution results: \t";
    for (int i = 0; i < optimal_results.size(); ++i) {
        cout << i << ' ' << (optimal_results[i].success?"A ": "F ")<< optimal_results[i].time << "ms \t";
    }
    cout << std::endl;
    cout << "Naive solution results: \t";
    for (int i = 0; i < naive_results.size(); ++i) {
        cout << i << ' ' << (naive_results[i].success?"A ": "F ")<< naive_results[i].time << "ms \t";
    }
    cout << std::endl;
}

Tester::test::test(int n) {
    for (int i = 0; i < n; ++i) {
        double x, y; std::cin >> x >> y;
        points.push_back({x, y});
    }
}

Tester::test::test(int n, double radius, double distribution) {
    for (int i = 0; i < n; ++i) {
        double r = radius*pow(1.*rand()/RAND_MAX, distribution);
        double angle = double(rand()) / RAND_MAX * 2 * PI;
        points.push_back({r*cos(angle), r*sin(angle)});
    }
}

result Tester::test::check(Solution& solution) const {
    result res;
    auto start = std::chrono::steady_clock::now();
    solution.Solve();
    auto end = std::chrono::steady_clock::now();
    res.time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    auto hull=solution.GetHull();

    for (int i = 0; i < hull.size(); ++i) {
        bool found = false;
        for (const auto& p : points) {
            if (hull[i] == p) {
                found = true;
                break;
            }
        }
        if (!found) { res.success = false; return res; }
        point u = vec(hull[i], hull[(i+1)%hull.size()]);
        for (auto p : points) {
            point v = vec(hull[i], p);
            if (p == hull[(i+1)%hull.size()]) { continue; }
            if (vector_product(u, v) < 0) {res.success = false; return res;}
        }
    }
    res.success = true;
    return res;
}

void Tester::test::FillFile(const char* filename) const {
    std::ofstream ofs(filename);
    if (!ofs.good()) throw BadFile();
    for (auto p: points) {
        ofs << p.x << ' ' << p.y << std::endl;
    }
    ofs.close();
}