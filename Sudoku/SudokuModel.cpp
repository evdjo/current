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
    if (the_sudoku == nullptr) return;
    
    Known_Values kvr(the_sudoku);
    HiddenSingles_NakedPairs hs(the_sudoku);
    
    kvr.eliminate_known_vals(); 
    hs.seek_hidden_singles();
  

    print_possible_values();
    print();
}

void SudokuModel::print() {
    if (the_sudoku == nullptr) return;
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
    if (the_sudoku == nullptr) return;
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
                        cout << " "; // << cell_val(row, column);
                    else
                        cout << " ";
            }
            cout << " ";

            // print space each three columns
            if ((8 - column) % 3 == 0) cout << "   ";
        }

        // print new line each three columns
        if ((8 - row) % 3 == 0) cout << endl << endl;

    }
}