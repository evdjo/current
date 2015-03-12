#ifndef OCCURRENCELISTTEST_H
#define	OCCURRENCELISTTEST_H

#include "../SudokuUtils.h"
#include <cppunit/extensions/HelperMacros.h>

class OccurrenceListTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(OccurrenceListTest);

    CPPUNIT_TEST(testListInsertion);
   

    CPPUNIT_TEST_SUITE_END();

public:
    OccurrenceListTest();
    virtual ~OccurrenceListTest();
    void setUp();
    void tearDown();

private:
    occurences_list ol;
    void testListInsertion(); 
};

#endif	/* OCCURRENCELISTTEST_H */

