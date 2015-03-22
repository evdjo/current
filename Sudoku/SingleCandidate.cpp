#include "SingleCandidate.h"

void SingleCandidate::apply() {
    while (true) {
        outcome _rows = rows();
        if (_rows == NEW_VALUE_FOUND) {
            hs.apply();
            continue;
        }
        outcome _columns = columns();
        if (_columns == NEW_VALUE_FOUND) {
            hs.apply();
            continue;
        }
        outcome _squares = squares();
        if (_squares == NEW_VALUE_FOUND) {
            hs.apply();
            continue;
        }
        if (_squares == NOTHING_FOUND &&
                _columns == NOTHING_FOUND &&
                _rows == NOTHING_FOUND) {
            return;
        }
    }
}

outcome SingleCandidate::rows() {
    outcome outcome_rows = NOTHING_FOUND;
    for (u row = 0; row < 9; ++row) {
        occurr_list occurrences[9];
        for (u column = 0; column < 9; ++column) {
            if (!cell(row, column).unknown()) continue;
            count_occurences(row, column, occurrences);
        }
        outcome row_outcome = search(occurrences);
        outcome_rows = SudokuUtils::max(outcome_rows, row_outcome);
        if (outcome_rows == NEW_VALUE_FOUND) break;
    }
    return outcome_rows;
}

outcome SingleCandidate::columns() {
    outcome outcome_columns = NOTHING_FOUND;
    for (u column = 0; column < 9; ++column) {
        occurr_list occurrences[9];
        for (u row = 0; row < 9; ++row) {
            if (!cell(row, column).unknown()) continue;
            count_occurences(row, column, occurrences);
        }
        outcome outcome_column = search(occurrences);
        outcome_columns = SudokuUtils::max(outcome_columns, outcome_column);
        if (outcome_columns == NEW_VALUE_FOUND) break;

    }
    return outcome_columns;
}

outcome SingleCandidate::squares() {
    outcome outcome_squares = NOTHING_FOUND;
    for (u srow = 0; srow <= 6; srow += 3) {
        for (u scmn = 0; scmn <= 6; scmn += 3) {
            occurr_list occurrences[9];
            for (u row = srow; row < srow + 3; ++row) {
                for (u cmn = scmn; cmn < scmn + 3; ++cmn) {
                    if (!cell(row, cmn).unknown()) continue;
                    count_occurences(row, cmn, occurrences);
                }
            }
            outcome outcome_square = search(occurrences);
            outcome_squares = SudokuUtils::max(outcome_squares, outcome_square);
            if (outcome_squares == NEW_VALUE_FOUND) break;
        }
    }
    return outcome_squares;
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

outcome SingleCandidate::search(occurr_list* list) {
    outcome outcome_ = NOTHING_FOUND;
    occurr_list pairs;
    u tripple_count = 0;

    for (u ocurring_val = 0; ocurring_val < 9; ++ocurring_val) {

        if (list[ocurring_val].m_count == 0)
            continue; // not interested if there are no occurrences

        occurr_list & current = list[ocurring_val];
        u val = ocurring_val + 1;

        if (current.m_count == 1) { // single candidate
            lock_single_candidate(current, val);
            outcome_ = NEW_VALUE_FOUND;
        } else if (current.m_count == 2) {
            outcome single_pair = seek_single_pair(current, val);
            outcome_ = SudokuUtils::max(outcome_, single_pair);

            pairs.add_(current.first(), val);
            pairs.add_(current.last(), val);

        } else {
            outcome double_pair = seek_naked_pairs(current);
            outcome_ = SudokuUtils::max(outcome_, double_pair);
        }
        if (outcome_ == NEW_VALUE_FOUND) return NEW_VALUE_FOUND;
    }

    if (pairs.count() == 4) {
        outcome hidden_pairs = seek_hidden_pairs(pairs);
        outcome_ = SudokuUtils::max(outcome_, hidden_pairs);
    }

    return outcome_;
}

outcome SingleCandidate::seek_hidden_pairs(occurr_list & pairs) {
    if (pairs.at(0) == (pairs.at(2)) && pairs.at(1) == (pairs.at(3))) {
        SudokuCell& cell_1 = cell(pairs.at(0).row, pairs.at(0).column);
        SudokuCell& cell_2 = cell(pairs.at(1).row, pairs.at(1).column);

        if (pairs.at(0).val == pairs.at(1).val
                && pairs.at(2).val == pairs.at(3).val) {

            outcome pair_1 = cell_1.rmall_but(pairs.at(0).val, pairs.at(2).val);
            if (pair_1 == NEW_VALUE_FOUND) return NEW_VALUE_FOUND;

            outcome pair_2 = cell_2.rmall_but(pairs.at(0).val, pairs.at(2).val);
            if (pair_2 == NEW_VALUE_FOUND) return NEW_VALUE_FOUND;

            outcome pairs = SudokuUtils::max(pair_1, pair_2);
            return pairs;
        } else {
            cerr << "THIS SHOUOLD NOT HAPPENZZZ" << endl;
            // to verify that this cant happen...
            exit(0);
        }
    }
    return NOTHING_FOUND;
}

outcome SingleCandidate::seek_naked_pairs(occurr_list & current) {
    outcome outcome_ = NOTHING_FOUND;
    occurr_list loc_list;
    for (u i = 0; i < current.m_count; ++i) {
        occur_node& node = current[i];
        SudokuCell& sc = cell(node.row, node.column);
        if (sc.unknown() && sc.cand_count() == 2) {
            //refactor to accept node... come on dude !
            loc_list.add_(node.row, node.column);
        }
    }
    if (loc_list.m_count == 2) {
        u rw1 = loc_list[0].row;
        u rw2 = loc_list[1].row;
        u cm1 = loc_list[0].column;
        u cm2 = loc_list[1].column;
        SudokuCell& sc_1 = cell(rw1, cm1);
        SudokuCell& sc_2 = cell(rw2, cm2);
        if (sc_1 == sc_2) {
            u val1 = sc_1.get_cand(0);
            u val2 = sc_1.get_cand(1);
            if (rw1 == rw2)
                outcome_ = elim_naked_pairs(val1, val2, cm1, cm2, rw2, true);
            else if (cm1 == cm2)
                outcome_ = elim_naked_pairs(val1, val2, rw1, rw2, cm2, false);
        }
    }
    return outcome_;
}

outcome SingleCandidate::elim_naked_pairs(const u& val_1, const u& val_2,
        const u& rw_cm_1, const u& rw_cm_2, const u& rw_cm, bool flag) {
    outcome outcome_ = NOTHING_FOUND;
    for (u iter = 0; iter < 9; ++iter) {
        if (iter != rw_cm_1 && iter != rw_cm_2) {
            if (flag) {
                if (cell(rw_cm, iter).unknown()) {

                    outcome outcome_1 = cell(rw_cm, iter).rm_cand(val_1);
                    if (outcome_1 == NEW_VALUE_FOUND) return NEW_VALUE_FOUND;

                    outcome outcome_2 = cell(rw_cm, iter).rm_cand(val_2);
                    outcome_ = SudokuUtils::max(outcome_1, outcome_2);

                }
            } else {
                if (cell(iter, rw_cm).unknown()) {

                    outcome outcome_1 = cell(iter, rw_cm).rm_cand(val_1);
                    if (outcome_1 == NEW_VALUE_FOUND) return NEW_VALUE_FOUND;

                    outcome outcome_2 = cell(iter, rw_cm).rm_cand(val_2);
                    outcome_ = SudokuUtils::max(outcome_1, outcome_2);

                }
            }
            if (outcome_ == NEW_VALUE_FOUND) break;
        }
        if (outcome_ == NEW_VALUE_FOUND) break;
    }

    return outcome_;
}

void SingleCandidate::lock_single_candidate
(const occurr_list& list, const u & value) {
    u row = list.first().row;
    u column = list.first().column;
    cell(row, column).set_val(value);

    if (hs.eliminate_val(row, column, value)) // if value was eliminated
        hs.apply();
}

outcome SingleCandidate::seek_single_pair
(const occurr_list& current, const u & value) {
    outcome outcome_ = NOTHING_FOUND;
    u rw1 = current.first().row;
    u rw2 = current.last().row;

    u cm1 = current.first().column;
    u cm2 = current.last().column;

    u zero_rw1 = SudokuUtils::zero_index(rw1);
    u zero_rw2 = SudokuUtils::zero_index(rw2);

    u zero_cm1 = SudokuUtils::zero_index(cm1);
    u zero_cm2 = SudokuUtils::zero_index(cm2);

    if (rw1 == rw2 && zero_cm1 == zero_cm2) {
        outcome_ = elim_single_pair(rw1, cm1, value, true);
    } else if (cm1 == cm2 && zero_rw1 == zero_rw2) {
        outcome_ == elim_single_pair(cm2, rw2, value, false);
    } else if (zero_rw1 == zero_rw2 && zero_cm1 == zero_cm2) {
        if (rw1 == rw2) {
            outcome_ = elim_single_pair(rw1, cm1, value, false);
        } else if (cm1 == cm2) {
            outcome_ = elim_single_pair(rw1, cm1, value, true);
        }
    }
    return outcome_;
}

outcome SingleCandidate::elim_single_pair
(const u& x, const u& y, const u& val, bool flag) {
    outcome outcome_ = NOTHING_FOUND;
    u zero_index = SudokuUtils::zero_index(y);
    for (u iter_over = 0; iter_over < 9; ++iter_over) {
        if (iter_over < zero_index || iter_over >= zero_index + 3) {
            SudokuCell& sc = flag ? cell(x, iter_over) : cell(iter_over, x);
            if (sc.unknown()) {
                outcome_ = SudokuUtils::max(outcome_, sc.rm_cand(val));
            }
        }
    }
    return outcome_;
}
