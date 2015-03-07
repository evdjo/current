#ifndef SUDOKUMODEL_H
#define	SUDOKUMODEL_H
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "SudokuUtils.h"
#include "SudokuCell.h"
using namespace std;

class SudokuModel final {
    friend class SudokuAlgorithmsTest;
public:
    void solve();
    explicit SudokuModel(const string& filename);
    virtual ~SudokuModel();
    
    
private:
    SudokuCell ** the_sudoku;
    SudokuModel();
    
    /**
     * Utility method that prints the sudoku.
     */
    void print();
    /**
     * Utility method that prints either the cell value,
     * or if unknown it prints the candidates.
     */
    void print_possible_values();
    

    inline u_short& get_cell_value(const u_short& row, const u_short& column) {
        return get_cell(row, column).value;
    }

    inline SudokuCell& get_cell(const u_short& row, const u_short& column) {
        return the_sudoku[row][column];
    }

    bool eliminate();
    bool eliminate_3x3square(const u_short& val,
            const u_short& row, const u_short& column);
    bool eliminate_column(const u_short& val,
            const u_short& row, const u_short& column);
    bool eliminate_row(const u_short& val,
            const u_short& row, const u_short& column);

};

#endif	/* SUDOKUMODEL_H */

