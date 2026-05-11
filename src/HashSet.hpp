#ifndef HASH_SET_HPP
#define HASH_SET_HPP

#include <stddef.h>
#include <functional>
#include "sequence/IEnumerator.hpp"
#include "sequence/IEnumerable.hpp"
#include "sequence/MutableArraySequence.hpp"
#include "sequence/LinkedList.hpp"

template <class Key, 
		  class Hash = std::hash<Key>>
class HashSet : IEnumerable<Key> {
public:
	class Enumerator : public IEnumerator<Key> {
    private:
        const MutableArraySequence<LinkedList<Key>>* buckets_;
        size_t bucket_index_;
        IEnumerator<Key>* current_enum_;   
    public:
        explicit Enumerator(const MutableArraySequence<LinkedList<Key>>& buckets);
        ~Enumerator() override;

        bool MoveNext() override;
        const Key& GetCurrent() const override;
    };

    IEnumerator<Key>* GetEnumerator() const override;

	HashSet();
	explicit HashSet(int capacity);
	HashSet(const HashSet<Key, Hash>& other);
	HashSet(HashSet<Key, Hash>&& other) noexcept = default;
	HashSet<Key, Hash>& operator=(HashSet<Key, Hash>&& other) noexcept = default;
	~HashSet() = default;

	bool add(const Key& element);
	bool remove(const Key& element);
	bool contains(const Key& element) const;
	bool includesSubset(const HashSet<Key, Hash>& subset) const;
	size_t getSize() const; 

	HashSet<Key, Hash> map(Key (*func)(Key)) const;
	HashSet<Key, Hash> where(bool (*predicate)(Key)) const;

	HashSet<Key, Hash> operator|(const HashSet<Key, Hash>& other) const;
	HashSet<Key, Hash> operator&(const HashSet<Key, Hash>& other) const;
	HashSet<Key, Hash> operator/(const HashSet<Key, Hash>& other) const;
	bool operator==(const HashSet<Key, Hash>& other) const;
	bool operator!=(const HashSet<Key, Hash>& other) const;

	void printSet() const;
private:
	size_t num_elements_;
	MutableArraySequence<LinkedList<Key>> buckets_;
	Hash hasher_;

	void rehash(size_t new_size);
};

#include "HashSet.tpp"
#endif // HASH_SET_HPP