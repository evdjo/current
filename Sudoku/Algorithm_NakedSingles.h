#ifndef KNOWNVALUESREMOVER_H
#define	KNOWNVALUESREMOVER_H
#include "SudokuUtils.h"
#include "SudokuCell.h"

class Algorithm_NakedSingles final {
public:
    explicit Algorithm_NakedSingles(SudokuCell ** the_sudoku);
    virtual ~Algorithm_NakedSingles();
    bool eliminate_known_vals();



private:
    SudokuCell ** the_sudoku;

    bool eliminate_3x3square(const u& val, const u& row, const u& column);
    bool eliminate_column(const u& val, const u& row, const u& column);
    bool eliminate_row(const u& val, const u& row, const u& column);

    inline u cell_val(const u& row, const u& column) {
        return cell(row, column).val();
    }

    inline SudokuCell& cell(const u& row, const u& column) {
        return the_sudoku[row][column];
    }

    void set_cell_val(const u& row, const u& column,
            const u& val) {
        cell(row, column).set_val(val);
        while (eliminate_known_vals());
    }
};

#endif	/* KNOWNVALUESREMOVER_H */

