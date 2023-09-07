/**
 * @file SmallMatrix.hpp
 * @author Mohamad Baydoun
 * @brief Header file for SmallMatrix.cpp
 */
#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace smallMatrix {

class SmallMatrix {
public:
    /**
     * @brief A constructor which initialises an empty matrix with no rows and no columns.
     */
    SmallMatrix();

    /**
     * @brief A constructor which initialises a zero matrix with the dimensions given by numRows
     *        and numCol.
     *
     * @param numRows Number of rows to initialise with.
     * @param numCols Number of columns to initialise with.
     */
    SmallMatrix(int numRows, int numCols);

    /**
     * @brief A constructor which intialises a matrix whose elements are all initialised with the
     *        given value, and has the dimensions given by numRows and numCols.
     *
     * @param numRows Number of rows to initialise with.
     * @param numCols Number of columns to initialise with.
     * @param value Value to initialise all matrix elements with.
     */
    SmallMatrix(int numRows, int numCols, double value);

    /**
     * @brief A constructor which initialises a matrix with a given initialiser list of initialiser
     *        list of doubles i.e. a 2D initialiser list of doubles. Each inner initialiser list
     *        represents a single row where each element in the inner initialiser list represents a
     *        column.
     *
     * @param il 2D initialiser list to initialise matrix.
     * @throw Throws invalid_argument if the initialiser list is not rectangular i.e. each row does
     *        not have the same number of columns.
     */
    SmallMatrix(std::initializer_list<std::initializer_list<double>> const& il);

    /**
     * @brief Copy constructor.
     *
     * @param sm SmallMatrix to make a copy of.
     */
    SmallMatrix(SmallMatrix const& sm);

    /**
     * @brief Move constructor.
     *
     * @param sm SmallMatrix whose resources will be transferred from.
     */
    SmallMatrix(SmallMatrix&& sm);

    /**
     * @brief Copy assignment.
     *
     * @param sm SmallMatrix to make a copy of.
     * @return SmallMatrix&
     */
    SmallMatrix& operator=(SmallMatrix const& sm);

    /**
     * @brief Move assignment.
     *
     * @param sm SmallMatrix whose resources will be transferred from.
     * @return SmallMatrix&
     */
    SmallMatrix& operator=(SmallMatrix&& sm);

    /**
     * @brief Destructor.
     */
    ~SmallMatrix();

    /**
     * @brief Returns the reference of the matrix element at the specified row and column index.
     *
     * @param numRow Row index.
     * @param numCol Column index.
     * @return double&
     * @throw Throws out_of_range if the specified row and column is outside the range [0, max_row)
     *        and [0, max_col) respectively.
     * @throw Throws out_of_range if attempting to access a 0 x 0 matrix.
     */
    double& operator()(int numRow, int numCol);

    /**
     * @brief Returns the constant reference of the matrix element at the specified row and column
     *        index. It is guaranteed that the returned element is not modified.
     *
     * @param numRow Row index.
     * @param numCol Column index.
     * @return const double&
     * @throw Throws out_of_range if the specified row and column is outside the range [0, max_row)
     *        and [0, max_col) respectively.
     * @throw Throws out_of_range if the matrix has no rows and no columns.
     */
    const double& operator()(int numRow, int numCol) const;

    /**
     * @brief Returns a vector of pointers to each of the elements of the row of the matrix at the
     *        specified row index.
     *
     * @param numRow Row index.
     * @return std::vector<double*>
     * @throw Throws out_of_range if the specified row index is outside the range [0, max_row).
     */
    std::vector<double*> row(int numRow);

    /**
     * @brief Returns a vector of pointers to each of the elements of constant type of the row of
     *        the matrix at the specified row index.
     *
     * @param numRow Row index.
     * @return std::vector<double const*>
     * @throw Throws out_of_range if the specified row index is outside the range [0, max_row).
     */
    std::vector<double const*> row(int numRow) const;

    /**
     * @brief Returns a vector of pointers to each of the elements of the column of the matrix at
     *        the specified column index.
     *
     * @param numCol Column index.
     * @return std::vector<double*>
     * @throw Throws out_of_range if the specified column index is outside the range [0, max_col).
     */
    std::vector<double*> col(int numCol);

    /**
     * @brief Returns a vector of pointers to each of the elements of constant type of the column of
     *        the matrix at the specified column index.
     *
     * @param numCol Column index.
     * @return std::vector<double const*>
     * @throw Throws out_of_range if the specified column index is outside the range [0, max_col).
     */
    std::vector<double const*> col(int numCol) const;

    /**
     * @brief Returns the size of the matrix where the first of the pair is the number of rows and
     *        the second of the pair is the number of columns.
     *
     * @return std::pair<int, int>
     */
    std::pair<int, int> size() const;

    /**
     * @brief Returns true if the matrix is using a small-storage-optimised data structure.
     *
     * @return true, if using stack.
     * @return false, otherwise.
     */
    bool isSmall() const;

    /**
     * @brief Resizes the matrix to the new number of rows and new number of columns. If any matrix
     *        dimension is increased, then the newly created dimension is zero-initialised. If any
     *        matrix dimension is decreased, then its previously-allocated elements are truncated.
     *
     * @param numRows Number of rows to resize to.
     * @param numCols Number of columns to resize to.
     * @throw Throws out_of_range if the specified row or column index is negative.
     */
    void resize(int numRows, int numCols);

    /**
     * @brief Inserts a row at the specified row index. If the number of columns in the matrix is
     *        zero, then the matrix is resized to match the size of the specified row vector.
     *
     * @param numRow Position of where the row is to be inserted.
     * @param row Row to be inserted.
     * @throw Throws out_of_range if the specified row index is outside the range [0, max_row).
     * @throw Throws invalid_argument if the size of the specified vector is not equal to the number
     *        of non-zero columns in the matrix.
     */
    void insertRow(int numRow, std::vector<double> const& row);

    /**
     * @brief Inserts a column at the specified column index. If the number of rows in the matrix is
     *        zero, then the matrix is resized to match the size of the specified column vector.
     *
     * @param numCol Position of where the column is to be inserted.
     * @param col Column to be inserted.
     * @throw Throws out_of_range if the specified column index is outside the range [0, max_col).
     * @throw Throws invalid_argument if the size of the specified vector is not equal to the number
     *        of non-zero rows in the matrix.
     */
    void insertCol(int numCol, std::vector<double> const& col);

    /**
     * @brief Erases the row at the specified row index.
     *
     * @param numRow Row index.
     * @throw Throws out_of_range if the specified row index is outside the range [0, max_row).
     */
    void eraseRow(int numRow);

    /**
     * @brief Erases the column at the specified column index.
     *
     * @param numCol Column index.
     * @throw Throws out_of_range if the specified column index is outside the range [0, max_col).
     */
    void eraseCol(int numCol);

    /**
     * @brief Returns true if all of the elements in the left-hand side matrix are equal to its
     *        positionally-corresponding element in the right-hand side matrix. Otherwise, false.
     *
     * @param lhs Left-hand side matrix.
     * @param rhs Right-hand side matrix.
     * @return true, if lhs and rhs are equal.
     * @return false, otherwise.
     */
    friend bool operator==(SmallMatrix const& lhs, SmallMatrix const& rhs);

    /**
     * @brief Returns false if any of the elements in the left-hand side matrix are not equal to its
     *        positionally-corresponding element in the right-hand side matrix. Otherwise, true.
     *
     * @param lhs Left-hand side matrix.
     * @param rhs Right-hand side matrix.
     * @return true, if lhs and rhs are not equal.
     * @return false, otherwise.
     */
    friend bool operator!=(SmallMatrix const& lhs, SmallMatrix const& rhs);

    /**
     * @brief Returns the matrix result of the element-wise addition of the two specified matrices.
     *
     * @param lhs Left-hand side matrix.
     * @param rhs Right-hand side matrix.
     * @return SmallMatrix
     * @throw Throws invalid_argument if the number of rows and columns on the left-hand side is not
     *        equal to the number of rows and columns on the right-hand side respectively.
     */
    friend SmallMatrix operator+(SmallMatrix const& lhs, SmallMatrix const& rhs);

    /**
     * @brief Returns the matrix result of the element-wise subtraction of the two specified
     *        matrices.
     *
     * @param lhs Left-hand side matrix.
     * @param rhs Right-hand side matrix.
     * @return SmallMatrix
     * @throw Throws invalid_argument if the number of rows and columns on the left-hand side is not
     *        equal to the number of rows and columns on the right-hand side respectively.
     */
    friend SmallMatrix operator-(SmallMatrix const& lhs, SmallMatrix const& rhs);

    /**
     * @brief Returns the matrix result of the matrix multiplication of the two specified matrices.
     *
     * @param lhs Left-hand side matrix.
     * @param rhs Right-hand side matrix.
     * @return SmallMatrix
     * @throw Throws invalid_argument if the number of columns on the left-hand side is not equal to
     *        the number of rows on the right-hand side.
     */
    friend SmallMatrix operator*(SmallMatrix const& lhs, SmallMatrix const& rhs);

    /**
     * @brief Returns the matrix result of the scalar multiplication of the the specified scalar
     *        value and specified matrix.
     *
     * @param s Scalar value.
     * @param sm SmallMatrix.
     * @return SmallMatrix
     */
    friend SmallMatrix operator*(double s, SmallMatrix const& sm);

    /**
     * @brief Returns the matrix result of the scalar multiplication of the the specified scalar
     *        value and specified matrix.
     *
     * @param sm SmallMatrix.
     * @param s Scalar value.
     * @return SmallMatrix
     */
    friend SmallMatrix operator*(SmallMatrix const& sm, double s);

    /**
     * @brief Returns *this after the element-wise addition of *this and the specified matrix. This
     *        operation is equivalent to *this = *this + sm.
     *
     * @param sm Addend matrix.
     * @return SmallMatrix&
     * @throw Throws invalid_argument if the number of rows and columns of *this is not equal to the
     *        number of rows and columns of the specified matrix respectively.
     */
    SmallMatrix& operator+=(SmallMatrix const& sm);

    /**
     * @brief Returns *this after the element-wise subtraction of *this and the specified matrix.
     *        This operation is equivalent to *this = *this - sm.
     *
     * @param sm Subtrahend matrix.
     * @return SmallMatrix&
     * @throw Throws invalid_argument if the number of columns of *this is not equal to the number
     *        of rows of the specified matrix.
     */
    SmallMatrix& operator-=(SmallMatrix const& sm);

    /**
     * @brief Returns *this after the matrix multiplication of *this and the specified matrix. This
     *        operation is equivalent to *this = *this * sm.
     *
     * @param sm Multiplier matrix.
     * @return SmallMatrix&
     * @throw Throws invalid_argument if the number of columns of *this is not equal to the number
     *        of rows of the specified matrix.
     */
    SmallMatrix& operator*=(SmallMatrix const& sm);

    /**
     * @brief Returns *this after the scalar multiplication of *this and the specified scalar value.
     *        This operation is equivalent to *this = *this * s.
     *
     * @param s Scalar value.
     * @return SmallMatrix&
     */
    SmallMatrix& operator*=(double s);

    /**
     * @brief Returns the result of the tranpose on the specified matrix.
     *
     * @param sm Matrix to be transposed.
     * @return SmallMatrix
     */
    friend SmallMatrix transpose(SmallMatrix const& sm);

    /**
     * @brief Writes the contents of the matrix to the output stream.
     *
     * @param os Output stream.
     * @param sm SmallMatrix.
     * @return std::ostream&
     */
    friend std::ostream& operator<<(std::ostream& os, SmallMatrix const& sm);

private:
    int mNumRows;
    int mNumCols;
    bool mIsLargeMatrix;
    static constexpr int mSmallSize = 144;
    std::array<std::array<double, mSmallSize>, mSmallSize> mStackData;
    std::vector<std::vector<double>> mHeapData;
};

// Forward declaring.
SmallMatrix transpose(SmallMatrix const&);

}  // namespace smallMatrix
