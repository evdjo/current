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
                cout << i << " row, " << o + 1 << " has occurrence > 1" << endl;
                return;
            } else if (occur_column[o] != 1) {
                cout << "Verify failed..." << endl;
                cout << i << " column, " << o + 1 << " has occurrence > 1" << endl;
                return;
            }
        }
    }
    for (u i = 0; i < 7; i += 3) {
        for (u o = 0; o < 7; o += 3) {

            u occur_square[9]{0};
            for (u z = i; z < i + 3; ++z) {
                for (u q = o; q < o + 3; ++q) {
                    ++occur_square[cell_val(z, q) - 1];
                }
            }

            for (u o = 0; o < 9; ++o) {
                if (occur_square[o] != 1) {
                    cout << "Verify failed..." << endl;
                    cout << i << o << " square , ";
                    cout << o + 1 << " has occurrence > 1" << endl;
                    return;
                }
            }
        }
    }
    cout << "Sudoku verified - OK." << endl;
}

SudokuModel::SudokuModel(const string & filename) {
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
        the_sudoku = new Cell*[9];
        for (u row = 0; row < 9; row++) {
            the_sudoku[row] = new Cell[9];
            for (u column = 0; column < 9; column++) {
                const char val = lines[row].at(column);
                u val_ = (val == ' ') ? 0 : val - '0';
                if (val_ >= 0 && val_ < 10) {
                    the_sudoku[row][column].init(val_, row, column);
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
    KnownValuesRemover kv(the_sudoku);
    Solver solver(the_sudoku);
    solver.apply();
    //    solver.debug_flag = true;
    //    print_possible_values();
}