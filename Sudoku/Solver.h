#ifndef HIDDENSINGLES_H
#define	HIDDENSINGLES_H
#include "SudokuUtils.h"
#include "Cell.h"
#include "KnownValuesRemover.h"

class Solver final : public SudokuAlgorithm {
public:
    //    bool debug_flag = false;
    u bla = 0;
    Solver(Cell**sudoku)
    : SudokuAlgorithm(sudoku), kvr(sudoku) { }
    void apply();
private:
    KnownValuesRemover kvr;
    outcome rows();
    outcome columns();
    outcome squares();

    // count candidate occurrences
    void count_cands(const u&, const u&, sud_list<sud_node>*);

    // search the results of the above count_cands function
    outcome search(sud_list<sud_node>* list);

    /*** <--- Hidden Single ---> ***/
    void lock_single_candidate(const sud_list<sud_node>&, const u&);

    // naked pairs/tripples/quads
    outcome naked(const sud_list<Cell>& list, iter_over what);
    outcome elim_naked_pair(const Cell&, const Cell&, iter_over);

    // pointing pair
    outcome pointing_pair(const sud_list<sud_node>&, const u&);
    outcome elim_pointing_pair(const u&, const u&, const u&, bool);

    // hidden pair
    outcome hidden_pairs(const sud_list<sud_list<sud_node>>&prs);

    // pointing tripple
    outcome pointing_trip(const sud_list<sud_node>&, const u&);
    outcome elim_pointing_trip(const u&, const u&, const u&, bool);
};
#endif	/* HIDDENSINGLES_H */