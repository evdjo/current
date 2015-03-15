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

    CPPUNIT_ASSERT_THROW(ol.at(0), invalid_argument);
    CPPUNIT_ASSERT_THROW(ol.at(-1), invalid_argument);
    CPPUNIT_ASSERT_THROW(ol.at(100), invalid_argument);
    CPPUNIT_ASSERT_THROW(ol.at(-100), invalid_argument);
    ol.add_(5, 5);
    CPPUNIT_ASSERT_THROW(ol.at(1), invalid_argument);
    CPPUNIT_ASSERT_THROW(ol.at(100), invalid_argument);
    CPPUNIT_ASSERT_THROW(ol.at(-100), invalid_argument);
    {
        occur_node node(5, 5);
        CPPUNIT_ASSERT(node == ol.first());
    }
    {
        occur_node node(1, 1);
        ol.add_(1, 1);
        CPPUNIT_ASSERT(node == ol.last());
    }
    


}



