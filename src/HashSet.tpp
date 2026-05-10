#include "HashSet.hpp"
#include "sequence/Exceptions.hpp"

#define DEFAULT_CAPACITY 10

template <class Key, class Hash>
HashSet<Key, Hash>::HashSet() : num_elements_(0), buckets_(DEFAULT_CAPACITY) {
	for (size_t i = 0; i < buckets_.GetCapacity(); ++i) {
		LinkedList<Key> empty;
		buckets_.Append(empty);
	}
}

template <class Key, class Hash>
HashSet<Key, Hash>::HashSet(int capacity) : num_elements_(0), buckets_(capacity) {
	for (size_t i = 0; i < buckets_.GetCapacity(); ++i) {
		LinkedList<Key> empty;
		buckets_.Append(empty);
	}
}

template <class Key, class Hash>
bool HashSet<Key, Hash>::add(const Key& element) {
	if (this->contains(element)) return false;

	size_t hashed = hasher_(element) % buckets_.GetLength();
	buckets_[hashed].Append(element);
	num_elements_++;

	if (static_cast<double>(num_elements_) / buckets_.GetLength() > 0.75) {
		rehash(buckets_.GetLength() * 2);
	}
	return true;
}

template <class Key, class Hash>
bool HashSet<Key, Hash>::remove(const Key& element) {
	if (!this->contains(element)) return false;

	size_t hashed = hasher_(element) % buckets_.GetLength();
	IEnumerator<Key>* en = buckets_[hashed].GetEnumerator();

	int index = 0;
	while (en->MoveNext()) {
		if (en->GetCurrent() == element) {
			buckets_[hashed].RemoveAt(index);
			break;
		}
		index++;
	}

	delete en;
	num_elements_--;
	return true;
}

template <class Key, class Hash>
bool HashSet<Key, Hash>::contains(const Key& element) const {
	size_t hashed = hasher_(element) % buckets_.GetLength();
	IEnumerator<Key>* en = buckets_[hashed].GetEnumerator();

	while (en->MoveNext()) {
		if (en->GetCurrent() == element) {
			delete en;
			return true;
		}
	}

	delete en;
	return false;
}

template <class Key, class Hash>
size_t HashSet<Key, Hash>::getSize() const {
	return num_elements_;
}

template <class Key, class Hash>
void HashSet<Key, Hash>::printSet() const {
	std::cout << "Size: " << num_elements_ << " ( ";
	for (size_t i = 0; i < buckets_.GetLength(); ++i) {
		IEnumerator<Key>* en = buckets_[i].GetEnumerator();
		while (en->MoveNext()) {
			std::cout << en->GetCurrent() << ' ';		
		}
	}
	std::cout << ")\n";
}

template <class Key, class Hash>
void HashSet<Key, Hash>::rehash(size_t new_size) {
	MutableArraySequence<LinkedList<Key>> new_buckets(new_size);
	for (size_t i = 0; i < buckets_.GetLength(); ++i) {
		IEnumerator<Key>* en = buckets_[i].GetEnumerator();
		new_buckets.Append(LinkedList<Key>());
		while (en->MoveNext()) {
			size_t new_index = hasher_(en->GetCurrent()) % new_buckets.GetCapacity();
			new_buckets[new_index].Append(en->GetCurrent());
		}
	}
	buckets_ = std::move(new_buckets);
}

