#include "doctest.h"
#include "Engine/Collection/Deque.h"

using namespace Engine;

TEST_SUITE("Collections") {
	TEST_CASE("Deque") {
		Deque<int32> deque;
		deque.PushBack(1);
		deque.PushBack(2);
		deque.PushBack(3);
		deque.PushFront(0);
		deque.PushFront(-1);
	}
}
