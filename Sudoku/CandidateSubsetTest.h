#ifndef CANDIDATESUBSETTEST_H
#define	CANDIDATESUBSETTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "Cell.h"
#include "SudokuUtils.h"

class CandidateSubsetTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(CandidateSubsetTest);

    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

public:
    CandidateSubsetTest();
    virtual ~CandidateSubsetTest();
    void setUp();
    void tearDown();

private:
    void test();

};

#endif	/* CANDIDATESUBSETTEST_H */

