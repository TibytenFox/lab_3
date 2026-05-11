#ifndef TYPES_HPP
#define TYPES_HPP

#include <stddef.h>
#include <functional>
#include <iostream>

struct Complex {
	double real;
	double imaginary;

	bool operator==(const Complex& other) const {
		return this->real == other.real && this->imaginary == other.imaginary;
	}

	friend std::ostream& operator<<(std::ostream& os, const Complex& c)  {
		if (c.imaginary >= 0) os << c.real << '+' << c.imaginary << 'i';
        else os << c.real << '-' << -c.imaginary << 'i'; 
        return os;
    }

};

struct ComplexHasher {
	size_t operator()(const Complex& c) const {
		return std::hash<double>()(c.real) ^ (std::hash<double>()(c.imaginary) << 1);
	}
};

using FuncPtr = void(*)(int);
struct FuncPtrHasher {
    size_t operator()(FuncPtr f) const {
        return std::hash<const void*>()(reinterpret_cast<const void*>(f));
    }
};

#endif // TYPES_HPP