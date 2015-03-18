#ifndef HIDDENSINGLES_H
#define	HIDDENSINGLES_H
#include "SudokuUtils.h"
#include "SudokuCell.h"
#include "SinglePosition.h"

class SingleCandidate final : public SudokuAlgorithm {
public:

    SingleCandidate(SudokuCell**sudoku)
    : SudokuAlgorithm(sudoku), hs(sudoku) {
    }
    void apply();
private:
    SinglePosition hs;
    outcome rows();
    outcome columns();
    outcome squares();
    // look for single candidates or naked pair
    void count_occurences(const u&, const u&, occurr_list*);
    //analyze the occurrences
    outcome search(occurr_list*);
    // singles
    void lock_single_candidate(const occurr_list&, const u&);
    // pairs
    outcome seek_single_pair(const occurr_list&, const u&);
    outcome elim_single_pair(const u&, const u&, const u&, bool);



    outcome seek_double_pair(occurr_list& current);
    outcome elim_double_pair(const u& val_1, const u& val_2,
            const u& rw_cm_1, const u& rw_cm_2, const u& rw_cm, bool flag);
};
#endif	/* HIDDENSINGLES_H */