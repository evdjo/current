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
    void process(iter_over what, outcome&);
    void process(const niner&, iter_over, outcome&);
    void count_occurr(const niner & n, sud_list<sud_node> *);
    void hidden_single(const sud_list<sud_node>*, outcome&);
    void pointing_pair(const sud_list<sud_node>*, const niner &, iter_over, outcome&);
    void rm_pointing_pair(Cell&, u, outcome&);
    void excludeKnownValues(outcome&);
    outcome excludeKnownVal(u row, u column, u cell_val);
    virtual ~Solver();
private:
    Grid grid;
};

#endif	/* SOLVER_H */

