#ifndef SUDOKUMODEL_H
#define	SUDOKUMODEL_H
#include <string>
#include <stdexcept>
#include "Algorithm_NakedSingles.h"
#include "Algorithm_HiddenSingles.h"
#include "SudokuUtils.h"
using namespace std;

class SudokuModel final {
public:
    void solve();
    explicit SudokuModel(const string& filename);
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
    SudokuModel();

    inline u cell_val(const u& row, const u& column) {
        return cell(row, column).val();
    }

    inline SudokuCell& cell(const u& row, const u& column) {
        return the_sudoku[row][column];
    }

};

#endif	/* SUDOKUMODEL_H */

