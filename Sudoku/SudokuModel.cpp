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
    unsigned short ** arr = *read(input_stream);

    the_sudoku = new SudokuCell*[9];
    for (unsigned short row = 0; row < 9; row++) {
        the_sudoku[row] = new SudokuCell[9];
        for (unsigned short column = 0; column < 9; column++) {
            the_sudoku[row][column].value = arr[row][column];
        }
    }
}

SudokuModel::~SudokuModel() {
    for (unsigned short row = 0; row < 9; row++) {
        if (the_sudoku[row] != NULL)
            delete[] the_sudoku[row];
    }
}

ifstream * SudokuModel::open_sudoku_file(const string & filename) {
    ifstream * input = new ifstream;
    input->open(filename.c_str());
    if (input == NULL) {
        throw invalid_argument("File not found");
    }
    return input;
}

unsigned short *** SudokuModel::read(ifstream * input) {

    string line;
    unsigned short *** array_ptr = new unsigned short**;
    (*array_ptr) = new unsigned short*[9];

    for (int row = 0; row < 9; ++row) {
        getline(*input, line);
        (*array_ptr)[row] = new unsigned short[9];
        for (int column = 0; column < 9; ++column) {
            const char token = line.at(column);
            if (token != ' ') (*array_ptr)[row][column] = (unsigned short) strtoul(&token, NULL, 0);
        }
    }
    input->close();
    return array_ptr;
}

void SudokuModel::print() {
    for (int row = 0; row < 9; row++) {
        for (int column = 0; column < 9; column++) {
            cout << the_sudoku[row][column].value;
            if ((8 - column) % 3 == 0) cout << " "; // print space each three columns
        }
        cout << endl;
        if ((8 - row) % 3 == 0) cout << endl; // print new line each three columns
    }
}

void SudokuModel::print_possible_values() {

    for (unsigned short i = 0; i < 9; i++) {
        for (unsigned short z = 0; z < 9; z++) {

            if (the_sudoku[i][z].is_unknown()) {
                cout << "The cell at row  " << i << " and column " << z << " could contain :   ";
                the_sudoku[i][z].cpv.print_possible_values();
                cout << endl;
            } else {
                cout << "The cell at row  " << i << " and column " << z << "  contains     :    ";
                cout << the_sudoku[i][z].value;
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
    int changes_occurred = 0;
    for (unsigned short current_row = 0; current_row < 9; current_row++) {
        for (unsigned short current_clmn = 0; current_clmn < 9; current_clmn++) {

            unsigned short current_value = the_sudoku[current_row][current_clmn].value;

            if (current_value != 0
                    && eliminate_row(current_value, current_row, current_clmn)) {
                changes_occurred++;
            }
        }
    }
    return changes_occurred != 0;
}

/**
 * Iterate over a single row. 
 * Exclude current_value from each cell's list of candidates.
 * @param current_value the value to exlude
 * @param current_row the row to iterate over
 * @param current_clmn the column that holds current_value
 * @return  whether values were excluded
 */
bool SudokuModel::eliminate_row(const unsigned short& current_value,
        const unsigned short& current_row, const unsigned short& current_clmn) {
    int changes_occurred = 0;

    for (unsigned short i = 0; i < 9; i++) {
        if (current_clmn != i // when this is true we've gotten to the cell we're getting the current_value from - skip this cell
                && the_sudoku[current_row][i].is_unknown()) {
            if (the_sudoku[current_row][i].remove(current_value)) {
                changes_occurred++;
            }
        }
    }

    return changes_occurred != 0;
}

bool SudokuModel::eliminate_columns() {
    int changes_occurred = 0;
    for (unsigned short current_row = 0; current_row < 9; current_row++) {
        for (unsigned short current_clmn = 0; current_clmn < 9; current_clmn++) {

            unsigned short current_value = the_sudoku[current_row][current_clmn].value;

            if (current_value != 0
                    && eliminate_column(current_value, current_row, current_clmn)) {
                changes_occurred++;
            }
        }
    }
    return changes_occurred != 0;
}

bool SudokuModel::eliminate_column(const unsigned short& current_value,
        const unsigned short& current_row, const unsigned short& current_clmn) {
    int changes_occurred = 0;

    for (unsigned short i = 0; i < 9; i++) {
        if (current_row != i // when this is true we've gotten to the cell we're getting the current_value from - skip this cell
                && the_sudoku[i][current_clmn].is_unknown()) {
            if (the_sudoku[i][current_clmn].remove(current_value)) {
                changes_occurred++;
            }
        }
    }

    return changes_occurred != 0;
}

bool SudokuModel::eliminate_3x3squares() {
    int changes_occurred = 0;
    for (unsigned short current_row = 0; current_row < 9; current_row++) {
        for (unsigned short current_clmn = 0; current_clmn < 9; current_clmn++) {

            unsigned short current_value = the_sudoku[current_row][current_clmn].value;
            if (current_value != 0
                    && eliminate_3x3square(current_value, current_row, current_clmn)) {
                changes_occurred++;
            }
        }
    }
    return changes_occurred != 0;

}

bool SudokuModel::eliminate_3x3square(const unsigned short& current_value,
        const unsigned short& current_row, const unsigned short& current_clmn) {
    int changes_occurred = 0;

    unsigned short square_row_start = (unsigned short) ((current_row / 3) * 3); // for 0, 1 and 2 return 0, for 3,4 and 5  return 1
    unsigned short square_clmn_start = (unsigned short) ((current_clmn / 3) * 3); // for 6, 7 and 8 return 2

    for (unsigned short local_row = square_row_start; local_row < square_row_start + 3; local_row++) {
        for (unsigned short local_clmn = square_clmn_start; local_clmn < square_clmn_start + 3; local_clmn++) {


            if (current_row != local_row && current_clmn != local_clmn) {
                if (the_sudoku[local_row][local_clmn].is_unknown()) {
                    if (the_sudoku[local_row][local_clmn].remove(current_value)) {
                        changes_occurred++;
                    }
                }
            }
        }
    }
    return changes_occurred != 0;
}

