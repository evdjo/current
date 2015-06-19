#include <iostream>
#include <exception>
#include <execinfo.h>
#include "SudokuModel.h"
#include "SudokuUtils.h"
#include "Cell.h"
using namespace std;

int main(int argc, char** argv) {
    SudokuModel sm("input/sudoku2993.sud");
    if (sm.is_good()) {
        sm.solve();
        sm.verify();
    } else {
        cerr << "Failed to solve the sudoku..."
                " see the above error message for more detail" << endl;
    }
    return 0;
}

//    if (argc == 2) {
//        SudokuModel sm(argv[1]);
//        if (sm.is_good()) {
//            sm.solve();
//            sm.verify();
//        } else {
//            cerr << "Failed to solve the sudoku..."
//                    " see the above error message for more detail" << endl;
//        }
//    } else {
//        cerr << "You must pass the path to the sudoku input file." << endl;
//    }