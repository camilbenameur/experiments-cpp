// Measurement filtering (Strategy + Template Method patterns)
//
// - Filter: the filtering strategy. Each sensor channel gets its own at
//   construction, swappable without touching the pipeline
// - WindowedFilter: the shared "sliding window" skeleton is written once
//   in the base (Template Method); derived classes only provide the
//   computation over the window
//
// Storage is a fixed-capacity std::array: no dynamic allocation on the
// data path (embedded reflex)

#ifndef FILTER_HPP
#define FILTER_HPP

#include <array>
#include <cstddef>

class Filter {
public:
    virtual ~Filter() = default;
    // Takes the raw value, returns the filtered one
    virtual double apply(double raw) = 0;
};

class WindowedFilter : public Filter {
public:
    static constexpr std::size_t kMaxWindow = 8;

    // window is clamped to [1, kMaxWindow] (static capacity)
    explicit WindowedFilter(std::size_t window);

    // final: window management is the non-specializable skeleton;
    // only compute() varies with the concrete strategy
    double apply(double raw) final;

private:
    virtual double compute(const std::array<double, kMaxWindow>& data,
                           std::size_t count) const = 0;

    std::array<double, kMaxWindow> data_{};
    std::size_t window_;
    std::size_t next_ = 0;    // next slot to write (circular)
    std::size_t count_ = 0;   // values received so far, capped at window_
};

// Moving average: smooths noise, but a single aberrant value pollutes
// the whole window
class MovingAverageFilter : public WindowedFilter {
public:
    explicit MovingAverageFilter(std::size_t window) : WindowedFilter(window) {}

private:
    double compute(const std::array<double, kMaxWindow>& data,
                   std::size_t count) const override;
};

// Moving median: robust to isolated aberrant values (a sensor glitch
// does not get through), at the cost of sorting the window
class MedianFilter : public WindowedFilter {
public:
    explicit MedianFilter(std::size_t window) : WindowedFilter(window) {}

private:
    double compute(const std::array<double, kMaxWindow>& data,
                   std::size_t count) const override;
};

#endif
