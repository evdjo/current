#include "../CellCandidates.h"
#include "CellCandidatesTest.h"
#include <string.h>


CPPUNIT_TEST_SUITE_REGISTRATION(CellCandidatesTest);

CellCandidatesTest::CellCandidatesTest() {
}

CellCandidatesTest::~CellCandidatesTest() {
}

void CellCandidatesTest::setUp() {
}

void CellCandidatesTest::tearDown() {
}

void CellCandidatesTest::last_value_throws_logical_error() {

    CellCandidates cc;
    try {
       cc.last_value();
    } catch (const std::logic_error& error) {
        CPPUNIT_ASSERT(strcmp("Not solved yet!", error.what()) == 0);
        return;
    }
    CPPUNIT_FAIL("Should have thrown logical_error!");
}

void CellCandidatesTest::remove_8_times_causes_cell_to_be_solved() {
    CellCandidates cc;
    for (unsigned short i = 1; i < 8; ++i) {
        CPPUNIT_ASSERT_MESSAGE("Should have been able to remove!", cc.remove(i));
        CPPUNIT_ASSERT_MESSAGE("Should NOT have been solved!", !cc.one_candidate_left());
    }
    CPPUNIT_ASSERT_MESSAGE("Should have been able to remove!", cc.remove(9));
    CPPUNIT_ASSERT_MESSAGE("Should have been solved!", cc.one_candidate_left());
    unsigned short actual = cc.last_value();
    unsigned short expected = 8;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("The last value should have been 8!",
            actual, expected);


}

void CellCandidatesTest::remove_9_times_throws_logical_error() {
    CellCandidates cc;
    for (unsigned short i = 1; i < 9; ++i) {
        cc.remove(i);
    }
    CPPUNIT_ASSERT_THROW_MESSAGE("Should have thrown logic_error!",
            cc.remove(9), logic_error);

}

void CellCandidatesTest::remove_with_invalid_args_throws_invalid_arg() {
    CellCandidates cc;
    CPPUNIT_ASSERT_THROW_MESSAGE("Should have rejected -1 as input!",
            cc.remove(-1), invalid_argument);
    CPPUNIT_ASSERT_THROW_MESSAGE("Should have rejected 10 as input!",
            cc.remove(10), invalid_argument);
}

void CellCandidatesTest::candidates_equals() {
//TODO
//    CellCandidates cc_0;
//    CellCandidates cc_1;
//    for (u_short i = 1; i < 9 ; ++i) {
//        cc_0.remove(i);
//        CPPUNIT_ASSERT(cc_0 != cc_1);
//        cc_1.remove(i);
//        CPPUNIT_ASSERT(cc_0 == cc_1);
//    }
}