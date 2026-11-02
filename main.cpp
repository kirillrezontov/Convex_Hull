#include <iostream>
#include <fstream>
#include "solution.h"
#include "ptester.h"
#define out "output.txt"
#define in "input.txt"
#define out_naive "output_naive.csv"
#define out_optimal "output_optimal.csv"

int main() {
    //     NaiveSolution solution1("/home/kirillr/Convex_Hull/input.txt");
    //     OptimalSolution solution2("/home/kirillr/Convex_Hull/input.txt");
    //     solution1.Solve(); solution2.Solve();
    //     solution1.PrintResult("/home/kirillr/Convex_Hull/output_naive.txt");
    //     solution2.PrintResult("/home/kirillr/Convex_Hull/output_optimal.txt");
    //Visualiser visualiser;
    // try{
        ptester tester(100, 10, 100000, 1, 1e9);
        //Tester tester;
        tester.run_tests();
        tester.write_naive_results(out_naive);
        tester.write_optimal_results(out_optimal);
    //} catch (const BadFile& e) {
    //     e.what();
    // }
    return 0;
}