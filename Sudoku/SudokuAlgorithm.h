#ifndef SUDOKUALGORITHM_H
#define	SUDOKUALGORITHM_H
#include "SudokuCell.h"

class SudokuAlgorithm {
public:

    virtual void apply() = 0;

    explicit SudokuAlgorithm(SudokuCell ** sudoku) : the_sudoku(sudoku) {
    };


protected:
    //the sudoku to apply the algorithms on
    SudokuCell ** the_sudoku;

    inline u cell_val(const u& row, const u& column) {
        return cell(row, column).val();
    }

    inline SudokuCell& cell(const u& row, const u& column) {
        return the_sudoku[row][column];
    }

    inline void check_sudoku() {
        if (the_sudoku == nullptr)
            throw logic_error("The don't have sudoku to work on...");
    }
};

#endif	/* SUDOKUALGORITHM_H */

