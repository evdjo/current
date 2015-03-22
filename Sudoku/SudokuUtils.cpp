#include "SudokuUtils.h"
using namespace std;

u SudokuUtils::zero_index(const u& index) {
    return static_cast<u> ((index / 3) * 3);
}

outcome SudokuUtils::max(const outcome& first, const outcome& second) {
    return first >= second ? first : second;
}