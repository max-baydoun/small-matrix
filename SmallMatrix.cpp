/*
Small Matrix program by Mohamad Baydoun.
*/

#include "SmallMatrix.hpp"
#include <cstdlib>
#include <algorithm>
namespace smallMatrix {

template<std::size_t arraySize>
const std::array<std::array<double, arraySize>, arraySize> shiftArrayElementsUp(const int startingRow, std::array<std::array<double, arraySize>, arraySize> &stackArray, const int numRows, const int numCols) {
    for (int i {startingRow}; i < numRows; i++) {
        for (int j {}; j < numCols; j++) {
            stackArray.at(i).at(j) = stackArray.at(i + 1).at(j);
        }
    }
    return stackArray;
}


template<std::size_t arraySize>
const std::array<std::array<double, arraySize>, arraySize> shiftArrayElementsDown(const int startingRow, std::array<std::array<double, arraySize>, arraySize> &stackArray, const int numRows, const int numCols) {
    for (int i {startingRow}; i > numRows; i--) {
        for (int j {}; j < numCols; j++) {
            stackArray.at(i).at(j) = stackArray.at(i - 1).at(j);
        }
    }
    return stackArray;
}


// Converts a 2D array to a 2D vector

// https://stackoverflow.com/questions/17156282/passing-a-stdarray-of-unknown-size-to-a-function
template<std::size_t arraySize>
const std::vector<std::vector<double>> convertStdArrayToStdVector(const std::array<std::array<double, arraySize>, arraySize> &stackArray, const int numRows, const int numCols) {
    auto rowIndex = 0;
    std::vector<std::vector<double>> newHeapData = std::vector<std::vector<double>> (numRows, std::vector<double>(numCols));;
    for (auto const &row : stackArray) {
        bool canCopy = rowIndex < numRows ? true : false;
        if (canCopy) {
            std::copy(row.cbegin(), row.cbegin() + numCols, newHeapData.at(rowIndex).begin());
            rowIndex++;
        }
        else { break; }
    }

    return newHeapData;
}


// Returns the number of elements of the matrix
const int getNumberOfElements(SmallMatrix const& sm) {
    return sm.size().first * sm.size().second;
}


SmallMatrix::SmallMatrix()
: mNumRows {0}, mNumCols {0} {};

SmallMatrix::SmallMatrix(int numRows, int numCols) 
    :   mNumRows {numRows}, 
        mNumCols {numCols},
        mIsLargeMatrix(mNumRows * mNumCols >= mSmallSize) {
    
    // Initialise the heap vector with 0 if the matrix has n.o elements  >= 144
    if (mIsLargeMatrix) {
       mHeapData = std::vector<std::vector<double>> (numRows, std::vector<double>(numCols));
    } else {
        SmallMatrix(numRows, numCols, 0);
    }
}

SmallMatrix::SmallMatrix(int numRows, int numCols, double value)
    :   mNumRows {numRows} ,
        mNumCols {numCols} ,
        mIsLargeMatrix(mNumRows * mNumCols >= mSmallSize) {
    
    /*
    Initialise the heap vector with a specified value if the n.o of elements >= 144.
    Otherwise, populate the stack array with that value
    */
    if (mIsLargeMatrix) {
        mHeapData = std::vector<std::vector<double>> (numRows, std::vector<double>(numCols, value));
    }
    else {
        std::for_each(mStackData.begin(), mStackData.end(), [&](auto& row){row.fill(value);});
    }
}

SmallMatrix::SmallMatrix(std::initializer_list<std::initializer_list<double>> const& il)
    :   mNumRows(il.size()),
        mNumCols(il.begin() == il.end() ? 0 : il.begin()->size()),
        mIsLargeMatrix(mNumRows * mNumCols >= mSmallSize) {
    if (std::adjacent_find(il.begin(), il.end(), [](auto const& lhs, auto const& rhs) {
            return lhs.size() != rhs.size();
        }) != il.end()) {
        throw std::invalid_argument("Rows have different sizes.");
    }

    if (mIsLargeMatrix) {
        mHeapData.resize(mNumRows);
    }

    int row_index{0};
    for (auto const& row : il) {
        if (mIsLargeMatrix) {
            mHeapData.at(row_index).resize(mNumCols);
            std::copy(row.begin(), row.end(), mHeapData.at(row_index).begin());
        } else {
            std::transform(row.begin(), row.end(), mStackData.at(row_index).begin(),
                           [](auto const& e) { return e; });
        }
        row_index++;
    }
}

SmallMatrix::SmallMatrix(SmallMatrix const& sm) 
    :   mNumRows {sm.mNumRows},
        mNumCols {sm.mNumCols},
        mIsLargeMatrix(mNumRows * mNumCols >= mSmallSize) {
    // Copy the elements of the other matrix into the current one, depending on the n.o of elements
    if (mIsLargeMatrix) {
        mHeapData = sm.mHeapData;
    } else {
       for (int i {}; i < mNumRows; i++) {
            for (int j {}; j < mNumCols; j++) {
                (*this)(i, j) = sm(i, j);
            }
        }
    }
}

SmallMatrix::SmallMatrix(SmallMatrix&& sm)
    :   mNumRows {sm.mNumRows},
        mNumCols {sm.mNumCols},
        mIsLargeMatrix(mNumRows * mNumCols >= mSmallSize) {
    // Move the elements of the other matrix into the current one, depending on the n.o of elements
    if (sm.mIsLargeMatrix) {
        mHeapData = std::exchange(sm.mHeapData, {{}});
    } else {
        mStackData = std::exchange(sm.mStackData, {{}});
    }
}

SmallMatrix& SmallMatrix::operator=(SmallMatrix const& sm) {
    if (this != &sm) {
        mNumRows = sm.mNumRows;
        mNumCols = sm.mNumCols;
        mIsLargeMatrix  = getNumberOfElements(sm) >= mSmallSize ? true : false;
        if (mIsLargeMatrix) {
            mHeapData = sm.mHeapData;
        } else {
            for (int i {}; i < mNumRows; i++) {
                for (int j {}; j < mNumCols; j++) {
                    (*this)(i, j) = sm(i, j);
                }
            }
        }
    }
    return *this;
   }

SmallMatrix& SmallMatrix::operator=(SmallMatrix&& sm) {
    if (this != &sm) {
        mNumRows = sm.mNumRows;
        mNumCols = sm.mNumCols;
        mIsLargeMatrix  = sm.mNumRows * sm.mNumCols >= mSmallSize ? true : false;
        if (sm.mIsLargeMatrix) {
            mHeapData = std::exchange(sm.mHeapData, {{}});
        } else {
            mStackData = std::exchange(sm.mStackData, {{}});
        }
    }
    return *this;
}

SmallMatrix::~SmallMatrix() {}

double& SmallMatrix::operator()(int numRow, int numCol) {
    // https://stackoverflow.com/questions/856542/elegant-solution-to-duplicate-const-and-non-const-getters
    return const_cast<double&>(const_cast<const SmallMatrix*>(this)->operator()(numRow, numCol));
}

const double& SmallMatrix::operator()(int numRow, int numCol) const {
    const bool outOfRange = (numRow >= mNumRows  || numCol >= mNumCols || numRow < 0 || numCol < 0) ? true : false;
    // Error thrown when the matrix has either no dimension or is being illegally accessed
    if (outOfRange) {
        throw std::out_of_range("Out Of Range!");
    } else { 
        return mIsLargeMatrix ? mHeapData.at(numRow).at(numCol) : mStackData.at(numRow).at(numCol);
    }

}

std::vector<double*> SmallMatrix::row(int numRow) {
    if (numRow >= mNumRows || numRow < 0) {
        throw std::out_of_range("Out of Range! Illegal row access");
    }

    std::vector<double*> cols;
    if (mIsLargeMatrix) {
        std::for_each(mHeapData.at(numRow).begin(), mHeapData.at(numRow).end(), [&](auto& col){cols.push_back(&col);});
    } else {
        std::for_each(mStackData.at(numRow).begin(), mStackData.at(numRow).begin() + mNumCols, [&](auto& col){cols.push_back(&col);});
    }

    return cols;
}

std::vector<double const*> SmallMatrix::row(int numRow) const {
    if (numRow >= mNumRows || numRow < 0) {
        throw std::out_of_range("Out of Range! Illegal row access");
    }
    std::vector<double const*> cols;
    if (mIsLargeMatrix) {
        std::for_each(mHeapData.at(numRow).cbegin(), mHeapData.at(numRow).cend(), [&](auto const& col){cols.push_back(&col);});
    } else {
        std::for_each(mStackData.at(numRow).cbegin(), mStackData.at(numRow).cbegin() + mNumCols, [&](auto const& col){cols.push_back(&col);});
    }
    return cols;
}

std::vector<double*> SmallMatrix::col(int numCol) { 
    if (numCol >= mNumCols || numCol < 0) {
        throw std::out_of_range("Out of Range! Illegal column access");
    }
    
    std::vector<double*> rows;
    if (mIsLargeMatrix) {
        std::for_each(mHeapData.begin(), mHeapData.end(), [&](auto& row){rows.push_back(&row.at(numCol));});
    } else {
        std::for_each(mStackData.begin(), mStackData.begin() + mNumCols, [&](auto& row){rows.push_back(&row.at(numCol));});
    }
   return rows;
}

std::vector<double const*> SmallMatrix::col(int numCol) const {
    if (numCol >= mNumCols || numCol < 0) {
        throw std::out_of_range("Out of Range! Illegal column access");
    }

    std::vector<double const*> rows;
    if (mIsLargeMatrix) {
        std::for_each(mHeapData.cbegin(), mHeapData.cend(), [&](auto const& row){rows.push_back(&row.at(numCol));});
    } else {
        std::for_each(mStackData.cbegin(), mStackData.cbegin() + mNumRows, [&](auto const& row){rows.push_back(&row.at(numCol));});
    }
    return rows;

}

std::pair<int, int> SmallMatrix::size() const { return {std::make_pair(mNumRows, mNumCols)}; }

bool SmallMatrix::isSmall() const { return !mIsLargeMatrix ? true : false; }

void SmallMatrix::resize(int numRows, int numCols) {

    if (numRows < 0 || numCols < 0) {
        throw std::out_of_range("Out of Range! Illegal row or column resize value/s");
    }


    auto const tempRowCount = mNumRows;
    auto const tempColCount = mNumCols;

    mNumRows = numRows;
    mNumCols = numCols;
    
    // Row and Column increase/decrease for large matrix
    if (getNumberOfElements(*this) >= mSmallSize) {
        if (isSmall()) {
            mIsLargeMatrix = true;
            mHeapData = convertStdArrayToStdVector(mStackData, tempRowCount, tempColCount);
        }
        mHeapData.resize(numRows);
        for (auto &row : mHeapData) {
            row.resize(numCols, 0);
        }
    } else {
        // Row and Column increase/decrease for small matrix
        if (mIsLargeMatrix) {
            mHeapData.resize(numRows);
            for (auto &row : mHeapData) {
                row.resize(numCols, 0);
            }
        }
        else {
            if (numRows > tempRowCount) {
                std::for_each(mStackData.begin() + tempRowCount, mStackData.begin() + numRows, [&](auto& row){row.fill(0.0);});
            }

            if (numCols > tempColCount) {
                std::for_each(mStackData.begin(), mStackData.begin() + numRows, [&](auto &row){std::fill(row.begin() + tempColCount, row.begin() + numCols, 0.0);});
            }
        }
    }
    
    
}

void SmallMatrix::insertRow(int numRow, std::vector<double> const& row) {
    const bool outOfRange = (numRow < 0 || numRow > mNumRows) ? true : false;
    const bool notValidNoOfCols = (row.size() != mNumCols) ? true : false;

    if (outOfRange) {
        throw std::out_of_range("Out of Range!");
    }

    if (notValidNoOfCols) {
        throw std::invalid_argument("Invalid number of columns!");
    }


    mNumRows++;
    if (mIsLargeMatrix) {
        mHeapData.insert(mHeapData.begin() + numRow, row);
    } else {
        if (getNumberOfElements(*this) >= mSmallSize) {
            mIsLargeMatrix = true;
            mHeapData = convertStdArrayToStdVector(mStackData, mNumRows, mNumCols);
            mHeapData.insert(mHeapData.begin() + numRow, row);
        } else {
            mStackData = std::move(shiftArrayElementsDown(mNumRows, mStackData, numRow, mNumCols));
            for (int i {}; i < mNumCols; i++) {
                mStackData.at(numRow).at(i) = row.at(i);
            }
        }
    }
}

void SmallMatrix::insertCol(int numCol, std::vector<double> const& col) {
    const bool outOfRange = (numCol < 0 || numCol > mNumCols) ? true : false;
    if (outOfRange) {
        throw std::out_of_range("Out of Range!");
    }

    const bool notValidNoOfCols = (col.size() != mNumRows) ? true : false;
    if (notValidNoOfCols) {
        throw std::invalid_argument("Invalid number of columns!");
    }

    SmallMatrix newTransposedMatrix = transpose(*this);
    newTransposedMatrix.insertRow(numCol, col);
    if (newTransposedMatrix.mIsLargeMatrix) {
        mIsLargeMatrix = true;
        mHeapData = transpose(newTransposedMatrix).mHeapData;
    } else {
        mStackData = transpose(newTransposedMatrix).mStackData;
    }

    mNumCols++;
}

void SmallMatrix::eraseRow(int numRow) {
    const bool outOfRange = (numRow < 0 || numRow >= mNumRows) ? true : false;
    if (outOfRange) {
        throw std::out_of_range("Out of Range!");
    }


    mNumRows--;
    if (mIsLargeMatrix) {
        mHeapData.erase(mHeapData.begin() + numRow);
    } else {
        mStackData = std::move(shiftArrayElementsUp(numRow, mStackData, mNumRows, mNumCols));
    }
}

void SmallMatrix::eraseCol(int numCol) {
    const bool outOfRange = (numCol < 0 || numCol >= mNumCols) ? true : false;
    if (outOfRange) {
        throw std::out_of_range("Out of Range!");
    }


    SmallMatrix newTransposedMatrix = transpose(*this);
    newTransposedMatrix.eraseRow(numCol);
    if (newTransposedMatrix.mIsLargeMatrix) {
        mHeapData = transpose(newTransposedMatrix).mHeapData;
    } else {
        mStackData = transpose(newTransposedMatrix).mStackData;
    }
    mNumCols--;
}

bool operator==(SmallMatrix const& lhs, SmallMatrix const& rhs) {
    if (lhs.size() != rhs.size()) { return false; }
    const double epsilon = 0.0000001;
    for (int i {}; i < lhs.mNumRows; i++) {
        for (int j {}; j < lhs.mNumCols; j++) { 
            if (std::abs(lhs(i, j) - rhs(i, j)) > epsilon) { return false; }
        }
    }
    return true;
}

bool operator!=(SmallMatrix const& lhs, SmallMatrix const& rhs) {
    return !operator==(lhs, rhs);
}

SmallMatrix operator+(SmallMatrix const& lhs, SmallMatrix const& rhs) { 
    if (lhs.mNumRows != rhs.mNumRows || lhs.mNumCols != rhs.mNumCols) {
        throw std::invalid_argument("Unequal dimensions!");

    }

    SmallMatrix m = SmallMatrix(lhs.mNumRows, lhs.mNumCols);

    for (int i {}; i < lhs.mNumRows; i++) {
        for (int j {}; j < lhs.mNumCols; j++) { 
            m(i, j) = lhs(i, j) + rhs(i, j);
        }
    }
    return m;
}

SmallMatrix operator-(SmallMatrix const& lhs, SmallMatrix const& rhs) { 
    if (lhs.mNumRows != rhs.mNumRows || lhs.mNumCols != rhs.mNumCols) {
        throw std::invalid_argument("Unequal dimensions!");

    }

    SmallMatrix m = SmallMatrix(lhs.mNumRows, lhs.mNumCols);

    for (int i {}; i < lhs.mNumRows; i++) {
        for (int j {}; j < lhs.mNumCols; j++) { 
            m(i, j) = lhs(i, j) - rhs(i, j);
        }
    }
    return m;
}

SmallMatrix operator*(SmallMatrix const& lhs, SmallMatrix const& rhs) {
    const bool unequalDimensions = (lhs.mNumRows != rhs.mNumRows) || (lhs.mNumCols != rhs.mNumCols);
    if (lhs.mNumCols != rhs.mNumRows) {
        throw std::invalid_argument("Unequal dimensions!");
    }
    SmallMatrix newSmallMatrix = SmallMatrix(lhs.mNumRows, rhs.mNumCols);

    for(int i = 0; i < lhs.mNumRows; i++) {
        for(int j = 0; j < rhs.mNumCols; j++) {
            for(int k = 0; k < lhs.mNumCols; k++) {
                newSmallMatrix(i, j) += lhs(i, k) * rhs(k, j);
            }
        }
    }

    return newSmallMatrix;
}

SmallMatrix operator*(double s, SmallMatrix const& sm) {
    SmallMatrix newSmallMatrix = SmallMatrix(sm.mNumRows, sm.mNumCols);
    const int rows = sm.mNumRows;
    const int cols = sm.mNumCols;
    for (int i {}; i < rows; i++) {
        for (int j {}; j < cols; j++) {
            newSmallMatrix(i, j) = s*sm(i, j);
        }
    }
    return newSmallMatrix;
}

SmallMatrix operator*(SmallMatrix const& sm, double s) {
    return operator*(s, sm);

}

SmallMatrix& SmallMatrix::operator+=(SmallMatrix const& sm) {
    if (mNumRows != sm.mNumRows || mNumCols != sm.mNumCols) {
        throw std::invalid_argument("Unequal dimensions!");
    }

    SmallMatrix m = SmallMatrix(mNumRows, sm.mNumCols);

    for (int i {}; i < mNumRows; i++) {
        for (int j {}; j < mNumCols; j++) { 
            (*this)(i, j) = (*this)(i, j) + sm(i, j);
        }
    }
    return *this;
}

SmallMatrix& SmallMatrix::operator-=(SmallMatrix const& sm) {
    if (mNumRows != sm.mNumRows || mNumCols != sm.mNumCols) {
        throw std::invalid_argument("Unequal dimensions!");
    }

    SmallMatrix m = SmallMatrix(mNumRows, sm.mNumCols);

    for (int i {}; i < mNumRows; i++) {
        for (int j {}; j < mNumCols; j++) { 
            (*this)(i, j) = (*this)(i, j) - sm(i, j);
        }
    }
    return *this;
}

SmallMatrix& SmallMatrix::operator*=(SmallMatrix const& sm) {
    if (mNumCols != sm.mNumRows) {
        throw std::invalid_argument("Unequal dimensions!");
    }

    SmallMatrix newSmallMatrix = SmallMatrix(mNumRows, sm.mNumCols);


    for(int i = 0; i < mNumRows; i++) {
        for(int j = 0; j < sm.mNumCols; j++) {
            for(int k = 0; k < mNumCols; k++) {
                newSmallMatrix(i, j) += (*this)(i, k) * sm(k, j);
            }
        }
    }
    
    *this = newSmallMatrix;
    return *this;
}

SmallMatrix& SmallMatrix::operator*=(double s) {
    for (int i {}; i < mNumRows; i++) {
        for (int j {}; j < mNumCols; j++) {
            (*this)(i, j) *= s;
        }
    }
    return *this;
}

SmallMatrix transpose(SmallMatrix const& sm) {
    SmallMatrix newSmallMatrix = SmallMatrix(sm.mNumCols, sm.mNumRows);

    const int rows = sm.mNumRows;
    const int cols = sm.mNumCols;

    for (int i {}; i < rows; i++) {
        for (int j {}; j < cols; j++) {
            if (newSmallMatrix.mIsLargeMatrix) {
                newSmallMatrix.mHeapData.at(j).at(i) = sm.mHeapData.at(i).at(j);
            } else {
                newSmallMatrix.mStackData.at(j).at(i) = sm.mStackData.at(i).at(j);
            }
        }
    }
    return newSmallMatrix;
}

std::ostream& operator<<(std::ostream& os, SmallMatrix const& sm) {
    os << "[\n";
    for (int i = 0; i < sm.mNumRows; i++) {
        os << "  [ ";
        for (int j = 0; j < sm.mNumCols; j++) {
            os << sm(i, j) << " ";
        }
        os << "]" << std::endl;
    }
    os << "]" << std::endl;
    return os; 
}

}  // namespace smallMatrix