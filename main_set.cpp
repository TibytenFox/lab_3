#include <iostream>
#include "Utilities.hpp"
#include "src/HashSet.hpp"
#include "src/Types.hpp"
#include "sequence/Exceptions.hpp"

// ----------------------------------------------------------------------
// Value reading helpers 
// ----------------------------------------------------------------------
template<typename T> T readValue(const std::string& prompt);

template<> int readValue<int>(const std::string& prompt) {
    return Utilities::readInt(prompt);
}
template<> double readValue<double>(const std::string& prompt) {
    return Utilities::readDouble(prompt);
}
template<> Complex readValue<Complex>(const std::string& prompt) {
    return Utilities::readComplex(prompt);
}

// ----------------------------------------------------------------------
// Mappings and filters (overloaded for each type)
// ----------------------------------------------------------------------
int square(int x) { return x * x; }
bool greaterThanFive(int x) { return x > 5; }

double square(double x) { return x * x; }
bool greaterThanFive(double x) { return x > 5; }

Complex square(Complex x) {
    Complex result;
    result.real = x.real * x.real + x.imaginary * x.imaginary;
    result.imaginary = 2 * x.real * x.imaginary;
    return result;
}
bool isReal(Complex x) { return x.imaginary == 0; }

// ----------------------------------------------------------------------
// Menu text
// ----------------------------------------------------------------------
void printMenu() {
    std::cout << "\n===== HashSet Tester (A + B) =====" << std::endl;
    std::cout << " 1. Add to A" << std::endl;
    std::cout << " 2. Remove from A" << std::endl;
    std::cout << " 3. Check A contains" << std::endl;
    std::cout << " 4. Use map for A" << std::endl;
    std::cout << " 5. Use where for A" << std::endl;
    std::cout << " 6. Check whether B is a subset of A" << std::endl;
    std::cout << " 7. Print A" << std::endl;
    std::cout << " 8. Add to B" << std::endl;
    std::cout << " 9. Remove from B" << std::endl;
    std::cout << "10. Check B contains" << std::endl;
    std::cout << "11. Use map for B" << std::endl;
    std::cout << "12. Use where for B" << std::endl;
    std::cout << "13. Check whether A is a subset of B" << std::endl;
    std::cout << "14. Print B" << std::endl;
    std::cout << "15. Show A | B (union)" << std::endl;
    std::cout << "16. Show A & B (intersection)" << std::endl;
    std::cout << "17. Show A / B (difference)" << std::endl;
    std::cout << "18. Show B / A (difference)" << std::endl;
    std::cout << "19. Check the equality of sets A and B" << std::endl;
    std::cout << "20. Show sizes of sets" << std::endl;
    std::cout << " 0. Exit" << std::endl;
}

// ----------------------------------------------------------------------
// Unified template‑based 
// ----------------------------------------------------------------------
template <
    typename Key,
    typename Hash = std::hash<Key>,
    Key(*MapFunc)(Key) = nullptr,       
    bool(*WhereFunc)(Key) = nullptr     
>
void setTypeMenu() {
    HashSet<Key, Hash> A, B, C;
    int choice;
    Key value;

    do {
        printMenu();
        choice = Utilities::readInt("Choose an option: ");

        switch (choice) {
            case 1: {   // Add to A
                try {
                    value = readValue<Key>("Value to add to A: ");
                    if (A.add(value))
                        std::cout << value << " added successfully." << std::endl;
                    else
                        std::cout << value << " already exists in the set." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 2: {   // Remove from A
                try {
                    value = readValue<Key>("Value to remove from A: ");
                    if (A.remove(value))
                        std::cout << value << " removed successfully." << std::endl;
                    else
                        std::cout << value << " not found in the set." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 3: {   // Check A contains
                try {
                    value = readValue<Key>("Value to check in A: ");
                    if (A.contains(value))
                        std::cout << value << " is in the set." << std::endl;
                    else
                        std::cout << value << " is not in the set." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 4: {   // map for A
                try {
                    C = A.map(MapFunc);
                    std::cout << "Mapped set: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 5: {   // where for A
                try {
                    C = A.where(WhereFunc);
                    std::cout << "Filtered set: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 6: {   // B subset of A?
                try {
                    if (A.includesSubset(B))
                        std::cout << "B is a subset of A" << std::endl;
                    else
                        std::cout << "B is not a subset of A" << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 7: {   // Print A
                try {
                    A.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 8: {   // Add to B
                try {
                    value = readValue<Key>("Value to add to B: ");
                    if (B.add(value))
                        std::cout << value << " added successfully." << std::endl;
                    else
                        std::cout << value << " already exists in the set." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 9: {   // Remove from B
                try {
                    value = readValue<Key>("Value to remove from B: ");
                    if (B.remove(value))
                        std::cout << value << " removed successfully." << std::endl;
                    else
                        std::cout << value << " not found in the set." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 10: {  // Check B contains
                try {
                    value = readValue<Key>("Value to check in B: ");
                    if (B.contains(value))
                        std::cout << value << " is in the set." << std::endl;
                    else
                        std::cout << value << " is not in the set." << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 11: {  // map for B
                try {
                    C = B.map(MapFunc);
                    std::cout << "Mapped set: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 12: {  // where for B
                try {
                    C = B.where(WhereFunc);
                    std::cout << "Filtered set: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 13: {  // A subset of B?
                try {
                    if (B.includesSubset(A))
                        std::cout << "A is a subset of B" << std::endl;
                    else
                        std::cout << "A is not a subset of B" << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 14: {  // Print B
                try {
                    B.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 15: {  // A | B
                try {
                    C = A | B;
                    std::cout << "Union A | B: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 16: {  // A & B
                try {
                    C = A & B;
                    std::cout << "Intersection A & B: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 17: {  // A / B
                try {
                    C = A / B;
                    std::cout << "Difference A / B: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 18: {  // B / A
                try {
                    C = B / A;
                    std::cout << "Difference B / A: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 19: {  // Equality
                try {
                    if (A == B)
                        std::cout << "Sets are equal" << std::endl;
                    else
                        std::cout << "Sets are not equal" << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 20: {  // Sizes
                try {
                    std::cout << "Size A: " << A.getSize() << ", Size B: " << B.getSize() << std::endl;
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 0:
                std::cout << "Exiting program. Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid option. Try again." << std::endl;
        }
    } while (choice != 0);
}

// ----------------------------------------------------------------------
// Type selection 
// ----------------------------------------------------------------------
void printTypes() {
    std::cout << "\nPlease choose type of HashSet:" << std::endl;
    std::cout << "1. Int" << std::endl;
    std::cout << "2. Double" << std::endl;
    std::cout << "3. Complex" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

int main() {
    int typeChoice;
    do {
        printTypes();
        typeChoice = Utilities::readInt("Your choice: ");

        switch (typeChoice) {
            case 1:
                setTypeMenu<int, std::hash<int>, square, greaterThanFive>();
                break;
            case 2:
                setTypeMenu<double, std::hash<double>, square, greaterThanFive>();
                break;
            case 3:
                setTypeMenu<Complex, ComplexHasher, square, isReal>();
                break;
            case 0:
                std::cout << "Exiting program. Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }
    } while (typeChoice != 0);

    return 0;
}