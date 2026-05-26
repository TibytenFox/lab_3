#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <iostream>
#include "Queue.hpp"
#include "HashSet.hpp"
#include "Matrix.hpp"
#include "Complex.hpp"
#include "Function.hpp"
#include "Person.hpp"
#include "String.hpp"

inline std::ostream& operator<<(std::ostream &os, const Complex& c) {
	if (c.imaginary >= 0) os << c.real << '+' << c.imaginary << 'i';
	else os << c.real << '-' << -c.imaginary << 'i'; 
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const StringData& str) {
    return os << str.c_str();
}

inline std::ostream& operator<<(std::ostream& os, const FunctionData& fd) {
    return os << fd.getName().c_str() << "()";
}

inline std::ostream& operator<<(std::ostream& os, const Student& s) {
    return os << "[Студент] " << s.GetLastName() << " " << s.GetFirstName() 
              << ", Группа: " << s.GetGroupName() << ", Ср.балл: " << s.GetAverageGrade();
}

template <class T>
void PrintQueue(const Queue<T>& queue, std::ostream& os = std::cout) {
    os << "Queue [";
    IEnumerator<T>* en = queue.GetEnumerator();
    bool first = true;
    while (en->MoveNext()) {
        if (!first) os << " -> ";
        os << en->GetCurrent();
        first = false;
    }
    os << "]\n";
    delete en;
}

template <class Key, class Hash>
void PrintHashSet(const HashSet<Key, Hash>& set, std::ostream& os = std::cout) {
    os << "HashSet (Size: " << set.getSize() << ") { ";
    IEnumerator<Key>* en = set.GetEnumerator();
    bool first = true;
    while (en->MoveNext()) {
        if (!first) os << ", ";
        os << en->GetCurrent();
        first = false;
    }
    os << " }\n";
    delete en;
}

template <class T>
void PrintMatrix(const RectangularMatrix<T>& matrix, std::ostream& os = std::cout) {
    os << "Matrix (" << matrix.getRows() << "x" << matrix.getColumns() << "):\n";
    for (int i = 0; i < matrix.getRows(); ++i) {
        os << "  [ ";
        for (int j = 0; j < matrix.getColumns(); ++j) {
            os << matrix.get(i, j) << " ";
        }
        os << "]\n";
    }
}

inline Student ReadStudentFromConsole() {
    int series, number;
	char *firstName, *lastName, *group;
    double grade;

    std::cout << "Введите серию и номер паспорта: ";
    std::cin >> series >> number;
    std::cout << "Введите Имя: ";
    std::cin >> firstName;
    std::cout << "Введите Фамилию: ";
    std::cin >> lastName;
    std::cout << "Введите группу: ";
    std::cin >> group;
    std::cout << "Введите средний балл: ";
    std::cin >> grade;

    return Student(PersonID{series, number}, firstName, lastName, group, grade);
}

#endif // OUTPUT_HPP