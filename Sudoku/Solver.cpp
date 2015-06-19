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
            print();
            return;
        }
    }
}

outcome Solver::rows() {
    outcome _rows = NOTHING;
    for (u row = 0; row < 9; ++row) {
        sud_list<sud_node> candidates[9];
        sud_list<Cell> candidates_fourorless;

        for (u column = 0; column < 9; ++column) {
            Cell & _cell = cell(row, column);
            if (!_cell.unknown()) continue;
            count_cands(row, column, candidates);
            if (_cell.cand_count() < 5) {
                candidates_fourorless.add(_cell);
            }
        }
        outcome _row = search(candidates);
        _rows = max(_rows, _row);
        if (_rows == NEW_VALUE) break;
        naked(candidates_fourorless, ROW);
    }
    return _rows;
}

outcome Solver::columns() {
    outcome _columns = NOTHING;
    for (u column = 0; column < 9; ++column) {
        sud_list<sud_node> candidates[9];
        sud_list<Cell> candidates_fourorless;
        for (u row = 0; row < 9; ++row) {
            Cell & _cell = cell(row, column);
            if (!_cell.unknown()) continue;
            count_cands(row, column, candidates);
            if (_cell.cand_count() < 5) {
                candidates_fourorless.add(_cell);
            }
        }
        outcome _column = search(candidates);
        _columns = max(_columns, _column);
        if (_columns == NEW_VALUE) break;
        if (_columns == NEW_VALUE) break;
        naked(candidates_fourorless, COLUMN);
    }
    return _columns;
}

outcome Solver::squares() {
    outcome _squares = NOTHING;
    for (u srow = 0; srow <= 6; srow += 3) {
        for (u scmn = 0; scmn <= 6; scmn += 3) {
            sud_list<sud_node> candidates[9];
            sud_list<Cell> candidates_fourorless;
            for (u row = srow; row < srow + 3; ++row) {
                for (u cmn = scmn; cmn < scmn + 3; ++cmn) {
                    Cell & _cell = cell(row, cmn);
                    if (!_cell.unknown()) continue;
                    count_cands(row, cmn, candidates);
                    if (_cell.cand_count() < 5) {
                        candidates_fourorless.add(_cell);
                    }
                }
            }
            outcome _square = search(candidates);
            _squares = max(_squares, _square);
            if (_squares == NEW_VALUE) break;
            naked(candidates_fourorless, SQUARE);
        }
    }
    return _squares;
}

void Solver::count_cands
(const u& row, const u& column, sud_list<sud_node>* list) {
    Cell& sc = cell(row, column);
    for (u i = 0; i < 9; ++i) {
        if (sc.is_cand(i + 1)) list[i].add(sud_node(row, column, i + 1));
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
            search_outcome = max(search_outcome, _pointing_pair);
            if (search_outcome == NEW_VALUE) return NEW_VALUE;
            pairs.add(sud_list<sud_node>());
            pairs[pairs.size() - 1].add(cand_occur[0]);
            pairs[pairs.size() - 1].add(cand_occur[1]);

        } else if (cand_occur.size() == 3) {

            outcome _pnt_tripple = pointing_trip(cand_occur, candidate);
            search_outcome = max(search_outcome, _pnt_tripple);
            if (search_outcome == NEW_VALUE) return NEW_VALUE;
            tripples.add(sud_list<sud_node>());
            tripples[tripples.size() - 1].add(cand_occur[0]);
            tripples[tripples.size() - 1].add(cand_occur[1]);
            tripples[tripples.size() - 1].add(cand_occur[2]);

        }
    }
    if (pairs.size() > 1) {
        outcome hidden_pairs_outcome = hidden_pairs(pairs);
        search_outcome = max(search_outcome, hidden_pairs_outcome);
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

                Cell& cell1 = cell(p1_occur1.rw, p1_occur1.cm);
                Cell& cell2 = cell(p1_occur2.rw, p1_occur2.cm);

                if (!(cell1.unknown() && cell2.unknown())) {
                    cout << "HEY LOOK AT ME CODE : 1005";
                    continue;
                }

                outcome _1 = cell1.rmall_but(prs[i][0].val, prs[o][0].val);
                outcome _2 = cell2.rmall_but(prs[i][0].val, prs[o][0].val);

                return max(_1, _2);
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

    u zero_rw1 = zero(rw1);
    u zero_rw2 = zero(rw2);

    u zero_cm1 = zero(cm1);
    u zero_cm2 = zero(cm2);

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
    u zero_index = zero(y);
    for (u iter_over = 0; iter_over < 9; ++iter_over) {
        if (iter_over < zero_index || iter_over >= zero_index + 3) {
            Cell& sc = flag ? cell(x, iter_over) : cell(iter_over, x);
            if (sc.unknown()) {
                outcome_ = max(outcome_, sc.rm_cand(val));
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

    u zero_rw1 = zero(rw1);
    u zero_rw2 = zero(rw2);
    u zero_rw3 = zero(rw3);

    u zero_cm1 = zero(cm1);
    u zero_cm2 = zero(cm2);
    u zero_cm3 = zero(cm3);

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
    u zero_x = zero(x);
    u zero_y = zero(y);
    for (u iter_x = zero_x; iter_x < zero_x + 3; ++iter_x) {
        if (iter_x == x) continue;
        for (u iter_y = zero_y; iter_y < zero_y + 3; ++iter_y) {
            Cell& sc = flag ? cell(iter_x, iter_y) : cell(iter_y, iter_x);
            if (sc.unknown()) {
                outcome_ = max(outcome_, sc.rm_cand(val));
            }
        }
    }
    return outcome_;
}

outcome Solver::naked(const sud_list<Cell>& list, iter_over what) {

    for (u current = 0; current < list.size(); ++current) {
        Cell& cell_1 = list[current];
        for (u i = current + 1; i < list.size(); ++i) {
            Cell& cell_2 = list[i];

            if (cell_1.cand_count() == 2 && cell_2.cand_count() == 2 &&
                    cell_1 == cell_2) {
                return elim_naked_pair(cell_1, cell_2, what);
            }

            for (u z = i + 1; z < list.size(); ++z) {
                Cell& cell_3 = list[z];
                if (cell_1.cand_count() <= 3 && cell_2.cand_count() <= 3 &&
                        cell_3.cand_count() <= 3) {
                    sud_list<u> _join = join(cell_1, join(cell_2, cell_3));
                    if (_join.size() == 3) {
                        // eliminate naked tripple
                    }
                }

                for (u j = z + 1; j < list.size(); ++j) {
                    Cell& cell_4 = list[j];
                    if (cell_1.cand_count() <= 4 &&
                            cell_2.cand_count() <= 4 &&
                            cell_3.cand_count() <= 4 &&
                            cell_4.cand_count() <= 4) {
                        sud_list<u> _join = join(cell_1,
                                join(cell_2, join(cell_3, cell_4)));
                        if (_join.size() == 4) {
                            // eliminate naked quad
                        }
                    }
                }
            }
        }
    }
}

outcome Solver::elim_naked_pair
(const Cell& cell_1, const Cell& cell_2, iter_over what) {


 }
