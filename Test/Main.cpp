#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Core/System/Allocator.h"

TEST_CASE("Testing Allocator") {
	Core::Allocator<int> allo{};
}
