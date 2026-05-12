#include <iostream>
#include "HashSet.hpp"
#include "sequence/Exceptions.hpp"

#define DEFAULT_CAPACITY 10

template <class Key, class Hash>
HashSet<Key, Hash>::Enumerator::Enumerator(const MutableArraySequence<LinkedList<Key>>& buckets)
    : buckets_(&buckets), bucket_index_(0), current_enum_(nullptr) {}


template <class Key, class Hash>
HashSet<Key, Hash>::Enumerator::~Enumerator() {
    delete current_enum_;
}

template <class Key, class Hash>
bool HashSet<Key, Hash>::Enumerator::MoveNext() {
    if (current_enum_ == nullptr) {
        for (size_t i = bucket_index_; i < buckets_->GetLength(); ++i) {
            const LinkedList<Key>& bucket = (*buckets_)[i];
            if (bucket.GetHead() != nullptr) {   
                current_enum_ = bucket.GetEnumerator();
                bucket_index_ = i;
                break;
            }
        }
        if (current_enum_ == nullptr) return false; 
    }

    if (current_enum_->MoveNext()) {
        return true;
    } else {
        delete current_enum_;
        current_enum_ = nullptr;
        ++bucket_index_;

        for (size_t i = bucket_index_; i < buckets_->GetLength(); ++i) {
            const LinkedList<Key>& bucket = (*buckets_)[i];
            if (bucket.GetHead() != nullptr) {
                current_enum_ = bucket.GetEnumerator();
                bucket_index_ = i;
                if (current_enum_->MoveNext())
                    return true;
                else {
                    delete current_enum_;
                    current_enum_ = nullptr;
                    continue;
                }
            }
        }
        return false;
    }
}

template <class Key, class Hash>
const Key& HashSet<Key, Hash>::Enumerator::GetCurrent() const {
    if (current_enum_ == nullptr) throw std::runtime_error("Enumerator: out of bounds");
    return current_enum_->GetCurrent();
}

template <class Key, class Hash>
IEnumerator<Key>* HashSet<Key, Hash>::GetEnumerator() const {
    return new Enumerator(buckets_);
}

template <class Key, class Hash>
HashSet<Key, Hash>::HashSet() : num_elements_(0), buckets_(DEFAULT_CAPACITY) {}

template <class Key, class Hash>
HashSet<Key, Hash>::HashSet(int capacity) : num_elements_(0), buckets_(capacity) {}

template <class Key, class Hash>
HashSet<Key, Hash>::HashSet(const HashSet<Key, Hash>& other) : num_elements_(other.num_elements_), buckets_(other.buckets_) {}

template <class Key, class Hash>
bool HashSet<Key, Hash>::add(const Key& element) {
	if (this->contains(element)) return false;

	size_t hashed = hasher_(element) % buckets_.GetLength();
	buckets_[hashed].Append(element);
	num_elements_++;

	if (static_cast<double>(num_elements_) / buckets_.GetLength() > 0.75) {
		this->rehash(buckets_.GetLength() * 2);
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
bool HashSet<Key, Hash>::includesSubset(const HashSet<Key, Hash>& subset) const {
	IEnumerator<Key>* en = subset.GetEnumerator();
	while (en->MoveNext()) {
		if (!this->contains(en->GetCurrent())) {
			delete en;
			return false;
		}
	}

	delete en;
	return true;
}

template <class Key, class Hash>
size_t HashSet<Key, Hash>::getSize() const {
	return num_elements_;
}

template <class Key, class Hash>
HashSet<Key, Hash> HashSet<Key, Hash>::map(Key (*func)(Key)) const {
	HashSet<Key, Hash> result(this->getSize());
	
	IEnumerator<Key>* en = this->GetEnumerator();
	while (en->MoveNext()) {
		result.add(func(en->GetCurrent()));
	}

	return result;
}

template <class Key, class Hash>
HashSet<Key, Hash> HashSet<Key, Hash>::where(bool (*predicate)(Key)) const {
	HashSet<Key, Hash> result(this->getSize());
	
	IEnumerator<Key>* en = this->GetEnumerator();
	while (en->MoveNext()) {
		if (predicate(en->GetCurrent())) result.add(en->GetCurrent());
	}

	return result;
}

template <class Key, class Hash>
HashSet<Key, Hash> HashSet<Key, Hash>::operator|(const HashSet<Key, Hash>& other) const {
	size_t total_size = this->getSize() + other.getSize();
	HashSet<Key, Hash> result(static_cast<int>(total_size * 1.5));

	IEnumerator<Key>* en_this = this->GetEnumerator();
	while (en_this->MoveNext()) {
		result.add(en_this->GetCurrent());
	}
	delete en_this;

	IEnumerator<Key>* en_other = other.GetEnumerator();
	while (en_other->MoveNext()) {
		result.add(en_other->GetCurrent());
	}
	delete en_other;

	return result;
}

template <class Key, class Hash>
HashSet<Key, Hash> HashSet<Key, Hash>::operator&(const HashSet<Key, Hash>& other) const {
	size_t total_size = (this->getSize() < other.getSize()) ? this->getSize() : other.getSize();
	HashSet<Key, Hash> result(static_cast<int>(total_size * 1.5));

	IEnumerator<Key>* en;
	if (total_size == this->getSize()) {
		en = this->GetEnumerator();
		while (en->MoveNext()) {
			if (other.contains(en->GetCurrent())) result.add(en->GetCurrent());
		}
	} else {
		en = other.GetEnumerator();
		while (en->MoveNext()) {
			if (this->contains(en->GetCurrent())) result.add(en->GetCurrent());
		}
	}

	delete en;
	return result;
}

template <class Key, class Hash>
HashSet<Key, Hash> HashSet<Key, Hash>::operator/(const HashSet<Key, Hash>& other) const {
	HashSet<Key, Hash> result(this->getSize());

	IEnumerator<Key>* en = this->GetEnumerator();
	while (en->MoveNext()) {
		if (other.contains(en->GetCurrent())) continue;
		result.add(en->GetCurrent());
	}

	delete en;
	return result;
}

template <class Key, class Hash>
bool HashSet<Key, Hash>::operator==(const HashSet<Key, Hash>& other) const {
	if (this->getSize() != other.getSize()) return false;

	IEnumerator<Key>* en = this->GetEnumerator();
	while (en->MoveNext()) {
		if (!other.contains(en->GetCurrent())) {
			delete en;
			return false;
		}
	}

	delete en;
	return true;
}

template <class Key, class Hash>
bool HashSet<Key, Hash>::operator!=(const HashSet<Key, Hash>& other) const { 
	return !(*this == other);
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
		while (en->MoveNext()) {
			size_t new_index = hasher_(en->GetCurrent()) % new_buckets.GetLength();
			new_buckets[new_index].Append(en->GetCurrent());
		}
	}
	buckets_ = std::move(new_buckets);
}

