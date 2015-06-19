#ifndef KNOWNVALUESREMOVER_H
#define	KNOWNVALUESREMOVER_H
#include "SudokuUtils.h"
#include "Cell.h"
#include "SudokuAlgorithm.h"

class KnownValuesRemover : public SudokuAlgorithm {
    using SudokuAlgorithm::SudokuAlgorithm;
public:
    void apply();


private:
    outcome eliminate_val(const sud_node& node);
    //    outcome eliminate_square(const u& row, const u& column, const u& val);
    //    outcome eliminate_column(const u& row, const u& column, const u& val);
    //    outcome eliminate_row(const u& row, const u& column, const u& val);
};

#endif	/* KNOWNVALUESREMOVER_H */

