#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "SudokuModel.h"
using namespace std;

int main(int argc, char** argv) {
    SudokuModel sm("book55.sud");
    sm.solve();
    return 0;
}

