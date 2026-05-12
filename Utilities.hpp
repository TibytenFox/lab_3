#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include <string>
#include "src/Types.hpp"

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
}

#endif // UTILITIES_HPP