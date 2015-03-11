#include "Algorithm_NakedSingles.h"

Algorithm_NakedSingles::Algorithm_NakedSingles(SudokuCell ** sudoku) :
the_sudoku(sudoku) {
};

Algorithm_NakedSingles::~Algorithm_NakedSingles() {
};

/**
 * Iterate over the SudokuCells. For any known value, 
 * exclude the value from the candidate lists of the cells
 *  on the same row, column or local 3x3 square.
 * @return were any candidates removed.
 */
bool Algorithm_NakedSingles::eliminate_known_vals() {
    bool change_occurred = false;
    for (u row = 0; row < 9; ++row) {
        for (u column = 0; column < 9; ++column) {
            u val = cell_val(row, column);
            if (val != 0) {
                bool change_row = eliminate_row(val, row, column);
                bool change_column = eliminate_column(val, row, column);
                bool change_3x3 = eliminate_3x3square(val, row, column);
                if (change_row || change_column || change_3x3)
                    change_occurred = true;

            }
        }
    }
    return change_occurred;
}

/**
 * Iterate over a single row. 
 * Exclude current_value from each cell's list of candidates.
 * @param current_value the value to exlude
 * @param row the row to iterate over
 * @param column the column that holds current_value
 * @return  whether values were excluded
 */
bool Algorithm_NakedSingles::eliminate_row(const u& val,
        const u& row, const u & column) {
    bool change_occurred = false;

    for (u column_ = 0; column_ < 9; ++column_) {
        if (column != column_ // unnecessary check ?
                && cell(row, column_).unknown()
                && cell(row, column_).rm_candidate(val)) {

            change_occurred = true;
        }
    }
    return change_occurred;
}

bool Algorithm_NakedSingles::eliminate_column(const u& val,
        const u& row, const u & column) {
    bool change_occurred = false;

    for (u row_ = 0; row_ < 9; ++row_) {
        if (row != row_ // unnecessary check ?
                && cell(row_, column).unknown()
                && cell(row_, column).rm_candidate(val)) {

            change_occurred = true;
        }
    }
    return change_occurred;
}

bool Algorithm_NakedSingles::eliminate_3x3square(const u& val,
        const u& row, const u & column) {
    bool change_occurred = false;

    // for 0-1-2 return 0 * 3 for 3-4-5 return 1 * 3, for 6-7-8 return 2 * 3
    // used to locate the local 3x3 square begin index row and column
    u row_0 = (u) ((row / 3) * 3);
    u clmn_0 = (u) ((column / 3) * 3);

    for (u row_ = row_0; row_ < (row_0 + 3); ++row_) {
        for (u column_ = clmn_0; column_ < (clmn_0 + 3); ++column_) {


            if (row != row_ && column != column_ // unnecessary check ?
                    && cell(row_, column_).unknown()
                    && cell(row_, column_).rm_candidate(val)) {
                change_occurred = true;
            }
        }
    }
    return change_occurred;
}

