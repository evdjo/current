#include "HiddenSingles_NakedPairs.h"

HiddenSingles_NakedPairs::
HiddenSingles_NakedPairs(SudokuCell ** sudoku)
: the_sudoku(sudoku), ns(sudoku) {
}

HiddenSingles_NakedPairs::~HiddenSingles_NakedPairs() {
}

void HiddenSingles_NakedPairs::seek_hidden_singles() {

    bool _rows = true, _columns = true, _3x3squares = true;
    while (_rows || _columns || _3x3squares) {
        _rows = hidden_singles_rows();
        _columns = hidden_singles_columns();
        _3x3squares = hidden_singles_3x3();
    }
}

bool HiddenSingles_NakedPairs::hidden_singles_rows() {
    bool singles_found = false;
    for (u row = 0; row < 9; ++row) {
        occurences_list occurrences[9];
        for (u column = 0; column < 9; ++column) {
            if (!cell(row, column).unknown()) continue;
            count_occurences(row, column, occurrences);
        }

        singles_found = lock_single_candidates(occurrences)
                ? true : singles_found;

    }
    return singles_found;
}

bool HiddenSingles_NakedPairs::hidden_singles_columns() {
    bool singles_found = false;
    for (u column = 0; column < 9; ++column) {
        occurences_list occurrences[9];
        for (u row = 0; row < 9; ++row) {
            if (!cell(row, column).unknown()) continue;
            count_occurences(row, column, occurrences);
        }
        singles_found = lock_single_candidates(occurrences)
                ? true : singles_found;

    }
    return singles_found;
}

bool HiddenSingles_NakedPairs::hidden_singles_3x3() {
    bool singles_found = false;
    for (u srow = 0; srow <= 6; srow += 3) {
        for (u scmn = 0; scmn <= 6; scmn += 3) {

            occurences_list occurrences[9];

            for (u row = srow; row < srow + 3; ++row) {
                for (u cmn = scmn; cmn < scmn + 3; ++cmn) {
                    if (!cell(row, cmn).unknown()) continue;
                    count_occurences(row, cmn, occurrences);
                }
            }
            singles_found = lock_single_candidates(occurrences)
                    ? true : singles_found;

        }
    }
    return singles_found;
}

void HiddenSingles_NakedPairs::count_occurences
(const u& row, const u& column, occurences_list * occurrences) {

    SudokuCell& sc = cell(row, column);
    for (u i = 0; i < 9; ++i) {
        if (sc.is_candidate(i + 1)) {
            occurrences[i].add_(row, column);
        }
    }
}

bool HiddenSingles_NakedPairs::lock_single_candidates(occurences_list* ocr) {
    bool change_occurred = false;
    for (u i = 0; i < 9; ++i) {
        occurences_list& current = ocr[i];
        if (current.m_count == 1) {
            lock_single(current, i + 1);
            change_occurred = true;
        }
        if (current.m_count == 2) {
            if (seek_pair(current, i + 1)) {
                change_occurred = true;
            }
        }
    }
    return change_occurred;
}

void HiddenSingles_NakedPairs::
lock_single(const occurences_list& current, const u& value) {
    u row = current.first().row;
    u column = current.first().column;
    cell(row, column).set_val(value);
    if (ns.eliminate_val(row, column, value)) // if value was eliminated
        ns.eliminate_known_vals();
}

bool HiddenSingles_NakedPairs::
seek_pair(const occurences_list& current, const u& value) {
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
    }
    return change_occurred;
}

bool HiddenSingles_NakedPairs::eliminate_pair
(const u& x, const u & y, const u& val, bool row_column_flag) {
    bool change_occurred = false;

    u zero_index = SudokuUtils::zero_index(y);

    for (u iter_over = 0; iter_over < 9; ++iter_over) {
        if (iter_over < zero_index || iter_over >= zero_index + 3) {

            SudokuCell& sc = row_column_flag
                    ? cell(x, iter_over) : cell(iter_over, x);

            if (sc.unknown() && sc.rm_candidate(val)) {
                change_occurred = true;
            }
        }
    }
    return change_occurred;
}

