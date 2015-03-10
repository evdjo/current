#ifndef SUDOKUCELL_H
#define	SUDOKUCELL_H
#include <stdexcept>
#include "SudokuUtils.h"
#include "CellCandidates.h"

using namespace std;

class SudokuCell final {
    friend class SudokuModel;
    friend class SudokuAlgorithmsTest;

    CellCandidates *cc = NULL;
    u val = 0;

    SudokuCell() {
    }

    /**
     * Remove the rm_val from the candidate list
     * @param rm_val the value to remove
     * @return was the value in the candidate list && was it removed?
     */

    bool rm_candidate(const u& rm_val) {

        if (!unknown()) exit(-1); // should never happen

        bool removal_occurred = cc->remove(rm_val);

        if (removal_occurred && cc->one_candidate_left()) {
            set_val(cc->last_value());
            delete cc;
            cc = nullptr;
        }
        return removal_occurred;
    }

    /**
     * Sets the value to indicate that the cell was solved.
     * @param val_ the value of the cell
     */
    void set_val(const u& val_) {
        val = val_;
        if (cc != nullptr) {
            delete cc;
            cc = nullptr;
        }
    }

    /**
     * Check if the val is candidate for this cell.
     * @param val_ the checked val
     * @return is the val a candidate for this cell ?
     */
    bool is_candidate(const u& val_) {
        return cc->candidate(val_) == val_;
    }

    /**
     * Check if the current cell is unsolved.
     * @return is the cell value unknown
     */
    bool unknown() {
        return val == 0;
    }

    /**
     * Initialise the cell value.
     * @param val_ the cell value that this cell will have.
     */
    void init_val(const u& val_) {
        val = val_;
        if (val_ == 0)
            cc = new CellCandidates();
    }

    /**
     * Print the possible candidates of this cell
     */
    void print() {
        cc->print_possible_values();
    }

    ~SudokuCell() {
        if (cc != NULL)
            delete cc;
    }

};

#endif	/* SUDOKUCELL_H */