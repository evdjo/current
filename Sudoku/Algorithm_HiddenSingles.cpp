#include "Algorithm_HiddenSingles.h"

Algorithm_HiddenSingles::Algorithm_HiddenSingles(SudokuCell ** sudoku)
: the_sudoku(sudoku), ns(sudoku) {
}

Algorithm_HiddenSingles::~Algorithm_HiddenSingles() {
}

bool Algorithm_HiddenSingles::seek_hidden_singles() {
    bool _rows = hidden_singles_rows();
    bool _columns = hidden_singles_columns();
    bool _3x3squares = hidden_singles_3x3();
    return (_rows || _columns || _3x3squares);
}

bool Algorithm_HiddenSingles::hidden_singles_rows() {
    bool singles_found = false;
    for (u row = 0; row < 9; ++row) {
        occurr_t occurrences[9];
        for (u column = 0; column < 9; ++column) {
            if (!cell(row, column).unknown()) continue;
            count_occurences(row, column, occurrences);
        }

        singles_found = lock_single_candidates(occurrences)
                ? true : singles_found;
        if (singles_found) while (ns.eliminate_known_vals()); // TODO

    }
    return singles_found;
}

bool Algorithm_HiddenSingles::hidden_singles_columns() {
    bool singles_found = false;
    for (u column = 0; column < 9; ++column) {
        occurr_t occurrences[9];
        for (u row = 0; row < 9; ++row) {
            if (!cell(row, column).unknown()) continue;
            count_occurences(row, column, occurrences);
        }
        singles_found = lock_single_candidates(occurrences)
                ? true : singles_found;
        if (singles_found) while (ns.eliminate_known_vals()); // TODO

    }
    return singles_found;
}

bool Algorithm_HiddenSingles::hidden_singles_3x3() {
    bool singles_found = false;
    for (u srow = 0; srow <= 6; srow += 3) {
        for (u scmn = 0; scmn <= 6; scmn += 3) {

            occurr_t occurrences[9];

            for (u row = srow; row < srow + 3; ++row) {
                for (u cmn = scmn; cmn < scmn + 3; ++cmn) {
                    if (!cell(row, cmn).unknown()) continue;
                    count_occurences(row, cmn, occurrences);
                }
            }
            singles_found = lock_single_candidates(occurrences)
                    ? true : singles_found;
            if (singles_found) while (ns.eliminate_known_vals()); // TODO

        }
    }
    return singles_found;
}

bool Algorithm_HiddenSingles::lock_single_candidates(const occurr_t* ocr) {
    bool singles_found = false;
    for (u i = 0; i < 9; ++i) {
        if (ocr[i].count == 1) {
            singles_found = true;
            cell(ocr[i].last_row, ocr[i].last_column).set_val(i + 1);
        }
    }
    return singles_found;
}

void Algorithm_HiddenSingles::count_occurences
(const u& row, const u& column, occurr_t * occurrences) {

    SudokuCell& sc = cell(row, column);
    for (u i = 0; i < 9; ++i) {
        if (sc.is_candidate(i + 1)) {
            occurrences[i].count++;
            occurrences[i].last_row = row;
            occurrences[i].last_column = column;
        }
    }
}

