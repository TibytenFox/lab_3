#include <iostream>
#include "sequence/Exceptions.hpp"
#include "src/Queue.hpp"
#include "src/Complex.hpp"
#include "src/String.hpp"
#include "src/Function.hpp"
#include "src/Person.hpp"

#define TEST(name) void name(); \
    struct Register_##name { Register_##name() { register_test(name); std::cout << #name << '\n'; } } reg_##name; \
    void name()

static int tests_passed = 0;
static int tests_failed = 0;

void register_test(void (*f)()) {
    try {
        f();
        ++tests_passed;
        std::cout << "[PASS]\t";
    } catch (const Exception &e) {
        ++tests_failed;
        std::cout << "[FAIL] " << e.GetMessage() << "\t";
    } catch (...) {
        ++tests_failed;
        std::cout << "[FAIL] Unknown exception\t";
    }
}

#define ASSERT_EQ(x, y) \
    if ((x) != (y)) throw RunTimeError("ASSERT_EQ failed: " #x " != " #y)

#define ASSERT_THROWS(expr, exc) \
    do { \
        bool caught = false; \
        try { expr; } \
        catch (const exc&) { caught = true; } \
        catch (...) {} \
        if (!caught) throw RunTimeError("ASSERT_THROWS failed: " #expr); \
    } while(0)

#define ASSERT_NO_THROW(expr) \
    do { \
        try { expr; } \
        catch (...) { throw RunTimeError("ASSERT_NO_THROW failed: " #expr); } \
    } while(0)

// ----------- TESTS -----------

TEST(Queue_DefaultConstructor) {
    Queue<int> q;
    ASSERT_EQ(q.GetLength(), 0);
    ASSERT_EQ(q.IsEmpty(), true);
    
    ASSERT_THROWS(q.Dequeue(), EmptyCollectionError);
    ASSERT_THROWS(q.Peek(), EmptyCollectionError);
}

TEST(Queue_ConstructorFromArray) {
    int arr[] = {1, 2, 3};
    Queue<int> q(arr, 3);
    
    ASSERT_EQ(q.GetLength(), 3);
    ASSERT_EQ(q.IsEmpty(), false);
    ASSERT_EQ(q.Peek(), 1);
}

TEST(Queue_CopyMoveSemantics) {
    int arr[] = {10, 20, 30};
    Queue<int> q1(arr, 3);
    
    // Copy constructor
    Queue<int> q2(q1);
    ASSERT_EQ(q2.GetLength(), 3);
    ASSERT_EQ(q2.Dequeue(), 10);
    ASSERT_EQ(q1.Peek(), 10); // q1 should remain unchanged

    // Copy assignment
    Queue<int> q3;
    q3 = q1;
    ASSERT_EQ(q3.GetLength(), 3);
    ASSERT_EQ(q3.Peek(), 10);

    // Move constructor
    Queue<int> q4(std::move(q1));
    ASSERT_EQ(q4.GetLength(), 3);
    ASSERT_EQ(q1.GetLength(), 0); 
    
    // Move assignment
    Queue<int> q5;
    q5 = std::move(q4);
    ASSERT_EQ(q5.GetLength(), 3);
    ASSERT_EQ(q4.GetLength(), 0);
}

TEST(Queue_EnqueueDequeuePeek) {
    Queue<int> q;
    q.Enqueue(100);
    q.Enqueue(200);
    q.Enqueue(300);
    
    ASSERT_EQ(q.GetLength(), 3);
    ASSERT_EQ(q.Peek(), 100);
    
    ASSERT_EQ(q.Dequeue(), 100);
    ASSERT_EQ(q.GetLength(), 2);
    ASSERT_EQ(q.Peek(), 200);
    
    ASSERT_EQ(q.Dequeue(), 200);
    ASSERT_EQ(q.Dequeue(), 300);
    
    ASSERT_EQ(q.IsEmpty(), true);
    ASSERT_THROWS(q.Dequeue(), EmptyCollectionError);
}

TEST(Queue_MapWhereReduce) {
    int arr[] = {1, 2, 3, 4, 5};
    Queue<int> q(arr, 5);

    // Map: умножаем каждый элемент на 2
    Queue<int> mapped = q.map([](int x) { return x * 2; });
    ASSERT_EQ(mapped.GetLength(), 5);
    ASSERT_EQ(mapped.Dequeue(), 2);
    ASSERT_EQ(mapped.Dequeue(), 4);

    // Where: оставляем только четные числа
    Queue<int> filtered = q.where([](int x) { return x % 2 == 0; });
    ASSERT_EQ(filtered.GetLength(), 2);
    ASSERT_EQ(filtered.Dequeue(), 2);
    ASSERT_EQ(filtered.Dequeue(), 4);

    // Reduce: сумма элементов (сигнатура: T func(T val, T acc))
    int sum = q.reduce([](int val, int acc) { return acc + val; }, 0);
    ASSERT_EQ(sum, 15);
}

TEST(Queue_Concat) {
    int arr1[] = {1, 2};
    int arr2[] = {3, 4, 5};
    Queue<int> q1(arr1, 2);
    Queue<int> q2(arr2, 3);
    
    Queue<int> q3 = q1.Concat(q2);
    ASSERT_EQ(q3.GetLength(), 5);
    
    ASSERT_EQ(q3.Dequeue(), 1);
    ASSERT_EQ(q3.Dequeue(), 2);
    ASSERT_EQ(q3.Dequeue(), 3);
    ASSERT_EQ(q3.Dequeue(), 4);
    ASSERT_EQ(q3.Dequeue(), 5);
}

TEST(Queue_GetSubsequence) {
    int arr[] = {10, 20, 30, 40, 50};
    Queue<int> q(arr, 5);
    
    // Получаем подпоследовательность со 2-го (включительно) по 4-й (не включительно)
    Queue<int> sub = q.GetSubsequence(1, 4); 
    ASSERT_EQ(sub.GetLength(), 3);
    ASSERT_EQ(sub.Dequeue(), 20);
    ASSERT_EQ(sub.Dequeue(), 30);
    ASSERT_EQ(sub.Dequeue(), 40);
    
    // Проверка выхода за границы
    ASSERT_THROWS(q.GetSubsequence(-1, 2), IndexOutOfRange);
    ASSERT_THROWS(q.GetSubsequence(0, 10), IndexOutOfRange);
    ASSERT_THROWS(q.GetSubsequence(3, 2), IndexOutOfRange);
}

TEST(Queue_FindSubsequence) {
    int arr[] = {1, 2, 3, 4, 5, 6};
    int sub1_arr[] = {3, 4};
    int sub2_arr[] = {5, 6, 7};
    
    Queue<int> q(arr, 6);
    Queue<int> sub1(sub1_arr, 2);
    Queue<int> sub2(sub2_arr, 3);
    Queue<int> empty_sub;
    
    ASSERT_EQ(q.FindSubsequence(sub1), 2); // Найдено по индексу 2
    ASSERT_EQ(q.FindSubsequence(sub2), -1); // Не найдено
    ASSERT_EQ(q.FindSubsequence(empty_sub), 0); // Пустая подпоследовательность найдена на 0-м индексе
}

TEST(Queue_Equality) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {1, 2, 3};
    int arr3[] = {1, 2};
    
    Queue<int> q1(arr1, 3);
    Queue<int> q2(arr2, 3);
    Queue<int> q3(arr3, 2);
    
    ASSERT_EQ(q1 == q2, true);
    ASSERT_EQ(q1 != q2, false);
    
    ASSERT_EQ(q1 == q3, false);
    ASSERT_EQ(q1 != q3, true);
}

TEST(Queue_Enumeration) {
    int arr[] = {7, 14, 21};
    Queue<int> q(arr, 3);
    
    IEnumerator<int>* en = q.GetEnumerator();
    int sum = 0;
    while (en->MoveNext()) {
        sum += en->GetCurrent();
    }
    delete en;
    
    ASSERT_EQ(sum, 42);
}

TEST(Queue_ComplexType) {
    Queue<Complex> q;
    q.Enqueue(Complex(1.0, 2.0));
    q.Enqueue(Complex(3.0, 4.0));
    
    ASSERT_EQ(q.GetLength(), 2);
    
    Complex c = q.Dequeue();
    ASSERT_EQ(c == Complex(1.0, 2.0), true);
    
    c = q.Peek();
    ASSERT_EQ(c == Complex(3.0, 4.0), true);
}

TEST(Queue_StringDataType) {
    Queue<StringData> q;
    q.Enqueue(StringData("First"));
    q.Enqueue(StringData("Second"));
    
    ASSERT_EQ(q.GetLength(), 2);
    ASSERT_EQ(q.Peek() == StringData("First"), true);
    
    StringData s = q.Dequeue();
    ASSERT_EQ(s == StringData("First"), true);
    ASSERT_EQ(q.GetLength(), 1);
    
    // Проверка глубокого копирования через Concat
    Queue<StringData> q2;
    q2.Enqueue(StringData("Third"));
    
    Queue<StringData> combined = q.Concat(q2);
    ASSERT_EQ(combined.GetLength(), 2);
    ASSERT_EQ(combined.Dequeue() == StringData("Second"), true);
    ASSERT_EQ(combined.Dequeue() == StringData("Third"), true);
}

int add_one(int x) { return x + 1; }
int minus_one(int x) { return x - 1; }

TEST(Queue_FunctionDataType) {
    Queue<FunctionData> q;
    FunctionData f1(add_one, "add_one");
    FunctionData f2(minus_one, "minus_one");
    
    q.Enqueue(f1);
    q.Enqueue(f2);
    
    ASSERT_EQ(q.GetLength(), 2);
    
    FunctionData dq_f1 = q.Dequeue();
    ASSERT_EQ(dq_f1(10), 11);
    ASSERT_EQ(dq_f1.getName() == StringData("add_one"), true);
    
    FunctionData dq_f2 = q.Dequeue();
    ASSERT_EQ(dq_f2(10), 9);
    ASSERT_EQ(dq_f2.getName() == StringData("minus_one"), true);
}

TEST(Queue_PersonHierarchyTypes) {
    // Очередь студентов
    Queue<Student> student_q;
    PersonID id1{1, 100};
    PersonID id2{1, 101};
    
    Student s1(id1, "Anna", "Ivanova", "A-1", 4.9);
    Student s2(id2, "Boris", "Petrov", "A-1", 4.2);
    
    student_q.Enqueue(s1);
    student_q.Enqueue(s2);
    
    ASSERT_EQ(student_q.GetLength(), 2);
    
    Student popped_s = student_q.Dequeue();
    ASSERT_EQ(popped_s == s1, true); // Проверка через operator== у Person (сравнивает ID)
    ASSERT_EQ(popped_s.GetAverageGrade(), 4.9);
    ASSERT_EQ(student_q.GetLength(), 1);
    
    // Очередь преподавателей
    Queue<Teacher> teacher_q;
    PersonID id3{2, 500};
    Teacher t1(id3, "Vladimir", "Sidorov", "Math", "PhD");
    
    teacher_q.Enqueue(t1);
    ASSERT_EQ(teacher_q.GetLength(), 1);
    
    Teacher popped_t = teacher_q.Peek();
    ASSERT_EQ(popped_t == t1, true);
    ASSERT_EQ(popped_t.GetDepartment() == StringData("Math"), true);
}

int main() {
    std::cout << "\nResults: passed " << tests_passed << ", failed " << tests_failed << "\n";
    return tests_failed == 0 ? 0 : 1;
}