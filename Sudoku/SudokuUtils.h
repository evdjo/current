#ifndef SUDOKUUTILS_H
#define	SUDOKUUTILS_H
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

typedef unsigned short u;

using namespace std;

class SudokuUtils final {
private:
    SudokuUtils();
public:
    /**
     * Parses the sudoku input file and returns pointer to 
     * a 9x9 array of shorts ints. Each unknown value is represented as 0.
     * @param input the ifstream pointer which leads us to the file
     * @return Pointer to 9x9 array containing the sudoku
     */
    static u *** read(const string & filename);
};

struct occurr_t {    
    u count = 0;
    u x = -1;
    u y = -1;
};


#endif	/* SUDOKUUTILS_H */

