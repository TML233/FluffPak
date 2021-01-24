#include "doctest.h"
#include "Engine/System/Allocator.h"

using namespace Engine;

TEST_CASE("Allocator") {
	Allocator<int> allo{};
	int* ptr = allo.Allocate(2);
	allo.Construct(ptr);
	allo.Construct(ptr + 1);
	CHECK(*ptr == 0);
	CHECK(*(ptr + 1) == 0);
	*ptr = 3;
	CHECK(*ptr == 3);
	allo.Deconstruct(ptr);
	allo.Deconstruct(ptr + 1);
	allo.Deallocate(ptr, 2);
}