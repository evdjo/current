#ifndef KNOWNVALUESREMOVER_H
#define	KNOWNVALUESREMOVER_H
#include "SudokuUtils.h"
#include "SudokuCell.h"

class Algorithm_NakedSingles final {
public:
    explicit Algorithm_NakedSingles(SudokuCell ** the_sudoku);
    virtual ~Algorithm_NakedSingles();
    bool eliminate_known_vals();
    bool eliminate_val(const u& row, const u& column, const u& val);



private:
    SudokuCell ** the_sudoku;

    bool eliminate_3x3square(const u& row, const u& column, const u& val);
    bool eliminate_column(const u& row, const u& column, const u& val);
    bool eliminate_row(const u& row, const u& column, const u& val);

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

#endif	/* KNOWNVALUESREMOVER_H */

