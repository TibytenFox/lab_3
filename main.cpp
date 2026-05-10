#include <iostream>
#include <string>
#include "src/HashSet.hpp"


void printMenu() {
    std::cout << "\n===== HashSet Tester =====" << std::endl;
    std::cout << "1. Add element" << std::endl;
    std::cout << "2. Remove element" << std::endl;
    std::cout << "3. Check if contains element" << std::endl;
    std::cout << "4. Get size" << std::endl;
    std::cout << "5. Print set" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose an option: ";
}

int main() {
    HashSet<int> set;
    int choice, value;
    
    do {
        printMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                std::cout << "Enter integer value to add: ";
                std::cin >> value;
                if (set.add(value)) {
                    std::cout << value << " added successfully." << std::endl;
                } else {
                    std::cout << value << " already exists in the set." << std::endl;
                }
                break;
                
            case 2:
                std::cout << "Enter integer value to remove: ";
                std::cin >> value;
                if (set.remove(value)) {
                    std::cout << value << " removed successfully." << std::endl;
                } else {
                    std::cout << value << " not found in the set." << std::endl;
                }
                break;
                
            case 3:
                std::cout << "Enter integer value to search: ";
                std::cin >> value;
                if (set.contains(value)) {
                    std::cout << value << " is in the set." << std::endl;
                } else {
                    std::cout << value << " is not in the set." << std::endl;
                }
                break;
                
            case 4:
                std::cout << "Current set size: " << set.getSize() << std::endl;
                break;
                
            case 5:
                set.printSet();
                break;
                
            case 0:
                std::cout << "Exiting program. Goodbye!" << std::endl;
                break;
                
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }
    } while (choice != 0);
    
    return 0;
}