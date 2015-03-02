#include "SudokuUtils.h"

ifstream * SudokuUtils::open_sudoku_file(const string & filename) {
    ifstream * input = new ifstream;
    input->open(filename.c_str());
    if (input == NULL) {
        throw invalid_argument("File not found");
    }
    return input;
}

u_short *** SudokuUtils::read(ifstream * input) {

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
