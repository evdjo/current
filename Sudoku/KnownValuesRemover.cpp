#include "KnownValuesRemover.h"

/**
 * Iterate over the SudokuCells. For any known value,
 * exclude the value from the candidate lists of the cells
 * on the same row, column or local 3x3 square.
 * @return were any candidates removed.
 */
void KnownValuesRemover::apply() {
    check_sudoku();
    bool keep_goin = true;
    while (keep_goin) {
        keep_goin = false;
        for (u row = 0; row < 9; ++row) {
            for (u column = 0; column < 9; ++column) {
                u val = cell_val(row, column);
                SudCell& _cell = cell(row, column);
                if (!_cell.unknown()) { // if value is known
                    if (eliminate_val(_cell) != NOTHING)
                        keep_goin = true;
                }
            }
        }
    }
}

outcome KnownValuesRemover::
eliminate_val(const SudCell& origin) {
    check_sudoku();

    auto elim = [&](iter_over what) {
        return iterate_over([](SudCell & cell, const SudCell & n) {
            return (!cell.unknown()) ? NOTHING : cell.rm_cand(n.val());
        }, origin, what);
    };

    outcome rows = elim(ROW);
    outcome columns = elim(COLUMN);
    outcome squares = elim(SQUARE);

    return max(rows, max(columns, squares));
}

///**
// * Iterate over a single row. The row is determined by the passed row.
// * For each unknown cell, remove the val from its list of candidates.
// * @param val the value to exlude
// * @param row the row of the cell that is known, it has value val
// * @param column the column of the cell that is known, it has value val
// * @return whether values were excluded
// */
//outcome KnownValuesRemover::
//eliminate_row(const u& row, const u & column, const u& val) {
//    outcome row_outcome = NOTHING;
//    for (u column_ = 0; column_ < 9; ++column_) {
//        SudCell& _cell = cell(row, column_);
//        if (_cell.unknown()) {
//            outcome cell_outcome = _cell.rm_cand(val);
//            row_outcome = SudokuUtils::max(row_outcome, cell_outcome);
//        }
//    }
//    return row_outcome;
//}
//
///**
// * Iterate over a single column. The column is determined by the passed column.
// * For each unknown cell, remove the val from its list of candidates.
// * @param current_value the value to exlude
// * @param val the value to exlude
// * @param row the row of the cell that is known, it has value val
// * @param column the column of the cell that is known, it has value val
// * @return whether values were excluded
// */
//outcome KnownValuesRemover::
//eliminate_column(const u& row, const u & column, const u& val) {
//    outcome column_outcome = NOTHING;
//    for (u row_ = 0; row_ < 9; ++row_) {
//        SudCell& _cell = cell(row_, column);
//        if (_cell.unknown()) {
//            outcome cell_outcome = _cell.rm_cand(val);
//            column_outcome = SudokuUtils::max(column_outcome, cell_outcome);
//        }
//    }
//    return column_outcome;
//}
//
///**
// * Iterate over a square. The square is determined by the passed row and column.
// * For each unknown cell, remove the val from its list of candidates.
// * @param val the value to exlude
// * @param row the row of the cell that is known, it has value val
// * @param column the column of the cell that is known, it has value val
// * @return whether values were excluded
// */
//outcome KnownValuesRemover::
//eliminate_square(const u& row, const u & column, const u& val) {
//    outcome square_outcome = NOTHING;
//
//    // for 0-1-2 return 0 * 3 for 3-4-5 return 1 * 3, for 6-7-8 return 2 * 3
//    // used to locate the local 3x3 square begin index row and column
//    // those two values show us the local square of cell with known val
//    u row_0 = SudokuUtils::zero_index(row);
//    u clmn_0 = SudokuUtils::zero_index(column);
//
//    for (u row_ = row_0; row_ < (row_0 + 3); ++row_) {
//        for (u column_ = clmn_0; column_ < (clmn_0 + 3); ++column_) {
//            SudCell& _cell = cell(row_, column_);
//            if (_cell.unknown()) {
//                outcome cell_outcome = _cell.rm_cand(val);
//                square_outcome = SudokuUtils::max(square_outcome, cell_outcome);
//            }
//        }
//    }
//    return square_outcome;
//}