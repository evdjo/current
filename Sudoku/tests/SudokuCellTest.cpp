/*
 * File:   SudokuTests.cpp
 * Author: evdjoint
 *
 * Created on 17-Mar-2015, 19:19:06
 */

#include "SudokuCellTest.h"
#include "../SudokuUtils.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SudokuTests);

SudokuTests::SudokuTests() {
}

SudokuTests::~SudokuTests() {
}

void SudokuTests::setUp() {
}

void SudokuTests::tearDown() {
}

void SudokuTests::test_init_unknown_val() {

    SudokuCell sc;
    sc.init(0);
    CPPUNIT_ASSERT_EQUAL(sc.cand_count(), static_cast<u> (9));

    for (u i = 1; i < 10; ++i)
        CPPUNIT_ASSERT(sc.is_candidate(i));



}

void SudokuTests::test_init_known_val() {
    SudokuCell sc;
    sc.init(1);

    CPPUNIT_ASSERT(!sc.unknown());

    CPPUNIT_ASSERT_THROW(sc.cand_count(), logic_error);
    CPPUNIT_ASSERT_THROW(sc.cands(), logic_error);
    CPPUNIT_ASSERT_THROW(sc.rm_candidate(5), logic_error);
    CPPUNIT_ASSERT_THROW(sc == sc, logic_error);
    CPPUNIT_ASSERT_THROW(sc.is_candidate(5), logic_error);
    CPPUNIT_ASSERT_THROW(sc.init(5), logic_error);

}

void SudokuTests::test_rm_candidate() {
    SudokuCell sc;
    sc.init(0);
    CPPUNIT_ASSERT_EQUAL(sc.cand_count(), static_cast<u> (9));

    for (u i = 1; i < 10; ++i)
        CPPUNIT_ASSERT(sc.is_candidate(i));
}

void SudokuTests::test_last_value() {
    SudokuCell sc;
    sc.init(0);

    for (u i = 1; i < 9; ++i)
        sc.rm_candidate(i);


    CPPUNIT_ASSERT_EQUAL(static_cast<u> (9), sc.last_candidate());

    CPPUNIT_ASSERT_EQUAL(static_cast<u> (9), sc.val());
}





