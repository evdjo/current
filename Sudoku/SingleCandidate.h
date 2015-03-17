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
    bool rows();
    bool columns();
    bool squares();
    // look for single candidates or naked pair
    void count_occurences(const u&, const u&, occurr_list*);
    //analyze the occurrences
    bool search(occurr_list*);
    // singles
    void lock_single(const occurr_list&, const u&);
    // pairs
    bool seek_pair(const occurr_list&, const u&);
    bool eliminate_pair(const u&, const u&, const u&, bool);
    // 3x3 pairs
    bool pair_3x3(const u& x, const u& y, const u& val, bool flag);


    bool exclude_pair(const u& val_1, const u& val_2,
            const u& rw_cm_1, const u& rw_cm_2, const u& rw_cm, bool flag);
};
#endif	/* HIDDENSINGLES_H */