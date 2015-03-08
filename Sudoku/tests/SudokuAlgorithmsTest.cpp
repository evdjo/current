#include "../CellCandidates.h"
#include "SudokuAlgorithmsTest.h"
#include <string.h>


CPPUNIT_TEST_SUITE_REGISTRATION(SudokuAlgorithmsTest);

SudokuAlgorithmsTest::SudokuAlgorithmsTest() {
}

SudokuAlgorithmsTest::~SudokuAlgorithmsTest() {
}

void SudokuAlgorithmsTest::setUp() {
    sm.the_sudoku = new SudokuCell*[9];
    for (int i = 0; i < 9; ++i) {
        sm.the_sudoku[i] = new SudokuCell[9];
        for (int o = 0; o < 9; o++) {
            sm.cell_val(i, o) = 0;
        }
    }
}

void SudokuAlgorithmsTest::tearDown() {
}

void SudokuAlgorithmsTest::eliminate_row_test() {
    //set up
    const u_short removed_value = 5;
    const u_short expected[9] = {1, 2, 3, 4, 0, 6, 7, 8, 9};
    u_short known_val_row = 3;
    u_short known_val_column = 6;

    // execute code under test
    sm.cell_val(known_val_row, known_val_column) = removed_value;
    sm.eliminate_known_vals();

    // assert removed values
    for (int i = 0; i < 9; ++i) {
        if (i == known_val_column) continue;
        SudokuCell& sc = sm.cell(known_val_row, i);
        assert_eliminated(expected, sc);
    }
}

void SudokuAlgorithmsTest::eliminate_column_test() {
    //set up
    const u_short expected[9] = {1, 2, 3, 4, 5, 6, 0, 8, 9};
    u_short known_val_row = 2;
    u_short known_val_column = 6;
    sm.cell_val(known_val_row, known_val_column) = 7;

    // execute code under test
    sm.eliminate_known_vals();

    // assert removed values
    for (int i = 0; i < 9; ++i) {
        if (i == known_val_row) continue;
        SudokuCell& sc = sm.cell(i, known_val_column);
        assert_eliminated(expected, sc);
    }
}

void SudokuAlgorithmsTest::eliminate_3x3square_test() {
    //set up
    const u_short expected[9] = {1, 0, 3, 4, 5, 6, 7, 8, 9};
    u_short known_val_row = 3;
    u_short known_val_column = 5;
    sm.cell_val(known_val_row, known_val_column) = 2;

    // execute code under test
    sm.eliminate_known_vals();

    // assert removed values
    for (int i = 3; i < 6; ++i) {
        for (int z = 3; z < 6; ++z) {
            if (i != known_val_row && z != known_val_column) {
                SudokuCell& sc = sm.cell(i, z);
                assert_eliminated(expected, sc);
            }
        }
    }

}

void SudokuAlgorithmsTest::
assert_eliminated(const u_short* expected, const SudokuCell& sc) {
    for (int z = 0; z < 9; z++) {
        u_short expected_val = expected[z];
        u_short actual_val = sc.cc->candidates[z];
        CPPUNIT_ASSERT_EQUAL(expected_val, actual_val);
    }
}