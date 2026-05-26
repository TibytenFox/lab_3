#include <iostream>
#include "src/Matrix.hpp"
#include "src/Complex.hpp"
#include "src/Output.hpp"
#include "Utilities.hpp"
#include "sequence/Exceptions.hpp"

template <class T>
void printMatrix(const RectangularMatrix<T>& mat) {
    int r = mat.getRows();
    int c = mat.getColumns();
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            std::cout << mat.get(i, j) << "\t";
        }
        std::cout << std::endl;
    }
}

template <class T>
void printMatrix(const SquareMatrix<T>& mat) {
    printMatrix(static_cast<const RectangularMatrix<T>&>(mat));
}

template <class T> T readValue(const std::string& prompt);

template<> int readValue<int>(const std::string& prompt) {
    return Utilities::readInt(prompt);
}
template<> double readValue<double>(const std::string& prompt) {
    return Utilities::readDouble(prompt);
}
template<> Complex readValue<Complex>(const std::string& prompt) {
    return Utilities::readComplex(prompt);
}

template <class T>
void matrixTypeMenu() {
    RectangularMatrix<T>* A = nullptr;
    SquareMatrix<T>* B = nullptr;
    int choice;

    do {
        std::cout << "\n===== Matrix Tester =====" << std::endl;
        std::cout << " 1. Create Rectangular matrix A" << std::endl;
        std::cout << " 2. Create Square matrix B" << std::endl;
        std::cout << " 3. Print A" << std::endl;
        std::cout << " 4. Print B" << std::endl;
        std::cout << " 5. Element A(i,j)" << std::endl;
        std::cout << " 6. Element B(i,j)" << std::endl;
        std::cout << " 7. A + A" << std::endl;
        std::cout << " 8. A * scalar" << std::endl;
        std::cout << " 9. A * A (matrix multiply, A must be square)" << std::endl;
        std::cout << "10. Norm of A" << std::endl;
        std::cout << "11. Norm of B" << std::endl;
        std::cout << "12. Swap rows in A" << std::endl;
        std::cout << "13. Multiply row in A" << std::endl;
        std::cout << "14. Add row multiple in A" << std::endl;
        std::cout << "15. Swap columns in A" << std::endl;
        std::cout << "16. Multiply column in A" << std::endl;
        std::cout << "17. Add column multiple in A" << std::endl;
        std::cout << "18. Scale A in place (multiplyInPlace)" << std::endl;
        std::cout << "19. Delete A" << std::endl;
        std::cout << "20. Delete B" << std::endl;
        std::cout << " 0. Back to type selection" << std::endl;
        choice = Utilities::readInt("Choose: ");

        switch (choice) {
            case 1: {
                try {
                    delete A;
                    int r = Utilities::readInt("Rows: ");
                    int c = Utilities::readInt("Columns: ");
                    A = new RectangularMatrix<T>(r, c);
                    std::cout << "Enter elements row by row:" << std::endl;
                    for (int i = 0; i < r; ++i)
                        for (int j = 0; j < c; ++j) {
                            std::cout << "  A(" << i << "," << j << "): ";
                            A->get(i, j) = readValue<T>("");
                        }
                    std::cout << "Matrix A created." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 2: {
                try {
                    delete B;
                    int s = Utilities::readInt("Size: ");
                    B = new SquareMatrix<T>(s);
                    std::cout << "Enter elements row by row:" << std::endl;
                    for (int i = 0; i < s; ++i)
                        for (int j = 0; j < s; ++j) {
                            std::cout << "  B(" << i << "," << j << "): ";
                            B->get(i, j) = readValue<T>("");
                        }
                    std::cout << "Square matrix B created." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 3: {
                try {
                    if (!A) { std::cout << "A is not created." << std::endl; break; }
                    std::cout << "Matrix A:" << std::endl;
                    printMatrix(*A);
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 4: {
                try {
                    if (!B) { std::cout << "B is not created." << std::endl; break; }
                    std::cout << "Matrix B:" << std::endl;
                    printMatrix(*B);
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 5: {
                try {
                    if (!A) { std::cout << "A is not created." << std::endl; break; }
                    int i = Utilities::readInt("Row index: ");
                    int j = Utilities::readInt("Col index: ");
                    std::cout << "A(" << i << "," << j << ") = " << A->get(i, j) << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 6: {
                try {
                    if (!B) { std::cout << "B is not created." << std::endl; break; }
                    int i = Utilities::readInt("Row index: ");
                    int j = Utilities::readInt("Col index: ");
                    std::cout << "B(" << i << "," << j << ") = " << B->get(i, j) << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 7: {
                try {
                    if (!A) { std::cout << "A is not created." << std::endl; break; }
                    RectangularMatrix<T> sum = *A + *A;
                    std::cout << "A + A:" << std::endl;
                    printMatrix(sum);
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 8: {
                try {
                    if (!A) { std::cout << "A is not created." << std::endl; break; }
                    T scalar = readValue<T>("Scalar: ");
                    RectangularMatrix<T> prod = *A * scalar;
                    std::cout << "A * scalar:" << std::endl;
                    printMatrix(prod);
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 9: {
                try {
                    if (!A) { std::cout << "A is not created." << std::endl; break; }
                    if (A->getRows() != A->getColumns()) {
                        std::cout << "A is not square, cannot compute A * A." << std::endl;
                        break;
                    }
                    RectangularMatrix<T> prod = (*A) * (*A);
                    std::cout << "A * A:" << std::endl;
                    printMatrix(prod);
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 10: {
                try {
                    if (!A) { std::cout << "A is not created." << std::endl; break; }
                    std::cout << "Norm of A = " << A->norm() << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 11: {
                try {
                    if (!B) { std::cout << "B is not created." << std::endl; break; }
                    std::cout << "Norm of B = " << B->norm() << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 12: {
                try {
                    if (!A) { std::cout << "A is not created." << std::endl; break; }
                    int r1 = Utilities::readInt("First row index: ");
                    int r2 = Utilities::readInt("Second row index: ");
                    A->swapRows(r1, r2);
                    std::cout << "Rows swapped." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 13: {
                try {
                    if (!A) { std::cout << "A is not created." << std::endl; break; }
                    int row = Utilities::readInt("Row index: ");
                    T scalar = readValue<T>("Scalar: ");
                    A->multiplyRow(row, scalar);
                    std::cout << "Row multiplied." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 14: {
                try {
                    if (!A) { std::cout << "A is not created." << std::endl; break; }
                    int target = Utilities::readInt("Target row index: ");
                    int source = Utilities::readInt("Source row index: ");
                    T factor = readValue<T>("Factor: ");
                    A->addRowMultiple(target, source, factor);
                    std::cout << "Row multiple added." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 15: {
                try {
                    if (!A) { std::cout << "A is not created." << std::endl; break; }
                    int c1 = Utilities::readInt("First column index: ");
                    int c2 = Utilities::readInt("Second column index: ");
                    A->swapColumns(c1, c2);
                    std::cout << "Columns swapped." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 16: {
                try {
                    if (!A) { std::cout << "A is not created." << std::endl; break; }
                    int col = Utilities::readInt("Column index: ");
                    T scalar = readValue<T>("Scalar: ");
                    A->multiplyColumn(col, scalar);
                    std::cout << "Column multiplied." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 17: {
                try {
                    if (!A) { std::cout << "A is not created." << std::endl; break; }
                    int target = Utilities::readInt("Target column index: ");
                    int source = Utilities::readInt("Source column index: ");
                    T factor = readValue<T>("Factor: ");
                    A->addColumnMultiple(target, source, factor);
                    std::cout << "Column multiple added." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 18: {
                try {
                    if (!A) { std::cout << "A is not created." << std::endl; break; }
                    T scalar = readValue<T>("Scalar: ");
                    A->multiplyInPlace(scalar);
                    std::cout << "A scaled in place." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 19: {
                try {
                    delete A;
                    A = nullptr;
                    std::cout << "A deleted." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 20: {
                try {
                    delete B;
                    B = nullptr;
                    std::cout << "B deleted." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Invalid option. Try again." << std::endl;
        }
    } while (choice != 0);

    delete A;
    delete B;
}

void printTypeMenu() {
    std::cout << "\nPlease choose element type:" << std::endl;
    std::cout << "1. Int" << std::endl;
    std::cout << "2. Double" << std::endl;
    std::cout << "3. Complex" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

int main() {
    int typeChoice;
    do {
        printTypeMenu();
        typeChoice = Utilities::readInt("Your choice: ");

        switch (typeChoice) {
            case 1: matrixTypeMenu<int>();    break;
            case 2: matrixTypeMenu<double>(); break;
            case 3: matrixTypeMenu<Complex>(); break;
            case 0: std::cout << "Exiting program. Goodbye!" << std::endl; break;
            default: std::cout << "Invalid option. Please try again." << std::endl;
        }
    } while (typeChoice != 0);

    return 0;
}