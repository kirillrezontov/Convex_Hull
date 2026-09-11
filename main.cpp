#include "solution.h"

int main() {
    OptimalSolution solution("/home/kirillr/Convex_Hull/input.txt");
    solution.Solve();
    solution.PrintResult("/home/kirillr/Convex_Hull/output.txt");
    //Visualiser visualiser;
    return 0;
}