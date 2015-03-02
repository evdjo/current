#ifndef SUDOKUUTILS_H
#define	SUDOKUUTILS_H
#ifndef SUDOKUUTILS_HPP
#define	SUDOKUUTILS_HPP
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
     * Get a handle of the sudoku file.
     * @param filename path to the file containing the sudoku
     * @return ifstream pointer to the file
     */
   static ifstream * open_sudoku_file(const string & filename);
    /**
     * Parses the sudoku input file and returns pointer to 
     * a 9x9 array of shorts ints. Each unknown value is represented as 0.
     * @param input the ifstream pointer which leads us to the file
     * @return Pointer to 9x9 array containing the sudoku
     */
   static u_short *** read(ifstream * input);
#endif	/* SUDOKUUTILS_HPP */
};

#endif	/* SUDOKUUTILS_H */

