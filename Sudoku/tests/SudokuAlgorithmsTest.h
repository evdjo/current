#include "../SudokuModel.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class SudokuAlgorithmsTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(SudokuAlgorithmsTest);

    CPPUNIT_TEST(eliminate_row_test);
    CPPUNIT_TEST(eliminate_column_test);
    CPPUNIT_TEST(eliminate_3x3square_test);

    CPPUNIT_TEST_SUITE_END();


public:
    SudokuAlgorithmsTest();
    virtual ~SudokuAlgorithmsTest();
    void setUp();
    void tearDown();
    SudokuModel sm;
    u expected_full[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

private:
    void eliminate_row_test();
    void eliminate_column_test();
    void eliminate_3x3square_test();
    void assert_eliminated(const u* expected,const SudokuCell& sc);
};