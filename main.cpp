#include <iostream>
#include <string>
#include "src/HashSet.hpp"
#include "src/Types.hpp"

int readInt(const std::string& prompt) {
    int val;
    std::cout << prompt;
    std::cin >> val;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter an integer: ";
        std::cin >> val;
    }
    return val;
}

double readDouble(const std::string& prompt) {
    double val;
    std::cout << prompt;
    std::cin >> val;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter an double: ";
        std::cin >> val;
    }
    return val;
}

Complex readComplex(const std::string& prompt) {
    Complex val;
    std::cout << prompt;
    std::cin >> val.real >> val.imaginary;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter an double: ";
        std::cin >> val.real >> val.imaginary;
    }
    return val;
}

template <class Key, class Hash = std::hash<Key>>
HashSet<Key, Hash>* createSetFromUser() {
    int n = readInt("How many elements? ");
    HashSet<Key, Hash>* res = new HashSet<Key, Hash>(n);

    for (int i = 0; i < n; ++i) {
        int temp = readInt("");
        res->add(temp);
    }

    return res;
}

int square(int x) { return x * x; }
bool greaterThanFive(int x) { return x > 5; }

double square(double x) { return x * x; }
bool greaterThanFive(double x) { return x > 5; }

Complex square(Complex x) {
    Complex result;
    result.real = x.real * x.real + x.imaginary + x.imaginary;
    result.imaginary = 2 * x.real * x.imaginary;
    return result;
}
bool isReal(Complex x) { return x.imaginary == 0; }

void printMenu() {
    std::cout << "\n===== HashSet Tester (A + B) =====" << std::endl;
    std::cout << "1. Add to A" << std::endl;
    std::cout << "2. Remove from A" << std::endl;
    std::cout << "3. Check A contains" << std::endl;
    std::cout << "4. Use map for A" << std::endl;
    std::cout << "5. Use where for A" << std::endl;
    std::cout << "6. Check whether B is a subset of A" << std::endl;
    std::cout << "7. Print A" << std::endl;
    std::cout << "8. Add to B" << std::endl;
    std::cout << "9. Remove from B" << std::endl;
    std::cout << "10. Check B contains" << std::endl;
    std::cout << "11. Use map for B" << std::endl;
    std::cout << "12. Use where for B" << std::endl;
    std::cout << "13. Check whether A is a subset of B" << std::endl;
    std::cout << "14. Print B" << std::endl;
    std::cout << "15. Show A | B (union)" << std::endl;
    std::cout << "16. Show A & B (intersection)" << std::endl;
    std::cout << "17. Show A / B (difference)" << std::endl;
    std::cout << "18. Show B / A (difference)" << std::endl;
    std::cout << "19. Show sizes of sets" << std::endl;
    std::cout << "20. Сheck the equality of sets A and B" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

void intMenu() {
    HashSet<int> A, B, C;
    int choice, value;
    
    do {
        printMenu();
        choice = readInt("Choose an option: ");
        
        switch (choice) {
            case 1:
                value = readInt("Value to add to A: ");
                if (A.add(value)) {
                    std::cout << value << " added successfully." << std::endl;
                } else {
                    std::cout << value << " already exists in the set." << std::endl;
                }
                break;
                
            case 2:
                value = readInt("Value to remove from A: ");
                if (A.remove(value)) {
                    std::cout << value << " removed successfully." << std::endl;
                } else {
                    std::cout << value << " not found in the set." << std::endl;
                }
                break;
                
            case 3:
                value = readInt("Value to check in A: ");
                if (A.contains(value)) {
                    std::cout << value << " is in the set." << std::endl;
                } else {
                    std::cout << value << " is not in the set." << std::endl;
                }
                break;

            case 4:
                C = A.map(square);
                std::cout << "Mapped set: ";
                C.printSet();
                break;

            case 5:
                C = A.where(greaterThanFive);
                std::cout << "Mapped set: ";
                C.printSet();
                break;

            case 6:
                if (A.includesSubset(B)) std::cout << "B is a subset of A" << std::endl;
                else std::cout << "B is not a subset of A" << std::endl;
                break;

            case 7:
                A.printSet();
                break;
            
            case 8:
                value = readInt("Value to add to B: ");
                if (B.add(value)) {
                    std::cout << value << " added successfully." << std::endl;
                } else {
                    std::cout << value << " already exists in the set." << std::endl;
                }
                break;
                
            case 9:
                value = readInt("Value to remove from B: ");
                if (B.remove(value)) {
                    std::cout << value << " removed successfully." << std::endl;
                } else {
                    std::cout << value << " not found in the set." << std::endl;
                }
                break;
                
            case 10:
                value = readInt("Value to check in B: ");
                if (B.contains(value)) {
                    std::cout << value << " is in the set." << std::endl;
                } else {
                    std::cout << value << " is not in the set." << std::endl;
                }
                break;

            case 11:
                C = A.map(square);
                std::cout << "Mapped set: ";
                C.printSet();
                break;

            case 12:
                C = A.where(greaterThanFive);
                std::cout << "Mapped set: ";
                C.printSet();
                break;

            case 13:
                if (B.includesSubset(A)) std::cout << "A is a subset of B" << std::endl;
                else std::cout << "A is not a subset of B" << std::endl;
                break;

            case 14:
                B.printSet();
                break;

            case 15:
                C = A | B;
                std::cout << "Union A | B: ";
                C.printSet();
                break;

            case 16:
                C = A & B;
                std::cout << "Intersection A & B: ";
                C.printSet();
                break;
            
            case 17:
                C = A / B;
                std::cout << "Difference A / B: ";
                C.printSet();
                break;

            case 18:
                C = B / A;
                std::cout << "Difference B / A: ";
                C.printSet();
                break;

            case 19:
                if (A == B) std::cout << "Sets are equal" << std::endl;
                else std::cout << "Sets are not equal" << std::endl;
                break;
            
            case 20:
                std::cout << "Size A: " << A.getSize() << ", Size B: " << B.getSize() << std::endl;
                break;

            case 0:
                std::cout << "Exiting program. Goodbye!" << std::endl;
                break;
                
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }
    } while (choice != 0);
}

void doubleMenu() {
    HashSet<double> A, B, C;
    int choice;
    double value;
    
    do {
        printMenu();
        choice = readInt("Choose an option: ");
        
        switch (choice) {
            case 1:
                value = readDouble("Value to add to A: ");
                if (A.add(value)) {
                    std::cout << value << " added successfully." << std::endl;
                } else {
                    std::cout << value << " already exists in the set." << std::endl;
                }
                break;
                
            case 2:
                value = readDouble("Value to remove from A: ");
                if (A.remove(value)) {
                    std::cout << value << " removed successfully." << std::endl;
                } else {
                    std::cout << value << " not found in the set." << std::endl;
                }
                break;
                
            case 3:
                value = readDouble("Value to check in A: ");
                if (A.contains(value)) {
                    std::cout << value << " is in the set." << std::endl;
                } else {
                    std::cout << value << " is not in the set." << std::endl;
                }
                break;

            case 4:
                C = A.map(square);
                std::cout << "Mapped set: ";
                C.printSet();
                break;

            case 5:
                C = A.where(greaterThanFive);
                std::cout << "Mapped set: ";
                C.printSet();
                break;

            case 6:
                if (A.includesSubset(B)) std::cout << "B is a subset of A" << std::endl;
                else std::cout << "B is not a subset of A" << std::endl;
                break;

            case 7:
                A.printSet();
                break;
            
            case 8:
                value = readDouble("Value to add to B: ");
                if (B.add(value)) {
                    std::cout << value << " added successfully." << std::endl;
                } else {
                    std::cout << value << " already exists in the set." << std::endl;
                }
                break;
                
            case 9:
                value = readDouble("Value to remove from B: ");
                if (B.remove(value)) {
                    std::cout << value << " removed successfully." << std::endl;
                } else {
                    std::cout << value << " not found in the set." << std::endl;
                }
                break;
                
            case 10:
                value = readDouble("Value to check in B: ");
                if (B.contains(value)) {
                    std::cout << value << " is in the set." << std::endl;
                } else {
                    std::cout << value << " is not in the set." << std::endl;
                }
                break;

            case 11:
                C = A.map(square);
                std::cout << "Mapped set: ";
                C.printSet();
                break;

            case 12:
                C = A.where(greaterThanFive);
                std::cout << "Mapped set: ";
                C.printSet();
                break;

            case 13:
                if (B.includesSubset(A)) std::cout << "A is a subset of B" << std::endl;
                else std::cout << "A is not a subset of B" << std::endl;
                break;

            case 14:
                B.printSet();
                break;

            case 15:
                C = A | B;
                std::cout << "Union A | B: ";
                C.printSet();
                break;

            case 16:
                C = A & B;
                std::cout << "Intersection A & B: ";
                C.printSet();
                break;
            
            case 17:
                C = A / B;
                std::cout << "Difference A / B: ";
                C.printSet();
                break;

            case 18:
                C = B / A;
                std::cout << "Difference B / A: ";
                C.printSet();
                break;

            case 19:
                if (A == B) std::cout << "Sets are equal" << std::endl;
                else std::cout << "Sets are not equal" << std::endl;
                break;
            
            case 20:
                std::cout << "Size A: " << A.getSize() << ", Size B: " << B.getSize() << std::endl;
                break;

            case 0:
                std::cout << "Exiting program. Goodbye!" << std::endl;
                break;
                
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }
    } while (choice != 0);
}

void complexMenu() {
    HashSet<Complex, ComplexHasher> A, B, C;
    int choice;
    Complex value;
    
    do {
        printMenu();
        choice = readInt("Choose an option: ");
        
        switch (choice) {
            case 1:
                value = readComplex("Value to add to A: ");
                if (A.add(value)) {
                    std::cout << value << " added successfully." << std::endl;
                } else {
                    std::cout << value << " already exists in the set." << std::endl;
                }
                break;
                
            case 2:
                value = readComplex("Value to remove from A: ");
                if (A.remove(value)) {
                    std::cout << value << " removed successfully." << std::endl;
                } else {
                    std::cout << value << " not found in the set." << std::endl;
                }
                break;
                
            case 3:
                value = readComplex("Value to check in A: ");
                if (A.contains(value)) {
                    std::cout << value << " is in the set." << std::endl;
                } else {
                    std::cout << value << " is not in the set." << std::endl;
                }
                break;

            case 4:
                C = A.map(square);
                std::cout << "Mapped set: ";
                C.printSet();
                break;

            case 5:
                C = A.where(isReal);
                std::cout << "Mapped set: ";
                C.printSet();
                break;

            case 6:
                if (A.includesSubset(B)) std::cout << "B is a subset of A" << std::endl;
                else std::cout << "B is not a subset of A" << std::endl;
                break;

            case 7:
                A.printSet();
                break;
            
            case 8:
                value = readComplex("Value to add to B: ");
                if (B.add(value)) {
                    std::cout << value << " added successfully." << std::endl;
                } else {
                    std::cout << value << " already exists in the set." << std::endl;
                }
                break;
                
            case 9:
                value = readComplex("Value to remove from B: ");
                if (B.remove(value)) {
                    std::cout << value << " removed successfully." << std::endl;
                } else {
                    std::cout << value << " not found in the set." << std::endl;
                }
                break;
                
            case 10:
                value = readComplex("Value to check in B: ");
                if (B.contains(value)) {
                    std::cout << value << " is in the set." << std::endl;
                } else {
                    std::cout << value << " is not in the set." << std::endl;
                }
                break;

            case 11:
                C = A.map(square);
                std::cout << "Mapped set: ";
                C.printSet();
                break;

            case 12:
                C = A.where(isReal);
                std::cout << "Mapped set: ";
                C.printSet();
                break;

            case 13:
                if (B.includesSubset(A)) std::cout << "A is a subset of B" << std::endl;
                else std::cout << "A is not a subset of B" << std::endl;
                break;

            case 14:
                B.printSet();
                break;

            case 15:
                C = A | B;
                std::cout << "Union A | B: ";
                C.printSet();
                break;

            case 16:
                C = A & B;
                std::cout << "Intersection A & B: ";
                C.printSet();
                break;
            
            case 17:
                C = A / B;
                std::cout << "Difference A / B: ";
                C.printSet();
                break;

            case 18:
                C = B / A;
                std::cout << "Difference B / A: ";
                C.printSet();
                break;

            case 19:
                if (A == B) std::cout << "Sets are equal" << std::endl;
                else std::cout << "Sets are not equal" << std::endl;
                break;
            
            case 20:
                std::cout << "Size A: " << A.getSize() << ", Size B: " << B.getSize() << std::endl;
                break;

            case 0:
                std::cout << "Exiting program. Goodbye!" << std::endl;
                break;
                
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }
    } while (choice != 0);
}

void printTypes() {
    std::cout << "Please choose type of HashSet:" << std::endl;
    std::cout << "1. Int" << std::endl;
    std::cout << "2. Double" << std::endl;
    std::cout << "3. Complex" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

int main() {
    int choice_type;

    do {
        printTypes();
        choice_type = readInt("Your choice: ");

        switch (choice_type) {
            case 1:
                intMenu();
                break;
            case 2:
                doubleMenu();
                break;
            
            case 3:
                complexMenu();
                break;

            case 0:
                std::cout << "Exiting program. Goodbye!" << std::endl;
                break;

            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }
    } while (choice_type != 0);

    return 0;
}