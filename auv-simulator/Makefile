# AUV simulator — targets: make test (build + run unit tests), make clean
# The simulator binary and make run will arrive with the remaining modules

CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic

APP_SRC  := $(filter-out src/main.cpp,$(wildcard src/*.cpp))
TEST_SRC := $(filter-out tests/test_main.cpp,$(wildcard tests/*.cpp))
HEADERS  := $(wildcard src/*.hpp)

all: bin/tests

test: bin/tests
	./bin/tests

# catch.hpp is large: isolate it in its own translation unit so its
# compilation cost is not paid again on every test change
build/test_main.o: tests/test_main.cpp tests/catch.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c tests/test_main.cpp -o $@

bin/tests: build/test_main.o $(TEST_SRC) $(APP_SRC) $(HEADERS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) build/test_main.o $(TEST_SRC) $(APP_SRC) -o $@

clean:
	rm -rf bin build

.PHONY: all test clean
