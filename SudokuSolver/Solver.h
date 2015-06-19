#ifndef SOLVER_H
#define	SOLVER_H
#include <string>
#include "Grid.h"
#include "SudokuUtils.h"

using namespace std;

class Solver final {
    friend class GridTest;
public:
    bool flag = false;
    explicit Solver(const string& filename);
    void solve();
    outcome process(iter_over what);
    outcome process(const niner& n, iter_over what);
    void count_occurr(const niner & n, sud_list<sud_node> * occurrences);
    outcome hidden_single(const sud_list<sud_node> * occurrences);
    outcome excludeKnownValues();
    outcome excludeKnownVal(u row, u column, u cell_val);
    virtual ~Solver();
private:
    Grid grid;
};

#endif	/* SOLVER_H */

