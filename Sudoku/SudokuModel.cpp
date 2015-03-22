#include "SudokuModel.h"
using namespace std;

bool SudokuModel::is_good() {
    return good;
}

void SudokuModel::verify() {
    if (the_sudoku == nullptr) cerr << "Can't verify" << endl;

    for (u i = 0; i < 9; ++i) {
        u occur_row[9]{0};
        u occur_column[9]{0};
        for (u o = 0; o < 9; ++o) {
            ++occur_row[cell_val(i, o) - 1];
            ++occur_column[cell_val(o, i) - 1];
        }

        for (u o = 0; o < 9; ++o) {
            if (occur_row[o] != 1) {
                cout << "Verify failed..." << endl;
                cout << o << " row, has occurrence > 1" << endl;
                return;
            } else if (occur_column[o] != 1) {
                cout << "Verify failed..." << endl;
                cout << o << " column, has occurrence > 1" << endl;
                return;
            }
        }
    }
    cout << "Sudoku verified - OK." << endl;
}

SudokuModel::SudokuModel(const string& filename) {
    ifstream input(filename);
    if (!input.good()) {
        cerr << "Could not open the Sudoku file." << endl;
        return;
    }

    string lines[9];
    for (u i = 0; i < 9; ++i) {
        getline(input, lines[i]);
        if (lines[i].length() != 9) {
            cerr << "Input file is not a valid sudoku." << endl;
            return;
        }
    }
    try {
        the_sudoku = new SudokuCell*[9];
        for (u row = 0; row < 9; row++) {
            the_sudoku[row] = new SudokuCell[9];
            for (u column = 0; column < 9; column++) {
                const char val = lines[row].at(column);
                u val_ = (val == ' ') ? 0 : val - '0';
                if (val_ >= 0 && val_ < 10) {
                    the_sudoku[row][column].init(val_);
                } else {
                    cerr << "Input file is not a valid sudoku." << endl;
                    return;
                }
            }
        }
        good = true;
    } catch (const bad_alloc& ba) {
        cerr << "Could not allocate memory for the sudoku..." << endl;
        throw ba;
    }
}

SudokuModel::~SudokuModel() {
    if (the_sudoku == nullptr) return;
    for (u row = 0; row < 9; row++) {
        if (the_sudoku[row] != nullptr)
            delete[] the_sudoku[row];
    }
    if (the_sudoku != nullptr)
        delete[] the_sudoku;
}

void SudokuModel::solve() {
    if (the_sudoku == nullptr) return;
    SinglePosition kv(the_sudoku);
    SingleCandidate sc(the_sudoku);
    kv.apply();
    sc.apply();
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
                cell(row, column).print_possible_values();
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