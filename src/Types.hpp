#ifndef TYPES_HPP
#define TYPES_HPP

#include <stddef.h>

struct IntHasher {
	size_t operator()(const int& num) const {
		return num;
	}
};

#endif // TYPES_HPP