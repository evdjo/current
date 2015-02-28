#ifndef SUDOKUMODEL_H
#define	SUDOKUMODEL_H
#include "SudokuCell.h"
#include <string>

using namespace std;

class SudokuModel {
public:
    void solve();
    SudokuModel(const string& filename);
    virtual ~SudokuModel();
    
    /**
     * Utility method that prints the sudoku.
     */
    void print();
    /**
     * Utility method that prints either the cell value,
     * or if unknown it prints the candidates.
     */
    void print_possible_values();

private:
    SudokuCell ** the_sudoku;



    /**
     * Get a handle of the sudoku file.
     * @param filename path to the file containing the sudoku
     * @return ifstream pointer to the file
     */
    ifstream * open_sudoku_file(const string& filename);
    /**
     * Parses the sudoku input file and returns pointer to 
     * a 9x9 array of shorts ints. Each unknown value is represented as 0.
     * @param input the ifstream pointer which leads us to the file
     * @return Pointer to 9x9 array containing the sudoku
     */
    unsigned short *** read(ifstream * input);

    bool eliminate_rows();
    bool eliminate_columns();
    bool eliminate_3x3squares();
    bool eliminate_3x3square(const unsigned short& current_value,
            const unsigned short& current_row, const unsigned short& current_clmn);
    bool eliminate_column(const unsigned short& current_value,
            const unsigned short& current_row, const unsigned short& current_clmn);
    bool eliminate_row(const unsigned short& current_value,
            const unsigned short& row_index, const unsigned short& clmn_index);

};

#endif	/* SUDOKUMODEL_H */

