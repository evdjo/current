#include <iostream>
#include <exception>
#include <execinfo.h>
#include "SudokuModel.h"
#include "SudokuUtils.h"
#include "SudCell.h"
using namespace std;

int main(int argc, char** argv) {
    //    if (argc == 2) {
    //        SudokuModel sm(argv[1]);
    //        if (sm.is_good()) {
    //            sm.solve();
    //            sm.verify();
    //        } else {
    //            cerr << "Failed to solve the sudoku..."
    //                    " see the above error message for more detail" << endl;
    //        }
    //    } else {
    //        cerr << "You must pass the path to the sudoku input file." << endl;
    //    }


    SudokuModel sm("input/sudoku2993.sud");
    if (sm.is_good()) {
        sm.solve();
        sm.verify();
    } else {
        cerr << "Failed to solve the sudoku..."
                " see the above error message for more detail" << endl;
    }



    //    SudCell cells[5];
    //    for (u i = 0; i < 5; ++i) {
    //        cells[i].init(0, i, i);
    //    }
    //    cells[1].rmall_but(1, 2);
    //    cells[2].rmall_but(1, 3);
    //    cells[3].rmall_but(2, 3);
    //
    //    //    for (u i = 0; i < 5; ++i) {
    //    //        cells[i].print_possible_values();
    //    //        cout << endl;
    //    //    }
    //
    //    sud_list<SudCell> four_or_less;
    //
    //
    //    four_or_less.add(cells[1]);
    //    four_or_less.add(cells[2]);
    //    four_or_less.add(cells[3]);
    //
    //
    //
    //    for (u i = 4; i > 1; --i) {
    //        if (four_or_less.size() >= i) {
    //
    //            sud_list<SudCell> list;
    //
    //            for (u z = 0; z < four_or_less.size(); ++z) {
    //                if (four_or_less[z].cand_count() <= i) {
    //                    list.add(four_or_less[z]);
    //                }
    //            }
    //
    //            sud_list<sud_node> occurrences[9];
    //            for (u i = 0; i < list.size(); ++i) {
    //                for (u z = 0; z < 9; ++z) {
    //                    if (list[i].is_cand(z + 1)) {
    //                        occurrences[i].add(sud_node(list[i].node().rw, list[i].node().cm, z + 1));
    //                    }
    //                }
    //            }
    //
    //            for (u i = 0; i < 9; ++i) {
    //                if (occurrences[i].s)
    //                }
    //
    //
    //
    //
    //
    //        }
    //    }
    return 0;
}