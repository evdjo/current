#include <new>

#include "SudokuModel.h"

SudokuModel::SudokuModel() {
}

SudokuModel::SudokuModel(const string& filename) {

    try {
        u_short ** arr = *(SudokuUtils::read(filename));
        the_sudoku = new SudokuCell*[9];
        for (u_short row = 0; row < 9; row++) {
            the_sudoku[row] = new SudokuCell[9];
            for (u_short column = 0; column < 9; column++) {
                get_cell_value(row, column) = arr[row][column];
            }
        }
    } catch (const bad_alloc& ba) {
        cerr << "Could not allocate memory for the sudoku...";
        throw ba;
    }
}

void SudokuModel::solve() {
    print();
    while (eliminate());
    cout << endl << endl;
    print_possible_values();
    print();
}

SudokuModel::~SudokuModel() {
    for (u_short row = 0; row < 9; row++) {
        if (the_sudoku[row] != NULL)
            delete[] the_sudoku[row];
    }
    if (the_sudoku != NULL)
        delete[] the_sudoku;
}

void SudokuModel::print() {
    for (int row = 0; row < 9; row++) {
        for (int column = 0; column < 9; column++) {
            cout << get_cell_value(row, column);
            // print space each three columns
            if ((8 - column) % 3 == 0) cout << " ";
        }
        cout << endl;
        // print new line each three columns
        if ((8 - row) % 3 == 0) cout << endl;

    }
}

void SudokuModel::print_possible_values() {

    for (u_short row = 0; row < 9; row++) {
        cout << endl;
        for (u_short column = 0; column < 9; column++) {
            cout << '[' << row;
            cout << "x";
            cout << column << ']';
            cout << "=";

            if (get_cell(row, column).is_unknown())
                get_cell(row, column).cpv.print_possible_values();
            else {
                for (u_short i = 1; i < 10; i++)
                    if (i == get_cell_value(row, column))
                        cout << get_cell_value(row, column);
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

/**
 * Iterate over the SudokuCells. For any known value, 
 * exclude the value from the candidate lists of the cells
 *  on the same row, column or local 3x3 square.
 * @return were any candidates removed.
 */
bool SudokuModel::eliminate() {
    bool change_occurred = false;
    for (u_short row = 0; row < 9; row++) {
        for (u_short column = 0; column < 9; column++) {

            u_short& val = get_cell_value(row, column);

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
bool SudokuModel::eliminate_row(const u_short& val,
        const u_short& row, const u_short& column) {
    bool change_occurred = false;

    for (u_short column_ = 0; column_ < 9; column_++) {
        if (column != column_ // unnecessary check ?
                && get_cell(row, column_).is_unknown()
                && get_cell(row, column_).remove_candidate(val)) {

            change_occurred = true;
        }
    }
    return change_occurred;
}

bool SudokuModel::eliminate_column(const u_short& val,
        const u_short& row, const u_short& column) {
    bool change_occurred = false;

    for (u_short row_ = 0; row_ < 9; row_++) {
        if (row != row_ // unnecessary check ?
                && get_cell(row_, column).is_unknown()
                && get_cell(row_, column).remove_candidate(val)) {

            change_occurred = true;
        }
    }
    return change_occurred;
}

bool SudokuModel::eliminate_3x3square(const u_short& val,
        const u_short& row, const u_short& column) {
    bool change_occurred = false;

    // for 0-1-2 return 0 * 3 for 3-4-5 return 1 * 3, for 6-7-8 return 2 * 3
    // used to locate the local 3x3 square begin index row and column
    u_short row_0 = (u_short) ((row / 3) * 3);
    u_short clmn_0 = (u_short) ((column / 3) * 3);

    for (u_short row_ = row_0; row_ < (row_0 + 3); row_++) {
        for (u_short column_ = clmn_0; column_ < (clmn_0 + 3); column_++) {


            if (row != row_ && column != column_ // unnecessary check ?
                    && get_cell(row_, column_).is_unknown()
                    && get_cell(row_, column_).remove_candidate(val)) {
                change_occurred = true;
            }
        }
    }
    return change_occurred;
}

