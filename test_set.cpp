#include <iostream>
#include "sequence/Exceptions.hpp"
#include "src/HashSet.hpp"
#include "src/Types.hpp"

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
    HashSet<Complex, ComplexHasher> set;

    ASSERT_EQ(set.add(Complex(1, 2)), true);
    ASSERT_EQ(set.getSize(), 1);
    ASSERT_EQ(set.contains(Complex(1, 2)), true);

    HashSet<Complex, ComplexHasher> another_set;
    ASSERT_EQ(another_set.add(Complex(1, 2)), true);
    ASSERT_EQ(set == another_set, true);
} 


int main() {
    std::cout << "\nResults: passed " << tests_passed << ", failed " << tests_failed << "\n";
    return tests_failed == 0 ? 0 : 1;
}