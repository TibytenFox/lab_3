#include <iostream>
#include "sequence/Exceptions.hpp"
#include "src/HashSet.hpp"
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

TEST(HashSet_DefaultConstructor) {
    HashSet<int> set;
    ASSERT_EQ(set.getSize(), 0);
    ASSERT_EQ(set.contains(0), false);
}

TEST(HashSet_CapacityConstructor) {
    HashSet<int> set(10);
    ASSERT_EQ(set.getSize(), 0);
    ASSERT_EQ(set.contains(0), false);

    ASSERT_THROWS(HashSet<int> invalid_set(-10), IndexOutOfRange);
    ASSERT_NO_THROW(HashSet<int> set3(0));
}

TEST(HashSet_ConstructorFromArray) {
    int arr1[] = {1, 2, 3};
    HashSet<int> set1(arr1, 3);
    ASSERT_EQ(set1.getSize(), 3);
    ASSERT_EQ(set1.contains(0), false);
    ASSERT_EQ(set1.contains(1), true);
    ASSERT_EQ(set1.contains(2), true);
    ASSERT_EQ(set1.contains(3), true);

    int arr2[] = {1, 1, 2};
    HashSet<int> set2(arr2, 3);
    ASSERT_EQ(set2.getSize(), 2);
    ASSERT_EQ(set2.contains(0), false);
    ASSERT_EQ(set2.contains(1), true);
    ASSERT_EQ(set2.contains(2), true);
}

TEST(HashSet_CopyAssignment) {
    int arr1[] = {1, 2, 3};
    HashSet<int> set1(arr1, 3);
    HashSet<int> set2;
    set2 = set1;
    ASSERT_EQ(set2.getSize(), 3);
    set2.remove(1);
    ASSERT_EQ(set1.contains(1), true);
}

TEST(HashSet_MoveConstructor) {
    int arr[] = {1, 2, 3};
    HashSet<int> set1(arr, 3);
    HashSet<int> set2(std::move(set1));
    ASSERT_EQ(set2.getSize(), 3);
    ASSERT_EQ(set1.getSize(), 0); 
}

TEST(HashSet_AddRemoveContains) {
    HashSet<int> set;
    ASSERT_EQ(set.add(10), true);
    ASSERT_EQ(set.getSize(), 1);
    ASSERT_EQ(set.contains(10), true);
    ASSERT_EQ(set.contains(20), false);
    
    ASSERT_EQ(set.add(10), false);
    ASSERT_EQ(set.getSize(), 1);

    ASSERT_EQ(set.add(20), true);
    ASSERT_EQ(set.getSize(), 2);
    ASSERT_EQ(set.contains(20), true);

    ASSERT_EQ(set.remove(100), false);
    ASSERT_EQ(set.remove(10), true);
    ASSERT_EQ(set.contains(10), false);
    ASSERT_EQ(set.contains(20), true);
}

TEST(HashSet_CopyConstructor) {
    int arr[] = {10, 20, 30};
    HashSet<int> set1(arr, 3);

    HashSet<int> set2(set1);
    ASSERT_EQ(set2.getSize(), 3);
    ASSERT_EQ(set2.contains(20), true);
    ASSERT_EQ(set2.contains(21), false);
    ASSERT_EQ(set2.remove(20), true);
    ASSERT_EQ(set1.contains(20), true);
}

TEST(HashSet_MapWhere) {
    int arr[] = {10, 25, 32, 43};
    HashSet<int> set(arr, 4);

    HashSet<int> where_res = set.where([](int x) { return x % 2 == 0; });
    ASSERT_EQ(where_res.getSize(), 2);
    ASSERT_EQ(where_res.contains(10), true);
    ASSERT_EQ(where_res.contains(25), false);
    ASSERT_EQ(where_res.contains(32), true);
    ASSERT_EQ(where_res.contains(43), false);

    HashSet<int> map_res = set.map([](int x) { return x * 2; });
    ASSERT_EQ(map_res.getSize(), 4);
    ASSERT_EQ(map_res.contains(20), true);
    ASSERT_EQ(map_res.contains(50), true);
    ASSERT_EQ(map_res.contains(64), true);
    ASSERT_EQ(map_res.contains(86), true);
    ASSERT_EQ(map_res.contains(10), false);

    int arr1[] = {1, 2, 3};
    HashSet<int> set_dup(arr1, 3);
    HashSet<int> mapped_dup = set_dup.map([](int x) { return x % 2; }); // {0, 1}
    ASSERT_EQ(mapped_dup.getSize(), 2);
    ASSERT_EQ(mapped_dup.contains(0), true);
    ASSERT_EQ(mapped_dup.contains(1), true);
}

TEST(HashSet_IncludesSubset) {
    int arr1[] = {10, 20, 30};
    int arr2[] = {20, 30};
    int arr3[] = {30, 40};
    int arr4[] = {};
    HashSet<int> set1(arr1, 3);
    HashSet<int> set2(arr2, 2);
    HashSet<int> set3(arr3, 3);
    HashSet<int> set4(arr4, 0);

    ASSERT_EQ(set1.includesSubset(set2), true);
    ASSERT_EQ(set1.includesSubset(set3), false);
    ASSERT_EQ(set1.includesSubset(set4), true);
    ASSERT_EQ(set3.includesSubset(set1), false);
}

TEST(HashSet_OperatorUnion) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {2, 3, 4};
    int arr3[] = {5, 6, 7};
    HashSet<int> set1(arr1, 3);
    HashSet<int> set2(arr2, 3);
    HashSet<int> set3(arr3, 3);
    HashSet<int> set_res;

    HashSet<int> old_set1 = set1;
    HashSet<int> old_set2 = set2;

    set_res = set1 | set2;
    ASSERT_EQ(set_res.getSize(), 4);
    ASSERT_EQ(set_res.contains(1), true);
    ASSERT_EQ(set_res.contains(2), true);
    ASSERT_EQ(set_res.contains(3), true);
    ASSERT_EQ(set_res.contains(4), true);

    ASSERT_EQ(set1 == old_set1, true);
    ASSERT_EQ(set2 == old_set2, true);

    set_res = set1 | set3;
    ASSERT_EQ(set_res.getSize(), 6);
    ASSERT_EQ(set_res.contains(1), true);
    ASSERT_EQ(set_res.contains(2), true);
    ASSERT_EQ(set_res.contains(3), true);
    ASSERT_EQ(set_res.contains(5), true);
    ASSERT_EQ(set_res.contains(6), true);
    ASSERT_EQ(set_res.contains(7), true);

    set_res = set1 | set1;
    ASSERT_EQ(set_res.getSize(), 3);
    ASSERT_EQ(set_res.contains(1), true);
    ASSERT_EQ(set_res.contains(2), true);
    ASSERT_EQ(set_res.contains(3), true); 
    ASSERT_EQ(set_res.contains(4), false);
}

TEST(HashSet_OperatorIntersection) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {2, 3, 4};
    int arr3[] = {5, 6, 7};
    HashSet<int> set1(arr1, 3);
    HashSet<int> set2(arr2, 3);
    HashSet<int> set3(arr3, 3);
    HashSet<int> set_res;

    set_res = set1 & set2;
    ASSERT_EQ(set_res.getSize(), 2);
    ASSERT_EQ(set_res.contains(1), false);
    ASSERT_EQ(set_res.contains(2), true);
    ASSERT_EQ(set_res.contains(3), true);
    ASSERT_EQ(set_res.contains(4), false);

    set_res = set1 & set3;
    ASSERT_EQ(set_res.getSize(), 0);
}

TEST(HashSet_OperatorDifference) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {2, 3, 4};
    int arr3[] = {5, 6, 7};
    HashSet<int> set1(arr1, 3);
    HashSet<int> set2(arr2, 3);
    HashSet<int> set3(arr3, 3);
    HashSet<int> set_res;

    set_res = set1 / set2;
    ASSERT_EQ(set_res.getSize(), 1);
    ASSERT_EQ(set_res.contains(1), true);

    set_res = set1 / set3;
    ASSERT_EQ(set_res.getSize(), 3);
    ASSERT_EQ(set_res.contains(1), true);
    ASSERT_EQ(set_res.contains(2), true);
    ASSERT_EQ(set_res.contains(3), true);
    ASSERT_EQ(set_res.contains(5), false);
    ASSERT_EQ(set_res.contains(6), false);
    ASSERT_EQ(set_res.contains(7), false);

    set_res = set2 / set1;
    ASSERT_EQ(set_res.getSize(), 1);
    ASSERT_EQ(set_res.contains(4), true);
}

TEST(HashSet_Equals) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {3, 2, 1};
    HashSet<int> set1(arr1, 3);
    HashSet<int> set2(arr2, 3);

    ASSERT_EQ(set1 == set2, true);
    ASSERT_EQ(set1 != set2, false);
}

TEST(HashSet_OperationsWithEmpty) {
    HashSet<int> empty;
    int arr[] = {1, 2};
    HashSet<int> nonEmpty(arr, 2);
    ASSERT_EQ((empty | nonEmpty).getSize(), 2);
    ASSERT_EQ((empty & nonEmpty).getSize(), 0);
    ASSERT_EQ((empty / nonEmpty).getSize(), 0);
}

TEST(HashSet_Enumeration) {
    int arr[] = {5, 10, 15};
    HashSet<int> set(arr, 3);
    IEnumerator<int>* enumerator = set.GetEnumerator();
    int count = 0;
    while (enumerator->MoveNext()) {
        int val = enumerator->GetCurrent();
        ASSERT_EQ(set.contains(val), true);
        count++;
    }
    ASSERT_EQ(count, set.getSize());
    delete enumerator;
}

TEST(HashSet_ComplexType) {
    HashSet<Complex> set;

    ASSERT_EQ(set.add(Complex(1, 2)), true);
    ASSERT_EQ(set.getSize(), 1);
    ASSERT_EQ(set.contains(Complex(1, 2)), true);

    HashSet<Complex> another_set;
    ASSERT_EQ(another_set.add(Complex(1, 2)), true);
    ASSERT_EQ(set == another_set, true);
} 

int duplicate(int x) { return x * 2; }
int square(int x) { return x * x; }

TEST(HashSet_StringDataType) {
    HashSet<StringData> set;

    // Проверка базового добавления и поиска
    ASSERT_EQ(set.add(StringData("Hello")), true);
    ASSERT_EQ(set.add(StringData("World")), true);
    ASSERT_EQ(set.add(StringData("Hello")), false); // Дубликат
    ASSERT_EQ(set.getSize(), 2);

    ASSERT_EQ(set.contains(StringData("Hello")), true);
    ASSERT_EQ(set.contains(StringData("C++")), false);

    // Проверка удаления
    ASSERT_EQ(set.remove(StringData("World")), true);
    ASSERT_EQ(set.getSize(), 1);
    ASSERT_EQ(set.contains(StringData("World")), false);

    // Проверка операций множеств на строках
    StringData arr[] = {StringData("A"), StringData("B")};
    HashSet<StringData> set2(arr, 2);
    
    HashSet<StringData> union_set = set | set2; // {"Hello"} | {"A", "B"}
    ASSERT_EQ(union_set.getSize(), 3);
    ASSERT_EQ(union_set.contains(StringData("A")), true);
    ASSERT_EQ(union_set.contains(StringData("Hello")), true);
}

TEST(HashSet_FunctionDataType) {
    HashSet<FunctionData> set;

    FunctionData f1(duplicate, "duplicate");
    FunctionData f2(square, "square");
    FunctionData f3(nullptr, "nullptr_func");

    // Добавление
    ASSERT_EQ(set.add(f1), true);
    ASSERT_EQ(set.add(f2), true);
    ASSERT_EQ(set.add(f1), false); // Повторное добавление указателя
    ASSERT_EQ(set.getSize(), 2);

    // Проверка вызова функций из множества через поиск
    ASSERT_EQ(set.contains(f1), true);
    ASSERT_EQ(set.contains(f3), false);

    // Перебор и вызов
    IEnumerator<FunctionData>* en = set.GetEnumerator();
    while (en->MoveNext()) {
        FunctionData f = en->GetCurrent();
        if (f.getName() == StringData("duplicate")) {
            ASSERT_EQ(f(5), 10);
        } else if (f.getName() == StringData("square")) {
            ASSERT_EQ(f(5), 25);
        }
    }
    delete en;
}

TEST(HashSet_PersonHierarchyTypes) {
    // 1. Тестируем Student
    HashSet<Student> student_set;
    PersonID id1{100, 12345};
    PersonID id2{100, 54321};

    Student s1(id1, "Ivan", "Ivanov", "B23-501", 4.75);
    Student s2(id2, "Petr", "Petrov", "B23-501", 3.8);
    Student s3(id1, "Ivan_Duplicate_ID", "Ivanov", "M23-501", 5.0); 

    ASSERT_EQ(student_set.add(s1), true);
    ASSERT_EQ(student_set.add(s2), true);
    
    // s3 имеет такой же PersonID (id1), как и s1. 
    // Поскольку равенство и хэш Person/Student зависят только от ID, это дубликат.
    ASSERT_EQ(student_set.add(s3), false); 
    ASSERT_EQ(student_set.getSize(), 2);
    ASSERT_EQ(student_set.contains(s2), true);

    // 2. Тестируем Teacher
    HashSet<Teacher> teacher_set;
    PersonID id3{200, 11111};
    PersonID id4{200, 22222};

    Teacher t1(id3, "Sidor", "Sidorov", "Department of Computer Science", "Professor");
    Teacher t2(id4, "Alex", "Alexandrov", "Department of Physics", "Docent");

    ASSERT_EQ(teacher_set.add(t1), true);
    ASSERT_EQ(teacher_set.add(t2), true);
    ASSERT_EQ(teacher_set.getSize(), 2);
    ASSERT_EQ(teacher_set.contains(t1), true);

    // Удаление из коллекции преподавателей
    ASSERT_EQ(teacher_set.remove(t1), true);
    ASSERT_EQ(teacher_set.getSize(), 1);
    ASSERT_EQ(teacher_set.contains(t1), false);
}


int main() {
    std::cout << "\nResults: passed " << tests_passed << ", failed " << tests_failed << "\n";
    return tests_failed == 0 ? 0 : 1;
}