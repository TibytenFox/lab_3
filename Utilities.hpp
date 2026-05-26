#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include <string>
#include "src/Complex.hpp"
#include "src/Function.hpp"
#include "src/String.hpp"
#include "src/Person.hpp"

namespace Utilities {
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

	int s_inc(int x) { return x + 1; }
	int s_sqr(int x) { return x * x; }

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

	FunctionData readFunction(const std::string& prompt) {
		std::cout << prompt << "\n1. x + 1\n2. x * x\nChoose function: ";
		int choice = readInt("");
		if (choice == 1) return FunctionData(&s_inc, "increment");
		return FunctionData(&s_sqr, "square");
	}

	StringData readString(const std::string& prompt) {
		std::cout << prompt;
		std::string s; 
		std::cin >> s;
		return StringData(s.c_str());
	}

	Student readStudent(const std::string& prompt) {
		std::cout << prompt << "\n";
		int series = readInt("Серия паспорта: ");
		int number = readInt("Номер паспорта: ");
		std::cout << "Имя: "; std::string fName; std::cin >> fName;
		std::cout << "Фамилия: "; std::string lName; std::cin >> lName;
		std::cout << "Группа: "; std::string group; std::cin >> group;
		double grade = readDouble("Средний балл: ");
		return Student(PersonID{series, number}, fName.c_str(), lName.c_str(), group.c_str(), grade);
	}
}

#endif // UTILITIES_HPP