/*
 * File:   SudokuTests.cpp
 * Author: evdjoint
 *
 * Created on 17-Mar-2015, 19:19:06
 */

#include "SudokuCellTest.h"
#include "../SudokuUtils.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SudokuCellTests);

SudokuCellTests::SudokuCellTests() { }

SudokuCellTests::~SudokuCellTests() { }

void SudokuCellTests::setUp() { }

void SudokuCellTests::tearDown() { }

void SudokuCellTests::test_init_unknown_val() {

    SudCell sc;
    sc.init(0, 3, 3);
    CPPUNIT_ASSERT_EQUAL(sc.cand_count(), static_cast<u> (9));

    for (u i = 1; i < 10; ++i)
        CPPUNIT_ASSERT(sc.is_cand(i));



}

void SudokuCellTests::test_init_known_val() {
    SudCell sc;
    sc.init(1, 3, 3);

    CPPUNIT_ASSERT(!sc.unknown());

    CPPUNIT_ASSERT_THROW(sc.cand_count(), logic_error);
    CPPUNIT_ASSERT_THROW(sc.cands(), logic_error);
    CPPUNIT_ASSERT_THROW(sc.rm_cand(5), logic_error);
    CPPUNIT_ASSERT_THROW(sc == sc, logic_error);
    CPPUNIT_ASSERT_THROW(sc.is_cand(5), logic_error);
    CPPUNIT_ASSERT_THROW(sc.init(5, 3, 3), logic_error);

}

void SudokuCellTests::test_rm_candidate() {
    SudCell sc;
    sc.init(0, 3, 3);
    CPPUNIT_ASSERT_EQUAL(sc.cand_count(), static_cast<u> (9));

    for (u i = 1; i < 10; ++i)
        CPPUNIT_ASSERT(sc.is_cand(i));
}

void SudokuCellTests::test_last_value() {
    SudCell sc;
    sc.init(0, 3, 3);

    for (u i = 1; i < 9; ++i)
        sc.rm_cand(i);


    CPPUNIT_ASSERT_EQUAL(static_cast<u> (9), sc.last_candidate());

    CPPUNIT_ASSERT_EQUAL(static_cast<u> (9), sc.val());
}

void SudokuCellTests::test_get_cand() {
    SudCell sc;
    sc.init(0, 3, 3);
    sc.rm_cand(1);
    sc.rm_cand(2);
    sc.rm_cand(4);
    sc.rm_cand(6);

    CPPUNIT_ASSERT_EQUAL(static_cast<u> (3), sc.get_cand(0));
    CPPUNIT_ASSERT_EQUAL(static_cast<u> (5), sc.get_cand(1));
    CPPUNIT_ASSERT_EQUAL(static_cast<u> (7), sc.get_cand(2));
    CPPUNIT_ASSERT_EQUAL(static_cast<u> (8), sc.get_cand(3));
}

void SudokuCellTests::test_rm_all_but_pair_cands_excluded() {
    SudCell sc;
    sc.init(0, 3, 3);

    outcome outcome_ = sc.rmall_but(5, 7);

    CPPUNIT_ASSERT_EQUAL(EXCLUDED_CAND, outcome_);
    CPPUNIT_ASSERT_EQUAL(static_cast<u> (5), sc.get_cand(0));
    CPPUNIT_ASSERT_EQUAL(static_cast<u> (7), sc.get_cand(1));
}

void SudokuCellTests::test_rm_all_but_pair_new_val_found() {
    SudCell sc;
    sc.init(0, 3, 3);
    sc.rm_cand(1);
    sc.rm_cand(2);
    //  sc.rm_cand(3);
    sc.rm_cand(4);
    //  sc.rm_cand(5);
    sc.rm_cand(6);
    sc.rm_cand(7);
    sc.rm_cand(8);
    sc.rm_cand(9);

    outcome outcome_ = sc.rmall_but(3, 4);

    CPPUNIT_ASSERT_EQUAL(NEW_VALUE, outcome_);

    CPPUNIT_ASSERT_THROW(sc.get_cand(0), logic_error);
}

void SudokuCellTests::test_rm_all_but_pair_nothing_found() {

    SudCell sc;
    sc.init(0, 3, 3);
    //  sc.rm_cand(1);
    sc.rm_cand(2);
    //  sc.rm_cand(3);
    sc.rm_cand(4);
    sc.rm_cand(5);
    sc.rm_cand(6);
    sc.rm_cand(7);
    sc.rm_cand(8);
    sc.rm_cand(9);

    outcome outcome_ = sc.rmall_but(1, 3);
    CPPUNIT_ASSERT_EQUAL(NOTHING, outcome_);


}