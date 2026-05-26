#ifndef STRING_HPP
#define STRING_HPP

#include <iostream>
#include <cstring>
#include <functional>

class StringData {
private:
    char* data_;
    size_t length_;

public:
    StringData() : data_(new char[1]{'\0'}), length_(0) {}

    StringData(const char* str) {
        if (str == nullptr) {
            data_ = new char[1]{'\0'};
            length_ = 0;
        } else {
            length_ = std::strlen(str);
            data_ = new char[length_ + 1];
            std::strcpy(data_, str);
        }
    }

    StringData(const StringData& other) : length_(other.length_) {
        data_ = new char[length_ + 1];
        std::strcpy(data_, other.data_);
    }

    StringData(StringData&& other) noexcept : data_(other.data_), length_(other.length_) {
        other.data_ = nullptr;
        other.length_ = 0;
    }

    ~StringData() { delete[] data_; }

    StringData& operator=(const StringData& other) {
        if (this != &other) {
            delete[] data_;
            length_ = other.length_;
            data_ = new char[length_ + 1];
            std::strcpy(data_, other.data_);
        }
        return *this;
    }

    StringData& operator=(StringData&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            length_ = other.length_;
            other.data_ = nullptr;
            other.length_ = 0;
        }
        return *this;
    }

    const char* c_str() const { return data_; }
    size_t getLength() const { return length_; }

    bool operator==(const StringData& other) const { return std::strcmp(data_, other.data_) == 0; }
    bool operator!=(const StringData& other) const { return !(*this == other); }
    bool operator<(const StringData& other) const { return std::strcmp(data_, other.data_) < 0; }
};

// Интеграция в std::hash для корректной работы с вашим HashSet
namespace std {
    template <>
    struct hash<StringData> {
        size_t operator()(const StringData& str) const {
            size_t h = 5381;
            const char* c = str.c_str();
            while (int ch = *c++) {
                h = ((h << 5) + h) + ch;
            }
            return h;
        }
    };
}

#endif // STRING_DATA_HPP