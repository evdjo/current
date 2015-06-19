

#ifndef SUDOKUTESTS_H
#define	SUDOKUTESTS_H

#include <cppunit/extensions/HelperMacros.h>
#include "../Cell.h"

class SudokuCellTests : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(SudokuCellTests);

    CPPUNIT_TEST(test_init_known_val);
    CPPUNIT_TEST(test_init_unknown_val);
    CPPUNIT_TEST(test_rm_candidate);
    CPPUNIT_TEST(test_last_value);
    CPPUNIT_TEST(test_get_cand);
    CPPUNIT_TEST(test_rm_all_but_pair_cands_excluded);
    CPPUNIT_TEST(test_rm_all_but_pair_new_val_found);
    CPPUNIT_TEST(test_rm_all_but_pair_nothing_found);
    CPPUNIT_TEST(test_join_no_intersection);
    CPPUNIT_TEST(test_join_with_intersection);

    CPPUNIT_TEST_SUITE_END();

public:
    SudokuCellTests();
    virtual ~SudokuCellTests();
    void setUp();
    void tearDown();

private:
    void test_init_known_val();
    void test_init_unknown_val();
    void test_rm_candidate();
    void test_last_value();
    void test_get_cand();
    void test_rm_all_but_pair_cands_excluded();
    void test_rm_all_but_pair_new_val_found();
    void test_rm_all_but_pair_nothing_found();
    void test_join_no_intersection();
    void test_join_with_intersection();
};

#endif	/* SUDOKUTESTS_H */

