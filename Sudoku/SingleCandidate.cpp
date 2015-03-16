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
        ocurr_list occurrences[9];
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
        ocurr_list occurrences[9];
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

            ocurr_list occurrences[9];

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
(const u& row, const u& column, ocurr_list * list) {

    SudokuCell& sc = cell(row, column);
    for (u i = 0; i < 9; ++i) {
        if (sc.is_candidate(i + 1)) {
            list[i].add_(row, column);
        }
    }
}

bool SingleCandidate::search(ocurr_list* list) {
    u two_occurrences_count = 0 ;
    ocurr_list pairs;
    bool change_occurred = false;
    for (u i = 0; i < 9; ++i) {
            ocurr_list& current = list[i];
        
        if (current.m_count == 1) {
            lock_single(current, i + 1);
            change_occurred = true;
        }
        if (current.m_count == 2) {
            if (seek_pair(current, i + 1)) {
                change_occurred = true;
            }
            occur_node& first_ = current.first();
            occur_node& second_ = current.last();
            pairs.add_(first_.m_row,first_.m_column,first_.m_val);
            pairs.add_(second_.m_row,second_.m_column,second_.m_val);
            ++two_occurrences_count;
        }
        
    }
    
    if(two_occurrences_count == 2) {
        if(pairs.first().m_row == pairs.last().m_row &&
            pairs.first().m_column == pairs.last().m_column &&
            pairs.first().m_val == pairs.last().m_val) {
                cout << "wait what ?" ;
            }
        
    } 
    
    return change_occurred;
}

void SingleCandidate::
lock_single(const ocurr_list& list, const u& value) {
    u row = list.first().row;
    u column = list.first().column;
    cell(row, column).set_val(value);
    if (hs.eliminate_val(row, column, value)) // if value was eliminated
        hs.apply();
}

bool SingleCandidate::
seek_pair(const ocurr_list& current, const u& value) {
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
    } else if (_flag && zero_rw1 == zero_rw2 && zero_cm1 == zero_cm2) {


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

