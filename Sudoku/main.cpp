#include <iostream>
#include <exception>
#include <execinfo.h>
#include "SudokuModel.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc == 2) {
        SudokuModel sm(argv[1]);
        if (sm.is_good()) {
            sm.solve();
            sm.print();
            sm.verify();
        } else {
            cerr << "Failed to solve the sudoku..."
                    " see the above error message for more detail" << endl;
        }
    } else {
        cerr << "You must pass the path to the sudoku input file." << endl;
    }
    return 0;
}

