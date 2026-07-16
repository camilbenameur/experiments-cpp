// Statically allocated ring buffer (promoted from my exercises, now with
// real unit tests — see tests/test_ring_buffer.cpp)
//
// Why not a std::vector? push_back may REallocate: unbounded cost,
// forbidden in an ISR-like context. Here the memory lives inside the
// object itself (std::array), size known at compile time, zero heap
// No overwrite allowed: push() fails when full, the caller decides what
// to do with the lost measurement

#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <array>
#include <cstddef>

template <typename T, std::size_t N>
class RingBuffer {
public:
    bool push(const T& value) {
        if (full()) return false;
        data_[tail_] = value;
        tail_ = (tail_ + 1) % N;
        ++count_;
        return true;
    }

    bool pop(T& out) {
        if (empty()) return false;
        out = data_[head_];
        head_ = (head_ + 1) % N;
        --count_;
        return true;
    }

    bool empty() const { return count_ == 0; }
    bool full() const { return count_ == N; }
    std::size_t size() const { return count_; }

private:
    std::array<T, N> data_{};
    std::size_t head_ = 0;   // next read
    std::size_t tail_ = 0;   // next write
    std::size_t count_ = 0;  // disambiguates full/empty when head_ == tail_
};

#endif
