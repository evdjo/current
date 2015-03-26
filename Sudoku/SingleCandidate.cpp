#include "SingleCandidate.h"

void SingleCandidate::apply() {
    while (true) {
        outcome _rows = rows();
        if (_rows == NEW_VALUE) {
            hs.apply();
            continue;
        }
        outcome _columns = columns();
        if (_columns == NEW_VALUE) {
            hs.apply();
            continue;
        }
        outcome _squares = squares();
        if (_squares == NEW_VALUE) {
            hs.apply();
            continue;
        }
        if (_rows == NOTHING && _columns == NOTHING && _squares == NOTHING)
            return;

    }
}

outcome SingleCandidate::rows() {
    outcome outcome_rows = NOTHING;
    for (u row = 0; row < 9; ++row) {
        sud_list<sud_node> candidates[9];
        for (u column = 0; column < 9; ++column) {
            if (!cell(row, column).unknown()) continue;
            count_candidates(row, column, candidates);
        }
        outcome row_outcome = search(candidates);
        outcome_rows = SudokuUtils::max(outcome_rows, row_outcome);
        if (outcome_rows == NEW_VALUE) break;
    }
    return outcome_rows;
}

outcome SingleCandidate::columns() {
    outcome outcome_columns = NOTHING;
    for (u column = 0; column < 9; ++column) {
        sud_list<sud_node> candidates[9];
        for (u row = 0; row < 9; ++row) {
            if (!cell(row, column).unknown()) continue;
            count_candidates(row, column, candidates);
        }
        outcome outcome_column = search(candidates);
        outcome_columns = SudokuUtils::max(outcome_columns, outcome_column);
        if (outcome_columns == NEW_VALUE) break;

    }
    return outcome_columns;
}

outcome SingleCandidate::squares() {
    outcome outcome_squares = NOTHING;
    for (u srow = 0; srow <= 6; srow += 3) {
        for (u scmn = 0; scmn <= 6; scmn += 3) {
            sud_list<sud_node> candidates[9];
            for (u row = srow; row < srow + 3; ++row) {
                for (u cmn = scmn; cmn < scmn + 3; ++cmn) {
                    if (!cell(row, cmn).unknown()) continue;
                    count_candidates(row, cmn, candidates);
                }
            }
            outcome outcome_square = search(candidates);
            outcome_squares = SudokuUtils::max(outcome_squares, outcome_square);
            if (outcome_squares == NEW_VALUE) break;
        }
    }
    return outcome_squares;
}

void SingleCandidate::count_candidates
(const u& row, const u& column, sud_list<sud_node>* list) {
    SudokuCell& sc = cell(row, column);
    for (u i = 0; i < 9; ++i) {
        if (sc.is_cand(i + 1)) {
            list[i].add(sud_node(row, column, i + 1));
        }
    }
}

outcome SingleCandidate::search(sud_list<sud_node>* list) {
    outcome search_outcome = NOTHING;
    sud_list<sud_list < sud_node>> prs;

    for (u candidate = 1; candidate < 10; ++candidate) {
        if (list[candidate - 1].size() == 0) continue; // not interested if there are no occurrences
        sud_list<sud_node>& cand_occurrence = list[candidate - 1];
        if (cand_occurrence.size() == 1) { // single candidate
            lock_single_candidate(cand_occurrence, candidate);
            return NEW_VALUE;
        } else if (cand_occurrence.size() == 2) {
            outcome _pointing_pair = pointing_pair(cand_occurrence, candidate);
            search_outcome = SudokuUtils::max(search_outcome, _pointing_pair);
            if (search_outcome == NEW_VALUE) return NEW_VALUE;
            prs.add(sud_list<sud_node>());
            prs[prs.size() - 1].add(cand_occurrence[0]);
            prs[prs.size() - 1].add(cand_occurrence[1]);

        }
    }
    if (prs.size() > 1) {
        outcome hidden_pairs_outcome = seek_hidden_pairs(prs);
        search_outcome = SudokuUtils::max(search_outcome, hidden_pairs_outcome);
    }

    return search_outcome;
}

outcome SingleCandidate::seek_hidden_pairs
(const sud_list<sud_list<sud_node>>&prs) {
    for (u i = 0; i < prs.size(); ++i) {
        for (u o = i + 1; o < prs.size(); ++o) {
            if (prs[i][0].rw == prs[o][0].rw
                    && prs[i][0].cm == prs[o][0].cm &&
                    prs[i][1].rw == prs[o][1].rw
                    && prs[i][1].cm == prs[o][1].cm) {

                SudokuCell& cell1 = cell(prs[i][0].rw, prs[o][0].cm);
                SudokuCell& cell2 = cell(prs[i][1].rw, prs[o][1].cm);

                if (cell1.cand_count() == 2 && cell1.cand_count() == 2) {
                    //                    cout << "BOOM";
                }

                u val1 = prs[i][0].val;
                u val2 = prs[o][0].val;

                outcome _1 = cell1.rmall_but(val1, val2);
                outcome _2 = cell2.rmall_but(val1, val2);

                return SudokuUtils::max(_1, _2);
            }
        }
    }
    return NOTHING;
}

void SingleCandidate::lock_single_candidate
(const sud_list<sud_node>& list, const u & value) {
    u row = list[0].rw;
    u column = list[0].cm;
    cell(row, column).set_val(value);

    if (hs.eliminate_val(row, column, value)) // if value was eliminated
        hs.apply();
}

outcome SingleCandidate::pointing_pair
(const sud_list<sud_node>& list, const u & value) {
    outcome outcome_ = NOTHING;
    u rw1 = list[0].rw;
    u rw2 = list[1].rw;

    u cm1 = list[0].cm;
    u cm2 = list[1].cm;

    u zero_rw1 = SudokuUtils::zero_index(rw1);
    u zero_rw2 = SudokuUtils::zero_index(rw2);

    u zero_cm1 = SudokuUtils::zero_index(cm1);
    u zero_cm2 = SudokuUtils::zero_index(cm2);

    if (rw1 == rw2 && zero_cm1 == zero_cm2) {
        outcome_ = elim_pointing_pair(rw1, cm1, value, true);
    } else if (cm1 == cm2 && zero_rw1 == zero_rw2) {
        outcome_ == elim_pointing_pair(cm2, rw2, value, false);
    } else if (zero_rw1 == zero_rw2 && zero_cm1 == zero_cm2) {
        if (rw1 == rw2) {
            outcome_ = elim_pointing_pair(rw1, cm1, value, false);
        } else if (cm1 == cm2) {
            outcome_ = elim_pointing_pair(rw1, cm1, value, true);
        }
    }
    return outcome_;
}

outcome SingleCandidate::elim_pointing_pair
(const u& x, const u& y, const u& val, bool flag) {
    outcome outcome_ = NOTHING;
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
