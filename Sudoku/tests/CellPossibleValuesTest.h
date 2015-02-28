/*
 * File:   CellPossibleValuesTest.h
 * Author: evdjoint
 *
 * Created on 28-Feb-2015, 10:50:28
 */

#ifndef CELLPOSSIBLEVALUESTEST_H
#define	CELLPOSSIBLEVALUESTEST_H

#include <cppunit/extensions/HelperMacros.h>

class CellPossibleValuesTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(CellPossibleValuesTest);

    CPPUNIT_TEST(last_value_throws_logical_error);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    CellPossibleValuesTest();
    virtual ~CellPossibleValuesTest();
    void setUp();
    void tearDown();

private:
    void last_value_throws_logical_error();
    void testFailedMethod();
};

#endif	/* CELLPOSSIBLEVALUESTEST_H */

