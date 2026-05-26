#include "Matrix.hpp"
#include <sequence/Exceptions.hpp>   

template <class T>
void RectangularMatrix<T>::validateRowIndex(int index) const {
    if (index < 0 || index >= rows_cnt) 
		throw IndexOutOfRange("Matrix: row index out of range");
}

template <class T>
void RectangularMatrix<T>::validateColumnIndex(int index) const {
    if (index < 0 || index >= cols_cnt)
        throw IndexOutOfRange("Matrix: column index out of range");
}

template <class T>
void RectangularMatrix<T>::validateSameDimensions(const RectangularMatrix<T>& other) const {
    if (rows_cnt != other.rows_cnt || cols_cnt != other.cols_cnt)
        throw RunTimeError("Matrix: dimensions must match for this operation");
}

template <class T>
RectangularMatrix<T>::RectangularMatrix() : rows_cnt(0), cols_cnt(0) {}

template <class T>
RectangularMatrix<T>::RectangularMatrix(int rows, int cols) : rows(rows), rows_cnt(rows), cols_cnt(cols) {
    if (rows < 0 || cols < 0)
        throw IndexOutOfRange("Matrix: dimensions must be non-negative");

    for (int i = 0; i < rows; ++i) {
        typename MutableArraySequence<T>::Builder builder;
        for (int j = 0; j < cols; ++j) {
            builder.Append(T());
        }
        MutableArraySequence<T>* builtRow = builder.Build();
        this->rows[i] = *builtRow;   
        delete builtRow;
    }
}

template <class T>
RectangularMatrix<T>::RectangularMatrix(const RectangularMatrix<T>& other) 
	: rows(other.rows), rows_cnt(other.rows_cnt), cols_cnt(other.cols_cnt) {}

template <class T>
RectangularMatrix<T>::RectangularMatrix(RectangularMatrix<T>&& other) noexcept
    : rows(std::move(other.rows)), rows_cnt(other.rows_cnt), cols_cnt(other.cols_cnt) {

    other.rows_cnt = 0;
    other.cols_cnt = 0;
}

template <class T>
RectangularMatrix<T>& RectangularMatrix<T>::operator=(const RectangularMatrix<T>& other) {
    if (this != &other) {
        rows = other.rows;   
        rows_cnt = other.rows_cnt;
        cols_cnt = other.cols_cnt;
    }
    return *this;
}

template <class T>
RectangularMatrix<T>& RectangularMatrix<T>::operator=(RectangularMatrix<T>&& other) noexcept {
    if (this != &other) {
        rows = std::move(other.rows);
        rows_cnt = other.rows_cnt;
        cols_cnt = other.cols_cnt;
        other.rows_cnt = 0;
        other.cols_cnt = 0;
    }
    return *this;
}

template <class T>
T& RectangularMatrix<T>::get(int row, int col) {
    validateRowIndex(row);
    validateColumnIndex(col);
    return rows[row][col];
}

template <class T>
const T& RectangularMatrix<T>::get(int row, int col) const {
    validateRowIndex(row);
    validateColumnIndex(col);
    return rows[row][col];
}

template <class T>
T& RectangularMatrix<T>::operator()(int row, int col) {
    return get(row, col);
}

template <class T>
const T& RectangularMatrix<T>::operator()(int row, int col) const {
    return get(row, col);
}

template <class T>
void RectangularMatrix<T>::swapRows(int r1, int r2) {
    validateRowIndex(r1);
    validateRowIndex(r2);
    if (r1 == r2) return;
    std::swap(rows[r1], rows[r2]);
}

template <class T>
void RectangularMatrix<T>::multiplyRow(int row, T scalar) {
    validateRowIndex(row);
    for (int j = 0; j < cols_cnt; ++j)
        rows[row][j] *= scalar;
}

template <class T>
void RectangularMatrix<T>::addRowMultiple(int targetRow, int sourceRow, T factor) {
    validateRowIndex(targetRow);
    validateRowIndex(sourceRow);
    for (int j = 0; j < cols_cnt; ++j)
        rows[targetRow][j] += factor * rows[sourceRow][j];
}

template <class T>
void RectangularMatrix<T>::swapColumns(int c1, int c2) {
    validateColumnIndex(c1);
    validateColumnIndex(c2);
    if (c1 == c2) return;
    for (int i = 0; i < rows_cnt; ++i)
        std::swap(rows[i][c1], rows[i][c2]);
}

template <class T>
void RectangularMatrix<T>::multiplyColumn(int col, T scalar) {
    validateColumnIndex(col);
    for (int i = 0; i < rows_cnt; ++i)
        rows[i][col] *= scalar;
}

template <class T>
void RectangularMatrix<T>::addColumnMultiple(int targetCol, int sourceCol, T factor) {
    validateColumnIndex(targetCol);
    validateColumnIndex(sourceCol);
    for (int i = 0; i < rows_cnt; ++i)
        rows[i][targetCol] += factor * rows[i][sourceCol];
}

// ---------- RectangularMatrix: norm ----------

template <class T>
double RectangularMatrix<T>::norm() const {
    double sum = 0.0;
    for (int i = 0; i < rows_cnt; ++i)
        for (int j = 0; j < cols_cnt; ++j) {
            double val = static_cast<double>(rows[i][j]); // require T convertible to double
            sum += val * val;
        }
    return std::sqrt(sum);
}

template <class T>
RectangularMatrix<T> RectangularMatrix<T>::operator+(const RectangularMatrix<T>& other) const {
    validateSameDimensions(other);
    RectangularMatrix<T> result(rows_cnt, cols_cnt);
    for (int i = 0; i < rows_cnt; ++i)
        for (int j = 0; j < cols_cnt; ++j)
            result(i, j) = (*this)(i, j) + other(i, j);
    return result;
}

template <class T>
RectangularMatrix<T> RectangularMatrix<T>::operator*(const RectangularMatrix<T>& other) const {
    if (cols_cnt != other.rows_cnt)
        throw RunTimeError("Matrix multiplication: incompatible dimensions");
    RectangularMatrix<T> result(rows_cnt, other.cols_cnt);
    for (int i = 0; i < rows_cnt; ++i)
        for (int j = 0; j < other.cols_cnt; ++j) {
            T sum = T();
            for (int k = 0; k < cols_cnt; ++k)
                sum += (*this)(i, k) * other(k, j);
            result(i, j) = sum;
        }
    return result;
}

template <class T>
RectangularMatrix<T> RectangularMatrix<T>::operator*(T scalar) const {
    RectangularMatrix<T> result(rows_cnt, cols_cnt);
    for (int i = 0; i < rows_cnt; ++i)
        for (int j = 0; j < cols_cnt; ++j)
            result(i, j) = (*this)(i, j) * scalar;
    return result;
}

template <class T>
RectangularMatrix<T>& RectangularMatrix<T>::addInPlace(const RectangularMatrix<T>& other) {
    validateSameDimensions(other);
    for (int i = 0; i < rows_cnt; ++i)
        for (int j = 0; j < cols_cnt; ++j)
            (*this)(i, j) += other(i, j);
    return *this;
}

template <class T>
RectangularMatrix<T>& RectangularMatrix<T>::multiplyInPlace(T scalar) {
    for (int i = 0; i < rows_cnt; ++i)
        for (int j = 0; j < cols_cnt; ++j)
            (*this)(i, j) *= scalar;
    return *this;
}

template <class T>
SquareMatrix<T>::SquareMatrix() : RectangularMatrix<T>() {}

template <class T>
SquareMatrix<T>::SquareMatrix(int size) : RectangularMatrix<T>(size, size) {}

template <class T>
SquareMatrix<T>::SquareMatrix(int size, bool identity) : RectangularMatrix<T>(size, size) {
    if (identity) {
        for (int i = 0; i < size; ++i) {
            (*this)(i, i) = T(1);
        }
    }
}

template <class T>
SquareMatrix<T>::SquareMatrix(const SquareMatrix<T>& other) : RectangularMatrix<T>(other) {}

template <class T>
SquareMatrix<T>::SquareMatrix(SquareMatrix<T>&& other) noexcept : RectangularMatrix<T>(std::move(other)) {}

template <class T>
SquareMatrix<T>::SquareMatrix(const RectangularMatrix<T>& other) : RectangularMatrix<T>(other) {
    if (other.getRows() != other.getColumns())
        throw RunTimeError("SquareMatrix: rectangular matrix is not square");
}

template <class T>
void SquareMatrix<T>::validateSameSize(const SquareMatrix<T>& other) const {
    if (this->getSize() != other.getSize())
        throw RunTimeError("Matrix: dimensions must match for this operation");
}

template <class T>
SquareMatrix<T>& SquareMatrix<T>::operator=(const SquareMatrix<T>& other) {
    if (this != &other) {
        RectangularMatrix<T>::operator=(other);
    }
    return *this;
}

template <class T>
SquareMatrix<T>& SquareMatrix<T>::operator=(SquareMatrix<T>&& other) noexcept {
    if (this != &other) {
        RectangularMatrix<T>::operator=(std::move(other));
    }
    return *this;
}

template <class T>
SquareMatrix<T> SquareMatrix<T>::operator+(const SquareMatrix<T>& other) const {
    validateSameSize(other);
    SquareMatrix<T> result(this->getSize());
    for (int i = 0; i < this->getSize(); ++i) {
        for (int j = 0; j < this->getSize(); ++j) {
            result(i, j) = (*this)(i, j) + other(i, j);
        }
    }
    return result;
}

template <class T>
SquareMatrix<T> SquareMatrix<T>::operator*(const SquareMatrix<T>& other) const {
    if (this->getSize() != other.getSize()) {
        throw RunTimeError("Matrix multiplication: incompatible dimensions");
    }
    SquareMatrix<T> result(this->getSize());
    for (int i = 0; i < this->getSize(); ++i) {
        for (int j = 0; j < this->getSize(); ++j) {
            T sum = T();
            for (int k = 0; k < this->getSize(); ++k)
                sum += (*this)(i, k) * other(k, j);
            result(i, j) = sum;
        }
    }
    return result;
}

template <class T>
SquareMatrix<T> SquareMatrix<T>::operator*(T scalar) const {
    SquareMatrix<T> result(this->getSize());
    for (int i = 0; i < this->getSize(); ++i) {
        for (int j = 0; j < this->getSize(); ++j) {
            result(i, j) = (*this)(i, j) * scalar;
        }
    }
    return result;
}

template <class T>
SquareMatrix<T>& SquareMatrix<T>::addInPlace(const SquareMatrix<T>& other) {
    validateSameDimensions(other);
    for (int i = 0; i < this->getSize(); ++i)
        for (int j = 0; j < this->getSize(); ++j)
            (*this)(i, j) += other(i, j);
    return *this;
}

template <class T>
SquareMatrix<T>& SquareMatrix<T>::multiplyInPlace(T scalar) {
    for (int i = 0; i < this->getSize(); ++i)
        for (int j = 0; j < this->getSize(); ++j)
            (*this)(i, j) *= scalar;
    return *this;
}