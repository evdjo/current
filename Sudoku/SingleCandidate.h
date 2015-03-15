#ifndef HIDDENSINGLES_H
#define	HIDDENSINGLES_H
#include "SudokuUtils.h"
#include "SudokuCell.h"
#include "KnownValues.h"

class SingleCandidate final : public SudokuAlgorithm {
public:
    bool _flag = false;

    SingleCandidate(SudokuCell**sudoku)
    : SudokuAlgorithm(sudoku), hs(sudoku) {
    }
    void apply();
private:
    KnownValues hs;

    bool rows();
    bool columns();
    bool squares();

    // look for single candidates or naked pair
    void count_occurences(const u&, const u&, ocurr_list*);

    //analyze the occurrences
    bool search(ocurr_list*);


    // singles
    void lock_single(const ocurr_list&, const u&);

    // pairs
    bool seek_pair(const ocurr_list&, const u&);
    bool eliminate_pair(const u&, const u&, const u&, bool);


    // 3x3 pairs
    bool pair_3x3(const u& x, const u& y, const u& val, bool flag);

};

#endif	/* HIDDENSINGLES_H */

