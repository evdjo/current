

#ifndef SUDOKUTESTS_H
#define	SUDOKUTESTS_H

#include <cppunit/extensions/HelperMacros.h>
#include "../SudokuCell.h"

class SudokuTests : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(SudokuTests);

    CPPUNIT_TEST(test_init_known_val);
    CPPUNIT_TEST(test_init_unknown_val);
    CPPUNIT_TEST(test_rm_candidate);
    CPPUNIT_TEST(test_last_value);

    CPPUNIT_TEST_SUITE_END();

public:
    SudokuTests();
    virtual ~SudokuTests();
    void setUp();
    void tearDown();

private:
    void test_init_known_val();
    void test_init_unknown_val();
    void test_rm_candidate();
    void test_last_value();
};

#endif	/* SUDOKUTESTS_H */

