#include "SudokuUtils.h"

u *** SudokuUtils::read(const string & filename) {

    ifstream input(filename);
    string line;
    u *** array_ptr = new u**;
    (*array_ptr) = new u*[9];

    try {
        for (int row = 0; row < 9; ++row) {
            getline(input, line);
            (*array_ptr)[row] = new u[9];
            for (int column = 0; column < 9; ++column) {
                const char token = line.at(column);
                if (token != ' ')
                    (*array_ptr)[row][column] =
                        static_cast<u> (strtoul(&token, NULL, 0));
            }
        }
        return array_ptr;
    } catch (const bad_alloc& ba) {
        cout << "OOM while reading the file" << endl;
        throw ba;
    }

}