#ifndef HIDDENSINGLES_H
#define	HIDDENSINGLES_H
#include "SudokuUtils.h"
#include "SudokuCell.h"
#include "Algorithm_NakedSingles.h"

class Algorithm_HiddenSingles {
public:
    explicit Algorithm_HiddenSingles(SudokuCell ** sudoku);
    virtual ~Algorithm_HiddenSingles();
    
    bool seek_hidden_singles();
   

private:
    SudokuCell ** the_sudoku;
    Algorithm_NakedSingles ns;

    bool hidden_singles_rows();
    bool hidden_singles_columns();
    bool hidden_singles_3x3();

    inline u cell_val(const u& row, const u& column) {
        return cell(row, column).val();
    }

    inline SudokuCell& cell(const u& row, const u& column) {
        return the_sudoku[row][column];
    }
    
    void count_occurences(const u& row,
            const u& column, occurences_list* values);
    bool lock_single_candidates(const occurences_list* occurrences);   
   

};

#endif	/* HIDDENSINGLES_H */

