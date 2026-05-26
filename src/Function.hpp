#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <iostream>
#include <functional>
#include "String.hpp"

class FunctionData {
public:
    using FuncPtr = int(*)(int);

private:
    FuncPtr func_ptr_;
    StringData name_; 

public:
    FunctionData() : func_ptr_(nullptr), name_("nullptr_func") {}
    FunctionData(FuncPtr ptr, const char* name) : func_ptr_(ptr), name_(name) {}

    int invoke(int arg) const {
        if (func_ptr_ == nullptr) throw std::runtime_error("Attempt to invoke a null function pointer");
        return func_ptr_(arg);
    }

    int operator()(int arg) const { return invoke(arg); }

    bool operator==(const FunctionData& other) const { return func_ptr_ == other.func_ptr_; }
    bool operator!=(const FunctionData& other) const { return !(*this == other); }

    const StringData& getName() const { return name_; }
    FuncPtr getPointer() const { return func_ptr_; }
};

namespace std {
    template <>
    struct hash<FunctionData> {
        size_t operator()(const FunctionData& fd) const {
            return reinterpret_cast<size_t>(fd.getPointer());
        }
    };
}

#endif // FUNCTION_DATA_HPP