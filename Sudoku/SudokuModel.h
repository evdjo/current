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

    inline u_short cell_val(const u_short& row, const u_short& column) {
        return cell(row, column).val;
    }

    void set_cell_val(const u_short& row, const u_short& column,
            const u_short& val) {
        cell(row, column).set_val(val);
        while (eliminate_known_vals());
    }

    inline SudokuCell& cell(const u_short& row, const u_short& column) {
        return the_sudoku[row][column];
    }

    // Easy Techniques 
    bool eliminate_known_vals();
    bool eliminate_3x3square(const u_short& val,
            const u_short& row, const u_short& column);
    bool eliminate_column(const u_short& val,
            const u_short& row, const u_short& column);
    bool eliminate_row(const u_short& val,
            const u_short& row, const u_short& column);
    bool hidden_singles_rows();
    bool hidden_singles_columns();
    void check_cell_candidates(const u_short& row,
            const u_short& column, u_short* values);

    //Medium Techniques 
    bool naked_pairs();

};

#endif	/* SUDOKUMODEL_H */

