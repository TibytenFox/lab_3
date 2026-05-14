#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "sequence/MutableArraySequence.hpp"
#include "sequence/Exceptions.hpp"
#include <cmath>

template <class T>
class RectangularMatrix {
protected:
    DynamicArray<MutableArraySequence<T>> rows;
    int rows_cnt;
    int cols_cnt;

    void validateRowIndex(int index) const;
    void validateColumnIndex(int index) const;
    void validateSameDimensions(const RectangularMatrix<T>& other) const;

public:
    // ---------- Constructors / Destructor ----------
    RectangularMatrix();                                      
    RectangularMatrix(int rows, int cols);                    
    RectangularMatrix(const RectangularMatrix<T>& other);
    RectangularMatrix(RectangularMatrix<T>&& other) noexcept;
    ~RectangularMatrix() = default;

    // ---------- Assignment operators ----------
    RectangularMatrix<T>& operator=(const RectangularMatrix<T>& other);
    RectangularMatrix<T>& operator=(RectangularMatrix<T>&& other) noexcept;

    // ---------- Dimension getters ----------
    int getRows() const { return rows_cnt; }
    int getColumns() const { return cols_cnt; }

    // ---------- Element access ----------
    T& get(int row, int col);
    const T& get(int row, int col) const;

    T& operator()(int row, int col);
    const T& operator()(int row, int col) const;

    // ---------- Elementary row operations ----------
    void swapRows(int r1, int r2);
    void multiplyRow(int row, T scalar);
    void addRowMultiple(int targetRow, int sourceRow, T factor);

    // ---------- Elementary column operations ----------
    void swapColumns(int c1, int c2);
    void multiplyColumn(int col, T scalar);
    void addColumnMultiple(int targetCol, int sourceCol, T factor);

    // ---------- Norm (Frobenius) ----------
    double norm() const;

    // ---------- Arithmetic (returns new matrix) ----------
    RectangularMatrix<T> operator+(const RectangularMatrix<T>& other) const;
    RectangularMatrix<T> operator*(const RectangularMatrix<T>& other) const;   // matrix multiplication
    RectangularMatrix<T> operator*(T scalar) const;

    friend RectangularMatrix<T> operator*(T scalar, const RectangularMatrix<T>& mat) {
        return mat * scalar;
    }

    // ---------- Mutating arithmetic ----------
    RectangularMatrix<T>& addInPlace(const RectangularMatrix<T>& other);
    RectangularMatrix<T>& multiplyInPlace(T scalar);
};

template <class T>
class SquareMatrix : public RectangularMatrix<T> {
private:
	using RectangularMatrix<T>::getRows;
	using RectangularMatrix<T>::getColumns;
protected:
    void validateSameSize(const SquareMatrix<T>& other) const;
public:
    // ---------- Constructors ----------
    SquareMatrix();                                          
    explicit SquareMatrix(int size);                         
    SquareMatrix(int size, bool identity);                   // if true, identity matrix
    SquareMatrix(const SquareMatrix<T>& other);
    SquareMatrix(SquareMatrix<T>&& other) noexcept;
    explicit SquareMatrix(const RectangularMatrix<T>& other); // throw if not square
    ~SquareMatrix() = default;

    // ---------- Assignment ----------
    SquareMatrix<T>& operator=(const SquareMatrix<T>& other);
    SquareMatrix<T>& operator=(SquareMatrix<T>&& other) noexcept;

    // ---------- Arithmetic (returns new square matrix) ----------
    SquareMatrix<T> operator+(const SquareMatrix<T>& other) const;
    SquareMatrix<T> operator*(const SquareMatrix<T>& other) const;   // matrix multiplication
    SquareMatrix<T> operator*(T scalar) const;

    // ---------- Mutating arithmetic ----------
    SquareMatrix<T>& addInPlace(const SquareMatrix<T>& other);
    SquareMatrix<T>& multiplyInPlace(T scalar);

    // ---------- Auxiliary ----------
    int getSize() const { return this->getRows(); }           // rows == columns
};

#include "Matrix.tpp"
#endif // MATRIX_HPP