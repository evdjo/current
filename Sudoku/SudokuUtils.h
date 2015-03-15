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
    static u zero_index(const u& index);
};

struct occur_node {

    occur_node(const u&_row, const u&_column)
    : row(_row), column(_column) {
    }

    bool operator==(const occur_node& other) {
        return row == other.row && column == other.column;
    }
    u row;
    u column;
    occur_node * next = nullptr;
};

struct occurences_list {
    u m_count = 0;
    occur_node * m_first = nullptr;
    occur_node * m_last = nullptr;
    occur_node ** m_next_free = &m_first;

    void add_(const u& row, const u& column) {
        *m_next_free = new occur_node(row, column);
        m_last = *m_next_free;
        m_next_free = &((*m_next_free)->next);
        *m_next_free = nullptr;
        ++m_count;
    }

    void print_list() {
        occur_node * curr = m_first;
        while (curr != nullptr) {
            cout << "[" << curr->row << "x";
            cout << curr->column << "]";
            curr = curr->next;
        }
    }

     occur_node& first() const {
        if (m_first == nullptr) throw logic_error("First is null...");
        return *m_first;
    }

    occur_node& last() const {
        if (m_first == nullptr) throw logic_error("First is null...");
        return *m_last;
    }

    occur_node& at(const u& index) {
        if (m_count > 0 && index < m_count) {
            occur_node * curr = m_first;
            for (u i = 0; i != index; ++i) {
                curr = curr->next;
            }
            if (curr == nullptr)
                throw logic_error("Could not find node!");

            return *curr;
        }
        throw invalid_argument("Bad index!");

    }

    u count() {
        return m_count;
    }

    ~occurences_list() {
        occur_node * curr = m_first;
        while (curr != nullptr) {
            occur_node * temp = curr->next;
            delete curr;
            curr = temp;
        }
    }
};


#endif	/* SUDOKUUTILS_H */
