#ifndef HIDDENSINGLES_H
#define	HIDDENSINGLES_H
#include <list>
#include "SudokuUtils.h"
#include "SudokuCell.h"
#include "SinglePosition.h"

class SingleCandidate final : public SudokuAlgorithm {
public:
    SingleCandidate(SudokuCell**sudoku)
    : SudokuAlgorithm(sudoku), hs(sudoku) { }
    void apply();
private:
    SinglePosition hs;
    outcome rows();
    outcome columns();
    outcome squares();

    // look for single candidates or naked pair
    void count_candidates(const u&, const u&, sud_list<sud_node>*);

    //analyze the occurrences
    outcome search(sud_list<sud_node>* list);

    // singles
    void lock_single_candidate(const sud_list<sud_node>&, const u&);

    // pairs

    // pointing pair
    outcome pointing_pair(const sud_list<sud_node>&, const u&);
    outcome elim_pointing_pair(const u&, const u&, const u&, bool);

    // hidden pair
    outcome seek_hidden_pairs(const sud_list<sud_list<sud_node>>&prs);
};
#endif	/* HIDDENSINGLES_H */