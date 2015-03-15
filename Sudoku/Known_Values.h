#ifndef KNOWNVALUESREMOVER_H
#define	KNOWNVALUESREMOVER_H
#include "SudokuUtils.h"
#include "SudokuCell.h"

class Known_Values final {
public:
    explicit Known_Values(SudokuCell ** the_sudoku);
    virtual ~Known_Values();
    void eliminate_known_vals();
    bool eliminate_val(const u& row, const u& column, const u& val);
    
  

private:
    SudokuCell ** the_sudoku;

    bool squares(const u& row, const u& column, const u& val);
    bool columns(const u& row, const u& column, const u& val);
    bool rows(const u& row, const u& column, const u& val);

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

