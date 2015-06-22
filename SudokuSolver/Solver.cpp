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

void Solver::excludeKnownValues(outcome& final_outcome) {
    for (u row = 0; row < 9; ++row) {
        for (u column = 0; column < 9; ++column) {
            u cell_val = grid.cell(row, column).get_value();
            if (cell_val != CELL_UNKNOWN_VAL) {
                final_outcome = max(excludeKnownVal(row, column, cell_val),
                        final_outcome);
            }
        }
    }
}

void Solver::solve() {
    if (!grid.is_good()) {
        return;
    }
    while (true) {
        outcome final_outcome = NOTHING;
        excludeKnownValues(final_outcome);
        if (final_outcome != NOTHING) continue;

        process(ROWS, final_outcome);
        if (final_outcome == NEW_VALUE) continue;

        process(COLUMNS, final_outcome);
        if (final_outcome == NEW_VALUE) continue;

        process(SQUARES, final_outcome);
        if (final_outcome == NEW_VALUE) continue;

        if (final_outcome == NOTHING) break;

    }
    grid.print();
    grid.print_possible_values();
    grid.verify();
}

void Solver::process(iter_over what, outcome& final_outcome) {
    if (SQUARES == what) {
        for (u row = 0; row < 3; ++row) {
            for (u col = 0; col < 3; ++col) {
                niner n = grid.get_niner_square(row, col);
                process(n, what, final_outcome);
                if (final_outcome == NEW_VALUE) return;
            }
        }
    } else {
        for (u i = 0; i < 9; ++i) {
            niner n = grid.get_niner_line(what, i);
            process(n, what, final_outcome);
            if (final_outcome == NEW_VALUE) return;
        }
    }
}

void Solver::process(const niner & n, iter_over what, outcome& final_outcome) {

    sud_list<sud_node> occurrences[9];
    count_occurr(n, occurrences);

    hidden_single(occurrences, final_outcome);
    if (final_outcome == NEW_VALUE) return;

    pointing_pair(occurrences, n, what, final_outcome);
    if (final_outcome == NEW_VALUE) return;

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

void Solver::hidden_single(const sud_list<sud_node> * occurrences, outcome& hs) {
    for (u i = 0; i < 9; ++i) {
        if (occurrences[i].size() == 1) {
            u cell_row = occurrences[i][0].rw;
            u cell_col = occurrences[i][0].cm;
            u single_candidate = i + 1;
            grid.cell(cell_row, cell_col).solve_cell(single_candidate);
            hs = NEW_VALUE;
            return;
        }
    }
}

void Solver::pointing_pair(const sud_list<sud_node> * occurrences,
        const niner & n, iter_over what, outcome& pp_outcome) {
    for (u i = 0; i < 9; ++i) {
        u candidate = i + 1;
        if (occurrences[i].size() == 2) {
            u occur1_row = occurrences[i][0].rw;
            u occur2_row = occurrences[i][1].rw;
            u occur1_col = occurrences[i][0].cm;
            u occur2_col = occurrences[i][1].cm;
            bool same_row = occur1_row == occur2_row;
            bool same_col = occur1_col == occur2_col;
            if (what != SQUARES) {
                if (zero(occur1_row) == zero(occur2_row) &&
                        zero(occur1_col) == zero(occur2_col)) {
                    for (u index = 0; index < 9; ++index) {
                        if ((same_row &&
                                index == occur1_col || index == occur2_col)
                                ||
                                (same_col &&
                                index == occur1_row || index == occur2_row)) {
                            continue;
                        }
                        rm_pointing_pair((*n[index]), candidate, pp_outcome);
                        if (pp_outcome == NEW_VALUE) return;
                    }
                }
            } else if (what == SQUARES) {

                u zero_col = zero(occur1_col);
                u zero_row = zero(occur1_row);

                for (u index = 0; index < 9; ++index) {
                    if (same_row || same_col) {
                        if ((same_row &&
                                zero(index) == zero_col)
                                ||
                                (same_col
                                && zero(index) == zero_row)) {
                            continue;
                        }
                        Cell & cell = same_row ? grid.cell(occur1_row, index) :
                                grid.cell(index, occur1_col);

                        rm_pointing_pair(cell, candidate, pp_outcome);
                        if (pp_outcome == NEW_VALUE) return;
                    }
                }
            }
        }
    }
}

void Solver::rm_pointing_pair(Cell& cell, u cand, outcome& pp) {
    if (!cell.is_solved()) {
        pp = max(pp, cell.remove_candidate(cand));
    }
}