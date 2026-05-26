#include "Queue.hpp"

template <class T>
Queue<T>::Queue(T* items, int count) : sequence_(items, count) {}

template <class T>
Queue<T>::Queue(const Queue<T>& other) : sequence_(other.sequence_) {}

template <class T>
Queue<T>::Queue(Queue<T>&& other) noexcept : sequence_(std::move(other.sequence_)) {}

template <class T>
Queue<T>& Queue<T>::operator=(const Queue<T>& other) {
    if (this != &other) sequence_ = other.sequence_;
    return *this;
}

template <class T>
Queue<T>& Queue<T>::operator=(Queue<T>&& other) noexcept {
    if (this != &other) sequence_ = std::move(other.sequence_);
    return *this;
}

template <class T>
void Queue<T>::Enqueue(const T& item) { sequence_.Append(item); }

template <class T>
T Queue<T>::Dequeue() {
    if (IsEmpty()) throw EmptyCollectionError("Dequeue(): Queue is empty");
    return sequence_.RemoveAt(0);
}

template <class T>
const T& Queue<T>::Peek() const {
    if (IsEmpty()) throw EmptyCollectionError("Peek(): Queue is empty");
    return sequence_.GetFirst();
}

template <class T>
int Queue<T>::GetLength() const { return sequence_.GetLength(); }

template <class T>
bool Queue<T>::IsEmpty() const { return sequence_.GetLength() == 0; }

template <class T>
IEnumerator<T>* Queue<T>::GetEnumerator() const { return sequence_.GetEnumerator(); }

template <class T>
Queue<T> Queue<T>::map(T (*func)(T)) const {
    Queue<T> result;
    for (int i = 0; i < sequence_.GetLength(); ++i) {
        result.Enqueue(func(sequence_.Get(i)));
    }
    return result;
}

template <class T>
Queue<T> Queue<T>::where(bool (*predicate)(T)) const {
    Queue<T> result;
    for (int i = 0; i < sequence_.GetLength(); ++i) {
        if (predicate(sequence_.Get(i))) {
            result.Enqueue(sequence_.Get(i));
        }
    }
    return result;
}

template <class T>
T Queue<T>::reduce(T (*func)(T, T), const T& initial_value) const {
    T accumulator = initial_value;
    for (int i = 0; i < sequence_.GetLength(); ++i) {
        accumulator = func(sequence_.Get(i), accumulator);
    }
    return accumulator;
}

template <class T>
Queue<T> Queue<T>::Concat(const Queue<T>& other) const {
    Queue<T> result(*this);
    for (int i = 0; i < other.GetLength(); ++i) {
        result.Enqueue(other.sequence_.Get(i));
    }
    return result;
}

template <class T>
Queue<T> Queue<T>::GetSubsequence(int start_index, int end_index) const {
	if (start_index < 0 || end_index >= this->GetLength() || end_index < start_index) throw IndexOutOfRange("GetSubsequence()");
    Queue<T> result;
    for (int i = start_index; i < end_index; ++i) {
        result.Enqueue(this->sequence_.Get(i));
    }
    return result;
}

template <class T>
int Queue<T>::FindSubsequence(const Queue<T>& sub) const {
    int n = this->GetLength();
    int m = sub.GetLength();
    if (m == 0) return 0;
    if (n < m) return -1;

    for (int i = 0; i <= n - m; ++i) {
        bool match = true;
        for (int j = 0; j < m; ++j) {
            if (this->sequence_.Get(i + j) != sub.sequence_.Get(j)) {
                match = false;
                break;
            }
        }
        if (match) return i;
    }
    return -1;
}

template <class T>
bool Queue<T>::operator==(const Queue<T>& other) const {
    if (GetLength() != other.GetLength()) return false;
    for (int i = 0; i < GetLength(); ++i) {
        if (sequence_.Get(i) != other.sequence_.Get(i)) return false;
    }
    return true;
}

template <class T>
bool Queue<T>::operator!=(const Queue<T>& other) const { return !(*this == other); }