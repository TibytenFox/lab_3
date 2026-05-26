#include <iostream>
#include "Utilities.hpp"
#include "src/HashSet.hpp"
#include "src/Complex.hpp"
#include "src/Function.hpp"
#include "src/String.hpp"
#include "src/Person.hpp"
#include "src/Output.hpp"
#include "sequence/Exceptions.hpp"

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
template<> FunctionData readValue<FunctionData>(const std::string& prompt) {
    return Utilities::readFunction(prompt);
}
template<> StringData readValue<StringData>(const std::string& prompt) {
    return Utilities::readString(prompt);
}
template<> Student readValue<Student>(const std::string& prompt) {
    return Utilities::readStudent(prompt);
}

int square(int x) { return x * x; }
bool greaterThanFive(int x) { return x > 5; }

double square(double x) { return x * x; }
bool greaterThanFive(double x) { return x > 5; }

Complex square(Complex x) { return Complex(x.real * x.real - x.imaginary * x.imaginary, 2 * x.real * x.imaginary); }
bool isReal(Complex x) { return x.imaginary == 0; }

StringData add_star(StringData s) { return StringData((std::string(s.c_str()) + "*").c_str()); }
bool len_gt_4(StringData s) { return std::strlen(s.c_str()) > 4; }

FunctionData func_ident(FunctionData f) { return f; }
bool is_inc_func(FunctionData f) { return f.getPointer() == &Utilities::s_inc; }

Student st_grade_up(Student s) { 
    return Student(s.GetID(), s.GetFirstName().c_str(), s.GetLastName().c_str(), s.GetGroupName().c_str(), s.GetAverageGrade() + 0.2); 
}
bool st_is_good(Student s) { return s.GetAverageGrade() >= 4.0; }

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
            case 1: {
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
            case 2: {
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
            case 3: {
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
            case 4: {
                try {
                    C = A.map(MapFunc);
                    std::cout << "Mapped set: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 5: {
                try {
                    C = A.where(WhereFunc);
                    std::cout << "Filtered set: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 6: {
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
            case 7: {
                try {
                    A.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 8: {
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
            case 9: { 
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
            case 10: {
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
            case 11: {
                try {
                    C = B.map(MapFunc);
                    std::cout << "Mapped set: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 12: {
                try {
                    C = B.where(WhereFunc);
                    std::cout << "Filtered set: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 13: {
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
            case 14: {
                try {
                    B.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 15: {
                try {
                    C = A | B;
                    std::cout << "Union A | B: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 16: {
                try {
                    C = A & B;
                    std::cout << "Intersection A & B: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 17: {
                try {
                    C = A / B;
                    std::cout << "Difference A / B: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 18: {
                try {
                    C = B / A;
                    std::cout << "Difference B / A: ";
                    C.printSet();
                } catch (const Exception& e) {
                    std::cout << "Error: " << e.GetMessage() << std::endl;
                }
                break;
            }
            case 19: {
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
            case 20: {
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

void printTypes() {
    std::cout << "\nPlease choose type of HashSet:" << std::endl;
    std::cout << "1. Int\n2. Double\n3. Complex\n4. StringData\n5. FunctionData\n6. Student\n0. Exit" << std::endl;
}

int main() {
    int typeChoice;
    do {
        printTypes();
        typeChoice = Utilities::readInt("Your choice: ");

        switch (typeChoice) {
            case 1: setTypeMenu<int, std::hash<int>, square, greaterThanFive>(); break;
            case 2: setTypeMenu<double, std::hash<double>, square, greaterThanFive>(); break;
            case 3: setTypeMenu<Complex, std::hash<Complex>, square, isReal>(); break;
            case 4: setTypeMenu<StringData, std::hash<StringData>, add_star, len_gt_4>(); break;
            case 5: setTypeMenu<FunctionData, std::hash<FunctionData>, func_ident, is_inc_func>(); break;
            case 6: setTypeMenu<Student, std::hash<Student>, st_grade_up, st_is_good>(); break;
            case 0: std::cout << "Goodbye!\n"; break;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }
    } while (typeChoice != 0);

    return 0;
}