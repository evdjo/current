#include "Solver.h"

void Solver::apply() {

    while (true) {
        kvr.apply();

        outcome _rows = rows();
        if (_rows == NEW_VALUE) continue;
        outcome _columns = columns();
        if (_columns == NEW_VALUE) continue;
        outcome _squares = squares();
        if (_squares == NEW_VALUE) continue;

        if (_rows == NOTHING &&
                _columns == NOTHING &&
                _squares == NOTHING) {

            print_possible_values();
            return;
        }

    }
}

outcome Solver::rows() {
    outcome outcome_rows = NOTHING;
    for (u row = 0; row < 9; ++row) {
        sud_list<sud_node> candidates[9];
        sud_list<sud_node> two_cands;
        sud_list<sud_node> three_cands;
        for (u column = 0; column < 9; ++column) {
            SudCell & _cell = cell(row, column);
            if (!_cell.unknown()) continue;
            count_candidates(row, column, candidates);
            if (_cell.cand_count() == 2) {
                two_cands.add(_cell.m_node);
            } else if (_cell.cand_count() == 3) {
                three_cands.add(_cell.m_node);
            }
        }
        outcome row_outcome = search(candidates);
        outcome_rows = SudokuUtils::max(outcome_rows, row_outcome);
        if (outcome_rows == NEW_VALUE) break;

        if (two_cands.size() > 1) {
            outcome _naked_pairs = naked_pairs(two_cands, false);
            outcome_rows = SudokuUtils::max(outcome_rows, _naked_pairs);
            if (outcome_rows == NEW_VALUE) break;
        }

        if (three_cands.size() > 1) {
            outcome _naked_trip = naked_trip(three_cands, two_cands, ROW);
            outcome_rows = SudokuUtils::max(outcome_rows, _naked_trip);
            if (outcome_rows == NEW_VALUE) break;
        }

    }
    return outcome_rows;
}

outcome Solver::columns() {
    outcome outcome_columns = NOTHING;
    for (u column = 0; column < 9; ++column) {
        sud_list<sud_node> candidates[9];
        sud_list<sud_node> two_cands;
        for (u row = 0; row < 9; ++row) {
            SudCell & _cell = cell(row, column);
            if (!_cell.unknown()) continue;
            count_candidates(row, column, candidates);
            if (_cell.cand_count() == 2) {
                two_cands.add(_cell.m_node);
            }
        }
        outcome outcome_column = search(candidates);
        outcome_columns = SudokuUtils::max(outcome_columns, outcome_column);
        if (outcome_columns == NEW_VALUE) break;

        if (two_cands.size() > 1) {
            outcome _naked_pairs = naked_pairs(two_cands, true);
            outcome_columns = SudokuUtils::max(outcome_columns, _naked_pairs);
            if (outcome_columns == NEW_VALUE) break;
        }


    }
    return outcome_columns;
}

outcome Solver::squares() {
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

void Solver::count_candidates
(const u& row, const u& column, sud_list<sud_node>* list) {
    SudCell& sc = cell(row, column);
    for (u i = 0; i < 9; ++i) {
        if (sc.is_cand(i + 1)) {
            list[i].add(sud_node(row, column, i + 1));
        }
    }
}

outcome Solver::search(sud_list<sud_node>* list) {
    outcome search_outcome = NOTHING;
    sud_list<sud_list < sud_node>> pairs;
    sud_list<sud_list < sud_node>> tripples;

    for (u candidate = 1; candidate < 10; ++candidate) {
        // not interested if there are no occurrences
        if (list[candidate - 1].size() == 0) continue;
        sud_list<sud_node>& cand_occur = list[candidate - 1];
        if (cand_occur.size() == 1) { // single candidate
            lock_single_candidate(cand_occur, candidate);
            return NEW_VALUE;
        } else if (cand_occur.size() == 2) {

            outcome _pointing_pair = pointing_pair(cand_occur, candidate);
            search_outcome = SudokuUtils::max(search_outcome, _pointing_pair);
            if (search_outcome == NEW_VALUE) return NEW_VALUE;
            pairs.add(sud_list<sud_node>());
            pairs[pairs.size() - 1].add(cand_occur[0]);
            pairs[pairs.size() - 1].add(cand_occur[1]);

        } else if (cand_occur.size() == 3) {

            outcome _pnt_tripple = pointing_trip(cand_occur, candidate);
            search_outcome = SudokuUtils::max(search_outcome, _pnt_tripple);
            if (search_outcome == NEW_VALUE) return NEW_VALUE;
            tripples.add(sud_list<sud_node>());
            tripples[tripples.size() - 1].add(cand_occur[0]);
            tripples[tripples.size() - 1].add(cand_occur[1]);
            tripples[tripples.size() - 1].add(cand_occur[2]);

        }
    }
    if (pairs.size() > 1) {
        outcome hidden_pairs_outcome = hidden_pairs(pairs);
        search_outcome = SudokuUtils::max(search_outcome, hidden_pairs_outcome);
    }

    return search_outcome;
}

void Solver::lock_single_candidate
(const sud_list<sud_node>& list, const u & value) {
    u row = list[0].rw;
    u column = list[0].cm;
    cell(row, column).set_val(value);
}

outcome Solver::hidden_pairs
(const sud_list<sud_list < sud_node>>&prs) {
    for (u i = 0; i < prs.size(); ++i) {
        for (u o = i + 1; o < prs.size(); ++o) {

            sud_node& p1_occur1 = prs[i][0];
            sud_node& p1_occur2 = prs[i][1];

            sud_node& p2_occur1 = prs[o][0];
            sud_node& p2_occur2 = prs[o][1];


            if (p1_occur1.rw == p2_occur1.rw
                    && p1_occur1.cm == p2_occur1.cm
                    && p1_occur2.rw == p2_occur2.rw
                    && p1_occur2.cm == p2_occur2.cm) {

                SudCell& cell1 = cell(p1_occur1.rw, p1_occur1.cm);
                SudCell& cell2 = cell(p1_occur2.rw, p1_occur2.cm);

                if (!(cell1.unknown() && cell2.unknown())) {
                    cout << "HEY LOOK AT ME CODE : 1005";
                    continue;
                }

                outcome _1 = cell1.rmall_but(prs[i][0].val, prs[o][0].val);
                outcome _2 = cell2.rmall_but(prs[i][0].val, prs[o][0].val);

                return SudokuUtils::max(_1, _2);
            }
        }
    }
    return NOTHING;
}

outcome Solver::pointing_pair
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
            cerr << "HEY LOOK AT ME CODE : 1002";
        } else if (cm1 == cm2) {
            cerr << "HEY LOOK AT ME CODE : 1003";
        }
    }
    return outcome_;
}

outcome Solver::elim_pointing_pair
(const u& x, const u& y, const u& val, bool flag) {
    outcome outcome_ = NOTHING;
    u zero_index = SudokuUtils::zero_index(y);
    for (u iter_over = 0; iter_over < 9; ++iter_over) {
        if (iter_over < zero_index || iter_over >= zero_index + 3) {
            SudCell& sc = flag ? cell(x, iter_over) : cell(iter_over, x);
            if (sc.unknown()) {
                outcome_ = SudokuUtils::max(outcome_, sc.rm_cand(val));
            }
        }
    }
    return outcome_;
}

/**
 * Look if the three occurrences are in same row or in same column.
 * If any of the two is true, check they are also in the same square.
 *
 * If any if the first is true, along with the second,
 * we've got pointing tripple. Look at elim_pointing_tripple to see how
 * we eliminate them.
 * @param list containing cooridnates of the three occurrences
 * @param val the occurring val
 * @return what was the outcome of this search
 */
outcome Solver::pointing_trip
(const sud_list<sud_node>& list, const u & val) {
    outcome outcome_ = NOTHING;
    u rw1 = list[0].rw;
    u rw2 = list[1].rw;
    u rw3 = list[2].rw;

    u cm1 = list[0].cm;
    u cm2 = list[1].cm;
    u cm3 = list[2].cm;

    u zero_rw1 = SudokuUtils::zero_index(rw1);
    u zero_rw2 = SudokuUtils::zero_index(rw2);
    u zero_rw3 = SudokuUtils::zero_index(rw3);

    u zero_cm1 = SudokuUtils::zero_index(cm1);
    u zero_cm2 = SudokuUtils::zero_index(cm2);
    u zero_cm3 = SudokuUtils::zero_index(cm3);

    bool occurr_same_row = (rw1 == rw2 && rw2 == rw3);
    bool occurr_row_sqr = (zero_cm1 == zero_cm2 && zero_cm2 == zero_cm3);

    bool occurr_same_clmn = (cm1 == cm2 && cm2 == cm3);
    bool occurr_in_clmn_sqr = (zero_rw1 == zero_rw2 && zero_rw2 == zero_rw3);


    if (occurr_same_row && occurr_row_sqr) {

        outcome_ = elim_pointing_trip(rw1, cm1, val, true);

    } else if (occurr_same_clmn && occurr_in_clmn_sqr) {

        outcome_ == elim_pointing_trip(cm1, rw1, val, false);
    } else if (occurr_row_sqr && occurr_in_clmn_sqr) {

        if (occurr_same_row) {
            cerr << "HEY LOOK AT ME CODE : 1000";
        } else if (occurr_same_clmn) {
            cerr << "HEY LOOK AT ME CODE : 1001";
        }

    }

    return outcome_;
}

/**
 * When we find that a value is candidate for only three cells in a row or a
 * column, and also those three cells are in the same square, we can exclude
 * that value from any other cells in the square that the three occurrences are
 * found.
 * @param x the x axis of the first cell of those three cells
 * @param y the y axis of the first cell of those three cells
 * @param val the candidate value that occurrs only in those three cells.
 * @param flag if true, x is the row axis and y is column axis, else vice-versa
 * @return the outcome of candidate exclusion
 */
outcome Solver::elim_pointing_trip
(const u& x, const u& y, const u& val, bool flag) {
    outcome outcome_ = NOTHING;
    u zero_x = SudokuUtils::zero_index(x);
    u zero_y = SudokuUtils::zero_index(y);
    for (u iter_x = zero_x; iter_x < zero_x + 3; ++iter_x) {
        if (iter_x == x) continue;
        for (u iter_y = zero_y; iter_y < zero_y + 3; ++iter_y) {
            SudCell& sc = flag ? cell(iter_x, iter_y) : cell(iter_y, iter_x);
            if (sc.unknown()) {
                outcome_ = SudokuUtils::max(outcome_, sc.rm_cand(val));
            }
        }
    }
    return outcome_;
}

outcome Solver::naked_pairs(const sud_list<sud_node>& two_cands, bool flag) {
    outcome naked_pairs = NOTHING;
    for (u i = 0; i < two_cands.size(); ++i) {
        for (u z = i + 1; z < two_cands.size(); ++z) {
            SudCell& cell_1 = cell(two_cands[i].rw, two_cands[i].cm);
            SudCell& cell_2 = cell(two_cands[z].rw, two_cands[z].cm);
            if (cell_1 == cell_2) {
                outcome _outcome = elim_naked_pair(cell_1, cell_2, flag);
                naked_pairs = SudokuUtils::max(naked_pairs, _outcome);
                if (naked_pairs == NEW_VALUE) return NEW_VALUE;
            }
        }
    }
    return naked_pairs;
}

outcome Solver::elim_naked_pair
(const SudCell& cell_1, const SudCell& cell_2, bool flag) {
    outcome _outcome = NOTHING;

    u curr = flag ? cell_1.m_node.cm : cell_1.m_node.rw;

    u skip_iter_1 = flag ? cell_1.m_node.rw : cell_1.m_node.cm;
    u skip_iter_2 = flag ? cell_2.m_node.rw : cell_2.m_node.cm;

    u val_1 = cell_1.get_cand(0);
    u val_2 = cell_1.get_cand(1);

    for (u iter = 0; iter < 9; ++iter) {
        SudCell& _cell = flag ? cell(iter, curr) : cell(curr, iter);

        if (!_cell.unknown() || iter == skip_iter_1 || iter == skip_iter_2)
            continue;

        outcome first = _cell.rm_cand(val_1);
        if (first == NEW_VALUE) return NEW_VALUE;

        outcome second = _cell.rm_cand(val_2);
        if (second == NEW_VALUE) return NEW_VALUE;

        _outcome = SudokuUtils::max(_outcome, first);
        _outcome = SudokuUtils::max(_outcome, second);
    }
    return _outcome;
}

outcome Solver::naked_trip
(const sud_list<sud_node>& threes, const sud_list<sud_node>& twos,
        iter_over flag) {
    outcome outcome_ = NOTHING;
    for (u first = 0; first < threes.size(); ++first) {

        SudCell& cell_1 = cell(threes[first]);

        for (u second = first + 1; second < threes.size(); ++second) {

            SudCell& cell_2 = cell(threes[second]);
            bool first_two_equal = cell_1 == cell_2;
            if (first_two_equal) {
                bool three_equal = false;

                for (u third = first + 2; third < threes.size(); ++third) {

                    SudCell& cell_3 = cell(threes[third]);
                    if (cell_1 == cell_3) {
                        three_equal = true;
                        outcome o_ = elim_trip(cell_1, cell_2, cell_3, flag);
                        outcome_ = SudokuUtils::max(outcome_, o_);
                        if (outcome_ == NEW_VALUE) return NEW_VALUE;
                    }
                }
                if (!three_equal && twos.size() > 0) {
                    outcome o_ = two_equal_threes(cell_1, cell_2, twos, flag);
                    outcome_ = SudokuUtils::max(outcome_, o_);
                    if (outcome_ == NEW_VALUE) return NEW_VALUE;
                }
            }
        }
    }
    return outcome_;
}

outcome Solver::two_equal_threes
(const SudCell& cell_1, const SudCell& cell_2,
        const sud_list<sud_node>& twos, iter_over flag) {
    for (u q = 0; q < twos.size(); ++q) {
        SudCell& cell_3 = cell(twos[q]);
        if (cell_1.subset_equals(cell_3)) {
            return elim_trip(cell_1, cell_2, cell_3, flag);
        }
    }
    return NOTHING;
}

outcome Solver::elim_trip
(const SudCell & cell_1, const SudCell & cell_2, const SudCell & cell_3,
        iter_over flag) {

    outcome _outcome = NOTHING;
    sud_list<sud_node> skip;
    skip.add(cell_1.node());
    skip.add(cell_2.node());
    skip.add(cell_3.node());

    return iterate_over([](SudCell& cell, const SudCell & origin) {
        if (cell.unknown()) {
            u val_1 = origin.get_cand(0);
            u val_2 = origin.get_cand(1);
            u val_3 = origin.get_cand(2);
            outcome otcm_1 = cell.rm_cand(val_1);
            if (otcm_1 == NEW_VALUE) return NEW_VALUE;
            outcome otcm_2 = cell.rm_cand(val_2);
            if (otcm_2 == NEW_VALUE) return NEW_VALUE;
            outcome otcm_3 = cell.rm_cand(val_3);
            if (otcm_3 == NEW_VALUE) return NEW_VALUE;
            return SudokuUtils::max(otcm_1, SudokuUtils::max(otcm_2, otcm_3));

        } else return NOTHING;
    }, cell_1, flag, &skip);
}

