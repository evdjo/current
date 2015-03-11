#include "SudokuModel.h"
#include <exception>
#include <execinfo.h>
using namespace std;

int main(int argc, char** argv) {
    SudokuModel sm("book58.sud");
    sm.solve();   
    sm.print_possible_values();
    
    sm.print();
    
       
    return 0;
}

