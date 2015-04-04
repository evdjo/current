#ifndef SUDOKUCELL_H
#define	SUDOKUCELL_H
#include <stdexcept>
#include "SudokuUtils.h"


using namespace std;

class SudCell final {
public:
    friend class SudokuCellTests;
    friend class Solver;
    /**
     * Init the cell with the known value.
     * If the _val is zero candidate list will be created for the cell.
     * @param _val cell's value
     * @throw invalid_argument if the _val is not in the 0-9 range
     */
    void init(const u& _val, const u& _row, const u& _column) {

        if (m_node.val != 0 || m_cnds != nullptr)
            throw logic_error("Tried to init a cell twice!");

        if (_val > 9)
            throw invalid_argument("_val must be in the 0-9 range");

        m_node.val = _val;
        m_node.rw = _row;
        m_node.cm = _column;

        if (_val == 0) {
            m_cnds = new candidates();
        }
    }
    /**
     * Solve the cell by setting its value. Will throw logic error
     * if the cell was already solved.
     * @param _val the cell's new value
     */
    void set_val(const u& _val) {
        if (m_cnds == nullptr)
            throw logic_error("Tried to set_val on known cell!");

        m_node.val = _val;
        delete m_cnds;
        m_cnds = nullptr;

    }
    /**
     * Check if this cell's value is unknown yet.
     * @return true if we know the cell's value, false otherwise
     */
    bool unknown() const {
        return m_node.val == 0;
    }
    /**
     *  Remove the passed value from from this cell's list of candidates.
     * @param candidate the value to remove
     * @return true if the value was in the list & was removed, false otherwise
     */
    outcome rm_cand(const u& candidate) {

        //for debugging purposes
        //        if (m_node.rw == 3 && m_node.cm == 2 && candidate == 4) {
        //            cout << "hi";
        //        }

        if (!unknown())
            throw logic_error("The cell is solved!");

        if (m_cnds == nullptr)
            throw logic_error("Something is terribly wrong!");

        check_val(candidate);

        if (is_cand(candidate)) {
            (*m_cnds)[candidate] = 0;
            --(m_cnds->count);

            if (m_cnds->count == 1) {
                m_node.val = last_candidate();
                return NEW_VALUE;
            }
            return EXCLUDED_CAND;
        } else {
            return NOTHING;
        }
    }
    /**
     * @return Cell's value
     */
    u val() const {
        return m_node.val;
    }
    /**
     * @return how many values are possible solution for this cell
     */
    u cand_count() const {
        if (!unknown()) throw logic_error("The cell is solved!");
        return m_cnds->count;
    }
    /**
     * Get a pointer to the candidates of this Cell.
     * cand_cout should be used to determine the size of this array.
     * @return pointer to the candidates of this cell.
     */
    u * cands() {
        if (!unknown()) throw logic_error("The cell is solved!");
        return m_cnds->candidates;
    }
    /**
     * Check if the passed value is candidate for this cell
     * @param val the checked value
     * @return true if this value is a candidate for the cell
     */
    bool is_cand(const u& _val) const {
        if (!unknown()) throw logic_error("The cell is solved!");
        check_val(_val);
        return (*m_cnds)[_val] == _val;
    }
    /**
     * Compare the list of candidate of the two cells
     * @param other the other to compare to
     * @return if the two cells have the same candidates
     */
    bool operator==(const SudCell& other) {
        if (!unknown()) throw logic_error("The cell is solved!");
        if (m_cnds->count != other.m_cnds->count)
            return false;

        for (u i = 0; i < 9; i++) {
            if (m_cnds->candidates[i] != other.m_cnds->candidates[i])
                return false;
        }
        return true;
    }
    u get_cand(const u& num) const {
        if (!unknown() || m_cnds == nullptr)
            if (!unknown()) throw logic_error("The cell is solved!");
        if (num < 0 || num > 8)
            throw invalid_argument("Value must be in the 0-8 range!");

        u o = 0;
        for (u i = 0; i < 9; ++i) {
            if (m_cnds->candidates[i] != 0 && o++ == num)
                return m_cnds->candidates[i];
        }

    }
    outcome rmall_but(const u& first, const u& second) {
        outcome outcome_ = NOTHING;
        for (u i = 1; i < 10; ++i) {
            if (i == first || i == second) continue;
            else {
                outcome curr = rm_cand(i);
                outcome_ = max(outcome_, curr);
                if (outcome_ == NEW_VALUE)
                    break;
            }
        }
        m_cnds->count = 2;
        return outcome_;
    }
    /**
     * Prints the possible values.
     */
    void print_possible_values() {
        for (u i = 1; i < 10; i++)
            if ((*m_cnds)[i] != 0) cout << (*m_cnds)[i];
            else cout << " ";
    }
    bool subset_equals(SudCell& other) const {
        if (cand_count() <= other.cand_count())
            return false;
        for (u i = 0; i < other.cand_count(); ++i) {
            if (!is_cand(other.get_cand(i)))
                return false;
        }
        return true;

    }
    sud_node node() const {
        return m_node;
    }
    ~SudCell() {
        if (m_cnds != nullptr) delete m_cnds;
        m_cnds = nullptr;
    }
    SudCell() { }
    //    SudCell(SudCell&& other) : m_node(other.m_node), m_cnds(other.m_cnds) {
    //        cerr << "hi";
    //        other.m_cnds = nullptr;
    //    }
    SudCell(const SudCell& other) {
        m_node = other.m_node;
        if (m_node.val == 0) {
            m_cnds = new candidates();
            for (u i = 0; i < 9; ++i) {
                m_cnds->candidates[i] = other.m_cnds->candidates[i];
            }
            m_cnds->count = other.m_cnds->count;
        }
    }
    SudCell operator=(const SudCell& other) {
        m_node = other.m_node;

        if (other.unknown()) {
            if (m_cnds == nullptr) m_cnds = new candidates();
            for (u i = 0; i < 9; ++i) {
                m_cnds->candidates[i] = other.m_cnds->candidates[i];
            }
            m_cnds->count = other.m_cnds->count;
        } else {
            if (m_cnds != nullptr) {
                delete m_cnds;
                m_cnds = nullptr;
            }
        }
    }



private:

    struct candidates {
        friend class SudCell;
    private:
        u candidates[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        u count = 9;
        u& operator[](const u& value) {
            return candidates[value - 1];
        }
    };
    sud_node m_node;
    candidates * m_cnds = nullptr;
    void check_val(const u & _val) const {
        if (_val <= 0 || _val > 9)
            throw invalid_argument("candidate must be in the 1-9 range!");
    }
    u last_candidate() const {
        u count = 0;
        u value;
        for (u i = 0; i < 9; ++i) {
            if (m_cnds->candidates[i] != 0) {
                count++;
                value = m_cnds->candidates[i];
            }
        }
        if (count == 1)
            return value;
        else
            throw logic_error("Something went terribly wrong!");

    }
};



#endif	/* SUDOKUCELL_H */