

#ifndef SUDOKUTESTS_H
#define	SUDOKUTESTS_H

#include <cppunit/extensions/HelperMacros.h>
#include "Cell.h"

class SudokuListTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(SudokuListTest);

    CPPUNIT_TEST(test_list);
    CPPUNIT_TEST(test_list_of_lists);


    CPPUNIT_TEST_SUITE_END();

public:
    SudokuListTest();
    virtual ~SudokuListTest();
    void setUp();
    void tearDown();

private:
    void test_list();
    void test_list_of_lists();

};

#endif	/* SUDOKUTESTS_H */

