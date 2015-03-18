#ifndef SUDOKUCELL_H
#define	SUDOKUCELL_H
#include <stdexcept>
#include "SudokuUtils.h"


using namespace std;

class SudokuCell final {
public:
    friend class SudokuTests;

    /**
     * Init the cell with the known value.
     * If the _val is zero candidate list will be created for the cell.
     * @param _val cell's value
     * @throw invalid_argument if the _val is not in the 0-9 range
     */
    void init(const u& _val) {

        if (m_val != 0 || m_cnds != nullptr)
            throw logic_error("Tried to init a cell twice!");

        if (_val > 9)
            throw invalid_argument("_val must be in the 0-9 range");

        m_val = _val;

        if (m_val == 0) {
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

        m_val = _val;
        delete m_cnds;

    }

    /**
     * Check if this cell's value is unknown yet.
     * @return true if we know the cell's value, false otherwise
     */
    bool unknown() {
        return m_val == 0;
    }

    /**
     *  Remove the passed value from from this cell's list of candidates.
     * @param candidate the value to remove
     * @return true if the value was in the list & was removed, false otherwise
     */
    bool rm_candidate(const u& candidate) {
        if (!unknown())
            throw logic_error("The cell is solved!");

        if (m_cnds == nullptr)
            throw logic_error("Something is terribly wrong!");

        check_val(candidate);

        if (is_candidate(candidate)) {
            (*m_cnds)[candidate] = 0;
            --(m_cnds->count);

            if (m_cnds->count == 1) {
                m_val = last_candidate();
            }


            return true;
        } else {
            return false;
        }
    }

    /**
     * @return Cell's value
     */
    u val() {
        return m_val;
    }

    /**
     * @return how many values are possible solution for this cell
     */
    u cand_count() {
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
    bool is_candidate(const u& _val) {
        if (!unknown()) throw logic_error("The cell is solved!");
        check_val(_val);
        return (*m_cnds)[_val] == _val;
    }

    /**
     * Compare the list of candidate of the two cells
     * @param other the other to compare to
     * @return if the two cells have the same candidates
     */
    bool operator==(const SudokuCell& other) {
        if (!unknown()) throw logic_error("The cell is solved!");
        if (m_cnds->count != other.m_cnds->count)
            return false;

        for (u i = 0; i < 9; i++) {
            if (m_cnds->candidates[i] != other.m_cnds->candidates[i])
                return false;
        }
        return true;
    }

    /**
     * Prints the possible values.
     */
    void print_possible_values() {
        for (u i = 1; i < 10; i++)
            if ((*m_cnds)[i] != 0) cout << (*m_cnds)[i];
            else cout << " ";
    }

    ~SudokuCell() {
        if (m_cnds != nullptr) delete m_cnds;
    }

private:

    struct candidates {
        friend class SudokuCell;
    private:
        u candidates[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        u count = 9;

        u& operator[](const u& index) {
            return candidates[index - 1];
        }
    };
    u m_val = 0;
    candidates * m_cnds = nullptr;

    void check_val(const u & _val) {
        if (_val <= 0 || _val > 9)
            throw invalid_argument("candidate must be in the 1-9 range!");
    }

    u last_candidate() {
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
            throw logic_error("Something went terrible wrong!");

    }
};



#endif	/* SUDOKUCELL_H */