#include <iostream>
#include <exception>
#include <execinfo.h>
#include "SudokuModel.h"
using namespace std;

int main(int argc, char** argv) {
    SudokuModel sm("input/book65.sud");
    sm.solve();
    return 0;
}

