#ifndef COMPLEX_HPP
#define COMPLEX_HPP

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

};

namespace std {
    template <>
    struct hash<Complex> {
        size_t operator()(const Complex& c) const {
            return std::hash<double>()(c.real) ^ (std::hash<double>()(c.imaginary) << 1);
        }
    };
}

#endif // COMPLEX_HPP