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
     * To keep track of how many possible values are left. When this number
     * drops to 1, it means we have solved the cell.
     */
    int count_possible_values = 9;

    /**
     * Verifies that the passed value is within the 1-9 range.
     * @param checked_value the value to check
     */
    void assert_value_within_range(const unsigned short& checked_value) {
        if (checked_value <= static_cast<unsigned short> (0) ||
                checked_value >= static_cast<unsigned short> (10)) {
            throw invalid_argument("Value must be in the range 1-9"
                    " - actual value " + to_string(checked_value));
        }
    }

    unsigned short& get_value(const unsigned short& value) {     
        return possible_values[value - 1 ];
    }

    bool remove(const unsigned short& removed_value) {
        assert_value_within_range(removed_value);
        if (solved()) {
            throw logic_error("Only one value left - cannot remove!");
        }
        if (get_value(removed_value) == removed_value) {
            get_value(removed_value) = 0; 
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
                return possible_values[i];
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

