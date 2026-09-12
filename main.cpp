#include "solution.h"
#include "tester.h"
#define out "/home/kirillr/Convex_Hull/output.txt"

int main() {
//     NaiveSolution solution1("/home/kirillr/Convex_Hull/input.txt");
//     OptimalSolution solution2("/home/kirillr/Convex_Hull/input.txt");
//     solution1.Solve(); solution2.Solve();
//     solution1.PrintResult("/home/kirillr/Convex_Hull/output_naive.txt");
//     solution2.PrintResult("/home/kirillr/Convex_Hull/output_optimal.txt");
    //Visualiser visualiser;
    Tester tester(10, 10, 10, 1);
    //Tester tester;
    tester.RunTests();
    tester.PrintResults(out);
    return 0;
}