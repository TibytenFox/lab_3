#include <iostream>
#include <string>
#include <cstring>
#include "Utilities.hpp"
#include "src/Queue.hpp"
#include "src/Complex.hpp"
#include "src/String.hpp"
#include "src/Function.hpp"
#include "src/Person.hpp"
#include "src/Output.hpp"
#include "sequence/Exceptions.hpp"

template<typename T> T readValue(const std::string& prompt);

template<> int readValue<int>(const std::string& prompt) { return Utilities::readInt(prompt); }
template<> double readValue<double>(const std::string& prompt) { return Utilities::readDouble(prompt); }
template<> Complex readValue<Complex>(const std::string& prompt) { return Utilities::readComplex(prompt); }
template<> StringData readValue<StringData>(const std::string& prompt) { return Utilities::readString(prompt); }
template<> FunctionData readValue<FunctionData>(const std::string& prompt) { return Utilities::readFunction(prompt); }
template<> Student readValue<Student>(const std::string& prompt) { return Utilities::readStudent(prompt); }

int int_sum(int a, int b) { return a + b; }
double double_sum(double a, double b) { return a + b; }
Complex complex_sum(Complex a, Complex b) { return a + b; }
StringData string_concat(StringData a, StringData b) {
    std::string res = std::string(b.c_str()) + std::string(a.c_str());
    return StringData(res.c_str());
}
FunctionData func_combine(FunctionData a, FunctionData b) { return a; }
Student student_max_grade(Student a, Student b) { return (a.GetAverageGrade() > b.GetAverageGrade()) ? a : b; }

template<typename T> T getInitialVal();
template<> int getInitialVal<int>() { return 0; }
template<> double getInitialVal<double>() { return 0.0; }
template<> Complex getInitialVal<Complex>() { return Complex(0.0, 0.0); }
template<> StringData getInitialVal<StringData>() { return StringData(""); }
template<> FunctionData getInitialVal<FunctionData>() { return FunctionData(); }
template<> Student getInitialVal<Student>() { return Student(); }

StringData append_excl(StringData s) { return StringData((std::string(s.c_str()) + "!").c_str()); }
bool len_greater_3(StringData s) { return std::strlen(s.c_str()) > 3; }

FunctionData func_map_ident(FunctionData f) { return f; }
bool is_square_func(FunctionData f) { return f.getPointer() == &Utilities::s_sqr; }

Student student_map_bonus(Student s) { 
    return Student(s.GetID(), s.GetFirstName().c_str(), s.GetLastName().c_str(), s.GetGroupName().c_str(), s.GetAverageGrade() + 0.1); 
}
bool is_excellent(Student s) { return s.GetAverageGrade() >= 4.5; }

int int_sqr(int x) { return x * x; }
bool int_gt5(int x) { return x > 5; }
double double_sqr(double x) { return x * x; }
bool double_gt5(double x) { return x > 5; }
Complex complex_sqr(Complex x) { return Complex(x.real * x.real - x.imaginary * x.imaginary, 2 * x.real * x.imaginary); }
bool complex_is_real(Complex x) { return x.imaginary == 0; }

void printQueueMenu() {
    std::cout << "\n===== Queue Tester (A + B) =====" << std::endl;
    std::cout << " 1. Enqueue в очередь А" << std::endl;
    std::cout << " 2. Dequeue из очереди А" << std::endl;
    std::cout << " 3. Peek (Посмотреть начало А)" << std::endl;
    std::cout << " 4. Применить map для А" << std::endl;
    std::cout << " 5. Применить where для А" << std::endl;
    std::cout << " 6. Применить reduce для А" << std::endl;
    std::cout << " 7. Вывести очередь А" << std::endl;
    std::cout << " 8. Enqueue в очередь B" << std::endl;
    std::cout << " 9. Dequeue из очереди B" << std::endl;
    std::cout << "10. Peek (Посмотреть начало B)" << std::endl;
    std::cout << "11. Вывести очередь B" << std::endl;
    std::cout << "12. Показать конкатенацию А + B" << std::endl;
    std::cout << "13. Извлечь подпоследовательность (срез) из А" << std::endl;
    std::cout << "14. Поиск вхождения подпоследовательности B в А" << std::endl;
    std::cout << "15. Проверить равенство А == B" << std::endl;
    std::cout << "16. Показать длины очередей" << std::endl;
    std::cout << " 0. Назад" << std::endl;
}

template <typename T, T(*MapFunc)(T), bool(*WhereFunc)(T), T(*ReduceFunc)(T, T)>
void queueTypeMenu() {
    Queue<T> A, B, C;
    int choice;
    T value;

    do {
        printQueueMenu();
        choice = Utilities::readInt("Выберите пункт: ");

        switch (choice) {
            case 1: { 
                try {
                    value = readValue<T>("Элемент для добавления в А: ");
                    A.Enqueue(value);
                    std::cout << "Успешно добавлен." << std::endl;
                } catch (const Exception& e) { std::cout << "Ошибка: " << e.GetMessage() << std::endl; }
                break;
            }
            case 2: { 
                try {
                    T popped = A.Dequeue();
                    std::cout << "Извлечен элемент: " << popped << std::endl;
                } catch (const Exception& e) { std::cout << "Ошибка: " << e.GetMessage() << std::endl; }
                break;
            }
            case 3: { 
                try {
                    std::cout << "В начале А находится: " << A.Peek() << std::endl;
                } catch (const Exception& e) { std::cout << "Ошибка: " << e.GetMessage() << std::endl; }
                break;
            }
            case 4: { 
                try {
                    C = A.map(MapFunc);
                    std::cout << "Результат map: "; PrintQueue(C);
                } catch (const Exception& e) { std::cout << "Ошибка: " << e.GetMessage() << std::endl; }
                break;
            }
            case 5: { 
                try {
                    C = A.where(WhereFunc);
                    std::cout << "Результат where: "; PrintQueue(C);
                } catch (const Exception& e) { std::cout << "Ошибка: " << e.GetMessage() << std::endl; }
                break;
            }
            case 6: { 
                try {
                    // Используем функцию-фабрику вместо параметра шаблона
                    T res = A.reduce(ReduceFunc, getInitialVal<T>());
                    std::cout << "Результат свёртки reduce: " << res << std::endl;
                } catch (const Exception& e) { std::cout << "Ошибка: " << e.GetMessage() << std::endl; }
                break;
            }
            case 7: PrintQueue(A); break;
            case 8: { 
                try {
                    value = readValue<T>("Элемент для добавления в B: ");
                    B.Enqueue(value);
                } catch (const Exception& e) { std::cout << "Ошибка: " << e.GetMessage() << std::endl; }
                break;
            }
            case 9: { 
                try {
                    std::cout << "Извлечен элемент: " << B.Dequeue() << std::endl;
                } catch (const Exception& e) { std::cout << "Ошибка: " << e.GetMessage() << std::endl; }
                break;
            }
            case 10: { 
                try { std::cout << "В начале B находится: " << B.Peek() << std::endl; } 
                catch (const Exception& e) { std::cout << "Ошибка: " << e.GetMessage() << std::endl; }
                break;
            }
            case 11: PrintQueue(B); break;
            case 12: { 
                C = A.Concat(B);
                std::cout << "А + B: "; PrintQueue(C);
                break;
            }
            case 13: { 
                int start = Utilities::readInt("Начальный индекс: ");
                int end = Utilities::readInt("Конечный индекс: ");
                try {
                    C = A.GetSubsequence(start, end);
                    std::cout << "Срез очереди А: "; PrintQueue(C);
                } catch (const Exception& e) { std::cout << "Ошибка: " << e.GetMessage() << std::endl; }
                break;
            }
            case 14: { 
                int pos = A.FindSubsequence(B);
                if (pos != -1) std::cout << "Очередь B входит в А начиная с индекса: " << pos << std::endl;
                else std::cout << "Вхождение не найдено." << std::endl;
                break;
            }
            case 15: { 
                if (A == B) std::cout << "Очереди равны." << std::endl;
                else std::cout << "Очереди не равны." << std::endl;
                break;
            }
            case 16:
                std::cout << "Длина А: " << A.GetLength() << ", Длина B: " << B.GetLength() << std::endl;
                break;
            case 0: break;
        }
    } while (choice != 0);
}

int main() {
    int typeChoice;
    do {
        std::cout << "\nВыберите тип данных Очереди:" << std::endl;
        std::cout << "1. Целые числа (Int)" << std::endl;
        std::cout << "2. Вещественные числа (Double)" << std::endl;
        std::cout << "3. Комплексные числа (Complex)" << std::endl;
        std::cout << "4. Строки (StringData)" << std::endl;
        std::cout << "5. Указатели на Функции (FunctionData)" << std::endl;
        std::cout << "6. Студенты (Student)" << std::endl;
        std::cout << "0. Выход" << std::endl;
        typeChoice = Utilities::readInt("Ваш выбор: ");

        switch (typeChoice) {
            case 1: queueTypeMenu<int, int_sqr, int_gt5, int_sum>(); break;
            case 2: queueTypeMenu<double, double_sqr, double_gt5, double_sum>(); break;
            case 3: queueTypeMenu<Complex, complex_sqr, complex_is_real, complex_sum>(); break;
            case 4: queueTypeMenu<StringData, append_excl, len_greater_3, string_concat>(); break;
            case 5: queueTypeMenu<FunctionData, func_map_ident, is_square_func, func_combine>(); break;
            case 6: queueTypeMenu<Student, student_map_bonus, is_excellent, student_max_grade>(); break;
            case 0: std::cout << "Выход из тестера Очереди." << std::endl; break;
            default: std::cout << "Неверный пункт меню." << std::endl; break;
        }
    } while (typeChoice != 0);
    return 0;
}