#ifndef KNOWNVALUESREMOVER_H
#define	KNOWNVALUESREMOVER_H
#include "SudokuUtils.h"
#include "SudokuCell.h"
#include "SudokuAlgorithm.h"

class SinglePosition : public SudokuAlgorithm {
    using SudokuAlgorithm::SudokuAlgorithm;
public:
    void apply();
    outcome eliminate_val(const u& row, const u& column, const u& val);

private:
    outcome eliminate_square(const u& row, const u& column, const u& val);
    outcome eliminate_column(const u& row, const u& column, const u& val);
    outcome eliminate_row(const u& row, const u& column, const u& val);
};

#endif	/* KNOWNVALUESREMOVER_H */

