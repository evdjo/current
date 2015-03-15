#ifndef HIDDENSINGLES_H
#define	HIDDENSINGLES_H
#include "SudokuUtils.h"
#include "SudokuCell.h"
#include "KnownValues.h"

class HiddenSingles_NakedPairs {
public:
    explicit HiddenSingles_NakedPairs(SudokuCell ** sudoku);
    virtual ~HiddenSingles_NakedPairs();
    void seek_hidden_singles();
   

private:
    SudokuCell ** the_sudoku;
    KnownValues ns;

    void lock_single(const occurences_list& current, const u& value);
    bool seek_pair(const occurences_list& current, const u& value);
    
    bool hidden_singles_rows();
    bool hidden_singles_columns();
    bool hidden_singles_3x3();

    bool eliminate_pair
    (const u& row, const u & column, const u& val, bool row_column_flag);

    inline u cell_val(const u& row, const u& column) {
        return cell(row, column).val();
    }

    inline SudokuCell& cell(const u& row, const u& column) {
        return the_sudoku[row][column];
    }

    void count_occurences(const u& row,
            const u& column, occurences_list* values);
    bool lock_single_candidates(occurences_list* occurrences);


};

#endif	/* HIDDENSINGLES_H */

