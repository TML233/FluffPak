#include "doctest.h"
#include "Engine/Collection/Deque.h"
#include "../System/MemoryObject.h"

using namespace Engine;

TEST_SUITE("Collections") {
	TEST_CASE("Deque") {
		Deque<MemoryObject> deque;
		deque.PushBack(1);
		deque.PushBack(2);
		deque.PushBack(3);
		deque.PushFront(0);
		deque.PushFront(-1);
		CHECK(deque.GetCount() == 5);


		// -1 0 1 2 3
		bool s = deque.PopBack() == MemoryObject(3);
		CHECK(s);
		CHECK(deque.GetCount() == 4);
		
		// -1 0 1 2
		s = deque.PopFront() == MemoryObject(-1);
		CHECK(s);
		CHECK(deque.GetCount() == 3);

		// 0 1 2
		s = deque.PopBack() == MemoryObject(2);
		CHECK(s);
		CHECK(deque.GetCount() == 2);

		// 0 1
		deque.PushBack(9);
		CHECK(deque.GetCount() == 3);

		// 0 1 9
		s = deque.PopFront() == MemoryObject(0);
		CHECK(s);
		CHECK(deque.GetCount() == 2);

		// 1 9
		s = deque.PopBack() == MemoryObject(9);
		CHECK(s);
		CHECK(deque.GetCount() == 1);

		// 1
		s = deque.PopFront() == MemoryObject(1);
		CHECK(s);
		CHECK(deque.GetCount() == 0);

		MemoryObject obj;
		CHECK(!deque.TryPopBack(obj));
		CHECK(!deque.TryPopFront(obj));

		deque.PushFront(8);
		CHECK(deque.GetCount() == 1);

		s = deque.PopBack() == MemoryObject(8);
		CHECK(s);
		CHECK(deque.GetCount() == 0);

		deque.PushBack(114);
		CHECK(deque.GetCount() == 1);
		
		s = deque.PopFront() == MemoryObject(114);
		CHECK(s);
		CHECK(deque.GetCount() == 0);


		deque.PushBack(0);
		deque.PushFront(1);
		deque.PushBack(2);
		deque.PushFront(3);
		deque.PushBack(4);
		deque.PushFront(5);
		deque.PushBack(6);
		CHECK(deque.GetCount() == 7);
	}
}
