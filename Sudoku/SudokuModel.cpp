#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "SudokuModel.h"

void SudokuModel::solve() {

    print();
    while (eliminate_columns() || eliminate_rows() || eliminate_3x3squares());
    cout << endl << endl;
    print_possible_values();
    print();
}

SudokuModel::SudokuModel(const string& filename) {
    ifstream * input_stream = open_sudoku_file(filename);
    u_short ** arr = *read(input_stream);

    the_sudoku = new SudokuCell*[9];
    for (u_short row = 0; row < 9; row++) {
        the_sudoku[row] = new SudokuCell[9];
        for (u_short column = 0; column < 9; column++) {
            get_cell_value(row, column) = arr[row][column];
        }
    }
}

SudokuModel::~SudokuModel() {
    for (u_short row = 0; row < 9; row++) {
        if (the_sudoku[row] != NULL)
            delete[] the_sudoku[row];
    }
    if (the_sudoku != NULL)
        delete[] the_sudoku;
}

ifstream * SudokuModel::open_sudoku_file(const string & filename) {
    ifstream * input = new ifstream;
    input->open(filename.c_str());
    if (input == NULL) {
        throw invalid_argument("File not found");
    }
    return input;
}

u_short *** SudokuModel::read(ifstream * input) {

    string line;
    u_short *** array_ptr = new u_short**;
    (*array_ptr) = new u_short*[9];

    for (int row = 0; row < 9; ++row) {
        getline(*input, line);
        (*array_ptr)[row] = new u_short[9];
        for (int column = 0; column < 9; ++column) {
            const char token = line.at(column);
            if (token != ' ')
                (*array_ptr)[row][column] =
                    (u_short) strtoul(&token, NULL, 0);
        }
    }
    input->close();
    return array_ptr;
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
        for (u_short column = 0; column < 9; column++) {

            cout << "The cell at row  ";
            cout << row;
            cout << " and column ";
            cout << column;

            if (get_cell(row, column).is_unknown()) {
                cout << " could contain :   ";
                get_cell(row, column).cpv.print_possible_values();
                cout << endl;
            } else {
                cout << " contains      :    ";
                cout << get_cell_value(row, column);
                cout << endl;
            }
        }
    }
}

/**
 * Iterate over the rows. For any known value, 
 * do exclude that value from the candidate list of the cells on the same row.
 * @return were any candidates removed.
 */
bool SudokuModel::eliminate_rows() {
    bool change_occurred = false;
    for (u_short row = 0; row < 9; row++) {
        for (u_short column = 0; column < 9; column++) {

            u_short& val = get_cell_value(row, column);

            if (val != 0 && eliminate_row(val, row, column))
                change_occurred = true;

        }
    }
    return change_occurred;
}

bool SudokuModel::eliminate_columns() {
    bool change_occurred = false;
    for (u_short row = 0; row < 9; row++) {
        for (u_short column = 0; column < 9; column++) {

            u_short val = get_cell_value(row, column);

            if (val != 0 && eliminate_column(val, row, column)) {
                change_occurred = true;
            }
        }
    }
    return change_occurred;
}

bool SudokuModel::eliminate_3x3squares() {
    bool change_occurred = false;
    for (u_short current_row = 0; current_row < 9; current_row++) {
        for (u_short current_clmn = 0; current_clmn < 9; current_clmn++) {

            u_short current_value = the_sudoku[current_row][current_clmn].value;
            if (current_value != 0
                    && eliminate_3x3square
                    (current_value, current_row, current_clmn)) {
                change_occurred++;
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
bool SudokuModel::eliminate_row(const u_short& value,
        const u_short& row, const u_short& column) {
    bool change_occurred = false;

    for (u_short column_ = 0; column_ < 9; column_++) {
        if (column != column_
                && get_cell(row, column_).is_unknown()
                && get_cell(row, column_).remove_candidate(value)) {
            change_occurred = true;
        }
    }
    return change_occurred;
}

bool SudokuModel::eliminate_column(const u_short& current_value,
        const u_short& row, const u_short& column) {
    bool change_occurred = false;

    for (u_short row_ = 0; row_ < 9; row_++) {

        if (row != row_
                && get_cell(row_, column).is_unknown()
                && get_cell(row_, column).remove_candidate(current_value)) {
            change_occurred = true;
        }
    }
    return change_occurred;
}

bool SudokuModel::eliminate_3x3square(const u_short& current_value,
        const u_short& row, const u_short& column) {
    bool change_occurred = false;

    // for 0-1-2 return 0 for 3-4-5 return 1, for 6-7-8 return 2
    // used to locate the local 3x3 square begin index row and column
    u_short row_0 = (u_short) ((row / 3) * 3);
    u_short clmn_0 = (u_short) ((column / 3) * 3);

    for (u_short row_ = row_0; row_ < row_0 + 3; row_++) {
        for (u_short column_ = clmn_0; column_ < clmn_0 + 3; column_++) {


            if (row != row_ && column != column_
                    && get_cell(row_, column_).is_unknown()
                    && get_cell(row_, column_).remove_candidate(current_value)) {
                change_occurred = true;
            }
        }
    }
    return change_occurred;
}

