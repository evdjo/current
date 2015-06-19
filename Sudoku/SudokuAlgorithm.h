#ifndef SUDOKUALGORITHM_H
#define	SUDOKUALGORITHM_H
#include "Cell.h"
#include "SudokuUtils.h"
#define zero(index) (((index) / 3) * 3)

class SudokuAlgorithm {
public:

    virtual void apply() = 0;
    explicit SudokuAlgorithm(Cell ** sudoku) : the_sudoku(sudoku) { };


protected:
    //the sudoku to apply the algorithms on
    Cell ** the_sudoku;
    inline u cell_val(const u& row, const u& column) {
        return cell(row, column).val();
    }
    inline Cell& cell(const u& row, const u& column) {
        return the_sudoku[row][column];
    }
    inline Cell& cell(const sud_node& n) {
        return the_sudoku[n.rw][n.cm];
    }
    inline void check_sudoku() {
        if (the_sudoku == nullptr) throw logic_error("The don't have sudoku to work on...");
    }
    void print() {
        if (the_sudoku == nullptr) return;
        for (u row = 0; row < 9; row++) {
            for (u column = 0; column < 9; column++) {
                cout << cell_val(row, column);
                // print space each three columns
                if ((8 - column) % 3 == 0) cout << " ";
            }
            cout << endl;
            // print new line each three columns
            if ((8 - row) % 3 == 0) cout << endl;
        }
    }
    void print_possible_values() {
        if (the_sudoku == nullptr) return;
        for (u row = 0; row < 9; row++) {
            cout << endl;
            for (u column = 0; column < 9; column++) {
                cout << '[' << row;
                cout << "x";
                cout << column << ']';
                cout << "=";
                if (cell(row, column).unknown())
                    cell(row, column).print_possible_values();
                else {
                    for (u i = 1; i < 10; i++)
                        if (i == cell_val(row, column))
                            cout << " "; // << cell_val(row, column);
                        else
                            cout << " ";
                }
                cout << " ";
                // print space each three columns
                if ((8 - column) % 3 == 0) cout << "   ";
            }
            // print new line each three columns
            if ((8 - row) % 3 == 0) cout << endl << endl;
        }
    }
    outcome iterate_over
    (outcome(*f)(Cell&, const sud_node&), const sud_node& n, iter_over what) {
        return iterate_over(f, n, what, nullptr);
    }
    outcome iterate_over(outcome(*f)(Cell&, const sud_node&),
            const sud_node& origin, iter_over what, sud_list<sud_node>* skip) {
        bool skips = skip != nullptr;
        outcome _otcm = NOTHING;
        u rw = origin.rw;
        u cm = origin.cm;
        if (what == SQUARE) {
            u zero_rw = zero(rw);
            u zero_cm = zero(cm);
            for (u iter_rw = zero_rw; iter_rw < zero_rw + 3; ++iter_rw) {
                for (u iter_cm = zero_cm; iter_cm < zero_cm + 3; ++iter_cm) {
                    if (skips && skip->contains(sud_node(iter_rw, iter_cm)))
                        continue;
                    _otcm = max(_otcm, f(cell(iter_rw, iter_cm), origin));
                    if (_otcm == NEW_VALUE) return NEW_VALUE;
                }
            }
        } else if (what == ROW) {
            for (u iter_index = 0; iter_index < 9; ++iter_index) {
                if (skips && skip->contains(sud_node(rw, iter_index)))
                    continue;
                _otcm = max(_otcm, f(cell(rw, iter_index), origin));
                if (_otcm == NEW_VALUE) return NEW_VALUE;
            }
        } else if (what == COLUMN) {
            for (u iter_index = 0; iter_index < 9; ++iter_index) {
                if (skips && skip->contains(sud_node(iter_index, cm)))
                    continue;
                _otcm = max(_otcm, f(cell(iter_index, cm), origin));
                if (_otcm == NEW_VALUE) return NEW_VALUE;
            }
        }
        return _otcm;

    }

};

#endif	/* SUDOKUALGORITHM_H */