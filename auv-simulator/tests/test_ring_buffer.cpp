#include "catch.hpp"

#include "../src/ring_buffer.hpp"

TEST_CASE("RingBuffer: empty when constructed") {
    RingBuffer<int, 3> rb;
    REQUIRE(rb.empty());
    REQUIRE_FALSE(rb.full());
    REQUIRE(rb.size() == 0);
}

TEST_CASE("RingBuffer: FIFO order") {
    RingBuffer<int, 3> rb;
    REQUIRE(rb.push(10));
    REQUIRE(rb.push(20));

    int out = 0;
    REQUIRE(rb.pop(out));
    REQUIRE(out == 10);
    REQUIRE(rb.pop(out));
    REQUIRE(out == 20);
}

TEST_CASE("RingBuffer: push refuses when full, no overwrite") {
    RingBuffer<int, 2> rb;
    REQUIRE(rb.push(1));
    REQUIRE(rb.push(2));
    REQUIRE(rb.full());

    REQUIRE_FALSE(rb.push(3));   // refused, nothing overwritten

    int out = 0;
    REQUIRE(rb.pop(out));
    REQUIRE(out == 1);           // content untouched
}

TEST_CASE("RingBuffer: pop refuses when empty") {
    RingBuffer<int, 2> rb;
    int out = 42;
    REQUIRE_FALSE(rb.pop(out));
    REQUIRE(out == 42);          // out intact on failure
}

TEST_CASE("RingBuffer: wrap-around reuses slots") {
    RingBuffer<int, 3> rb;
    rb.push(1);
    rb.push(2);
    rb.push(3);

    int out = 0;
    rb.pop(out);
    rb.pop(out);                 // frees two slots
    REQUIRE(rb.push(4));         // reuses the start of the array
    REQUIRE(rb.push(5));
    REQUIRE(rb.full());

    rb.pop(out);
    REQUIRE(out == 3);
    rb.pop(out);
    REQUIRE(out == 4);
    rb.pop(out);
    REQUIRE(out == 5);
    REQUIRE(rb.empty());
}
