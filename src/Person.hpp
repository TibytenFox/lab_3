#ifndef PERSON_HPP
#define PERSON_HPP

#include "String.hpp"
#include <iostream>

struct PersonID {
    int series;
    int number;

    bool operator==(const PersonID& other) const {
        return series == other.series && number == other.number;
    }
};

class Person {
protected:
    PersonID id_;
    StringData firstName_;
    StringData lastName_;

public:
    Person() : id_{0, 0}, firstName_(""), lastName_("") {}
    Person(PersonID id, const char* firstName, const char* lastName)
        : id_(id), firstName_(firstName), lastName_(lastName) {}
    virtual ~Person() = default;

    PersonID GetID() const { return id_; }
    const StringData& GetFirstName() const { return firstName_; }
    const StringData& GetLastName() const { return lastName_; }

    bool operator==(const Person& other) const {
        return id_ == other.id_;
    }
	bool operator!=(const Person& other) const {return !(*this == other); }
};


class Student : public Person {
private:
    StringData groupName_;
    double averageGrade_;

public:
    Student() : Person(), groupName_(""), averageGrade_(0.0) {}
    Student(PersonID id, const char* firstName, const char* lastName, const char* group, double grade)
        : Person(id, firstName, lastName), groupName_(group), averageGrade_(grade) {}

    const StringData& GetGroupName() const { return groupName_; }
    double GetAverageGrade() const { return averageGrade_; }
};


class Teacher : public Person {
private:
    StringData department_;
    StringData academicDegree_;

public:
    Teacher() : Person(), department_(""), academicDegree_("") {}
    Teacher(PersonID id, const char* firstName, const char* lastName, const char* dept, const char* degree)
        : Person(id, firstName, lastName), department_(dept), academicDegree_(degree) {}

    const StringData& GetDepartment() const { return department_; }
    const StringData& GetAcademicDegree() const { return academicDegree_; }
};

namespace std {
    template <>
    struct hash<Student> {
        size_t operator()(const Student& s) const {
            return std::hash<int>()(s.GetID().series) ^ (std::hash<int>()(s.GetID().number) << 1);
        }
    };

    template <>
    struct hash<Teacher> {
        size_t operator()(const Teacher& t) const {
            return std::hash<int>()(t.GetID().series) ^ (std::hash<int>()(t.GetID().number) << 1);
        }
    };
}

#endif // PERSON_HPP