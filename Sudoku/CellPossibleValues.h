#ifndef CELLPOSSIBLEVALUES_H
#define	CELLPOSSIBLEVALUES_H
#include <stdexcept>
#include <iostream>
using namespace std;

class CellPossibleValues {
    friend class SudokuCell;
    friend class SudokuModel;
    friend class CellPossibleValuesTest;

private:
    /**
     * Array to keep track of the possible values for a cell. 
     * Value of 0 at a position means that the position's original value
     * is not a candidate anymore.
     */
    unsigned short possible_values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    /**
     * Integer to keep track of how many possible valule are there left.
     */
    int count_possible_values = 9;

    bool remove(const unsigned short& removed_value) {
        if (solved()) {
            throw logic_error("Only one value left - cannot remove!");
        }
        if (possible_values[removed_value - 1] == removed_value) {
            possible_values[removed_value - 1] = 0;
            --count_possible_values;
            return true;
        }
        return false;
    }

    /**
     * Is the sudoku cell solved?
     */
    bool solved() {
        return count_possible_values == 1;
    }

    /**     
     * @return returns the last and only value in the possible_values array with 
     non zero value
     */
    unsigned short last_value() {

        if (!solved()) {
            throw logic_error("Not solved yet!");
        }

        for (unsigned short i = 0; i < 9; i++) {
            if (possible_values[i] != 0) {
                return possible_values[i+1];
            }
        }

        throw logic_error("Could not find the value!");
    }

    void print_possible_values() {
        for (unsigned short i = 0; i < 9; i++) {
            if (possible_values[i] != 0) {
                cout << possible_values[i];
            }
        }
    }
};

#endif	/* CELLPOSSIBLEVALUES_H */

