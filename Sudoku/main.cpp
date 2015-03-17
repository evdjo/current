#include <iostream>
#include <exception>
#include <execinfo.h>
#include "SudokuModel.h"
using namespace std;

int main(int argc, char** argv) {

    SudokuModel sm("book68.sud");
    sm.solve();

    sm.print_possible_values();

    sm.print();
    return 0;
}

