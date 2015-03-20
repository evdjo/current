#include "SudokuUtils.h"

void SudokuUtils::read(const string & filename, u *** array_ptr) {

    ifstream input(filename);
    if (!input.good()) {
        throw logic_error("Sudoku file not found");
    }
    string line;
    try {
        for (int row = 0; row < 9; ++row) {
            getline(input, line);
            for (int column = 0; column < 9; ++column) {
                const char token = line.at(column);
                if (token == ' ') {
                    (*array_ptr)[row][column] = static_cast<u> (0);
                } else {
                    (*array_ptr)[row][column] = static_cast<u> (token - '0');
                }
            }
        }
    } catch (const exception& e) {
        cout << "Error whilst reading the sudoku file..." << endl;
        throw e;
    }

}

u SudokuUtils::zero_index(const u& index) {
    return static_cast<u> ((index / 3) * 3);
}

outcome SudokuUtils::max(const outcome& first, const outcome& second) {
    return first >= second ? first : second;
}