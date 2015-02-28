#ifndef SUDOKUCELL_H
#define	SUDOKUCELL_H
#include "CellPossibleValues.h"
#include <stdexcept>

using namespace std;

class SudokuCell {
    friend class SudokuModel;

    CellPossibleValues cpv;
    unsigned short value;

    SudokuCell() {
        value = 0;
    }

    virtual ~SudokuCell() {
    }

    bool remove(const unsigned short& removed_value) {
        bool removal_occurred = cpv.remove(removed_value);
        if (removal_occurred && cpv.solved()) {
            value = cpv.last_value();
        }
        return removal_occurred;
    }

    bool is_unknown() {
        return value == 0;
    }
};

#endif	/* SUDOKUCELL_H */

