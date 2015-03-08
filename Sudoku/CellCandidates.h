#ifndef CELLPOSSIBLEVALUES_H
#define	CELLPOSSIBLEVALUES_H
#include <stdexcept>
#include <iostream>
#include "SudokuUtils.h"

using namespace std;

class CellCandidates final {
    friend class SudokuCell;
    friend class CellCandidatesTest;
    friend class SudokuAlgorithmsTest;


    /**
     * Array to keep track of the possible values for a cell. 
     * Value of 0 at a position means that the position's original value
     * is not a candidate anymore.
     */
    u_short candidates[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};

    /**
     * To keep track of how many possible values are left. When this number
     * drops to 1, it means we have solved the cell.
     */
    u_short candidates_count = 9;

    /**
     * Get the candidate from the list of candidates. 
     * If the the val is in the list it will return the val itself,
     * else it will return 0
     * @param val the value to get
     * @return the val if it is a candidate else 0
     */
    inline u_short& candidate(const u_short& val) {
        return candidates[val - 1];
    }

    /**
     * Remove all but the passed not_rm value 
     * from the candidate list.
     * @param not_rm the only val to not remove
     */

    void remove_all(const u_short& not_rm) {
        assert_value_within_range(not_rm);
        for (u_short candidate_index = 0; candidate_index < 9; ++candidate_index) {
            if (candidate_index != not_rm - 1) {
                candidates[candidate_index] = 0;
            }
        }
    }

    /**
     * Remove the rm_val from the candidate list
     * @param rm_val the value to remove
     * @return whether the value was present & removed
     */
    bool remove(const u_short& rm_val) {
        assert_value_within_range(rm_val);
        if (one_candidate_left()) {
            throw logic_error("Only one value left - cannot remove!");
        }
        if (candidate(rm_val) == rm_val) {
            candidate(rm_val) = 0;
            --candidates_count;
            return true;
        }
        return false;
    }

    /**
     * Check if only one candidate is left.
     */
    inline bool one_candidate_left() {
        return candidates_count == 1;
    }

    /**     
     * @return returns the last and only value
     * in the possible_values array with non zero value
     */
    u_short last_value() {
        if (!one_candidate_left()) throw logic_error("Not solved yet!");

        for (u_short i = 0; i < 9; i++)
            if (candidates[i] != 0) return candidates[i];

        throw logic_error("Could not find the value!");
    }

    /**
     * Verifies that the passed value is within the 1-9 range.
     * @param checked_value the value to check
     */
    void assert_value_within_range(const u_short& checked_value) {
        if (checked_value <= static_cast<u_short> (0) ||
                checked_value >= static_cast<u_short> (10)) {
            throw invalid_argument("Value must be in the range 1-9"
                    " - actual value " + to_string(checked_value));
        }
    }

    /**
     * Prints the possible values.
     */
    void print_possible_values() {
        for (u_short i = 0; i < 9; i++)
            if (candidates[i] != 0) cout << candidates[i];
            else cout << " ";
    }


};

#endif	/* CELLPOSSIBLEVALUES_H */

