/*
 * File:   SudokuTests.cpp
 * Author: evdjoint
 *
 * Created on 17-Mar-2015, 19:19:06
 */

#include "SudokuListTest.h"
#include "SudokuUtils.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SudokuListTest);

SudokuListTest::SudokuListTest() { }

SudokuListTest::~SudokuListTest() { }

void SudokuListTest::setUp() { }

void SudokuListTest::tearDown() { }

void SudokuListTest::test_list() {

    sud_list<sud_node> list1;
    list1.add(sud_node(5, 5, 5));
    list1.add(sud_node(1, 1, 1));
    list1.add(sud_node(7, 7, 7));
    sud_list<sud_node> list2;
    list2.add(sud_node(5, 5, 5));
    list2.add(sud_node(1, 1, 1));
    list2.add(sud_node(7, 7, 7));

    CPPUNIT_ASSERT(list1 == list2);

    list2.add(sud_node(3, 3, 3));
    CPPUNIT_ASSERT(list1 != list2);


    list1.add(sud_node(3, 3, 3));
    CPPUNIT_ASSERT(list1 == list2);


    list2.add(sud_node(8, 8, 8));
    list1.add(sud_node(0, 0, 0));
    CPPUNIT_ASSERT(list1 != list2);


}

void SudokuListTest::test_list_of_lists() {

    sud_list<sud_list < sud_node>> list_list1;
    sud_list<sud_list < sud_node>> list_list2;

    sud_list<sud_node> list1;
    for (u i = 0; i < 5; ++i) {
        list1.add(sud_node(i, i, i));
    }

    sud_list<sud_node> list2;
    for (u i = 5; i < 10; ++i) {
        list2.add(sud_node(i, i, i));
    }
    sud_list<sud_node> list3;

    for (u i = 10; i < 15; ++i) {
        list3.add(sud_node(i, i, i));
    }
    list_list1.add(list1);
    list_list1.add(list2);
    list_list1.add(list3);


    list1.add(sud_node(55, 55, 55));
    list1.add(sud_node(88, 88, 88));
    list1.add(sud_node(111, 111, 111));

    list2.add(sud_node(77, 77, 77));
    list2.add(sud_node(33, 33, 33));

    list3.add(sud_node(22, 22, 22));

    list_list2.add(list1);
    list_list2.add(list2);
    list_list2.add(list3);

    CPPUNIT_ASSERT(list_list1 != list_list2);

    list_list1[0].add(sud_node(55, 55, 55));
    list_list1[0].add(sud_node(88, 88, 88));
    list_list1[0].add(sud_node(111, 111, 111));

    list_list1[1].add(sud_node(77, 77, 77));
    list_list1[1].add(sud_node(33, 33, 33));

    CPPUNIT_ASSERT(list_list1 != list_list2);

    list_list1[2].add(sud_node(22, 22, 22));

    bool equal = list_list1 == list_list2;

    CPPUNIT_ASSERT(equal);



}

