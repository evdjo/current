#include "KnownValues.h"

/**
 * Iterate over the SudokuCells. For any known value,
 * exclude the value from the candidate lists of the cells
 *  on the same row, column or local 3x3 square.
 * @return were any candidates removed.
 */
void KnownValues::apply() {
    check_sudoku();
    bool keep_goin = true;
    while (keep_goin) {
        keep_goin = false;
        for (u row = 0; row < 9; ++row) {
            for (u column = 0; column < 9; ++column) {
                u val = cell_val(row, column);
                if (val != 0) { // if value is known
                    keep_goin = eliminate_val(row, column, val) ?
                            true : keep_goin;
                }
            }
        }
    }
}

bool KnownValues::eliminate_val(const u& row, const u& column, const u& val) {
    check_sudoku();
    bool _rows = rows(row, column, val);
    bool _columns = columns(row, column, val);
    bool _squares = squares(row, column, val);
    return (_rows || _columns || _squares);
}

/**
 * Iterate over a single row.
 * Exclude current_value from each cell's list of candidates.
 * @param current_value the value to exlude
 * @param row the row to iterate over
 * @param column the column that holds current_value
 * @return whether values were excluded
 */
bool KnownValues::rows(const u& row, const u & column, const u& val) {
    bool change_occurred = false;

    for (u column_ = 0; column_ < 9; ++column_) {
        if (cell(row, column_).unknown()
                && cell(row, column_).rm_candidate(val)) {

            change_occurred = true;
        }
    }
    return change_occurred;
}

bool KnownValues::columns(const u& row, const u & column, const u& val) {
    bool change_occurred = false;

    for (u row_ = 0; row_ < 9; ++row_) {
        if (cell(row_, column).unknown()
                && cell(row_, column).rm_candidate(val)) {
            change_occurred = true;
        }
    }
    return change_occurred;
}

bool KnownValues::squares(const u& row, const u & column, const u& val) {
    bool change_occurred = false;

    // for 0-1-2 return 0 * 3 for 3-4-5 return 1 * 3, for 6-7-8 return 2 * 3
    // used to locate the local 3x3 square begin index row and column
    u row_0 = SudokuUtils::zero_index(row);
    u clmn_0 = SudokuUtils::zero_index(column);

    for (u row_ = row_0; row_ < (row_0 + 3); ++row_) {
        for (u column_ = clmn_0; column_ < (clmn_0 + 3); ++column_) {
            if (cell(row_, column_).unknown()
                    && cell(row_, column_).rm_candidate(val)) {

                change_occurred = true;
            }
        }
    }
    return change_occurred;
}

