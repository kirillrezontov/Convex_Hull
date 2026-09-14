#include <iostream>
#include <fstream>
#include "solution.h"
#include "tester.h"
#define out "output.txt"
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
    try{
        Tester tester(100, 100000, 1e9, 1);
        //Tester tester;
        tester.RunTests();
        tester.PrintResults(out);
    }
    catch (BadFile& e) {
        e.what();
    }
    catch (BadIndex& e) {
        e.what();
    }
    catch (BadAlloc& e) {
        e.what();
    }
    return 0;
}