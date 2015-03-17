#include "SingleCandidate.h"

void SingleCandidate::apply() {

    bool _rows = true, _columns = true, _3x3squares = true;
    while (_rows || _columns || _3x3squares) {
        _rows = rows();
        _columns = columns();
        _3x3squares = squares();
    }
}

bool SingleCandidate::rows() {
    bool singles_found = false;
    for (u row = 0; row < 9; ++row) {
        occurr_list occurrences[9];
        for (u column = 0; column < 9; ++column) {
            if (!cell(row, column).unknown()) continue;
            count_occurences(row, column, occurrences);
        }

        singles_found = search(occurrences)
                ? true : singles_found;

    }
    return singles_found;
}

bool SingleCandidate::columns() {
    bool singles_found = false;
    for (u column = 0; column < 9; ++column) {
        occurr_list occurrences[9];
        for (u row = 0; row < 9; ++row) {
            if (!cell(row, column).unknown()) continue;
            count_occurences(row, column, occurrences);
        }
        singles_found = search(occurrences)
                ? true : singles_found;

    }
    return singles_found;
}

bool SingleCandidate::squares() {
    bool singles_found = false;
    for (u srow = 0; srow <= 6; srow += 3) {
        for (u scmn = 0; scmn <= 6; scmn += 3) {

            occurr_list occurrences[9];

            for (u row = srow; row < srow + 3; ++row) {
                for (u cmn = scmn; cmn < scmn + 3; ++cmn) {
                    if (!cell(row, cmn).unknown()) continue;
                    count_occurences(row, cmn, occurrences);
                }
            }
            singles_found = search(occurrences)
                    ? true : singles_found;

        }
    }
    return singles_found;
}

void SingleCandidate::count_occurences
(const u& row, const u& column, occurr_list * list) {

    SudokuCell& sc = cell(row, column);
    for (u i = 0; i < 9; ++i) {
        if (sc.is_candidate(i + 1)) {
            list[i].add_(row, column);
        }
    }
}

bool SingleCandidate::search(occurr_list* list) {
    bool change_occurred = false;
    for (u ocurring_val = 0; ocurring_val < 9; ++ocurring_val) {

        if (list[ocurring_val].m_count == 0)
            continue; // not interested in those cases

        occurr_list & current = list[ocurring_val];
        u val = ocurring_val + 1;

        if (current.m_count == 1) { // hidden single
            lock_single(current, val);
            change_occurred = true;
        } else if (current.m_count == 2) {
            if (seek_pair(current, val)) {
                change_occurred = true; // single pair
            }

        } else {
            occurr_list loc_list;
            for (u i = 0; i < current.m_count; ++i) {
                occur_node& node = current[i];
                SudokuCell& sc = cell(node.row, node.column);
                if (sc.cand_count() == 2) {
                    //refactor to accept node... come on dude !
                    loc_list.add_(node.row, node.column);
                }
            }
            if (loc_list.m_count == 2) {

                SudokuCell& sc_1 = cell(loc_list[0].row, loc_list[0].column);
                SudokuCell& sc_2 = cell(loc_list[1].row, loc_list[1].column);

                if (sc_1 == sc_2) {

                    u val_1;
                    u val_2;
                    for (u i = 0; i < 9; ++i) {
                        if (sc_1.cands()[i] != 0)
                            val_1 = sc_1.cands()[i];
                    }
                    for (u i = 8; i != 0; --i) {
                        if (sc_1.cands()[i] != 0)
                            val_1 = sc_1.cands()[i];
                    }


                    //get the two values
                    if (loc_list[0].row == loc_list[1].row) {
                        //  and exclude them from the row
                        exclude_pair(val_1, val_2, loc_list[0].column,
                                loc_list[1].column, loc_list[1].row, true);


                    } else if (loc_list[0].column == loc_list[1].column) {
                        // and exclude them from the column
                        exclude_pair(val_1, val_2, loc_list[0].row,
                                loc_list[1].row, loc_list[1].column, false);
                    }
                }


            }


        }
        return change_occurred;
    }
}

bool SingleCandidate::exclude_pair(const u& val_1, const u& val_2,
        const u& rw_cm_1, const u& rw_cm_2, const u& rw_cm, bool flag) {
    bool change_occurred = false;
    for (u iter_over = 0; iter_over < 9; ++iter_over) {
        if (iter_over != rw_cm_1 || iter_over != rw_cm_2) {
            if (flag) {
                if (cell(rw_cm, iter_over).rm_candidate(val_1) ||
                        cell(rw_cm, iter_over).rm_candidate(val_2)) {
                    change_occurred = true;
                }
            } else {
                if (cell(iter_over, rw_cm).rm_candidate(val_1) ||
                        cell(iter_over, rw_cm).rm_candidate(val_2)) {
                    change_occurred = true;
                }

            }
        }
    }

    return change_occurred;
}

void SingleCandidate::
lock_single(const occurr_list& list, const u & value) {
    u row = list.first().row;
    u column = list.first().column;
    cell(row, column).set_val(value);

    if (hs.eliminate_val(row, column, value)) // if value was eliminated
        hs.apply();
}

bool SingleCandidate::
seek_pair(const occurr_list& current, const u & value) {
    bool change_occurred = false;
    u rw1 = current.first().row;
    u rw2 = current.last().row;

    u cm1 = current.first().column;
    u cm2 = current.last().column;

    u zero_rw1 = SudokuUtils::zero_index(rw1);
    u zero_rw2 = SudokuUtils::zero_index(rw2);

    u zero_cm1 = SudokuUtils::zero_index(cm1);
    u zero_cm2 = SudokuUtils::zero_index(cm2);

    if (rw1 == rw2 && zero_cm1 == zero_cm2) {
        if (eliminate_pair(rw1, cm1, value, true)) {
            change_occurred = true;
        }
    } else if (cm1 == cm2 && zero_rw1 == zero_rw2) {
        if (eliminate_pair(cm2, rw2, value, false)) {
            change_occurred = true;
        }
    } else if (zero_rw1 == zero_rw2 && zero_cm1 == zero_cm2) {


        if (rw1 == rw2) {
            cout << "WHOHOO";
            if (eliminate_pair(rw1, cm1, value, false)) {
                change_occurred = true;
            }

        } else if (cm1 == cm2) {
            cout << "WHOHOO";

            if (eliminate_pair(rw1, cm1, value, true))
                change_occurred = true;
        }
    }

    return change_occurred;
}

bool SingleCandidate::
pair_3x3(const u& x, const u& y, const u& val, bool flag) {
    bool change_occurred = false;
    u zero_row = SudokuUtils::zero_index(x);
    u zero_column = SudokuUtils::zero_index(y);
    for (u i = zero_row; i < zero_row + 3; ++i) {

        for (u o = zero_column; o < zero_column + 3; ++o) {

            if (flag) {
                if (i == x) continue;
            } else {
                if (o == y) continue;
            }

            if (cell(i, o).rm_candidate(val)) {

                change_occurred = true;
            }

        }

    }
    return change_occurred;

}

bool SingleCandidate::eliminate_pair
(const u& x, const u& y, const u& val, bool flag) {
    bool change_occurred = false;

    u zero_index = SudokuUtils::zero_index(y);

    for (u iter_over = 0; iter_over < 9; ++iter_over) {
        if (iter_over < zero_index || iter_over >= zero_index + 3) {

            SudokuCell& sc = flag ? cell(x, iter_over) : cell(iter_over, x);

            if (sc.unknown() && sc.rm_candidate(val)) {
                change_occurred = true;
            }
        }
    }
    return change_occurred;
}

