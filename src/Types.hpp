#ifndef TYPES_HPP
#define TYPES_HPP

#include <stddef.h>
#include <functional>
#include <iostream>

struct Complex {
	double real;
	double imaginary;

	Complex() : real(0), imaginary(0) {}

    Complex(double r, double i) : real(r), imaginary(i) {}

	bool operator==(const Complex& other) const {
		return this->real == other.real && this->imaginary == other.imaginary;
	}

	operator double() const {
		return this->real * this->real + this->imaginary * this->imaginary;
	}

	Complex& operator=(const Complex& other) {
		this->real = other.real;
		this->imaginary = other.imaginary;
		return *this;
	}

	Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imaginary + other.imaginary);
    }

    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imaginary - other.imaginary);
    }

    Complex operator*(const Complex& other) const {
        return Complex(real * other.real - imaginary * other.imaginary,
                       real * other.imaginary + imaginary * other.real);
    }

    Complex& operator+=(const Complex& other) {
        real += other.real;
        imaginary += other.imaginary;
        return *this;
    }

    Complex& operator-=(const Complex& other) {
        real -= other.real;
        imaginary -= other.imaginary;
        return *this;
    }

    Complex& operator*=(const Complex& other) {
        double new_real = real * other.real - imaginary * other.imaginary;
        double new_imag = real * other.imaginary + imaginary * other.real;
        real = new_real;
        imaginary = new_imag;
        return *this;
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