//#ifndef LIST_H
//#define	LIST_H
//#include <cstdlib>
//#include <iostream>
//
//using namespace std;
//
//struct node {
//    SudokuCell sc;
//    node * next;
//
//    node(const SudokuCell& sc) {
//        this->sc = sc;
//    }
//
//    bool operator==(const node& other) {
//        return false;
//    }
//};
//
//struct List {
//    u_short size = 0;
//    node * first = NULL;
//    node ** next_ = &first;
//
//    void add_(u_short row, u_short column) {
//        *next_ = new node(row, column);
//        next_ = &((*next_)->next);
//        *next_ = NULL;
//        ++size;
//    }
//
//    void add(u_short row, u_short column) {
//        if (first == NULL) {
//            first = new node(row, column);
//            first->next = NULL;
//        } else if (first->next == NULL) {
//            first->next = new node(row, column);
//        } else {
//            node * current = first;
//            while (current->next != NULL) {
//                current = current->next;
//            }
//            current = current->next;
//            current = new node(row, column);
//            current->next = NULL;
//
//        }
//        ++size;
//    }
//
//    void print_list() {
//        node * curr = first;
//        while (curr != NULL) {
//            cout << '[' << curr->row;
//            cout << 'x';
//            cout << curr->column << ']';
//            curr = curr->next;
//            cout << endl << endl;
//        }
//    }
//
//    ~List() {
//        if (first == NULL) return;
//        node*curr = first;
//        while (curr != NULL) {
//            node * next = curr->next;
//            delete curr;
//            curr = next;
//        }
//    }
//
//};
//
//
//
//#endif	/* LIST_H */
//
