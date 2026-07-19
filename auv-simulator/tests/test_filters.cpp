#include "catch.hpp"

#include "../src/filter.hpp"

TEST_CASE("MovingAverage: averages received values during warm-up") {
    MovingAverageFilter f(3);
    REQUIRE(f.apply(3.0) == Approx(3.0));          // 1 value
    REQUIRE(f.apply(5.0) == Approx(4.0));          // (3+5)/2
}

TEST_CASE("MovingAverage: full window then sliding") {
    MovingAverageFilter f(3);
    f.apply(1.0);
    f.apply(2.0);
    REQUIRE(f.apply(3.0) == Approx(2.0));          // (1+2+3)/3
    REQUIRE(f.apply(6.0) == Approx(11.0 / 3.0));   // 1 slid out: (6+2+3)/3
}

TEST_CASE("MovingAverage: an aberrant value pollutes the average") {
    MovingAverageFilter f(3);
    f.apply(10.0);
    f.apply(10.0);
    REQUIRE(f.apply(999.0) > 300.0);               // the assumed weakness
}

TEST_CASE("Median: rejects an isolated aberrant value") {
    MedianFilter f(3);
    f.apply(10.0);
    f.apply(10.5);
    REQUIRE(f.apply(999.0) == Approx(10.5));       // the glitch does not get through
}

TEST_CASE("Median: odd window, middle value") {
    MedianFilter f(3);
    f.apply(30.0);
    f.apply(10.0);
    REQUIRE(f.apply(20.0) == Approx(20.0));
}

TEST_CASE("Median: even count, mean of the two middle values") {
    MedianFilter f(4);
    f.apply(1.0);
    REQUIRE(f.apply(3.0) == Approx(2.0));          // warm-up: 2 values
}

TEST_CASE("WindowedFilter: window size clamped to static capacity") {
    // A window requested too large is brought back to kMaxWindow: the
    // filter stays usable, without dynamic allocation
    MovingAverageFilter f(1000);
    for (int i = 0; i < 100; ++i) {
        f.apply(2.0);
    }
    REQUIRE(f.apply(2.0) == Approx(2.0));
}
