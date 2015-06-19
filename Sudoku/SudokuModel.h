#ifndef SUDOKUMODEL_H
#define	SUDOKUMODEL_H
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <string>

#include "KnownValuesRemover.h"
#include "Solver.h"
#include "SudokuUtils.h"
#include "Cell.h"
using namespace std;

class SudokuModel final {
public:

    struct cells {
        Cell* m_cells[9];
    };

    explicit SudokuModel(const string& filename);
    virtual ~SudokuModel();
    cells row_cells(u row) {
        cells the_cells;
        for (u i = 0; i < 9; ++i) {
            the_cells.m_cells[i] = &(the_sudoku[row][i]);
        }
        return the_cells;
    }
    cells column_cells(u column) {
        cells the_cells;
        for (u i = 0; i < 9; ++i) {
            the_cells.m_cells[i] = &(the_sudoku[i][column]);
        }
        return the_cells;
    }

    void solve();
    bool is_good();
    void verify();

private:
    Cell ** the_sudoku = nullptr;
    SudokuModel();

    bool good = false;
    inline u cell_val(const u& row, const u& column) {
        return cell(row, column).val();
    }
    inline Cell& cell(const u& row, const u& column) {
        return the_sudoku[row][column];
    }

};

#endif	/* SUDOKUMODEL_H */

