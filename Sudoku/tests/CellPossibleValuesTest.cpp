/*
 * File:   CellPossibleValuesTest.cpp
 * Author: evdjoint
 *
 * Created on 28-Feb-2015, 10:50:28
 */
#include "../CellPossibleValues.h"
#include "CellPossibleValuesTest.h"
#include <string.h>


CPPUNIT_TEST_SUITE_REGISTRATION(CellPossibleValuesTest);

CellPossibleValuesTest::CellPossibleValuesTest() {
}

CellPossibleValuesTest::~CellPossibleValuesTest() {
}

void CellPossibleValuesTest::setUp() {
}

void CellPossibleValuesTest::tearDown() {
}

void CellPossibleValuesTest::last_value_throws_logical_error() {

    CellPossibleValues cpv;
    try {
        cpv.last_value();
    } catch (const std::logic_error& error) {
        CPPUNIT_ASSERT(strcmp("Not solved yet!", error.what()) == 0);
        return;
    }
    CPPUNIT_FAIL("Should have thrown logical_error!");
}

void CellPossibleValuesTest::testFailedMethod() {
    CellPossibleValues cpv;
    for(unsigned short i = 0 ; i < 7 ; ++i) {
        CPPUNIT_ASSERT_MESSAGE("Should have been able to remove!",cpv.remove(i));
        CPPUNIT_ASSERT_MESSAGE("Should NOT have been solved!",!cpv.solved());
    }
    CPPUNIT_ASSERT_MESSAGE("Should have been able to remove!", cpv.remove(7));
    CPPUNIT_ASSERT_MESSAGE("Should have been solved!", cpv.solved());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Should have been solved!",
            static_cast<unsigned short>(9),cpv.last_value());
    
    
}

