#include "SudokuModel.h"
#include <exception>
#include <execinfo.h>
using namespace std;

int main(int argc, char** argv) {
    SudokuModel sm("book55.sud");

    sm.solve();    
       
    return 0;
}

