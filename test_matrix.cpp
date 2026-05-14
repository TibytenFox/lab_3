// test_matrix.cpp
#include <iostream>
#include "src/Matrix.hpp"
#include "src/Types.hpp"
#include "sequence/Exceptions.hpp"

#define TEST(name) void name(); \
    struct Register_##name { Register_##name() { register_test(name); std::cout << #name << '\n'; } } reg_##name; \
    void name()

static int tests_passed = 0;
static int tests_failed = 0;

void register_test(void (*f)()) {
    try {
        f();
        ++tests_passed;
        std::cout << "[PASS]\t";
    } catch (const Exception &e) {
        ++tests_failed;
        std::cout << "[FAIL] " << e.GetMessage() << "\t";
    } catch (...) {
        ++tests_failed;
        std::cout << "[FAIL] Unknown exception\t";
    }
}

#define ASSERT_EQ(x, y) \
    if ((x) != (y)) throw RunTimeError("ASSERT_EQ failed: " #x " != " #y)

#define ASSERT_THROWS(expr, exc) \
    do { \
        bool caught = false; \
        try { expr; } \
        catch (const exc&) { caught = true; } \
        catch (...) {} \
        if (!caught) throw RunTimeError("ASSERT_THROWS failed: " #expr); \
    } while(0)

#define ASSERT_NO_THROW(expr) \
    do { \
        try { expr; } \
        catch (...) { throw RunTimeError("ASSERT_NO_THROW failed: " #expr); } \
    } while(0)

// ----------- TESTS FOR RectangularMatrix -----------

TEST(RectangularMatrix_DefaultConstructor) {
    RectangularMatrix<int> m;
    ASSERT_EQ(m.getRows(), 0);
    ASSERT_EQ(m.getColumns(), 0);
}

TEST(RectangularMatrix_ConstructorWithDimensions) {
    RectangularMatrix<int> m(3, 4);
    ASSERT_EQ(m.getRows(), 3);
    ASSERT_EQ(m.getColumns(), 4);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_EQ(m.get(i, j), 0);
		}
	}
}

TEST(RectangularMatrix_ConstructorWithNegativeDimensions) {
    ASSERT_THROWS(RectangularMatrix<int> m(-1, 5), IndexOutOfRange);
    ASSERT_THROWS(RectangularMatrix<int> m(5, -1), IndexOutOfRange);
    ASSERT_THROWS(RectangularMatrix<int> m(-2, -3), IndexOutOfRange);
    ASSERT_NO_THROW(RectangularMatrix<int> m(0, 5));
    ASSERT_NO_THROW(RectangularMatrix<int> m(5, 0));
}

TEST(RectangularMatrix_CopyConstructor) {
    RectangularMatrix<int> m1(2, 3);
    m1.get(0, 0) = 1; m1.get(0, 1) = 2; m1.get(0, 2) = 3;
    m1.get(1, 0) = 4; m1.get(1, 1) = 5; m1.get(1, 2) = 6;

    RectangularMatrix<int> m2(m1);
    ASSERT_EQ(m2.getRows(), 2);
    ASSERT_EQ(m2.getColumns(), 3);
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
            ASSERT_EQ(m2.get(i, j), m1.get(i, j));

    m2.get(0, 0) = 100;
    ASSERT_EQ(m1.get(0, 0), 1);
}

TEST(RectangularMatrix_MoveConstructor) {
    RectangularMatrix<int> m1(2, 3);
    m1.get(0, 0) = 1;
    int rows = m1.getRows();
    int cols = m1.getColumns();
    RectangularMatrix<int> m2(std::move(m1));
    ASSERT_EQ(m2.getRows(), rows);
    ASSERT_EQ(m2.getColumns(), cols);
    ASSERT_EQ(m2.get(0, 0), 1);

    ASSERT_NO_THROW(m1.getRows());
}

TEST(RectangularMatrix_Assignment) {
    RectangularMatrix<int> m1(2, 2);
    m1.get(0, 0) = 1; m1.get(0, 1) = 2;
    m1.get(1, 0) = 3; m1.get(1, 1) = 4;
    RectangularMatrix<int> m2;
    m2 = m1;
    ASSERT_EQ(m2.getRows(), 2);
    ASSERT_EQ(m2.getColumns(), 2);
    ASSERT_EQ(m2.get(0, 0), 1);
    m2.get(0, 0) = 10;
    ASSERT_EQ(m1.get(0, 0), 1); 

    RectangularMatrix<int> m3;
    m3 = std::move(m1);
    ASSERT_EQ(m3.get(0, 0), 1);
}

TEST(RectangularMatrix_ElementAccess) {
    RectangularMatrix<double> m(3, 3);
    m.get(0, 0) = 1.5;
    m(1, 2) = 2.7;
    ASSERT_EQ(m.get(0, 0), 1.5);
    ASSERT_EQ(m(1, 2), 2.7);
    const RectangularMatrix<double> cm = m;
    ASSERT_EQ(cm.get(0, 0), 1.5);
    ASSERT_EQ(cm(1, 2), 2.7);
}

TEST(RectangularMatrix_ElementAccessOutOfRange) {
    RectangularMatrix<int> m(2, 3);
    ASSERT_THROWS(m.get(-1, 0), IndexOutOfRange);
    ASSERT_THROWS(m.get(0, -1), IndexOutOfRange);
    ASSERT_THROWS(m.get(2, 0), IndexOutOfRange);
    ASSERT_THROWS(m.get(0, 3), IndexOutOfRange);
    ASSERT_THROWS(m(2, 2), IndexOutOfRange);
    ASSERT_NO_THROW(m(1, 2));
}

TEST(RectangularMatrix_SwapRows) {
    RectangularMatrix<int> m(2, 3);
    for (int j = 0; j < 3; ++j) {
        m.get(0, j) = j + 1;
        m.get(1, j) = (j + 1) * 10;
    }
    m.swapRows(0, 1);
    for (int j = 0; j < 3; ++j) {
        ASSERT_EQ(m.get(0, j), (j + 1) * 10);
        ASSERT_EQ(m.get(1, j), j + 1);
    }

    m.swapRows(0, 0);
    ASSERT_EQ(m.get(0, 0), 10);
    ASSERT_THROWS(m.swapRows(0, 2), IndexOutOfRange);
}

TEST(RectangularMatrix_MultiplyRow) {
    RectangularMatrix<int> m(2, 2);
    m.get(0, 0) = 1; m.get(0, 1) = 2;
    m.get(1, 0) = 3; m.get(1, 1) = 4;
    m.multiplyRow(1, 3);
    ASSERT_EQ(m.get(1, 0), 9);
    ASSERT_EQ(m.get(1, 1), 12);
    ASSERT_EQ(m.get(0, 0), 1); // unchanged
    ASSERT_THROWS(m.multiplyRow(-1, 2), IndexOutOfRange);
    ASSERT_THROWS(m.multiplyRow(2, 2), IndexOutOfRange);
}

TEST(RectangularMatrix_AddRowMultiple) {
    RectangularMatrix<int> m(2, 3);
    for (int j = 0; j < 3; ++j) {
        m.get(0, j) = 1;
        m.get(1, j) = 2;
    }

    m.addRowMultiple(0, 1, 2);
    for (int j = 0; j < 3; ++j) {
        ASSERT_EQ(m.get(0, j), 1 + 2*2);
        ASSERT_EQ(m.get(1, j), 2);
    }
    ASSERT_THROWS(m.addRowMultiple(2, 0, 1), IndexOutOfRange);
    ASSERT_THROWS(m.addRowMultiple(0, 2, 1), IndexOutOfRange);
}

TEST(RectangularMatrix_ColumnOperations) {
    RectangularMatrix<int> m(2, 3);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            m.get(i, j) = j + 1;
		}
	}

    m.swapColumns(0, 2);
    ASSERT_EQ(m.get(0, 0), 3);
    ASSERT_EQ(m.get(0, 2), 1);

    m.multiplyColumn(1, 2);
    ASSERT_EQ(m.get(0, 1), 2 * 2);

    m.addColumnMultiple(0, 2, 1);
    ASSERT_EQ(m.get(0, 0), 3 + 1);
    ASSERT_THROWS(m.swapColumns(0, 3), IndexOutOfRange);
    ASSERT_THROWS(m.multiplyColumn(-1, 2), IndexOutOfRange);
    ASSERT_THROWS(m.addColumnMultiple(0, 3, 1), IndexOutOfRange);
}

TEST(RectangularMatrix_Norm) {
    RectangularMatrix<double> m(2, 2);
    m.get(0, 0) = 3; m.get(0, 1) = 0;
    m.get(1, 0) = 0; m.get(1, 1) = 4;
    // sqrt(3^2+4^2) = 5
    ASSERT_EQ(m.norm(), 5.0);
    RectangularMatrix<double> m2(1, 3);
    m2.get(0, 0) = 1; m2.get(0, 1) = 2; m2.get(0, 2) = 2;
    ASSERT_EQ(m2.norm(), 3.0); // sqrt(1+4+4)=3
    RectangularMatrix<double> empty;
    ASSERT_EQ(empty.norm(), 0.0);
}

TEST(RectangularMatrix_Addition) {
    RectangularMatrix<int> a(2, 2), b(2, 2);
    a.get(0, 0) = 1; a.get(0, 1) = 2;
    a.get(1, 0) = 3; a.get(1, 1) = 4;
    b.get(0, 0) = 5; b.get(0, 1) = 6;
    b.get(1, 0) = 7; b.get(1, 1) = 8;
    RectangularMatrix<int> c = a + b;
    ASSERT_EQ(c.getRows(), 2);
    ASSERT_EQ(c.getColumns(), 2);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            ASSERT_EQ(c.get(i, j), a.get(i, j) + b.get(i, j));
		}
	}

    ASSERT_EQ(a.get(0, 0), 1);
    ASSERT_EQ(b.get(0, 0), 5);
}

TEST(RectangularMatrix_AdditionDimensionMismatch) {
    RectangularMatrix<int> a(2, 3), b(2, 4);
    ASSERT_THROWS(a + b, RunTimeError);
    RectangularMatrix<int> c(3, 3), d(2, 3);
    ASSERT_THROWS(c + d, RunTimeError);
}

TEST(RectangularMatrix_MatrixMultiplication) {
    RectangularMatrix<int> a(2, 3), b(3, 2);
    int a_vals[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int b_vals[3][2] = {{7, 8}, {9, 10}, {11, 12}};
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            a.get(i, j) = a_vals[i][j];
		}
	}
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            b.get(i, j) = b_vals[i][j];
		}
	}
    RectangularMatrix<int> c = a * b;
    ASSERT_EQ(c.getRows(), 2);
    ASSERT_EQ(c.getColumns(), 2);
    // expected: c[0][0]=1*7+2*9+3*11=7+18+33=58
    // c[0][1]=1*8+2*10+3*12=8+20+36=64
    // c[1][0]=4*7+5*9+6*11=28+45+66=139
    // c[1][1]=4*8+5*10+6*12=32+50+72=154
    ASSERT_EQ(c.get(0, 0), 58);
    ASSERT_EQ(c.get(0, 1), 64);
    ASSERT_EQ(c.get(1, 0), 139);
    ASSERT_EQ(c.get(1, 1), 154);
}

TEST(RectangularMatrix_MatrixMultiplicationDimensionMismatch) {
    RectangularMatrix<int> a(2, 3), b(4, 2);
    ASSERT_THROWS(a * b, RunTimeError);
    RectangularMatrix<int> c(2, 3), d(3, 2);
    ASSERT_NO_THROW(c * d);
}

TEST(RectangularMatrix_ScalarMultiplication) {
    RectangularMatrix<int> m(2, 2);
    m.get(0, 0) = 1; m.get(0, 1) = 2;
    m.get(1, 0) = 3; m.get(1, 1) = 4;
    RectangularMatrix<int> res = m * 3;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            ASSERT_EQ(res.get(i, j), m.get(i, j) * 3);
		}
	}
    RectangularMatrix<int> res2 = 3 * m;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            ASSERT_EQ(res2.get(i, j), m.get(i, j) * 3);
		}
	}

    ASSERT_EQ(m.get(0, 0), 1);
}

TEST(RectangularMatrix_AddInPlace) {
    RectangularMatrix<int> a(2, 2), b(2, 2);
    a.get(0, 0) = 1; a.get(0, 1) = 2;
    a.get(1, 0) = 3; a.get(1, 1) = 4;
    b.get(0, 0) = 5; b.get(0, 1) = 6;
    b.get(1, 0) = 7; b.get(1, 1) = 8;
    a.addInPlace(b);
    ASSERT_EQ(a.get(0, 0), 6);
    ASSERT_EQ(a.get(0, 1), 8);
    ASSERT_EQ(a.get(1, 0), 10);
    ASSERT_EQ(a.get(1, 1), 12);

    ASSERT_EQ(b.get(0, 0), 5);
}

TEST(RectangularMatrix_MultiplyInPlace) {
    RectangularMatrix<int> m(2, 2);
    m.get(0, 0) = 1; m.get(0, 1) = 2;
    m.get(1, 0) = 3; m.get(1, 1) = 4;
    m.multiplyInPlace(2);
    ASSERT_EQ(m.get(0, 0), 2);
    ASSERT_EQ(m.get(0, 1), 4);
    ASSERT_EQ(m.get(1, 0), 6);
    ASSERT_EQ(m.get(1, 1), 8);
}

// ----------- TESTS FOR SquareMatrix -----------

TEST(SquareMatrix_DefaultConstructor) {
    SquareMatrix<int> m;
    ASSERT_EQ(m.getSize(), 0);
}

TEST(SquareMatrix_ConstructorWithSize) {
    SquareMatrix<int> m(4);
    ASSERT_EQ(m.getSize(), 4);

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            ASSERT_EQ(m.get(i, j), 0);
    ASSERT_THROWS(SquareMatrix<int> m2(-1), IndexOutOfRange);
}

TEST(SquareMatrix_ConstructorIdentity) {
    SquareMatrix<int> m(3, true);
    ASSERT_EQ(m.getSize(), 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == j) { 
				ASSERT_EQ(m.get(i, j), 1);
			} else {
				ASSERT_EQ(m.get(i, j), 0);
			}
        }
    }

    SquareMatrix<int> m0(0, true);
    ASSERT_EQ(m0.getSize(), 0);
}

TEST(SquareMatrix_CopyConstructor) {
    SquareMatrix<int> m1(2);
    m1.get(0, 0) = 1; m1.get(0, 1) = 2;
    m1.get(1, 0) = 3; m1.get(1, 1) = 4;
    SquareMatrix<int> m2(m1);
    ASSERT_EQ(m2.getSize(), 2);
    ASSERT_EQ(m2.get(0, 0), 1);
    m2.get(0, 0) = 10;
    ASSERT_EQ(m1.get(0, 0), 1); // deep copy
}

TEST(SquareMatrix_MoveConstructor) {
    SquareMatrix<int> m1(2);
    m1.get(0, 0) = 42;
    SquareMatrix<int> m2(std::move(m1));
    ASSERT_EQ(m2.getSize(), 2);
    ASSERT_EQ(m2.get(0, 0), 42);
    ASSERT_NO_THROW(m1.getSize());
}

TEST(SquareMatrix_ConstructorFromRectangularValid) {
    RectangularMatrix<int> rect(3, 3);
    rect.get(0, 0) = 1; rect.get(1, 1) = 5; rect.get(2, 2) = 9;
    SquareMatrix<int> sq(rect);
    ASSERT_EQ(sq.getSize(), 3);
    ASSERT_EQ(sq.get(0, 0), 1);
    ASSERT_EQ(sq.get(1, 1), 5);
    ASSERT_EQ(sq.get(2, 2), 9);
}

TEST(SquareMatrix_ConstructorFromRectangularInvalid) {
    RectangularMatrix<int> rect(2, 3);
    ASSERT_THROWS(SquareMatrix<int> sq(rect), RunTimeError);
    RectangularMatrix<int> rect2(4, 4);
    ASSERT_NO_THROW(SquareMatrix<int> sq(rect2));
}

TEST(SquareMatrix_Assignment) {
    SquareMatrix<int> m1(2);
    m1.get(0, 0) = 1; m1.get(0, 1) = 2;
    m1.get(1, 0) = 3; m1.get(1, 1) = 4;
    SquareMatrix<int> m2;
    m2 = m1;
    ASSERT_EQ(m2.getSize(), 2);
    ASSERT_EQ(m2.get(0, 0), 1);
    m2.get(0, 0) = 10;
    ASSERT_EQ(m1.get(0, 0), 1);
    SquareMatrix<int> m3;
    m3 = std::move(m1);
    ASSERT_EQ(m3.get(0, 0), 1);
}

TEST(SquareMatrix_GetSize) {
    SquareMatrix<int> m1;
    SquareMatrix<int> m2(5);
    SquareMatrix<int> m3(5, true);
    ASSERT_EQ(m1.getSize(), 0);
    ASSERT_EQ(m2.getSize(), 5);
    ASSERT_EQ(m3.getSize(), 5);
}

TEST(SquareMatrix_InheritedOperations) {
    SquareMatrix<int> a(2), b(2);
    a.get(0, 0) = 1; a.get(0, 1) = 2;
    a.get(1, 0) = 3; a.get(1, 1) = 4;
    b.get(0, 0) = 5; b.get(0, 1) = 6;
    b.get(1, 0) = 7; b.get(1, 1) = 8;
    SquareMatrix<int> c = a + b;
    ASSERT_EQ(c.getSize(), 2);
    ASSERT_EQ(c.get(0, 0), 6);

    SquareMatrix<int> d = a * b;
    ASSERT_EQ(d.get(0, 0), 1*5 + 2*7);
    ASSERT_EQ(d.get(0, 1), 1*6 + 2*8);
    ASSERT_EQ(d.get(1, 0), 3*5 + 4*7);
    ASSERT_EQ(d.get(1, 1), 3*6 + 4*8);

    a.multiplyInPlace(2);
    ASSERT_EQ(a.get(0, 0), 2);
}

TEST(SquareMatrix_IdentityProperties) {
    SquareMatrix<int> I(3, true);
    SquareMatrix<int> A(3);
    A.get(0, 0) = 1; A.get(0, 1) = 2; A.get(0, 2) = 3;
    A.get(1, 0) = 4; A.get(1, 1) = 5; A.get(1, 2) = 6;
    A.get(2, 0) = 7; A.get(2, 1) = 8; A.get(2, 2) = 9;
    SquareMatrix<int> AI = A * I;
    SquareMatrix<int> IA = I * A;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(AI.get(i, j), A.get(i, j));
            ASSERT_EQ(IA.get(i, j), A.get(i, j));
        }
	}
}

// ----------- TEST WITH CUSTOM TYPE Complex -----------
TEST(Matrix_ComplexType) {
    RectangularMatrix<Complex> m(2, 2);
    m.get(0, 0) = Complex(1, 2);
    m.get(0, 1) = Complex(3, 4);
    m.get(1, 0) = Complex(5, 6);
    m.get(1, 1) = Complex(7, 8);

    RectangularMatrix<Complex> m2 = m * Complex(0, 1); 
    // Check norm: should be sqrt(1^2+2^2+3^2+4^2+5^2+6^2+7^2+8^2)
    double expected_norm = 0;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j) {
            double re = m.get(i, j).real;
            double im = m.get(i, j).imaginary;
			double temp = re * re + im * im;
            expected_norm += temp * temp;
        }
    expected_norm = sqrt(expected_norm);
    ASSERT_EQ(m.norm(), expected_norm);
}

int main() {
    std::cout << "\nMatrix tests results: passed " << tests_passed << ", failed " << tests_failed << "\n";
    return tests_failed == 0 ? 0 : 1;
}