#include "SudokuModel.h"

SudokuModel::SudokuModel() {
}

SudokuModel::SudokuModel(const string& filename) {

    try {
        u ** arr = *(SudokuUtils::read(filename));
        the_sudoku = new SudokuCell*[9];
        for (u row = 0; row < 9; row++) {
            the_sudoku[row] = new SudokuCell[9];
            for (u column = 0; column < 9; column++) {
                u& val = arr[row][column];
                the_sudoku[row][column].init_val(val);
            }
        }
    } catch (const bad_alloc& ba) {
        cerr << "Could not allocate memory for the sudoku...";
        throw ba;
    }
}

SudokuModel::~SudokuModel() {
    for (u row = 0; row < 9; row++) {
        if (the_sudoku[row] != NULL)
            delete[] the_sudoku[row];
    }
    if (the_sudoku != NULL)
        delete[] the_sudoku;
}

void SudokuModel::solve() {
    while (eliminate_known_vals() ||
            hidden_singles_rows() ||
            hidden_singles_columns() ||
            hidden_singles_3x3());
    
    print();


}

bool SudokuModel::hidden_singles_rows() {
    bool singles_found = false;
    for (u row = 0; row < 9; ++row) {
        occurr_t occurrences[9];
        for (u column = 0; column < 9; ++column) {
            if (!cell(row, column).unknown()) continue;
            count_occurences(row, column, occurrences);
        }

        singles_found = lock_single_candidates(occurrences)
                ? true : singles_found;
        if (singles_found) while (eliminate_known_vals());

    }
    return singles_found;
}

bool SudokuModel::hidden_singles_columns() {
    bool singles_found = false;
    for (u column = 0; column < 9; ++column) {
        occurr_t occurrences[9];
        for (u row = 0; row < 9; ++row) {
            if (!cell(row, column).unknown()) continue;
            count_occurences(row, column, occurrences);
        }
        singles_found = lock_single_candidates(occurrences)
                ? true : singles_found;
        if (singles_found) while (eliminate_known_vals());

    }
    return singles_found;
}

bool SudokuModel::hidden_singles_3x3() {
    bool singles_found = false;
    for (u srow = 0; srow <= 6; srow += 3) {
        for (u scmn = 0; scmn <= 6; scmn += 3) {

            occurr_t occurrences[9];

            for (u row = srow; row < srow + 3; ++row) {
                for (u cmn = scmn; cmn < scmn + 3; ++cmn) {
                    if (!cell(row, cmn).unknown()) continue;
                    count_occurences(row, cmn, occurrences);
                }
            }
            singles_found = lock_single_candidates(occurrences)
                    ? true : singles_found;
            if (singles_found) while (eliminate_known_vals());

        }
    }
    return singles_found;
}

bool SudokuModel::
lock_single_candidates(const occurr_t* ocr) {
    bool singles_found = false;
    for (u i = 0; i < 9; ++i) {
        if (ocr[i].count == 1) {
            singles_found = true;
            cell(ocr[i].x, ocr[i].y).set_val(i + 1);
        }
    }
    return singles_found;
}

void SudokuModel::count_occurences
(const u& row, const u& column, occurr_t * occurrences) {

    SudokuCell& sc = cell(row, column);
    for (u i = 0; i < 9; ++i) {
        if (sc.is_candidate(i + 1)) {
            occurrences[i].count++;
            occurrences[i].x = row;
            occurrences[i].y = column;
        }
    }
}

/**
 * Iterate over the SudokuCells. For any known value, 
 * exclude the value from the candidate lists of the cells
 *  on the same row, column or local 3x3 square.
 * @return were any candidates removed.
 */
bool SudokuModel::eliminate_known_vals() {
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
bool SudokuModel::eliminate_row(const u& val,
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

bool SudokuModel::eliminate_column(const u& val,
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

bool SudokuModel::eliminate_3x3square(const u& val,
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

bool SudokuModel::naked_pairs() {
    //TODO 
    return false;
}

void SudokuModel::print() {
    for (u row = 0; row < 9; row++) {
        for (u column = 0; column < 9; column++) {
            cout << cell_val(row, column);
            // print space each three columns
            if ((8 - column) % 3 == 0) cout << " ";
        }
        cout << endl;
        // print new line each three columns
        if ((8 - row) % 3 == 0) cout << endl;

    }
}

void SudokuModel::print_possible_values() {

    for (u row = 0; row < 9; row++) {
        cout << endl;
        for (u column = 0; column < 9; column++) {
            cout << '[' << row;
            cout << "x";
            cout << column << ']';
            cout << "=";

            if (cell(row, column).unknown())
                cell(row, column).print();
            else {
                for (u i = 1; i < 10; i++)
                    if (i == cell_val(row, column))
                        cout << cell_val(row, column);
                    else
                        cout << " ";
            }
            cout << "   ";

            // print space each three columns
            if ((8 - column) % 3 == 0) cout << "   ";
        }

        // print new line each three columns
        if ((8 - row) % 3 == 0) cout << endl;

    }
}