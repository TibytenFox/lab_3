#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stddef.h>
#include "sequence/LinkedList.hpp"
#include "sequence/IEnumerable.hpp"
#include "sequence/Exceptions.hpp"

template <class T>
class Queue : public IEnumerable<T> {
private:
    LinkedList<T> sequence_;

public:
    Queue() = default;
    Queue(T* items, int count);
    Queue(const Queue<T>& other);
    Queue(Queue<T>&& other) noexcept;
    ~Queue() override = default;

    Queue<T>& operator=(const Queue<T>& other);
    Queue<T>& operator=(Queue<T>&& other) noexcept;

    void Enqueue(const T& item);
    T Dequeue();
    const T& Peek() const;
    int GetLength() const;
    bool IsEmpty() const;

    IEnumerator<T>* GetEnumerator() const override;

    Queue<T> map(T (*func)(T)) const;
    Queue<T> where(bool (*predicate)(T)) const;
    T reduce(T (*func)(T, T), const T& initial_value) const;

    Queue<T> Concat(const Queue<T>& other) const;
    Queue<T> GetSubsequence(int start_index, int end_index) const;
    int FindSubsequence(const Queue<T>& sub) const;

    bool operator==(const Queue<T>& other) const;
    bool operator!=(const Queue<T>& other) const;
};

#include "Queue.tpp"
#endif // QUEUE_HPP