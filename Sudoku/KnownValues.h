#ifndef KNOWNVALUESREMOVER_H
#define	KNOWNVALUESREMOVER_H
#include "SudokuUtils.h"
#include "SudokuCell.h"
#include "SudokuAlgorithm.h"

class KnownValues : public SudokuAlgorithm {
    using SudokuAlgorithm::SudokuAlgorithm;
public:
    void apply();
    bool eliminate_val(const u& row, const u& column, const u& val);

private:
    bool squares(const u& row, const u& column, const u& val);
    bool columns(const u& row, const u& column, const u& val);
    bool rows(const u& row, const u& column, const u& val);
};

#endif	/* KNOWNVALUESREMOVER_H */

