#ifndef SUDOKUMODEL_H
#define	SUDOKUMODEL_H
#include "SudokuCell.h"
#include <string>

typedef unsigned short u_short;

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
    u_short *** read(ifstream * input);

    inline u_short& get_cell_value(const int& row, const int& column) {
        return get_cell(row, column).value;
    }

    inline SudokuCell& get_cell(const int& row, const int& column) {
        return the_sudoku[row][column];
    }

    bool eliminate_rows();
    bool eliminate_columns();
    bool eliminate_3x3squares();
    bool eliminate_3x3square(const u_short& value,
            const u_short& row, const u_short& column);
    bool eliminate_column(const u_short& value,
            const u_short& row, const u_short& column);
    bool eliminate_row(const u_short& value,
            const u_short& row, const u_short& column);

};

#endif	/* SUDOKUMODEL_H */

