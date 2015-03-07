#ifndef SUDOKUCELL_H
#define	SUDOKUCELL_H
#include <stdexcept>
#include "SudokuUtils.h"
#include "CellPossibleValues.h"

using namespace std;

class SudokuCell final {
    friend class SudokuModel;
    friend class SudokuAlgorithmsTest;

    CellPossibleValues cpv;
    u_short value;

    SudokuCell() {
        value = 0;
    }

    bool remove_candidate(const u_short& removed_value) {
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