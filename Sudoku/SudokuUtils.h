#ifndef SUDOKUUTILS_H
#define	SUDOKUUTILS_H
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

typedef unsigned short u;

using namespace std;

class SudokuUtils final {
private:
    SudokuUtils();
public:
    /**
     * Parses the sudoku input file and returns pointer to 
     * a 9x9 array of shorts ints. Each unknown value is represented as 0.
     * @param input the ifstream pointer which leads us to the file
     * @return Pointer to 9x9 array containing the sudoku
     */
    static u *** read(const string & filename);
};

struct occur_node {

    occur_node(const u&_row, const u&_column)
    : row(_row), column(_column) {
    }

    u row;
    u column;
    occur_node * next = nullptr;
};

struct occurences_list { // TODO make this into linked list of occurrences
    u count = 0;
    occur_node * first = nullptr;
    occur_node ** next_ = &first;

    void add(const u& row, const u& column) {
        if (first == nullptr)
            first = new occur_node(row, column);

    }

    void add_(const u& row, const u& column) {
        *next_ = new occur_node(row, column);
        next_ = &((*next_)->next);
        *next_ = NULL;
        ++count;
    }

    void print_list() {
        occur_node * curr = first;
        while (curr != nullptr) {
            cout << "[" << curr->row << "x";
            cout << curr->column << "]";
            
            curr = curr->next;
        }
    }

    ~occurences_list() {
        occur_node * curr = first;

        while (curr != nullptr) {
            occur_node * temp = curr->next;
            delete curr;
            curr = temp;
        }

    }
};


#endif	/* SUDOKUUTILS_H */

