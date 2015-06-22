#include <cstdlib>
#include "Solver.h"
using namespace std;

int main(int argc, char** argv) {
    Solver solver(SUDOKU_FILES[3]);
    solver.solve();
    return 0;
}

