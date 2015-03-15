#ifndef SUDOKUALGORITHM_H
#define	SUDOKUALGORITHM_H
#include "SudokuCell.h"

class SudokuAlgorithm {
public:

    virtual void apply() = 0;
    explicit SudokuAlgorithm(const SudokuCell ** sudoku);
    virtual ~SudokuAlgorithm();

private:
    //the sudoku to apply the algorithms on
    const SudokuCell ** the_sudoku;

    //prevent from using
    SudokuAlgorithm(const SudokuAlgorithm& orig);
    SudokuAlgorithm&operator=(const SudokuAlgorithm& other);

};

#endif	/* SUDOKUALGORITHM_H */

