#ifndef SUDOKUUTILS_H
#define	SUDOKUUTILS_H
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

typedef unsigned short u;

enum outcome {
    NOTHING = 0,
    EXCLUDED_CAND = 1,
    NEW_VALUE = 2
};

class SudokuUtils final {
private:
    SudokuUtils();
public:
    static u zero_index(const u& index);
    static outcome max(const outcome& first, const outcome& second);
};

template<class T> class sud_list final {

    class list_node {
        friend class sud_list;
        list_node(const T& elem) : element(elem) { };
        T element;
        list_node * next;
    };

    u m_count = 0;
    list_node * m_first = nullptr;
    list_node ** m_next_free = &m_first;
public:
    u size() const {
        return m_count;
    }
    void add(const T& element) {
        *m_next_free = new list_node(element);
        m_next_free = &((*m_next_free)->next);
        *m_next_free = nullptr;
        ++m_count;
    }
    T& operator[](const u& index) const {
        if (index < 0 || index >= m_count) {
            throw invalid_argument("Out of bound! Requested element index=" +
                                   std::to_string(index) +
                                   ", list size=" +
                                   std::to_string(m_count));
        }
        list_node * current = m_first;
        u i = 0;
        while (current != nullptr && i++ != index && (current = current->next));
        return (current->element);
    }
    bool operator==(const sud_list&other) {
        if (m_count == other.m_count) {
            list_node * this_node = m_first;
            list_node * other_node = other.m_first;
            for (u i = 0; i < m_count; ++i) {
                if (this_node->element != other_node->element) {
                    return false;
                }
                this_node = this_node->next;
                other_node = other_node->next;
            }
            return true;
        }
        return false;
    }
    bool operator!=(const sud_list&other) {
        return !(operator==(other));
    }
    void print() {
        for_each([](list_node * node){
            cout << node->element;
        });
    }
    sud_list() { }
    sud_list(const sud_list& other) {
        for (u i = 0; i < other.size(); ++i) {
            this->add(other[i]);
        }
    }
    virtual ~sud_list() {
        for_each([](list_node * node){
            delete node;
        });
    }
    void for_each(void(*function) (list_node*)) {
        list_node * current = m_first;
        while (current != nullptr) {
            list_node * temp = current->next;
            function(current);
            current = temp;
        }
    }

};

struct sud_node {
    u rw = 0;
    u cm = 0;
    u val = 0;
    sud_node(const u& row = 0, const u& column = 0, const u& value = 0) :
    rw(row), cm(column), val(value) { }
    bool operator==(const sud_node& other) {
        return rw == other.rw && cm == other.cm && val == other.val;
    }
    bool equals_row(const sud_node& other) {
        return rw == other.rw && val == other.val;
    }
    bool equals_column(const sud_node& other) {
        return val == other.val && cm == other.cm;
    }
    bool operator!=(const sud_node& other) {
        return !(*this == (other));
    }
    friend std::ostream& operator<<(std::ostream& out, const sud_node& s) {
        out << "[" << s.rw << "x" << s.cm << "]" << "=" << s.val << "   ";
        return out;
    }
};



#endif	/* SUDOKUUTILS_H */