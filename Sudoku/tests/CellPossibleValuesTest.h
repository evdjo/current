#ifndef CELLPOSSIBLEVALUESTEST_H
#define	CELLPOSSIBLEVALUESTEST_H

#include <cppunit/extensions/HelperMacros.h>

class CellPossibleValuesTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(CellPossibleValuesTest);

    CPPUNIT_TEST(last_value_throws_logical_error);
    CPPUNIT_TEST(remove_8_times_causes_cell_to_be_solved);
    CPPUNIT_TEST(remove_with_invalid_args_throws_invalid_arg);
    CPPUNIT_TEST(remove_9_times_throws_logical_error);

    CPPUNIT_TEST_SUITE_END();

public:
    CellPossibleValuesTest();
    virtual ~CellPossibleValuesTest();
    void setUp();
    void tearDown();

private:
    void last_value_throws_logical_error();
    void remove_8_times_causes_cell_to_be_solved();
    void remove_with_invalid_args_throws_invalid_arg();
    void remove_9_times_throws_logical_error();
};

#endif	/* CELLPOSSIBLEVALUESTEST_H */

