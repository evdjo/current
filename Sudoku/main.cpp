#include "SudokuModel.h"
#include "SudokuAlgorithm.h"
#include <iostream>
#include <exception>
#include <execinfo.h>
#include "SudokuAlgorithm.h"
using namespace std;

int main(int argc, char** argv) {
    SudokuModel sm("book68.sud");
    //    SudokuModel sm("book55.sud");
    sm.solve();


    return 0;
}

