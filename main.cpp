#include <iostream>
#include <fstream>
#include "solution.h"
#include "tester.h"
#define out "output2.txt"
#define in "input.txt"
#define out_naive "output_naive.txt"
#define out_optimal "output_optimal.txt"

int main() {
    //     NaiveSolution solution1("/home/kirillr/Convex_Hull/input.txt");
    //     OptimalSolution solution2("/home/kirillr/Convex_Hull/input.txt");
    //     solution1.Solve(); solution2.Solve();
    //     solution1.PrintResult("/home/kirillr/Convex_Hull/output_naive.txt");
    //     solution2.PrintResult("/home/kirillr/Convex_Hull/output_optimal.txt");
    //Visualiser visualiser;
    //Tester tester(100, 100000, 1e9, 1);
    //Tester tester;
    //try{
    Tester tester(0, 100);
    tester.RunTests();
    tester.PrintResults(out);
    //} catch (const BadCase& e) {
    //     e.what();
    //}
    return 0;
}