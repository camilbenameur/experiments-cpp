// Test entry point: Catch2 generates main()
// This file sits alone in its translation unit because catch.hpp is slow
// to compile — see the Makefile
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
