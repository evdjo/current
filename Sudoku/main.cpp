#include "SudokuModel.h"
#include <iostream>
#include <exception>
#include <execinfo.h>
using namespace std;

int main(int argc, char** argv) {
    SudokuModel sm("book68.sud");
    sm.solve();
    return 0;
}

