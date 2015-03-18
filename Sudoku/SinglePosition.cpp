#include "SinglePosition.h"

/**
 * Iterate over the SudokuCells. For any known value,
 * exclude the value from the candidate lists of the cells
 *  on the same row, column or local 3x3 square.
 * @return were any candidates removed.
 */
void SinglePosition::apply() {
    check_sudoku();
    bool keep_goin = true;
    while (keep_goin) {
        keep_goin = false;
        for (u row = 0; row < 9; ++row) {
            for (u column = 0; column < 9; ++column) {
                u val = cell_val(row, column);
                if (val != 0) { // if value is known
                    if (eliminate_val(row, column, val))
                        keep_goin = true;
                }
            }
        }
    }
}

outcome SinglePosition::
eliminate_val(const u& row, const u& column, const u& val) {
    check_sudoku();
    outcome _outcome = NOTHING_FOUND;

    outcome rows = eliminate_row(row, column, val);
    outcome columns = eliminate_column(row, column, val);
    outcome squares = eliminate_square(row, column, val);

    return SudokuUtils::max(rows, SudokuUtils::max(columns, squares));

}

/**
 * Iterate over a single row.
 * Exclude current_value from each cell's list of candidates.
 * @param current_value the value to exlude
 * @param row the row to iterate over
 * @param column the column that holds current_value
 * @return whether values were excluded
 */
outcome SinglePosition::
eliminate_row(const u& row, const u & column, const u& val) {
    outcome row_outcome = NOTHING_FOUND;
    for (u column_ = 0; column_ < 9; ++column_) {
        SudokuCell& _cell = cell(row, column_);
        if (column != column_ && _cell.unknown()) {
            outcome cell_outcome = _cell.rm_cand(val);
            if (row_outcome < cell_outcome) {
                row_outcome = cell_outcome;
            }
        }
    }
    return row_outcome;
}

outcome SinglePosition::
eliminate_column(const u& row, const u & column, const u& val) {
    outcome column_outcome = NOTHING_FOUND;
    for (u row_ = 0; row_ < 9; ++row_) {
        SudokuCell& _cell = cell(row_, column);
        if (row_ != row && _cell.unknown()) {
            outcome cell_outcome = _cell.rm_cand(val);
            if (column_outcome < cell_outcome) {
                column_outcome = cell_outcome;
            }
        }
    }
    return column_outcome;
}

outcome SinglePosition::
eliminate_square(const u& row, const u & column, const u& val) {
    outcome square_outcome = NOTHING_FOUND;

    // for 0-1-2 return 0 * 3 for 3-4-5 return 1 * 3, for 6-7-8 return 2 * 3
    // used to locate the local 3x3 square begin index row and column
    u row_0 = SudokuUtils::zero_index(row);
    u clmn_0 = SudokuUtils::zero_index(column);

    for (u row_ = row_0; row_ < (row_0 + 3); ++row_) {
        for (u column_ = clmn_0; column_ < (clmn_0 + 3); ++column_) {
            SudokuCell& _cell = cell(row_, column_);
            if (_cell.unknown()) {
                outcome cell_outcome = _cell.rm_cand(val);
                if (square_outcome < cell_outcome) {
                    square_outcome = cell_outcome;
                }
            }
        }
    }
    return square_outcome;
}

