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
                if (process(n) == NEW_VALUE) return NEW_VALUE;
            }
        }
    } else {
        for (u i = 0; i < 9; ++i) {
            niner n = grid.get_niner_line(what, i);
            if (process(n) == NEW_VALUE) return NEW_VALUE;
        }
    }
    return NOTHING;
}

outcome Solver::process(const niner & n) {
    outcome final_outcome = NOTHING;
    sud_list<sud_node> occurrences[9];
    for (u i = 0; i < 9; ++i) {
        Cell& cell = *(n[i]);
        if (cell.is_solved()) continue;
        for (u candidate = 0; candidate < 9; ++candidate) {
            if (cell.is_candidate(candidate + 1)) {
                occurrences[candidate].add(cell.node());
            }
        }
    }

    for (u i = 0; i < 9; ++i) {
        if (occurrences[i].size() == 1) {
            u cell_row = occurrences[i][0].rw;
            u cell_col = occurrences[i][0].cm;
            u single_candidate = i + 1;
            grid.cell(cell_row, cell_col).solve_cell(single_candidate);
            final_outcome = NEW_VALUE;
        }
    }
    return final_outcome;
}