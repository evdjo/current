#include "SudokuUtils.h"

u_short *** SudokuUtils::read(const string & filename) {

    ifstream input(filename);
    string line;
    u_short *** array_ptr = new u_short**;
    (*array_ptr) = new u_short*[9];

    try {
        for (int row = 0; row < 9; ++row) {
            getline(input, line);
            (*array_ptr)[row] = new u_short[9];
            for (int column = 0; column < 9; ++column) {
                const char token = line.at(column);
                if (token != ' ')
                    (*array_ptr)[row][column] =
                        static_cast<u_short> (strtoul(&token, NULL, 0));
            }
        }
        return array_ptr;
    } catch (const bad_alloc& ba) {
        cout << "OOM while reading the file" << endl;
        throw ba;
    }

}