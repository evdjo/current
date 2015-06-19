#include "Solver.h"

Solver::Solver(const string& filename) : grid(filename) { }

Solver::~Solver() { }

outcome Solver::excludeKnownVal(u row, u column, u val) {
    outcome row_otcm = NOTHING;
    for (u cl = 0; cl < 9; ++cl) {
        Cell& cell = grid.cell(row, cl);
        if (cell.is_solved()) continue;
        row_otcm = max(cell.remove_candidate(val), row_otcm);

    }
    if (row_otcm == NEW_VALUE) return NEW_VALUE;

    outcome col_otcm = NOTHING;
    for (u rw = 0; rw < 9; ++rw) {
        Cell& cell = grid.cell(rw, column);
        if (cell.is_solved()) continue;
        col_otcm = max(cell.remove_candidate(val), col_otcm);
    }
    if (col_otcm == NEW_VALUE) return NEW_VALUE;


    outcome sqr_otcm = NOTHING;
    u begin_row = zero(row);
    u begin_col = zero(column);
    for (u rw = begin_row; rw < begin_row + 3; ++rw) {
        for (u cl = begin_col; cl < begin_col + 3; ++cl) {
            Cell& cell = grid.cell(rw, cl);
            if (cell.is_solved()) continue;
            sqr_otcm = max(cell.remove_candidate(val), sqr_otcm);
        }
    }
    if (sqr_otcm == NEW_VALUE) return NEW_VALUE;
    return max(row_otcm, max(col_otcm, sqr_otcm));
}

outcome Solver::excludeKnownValues() {
    outcome _outcome = NOTHING;
    for (u row = 0; row < 9; ++row) {
        for (u column = 0; column < 9; ++column) {
            u cell_val = grid.cell(row, column).get_value();
            if (cell_val != CELL_UNKNOWN_VAL) {
                _outcome = max(excludeKnownVal(row, column, cell_val), _outcome);
            }
        }

    }
    return _outcome;
}

void Solver::solve() {
    while (true) {
        if (excludeKnownValues() != NOTHING) continue;
        outcome _rows = process(ROWS);
        if (_rows == NEW_VALUE) continue;
        outcome _columns = process(COLUMNS);
        if (_columns == NEW_VALUE) continue;
        outcome _squares = process(SQUARES);
        if (_squares == NEW_VALUE) continue;

        if (_rows == NOTHING
                &&
                _columns == NOTHING
                &&
                _squares == NOTHING
                ) {
            break;
        }
    }
    grid.print();
    grid.print_possible_values();
    grid.verify();
}

outcome Solver::process(iter_over what) {
    if (SQUARES == what) {
        for (u row = 0; row < 3; ++row) {
            for (u col = 0; col < 3; ++col) {
                niner n = grid.get_niner_square(row, col);
                if (process(n, what) == NEW_VALUE) return NEW_VALUE;
            }
        }
    } else {
        for (u i = 0; i < 9; ++i) {
            niner n = grid.get_niner_line(what, i);
            if (process(n, what) == NEW_VALUE) return NEW_VALUE;
        }
    }
    return NOTHING;
}

outcome Solver::process(const niner & n, iter_over what) {
    outcome final_outcome = NOTHING;

    sud_list<sud_node> occurrences[9];
    count_occurr(n, occurrences);

    outcome hs_outcome = hidden_single(occurrences);
    final_outcome = max(hs_outcome, final_outcome);
    if (final_outcome == NEW_VALUE) return NEW_VALUE;


    outcome pp_outcome = NOTHING;
    for (u i = 0; i < 9; ++i) {
        u candidate = i + 1;
        if (occurrences[i].size() == 2) {
            u occur1_row = occurrences[i][0].rw;
            u occur2_row = occurrences[i][1].rw;
            u occur1_col = occurrences[i][0].cm;
            u occur2_col = occurrences[i][1].cm;
            if (what != SQUARES) {
                if (zero(occur1_row) == zero(occur2_row) &&
                        zero(occur1_col) == zero(occur2_col)) {

                    if (occur1_row == occur2_row) {
                        for (u iter_col = 0; iter_col < 9; ++iter_col) {
                            if (iter_col == occur1_col || iter_col == occur2_col) continue;
                            Cell& cell = *(n[iter_col]);
                            if (!cell.is_solved()) {
                                outcome _outcome = cell.remove_candidate(candidate);
                                pp_outcome = max(pp_outcome, _outcome);
                                if (pp_outcome == NEW_VALUE) return NEW_VALUE;
                            }
                        }

                    } else if (occur1_col == occur2_col) {
                        for (u iter_row = 0; iter_row < 9; ++iter_row) {
                            if (iter_row == occur1_row || iter_row == occur2_row) continue;
                            Cell& cell = *(n[iter_row]);
                            if (!cell.is_solved()) {
                                outcome _outcome = cell.remove_candidate(candidate);
                                pp_outcome = max(pp_outcome, _outcome);
                                if (pp_outcome == NEW_VALUE) return NEW_VALUE;
                            }
                        }
                    }
                }
            }
        }
    }
    final_outcome = max(final_outcome, pp_outcome);

    return final_outcome;

}

void Solver::count_occurr(const niner & n, sud_list<sud_node> * occurrences) {
    for (u i = 0; i < 9; ++i) {
        Cell& cell = *(n[i]);
        if (cell.is_solved()) continue;
        for (u candidate = 0; candidate < 9; ++candidate) {
            if (cell.is_candidate(candidate + 1)) {

                occurrences[candidate].add(cell.node());
            }
        }
    }
}

outcome Solver::hidden_single(const sud_list<sud_node> * occurrences) {
    for (u i = 0; i < 9; ++i) {
        if (occurrences[i].size() == 1) {
            u cell_row = occurrences[i][0].rw;
            u cell_col = occurrences[i][0].cm;
            u single_candidate = i + 1;
            grid.cell(cell_row, cell_col).solve_cell(single_candidate);
            return NEW_VALUE;
        }
    }
    return NOTHING;
}