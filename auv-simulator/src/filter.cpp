#include "filter.hpp"

#include <algorithm>
#include <numeric>

WindowedFilter::WindowedFilter(std::size_t window)
    : window_(std::clamp<std::size_t>(window, 1, kMaxWindow)) {}

double WindowedFilter::apply(double raw) {
    data_[next_] = raw;
    next_ = (next_ + 1) % window_;
    count_ = std::min(count_ + 1, window_);
    return compute(data_, count_);
}

double MovingAverageFilter::compute(const std::array<double, kMaxWindow>& data,
                                    std::size_t count) const {
    double sum = std::accumulate(data.begin(), data.begin() + count, 0.0);
    return sum / static_cast<double>(count);
}

double MedianFilter::compute(const std::array<double, kMaxWindow>& data,
                             std::size_t count) const {
    auto sorted = data;   // local copy: compute must not alter the window
    std::sort(sorted.begin(), sorted.begin() + count);
    if (count % 2 == 1) {
        return sorted[count / 2];
    }
    return (sorted[count / 2 - 1] + sorted[count / 2]) / 2.0;
}
