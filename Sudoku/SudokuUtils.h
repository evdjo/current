#ifndef SUDOKUUTILS_H
#define	SUDOKUUTILS_H
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

typedef unsigned short u_short;

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
    static u_short *** read(const string & filename);  
};

#endif	/* SUDOKUUTILS_H */

