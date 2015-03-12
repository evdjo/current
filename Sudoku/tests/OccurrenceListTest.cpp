/*
 * File:   OccurrenceListTest.cpp
 * Author: evdjoint
 *
 * Created on 11-Mar-2015, 21:52:48
 */

#include "OccurrenceListTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(OccurrenceListTest);

OccurrenceListTest::OccurrenceListTest() {
}

OccurrenceListTest::~OccurrenceListTest() {
}

void OccurrenceListTest::setUp() {
}

void OccurrenceListTest::tearDown() {
    
    
}

void OccurrenceListTest::testListInsertion() {
    for (u i = 1; i < 5; ++i) {
        ol.add_(i, i);
    }
    ol.print_list();
   
}



