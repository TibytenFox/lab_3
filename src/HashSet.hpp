#ifndef HASH_SET_HPP
#define HASH_SET_HPP

#include <stddef.h>
#include <functional>
#include "sequence/MutableArraySequence.hpp"
#include "sequence/LinkedList.hpp"

template <class Key, 
		  class Hash = std::hash<Key>>
class HashSet {
public:
	HashSet();
	explicit HashSet(int capacity);
	~HashSet() = default;

	bool add(const Key& element);
	bool remove(const Key& element);
	bool contains(const Key& element) const;
	size_t getSize() const; 

	void printSet() const;
private:
	size_t num_elements_;
	MutableArraySequence<LinkedList<Key>> buckets_;
	Hash hasher_;

	void rehash(size_t new_size);
};

#include "HashSet.tpp"
#endif // HASH_SET_HPP