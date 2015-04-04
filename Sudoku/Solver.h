#ifndef HIDDENSINGLES_H
#define	HIDDENSINGLES_H
#include "SudokuUtils.h"
#include "SudCell.h"
#include "KnownValuesRemover.h"

class Solver final : public SudokuAlgorithm {
public:
    //    bool debug_flag = false;
    u bla = 0;
    Solver(SudCell**sudoku)
    : SudokuAlgorithm(sudoku), kvr(sudoku) { }
    void apply();
private:
    KnownValuesRemover kvr;
    outcome rows();
    outcome columns();
    outcome squares();

    // count candidate occurrences
    void count_cands(const u&, const u&, sud_list<sud_node>*);

    //analyze the occurrences
    outcome search(sud_list<sud_node>* list);

    /*** <--- Hidden Single ---> ***/
    void lock_single_candidate(const sud_list<sud_node>&, const u&);

    /*** <--- Pairs start ---> ***/
    // naked pairs
    outcome naked_pairs(const sud_list<sud_node>&, bool);
    outcome elim_naked_pair(const SudCell&, const SudCell&, bool);

    // pointing pair
    outcome pointing_pair(const sud_list<sud_node>&, const u&);
    outcome elim_pointing_pair(const u&, const u&, const u&, bool);

    // hidden pair
    outcome hidden_pairs(const sud_list<sud_list<sud_node>>&prs);
    /*** <--- Pairs end ---> ***/


    /*** <--- Tripples start ---> ***/
    //naked tripples
    outcome naked_trip(const sud_list<sud_node>&, const sud_list<sud_node>&, iter_over);
    outcome two_equal_threes(const SudCell&, const SudCell&, const sud_list<sud_node>&, iter_over);
    outcome elim_trip(const SudCell &, const SudCell &, const SudCell &, iter_over);

    // pointing tripple
    outcome pointing_trip(const sud_list<sud_node>&, const u&);
    outcome elim_pointing_trip(const u&, const u&, const u&, bool);
    /*** <--- Tripples end ---> ***/


};
#endif	/* HIDDENSINGLES_H */