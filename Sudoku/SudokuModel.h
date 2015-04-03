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
#include "SudCell.h"
using namespace std;

class SudokuModel final {
public:
    explicit SudokuModel(const string& filename);
    virtual ~SudokuModel();

    void solve();
    bool is_good();
    void verify();

private:
    SudCell ** the_sudoku = nullptr;
    SudokuModel();

    bool good = false;
    inline u cell_val(const u& row, const u& column) {
        return cell(row, column).val();
    }
    inline SudCell& cell(const u& row, const u& column) {
        return the_sudoku[row][column];
    }

};

#endif	/* SUDOKUMODEL_H */

