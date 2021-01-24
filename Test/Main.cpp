#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Engine/System/Allocator.h"

TEST_CASE("Testing Allocator") {
	Engine::Allocator<int> allo{};
}
