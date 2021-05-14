#include "doctest.h"
#include "Engine/Collection/List.h"
#include "../System/MemoryObject.h"

using namespace Engine;

TEST_SUITE("Collections") {
	TEST_CASE("List") {
		List<MemoryObject> list{};

		for (int32 i = 0; i < 10; i += 1) {
			list.Add(MemoryObject(i));
		}

		// 0 1 2 3 4 5 6 7 8 9
		CHECK(list.GetCount() == 10);
		CHECK(list.Get(0).Get() == 0);
		CHECK(list.Get(1).Get() == 1);
		CHECK(list.Get(2).Get() == 2);
		CHECK(list.Get(3).Get() == 3);
		CHECK(list.Get(4).Get() == 4);
		CHECK(list.Get(5).Get() == 5);
		CHECK(list.Get(6).Get() == 6);
		CHECK(list.Get(7).Get() == 7);
		CHECK(list.Get(8).Get() == 8);
		CHECK(list.Get(9).Get() == 9);
		
		list.RemoveAt(1);
		// 0 2 3 4 5 6 7 8 9
		CHECK(list.GetCount() == 9);
		CHECK(list.Get(0).Get() == 0);
		CHECK(list.Get(1).Get() == 2);
		CHECK(list.Get(2).Get() == 3);
		CHECK(list.Get(3).Get() == 4);
		CHECK(list.Get(4).Get() == 5);
		CHECK(list.Get(5).Get() == 6);
		CHECK(list.Get(6).Get() == 7);
		CHECK(list.Get(7).Get() == 8);
		CHECK(list.Get(8).Get() == 9);

		// 0 2 3 4 5 100 6 7 8 9
		list.Insert(5, MemoryObject(100));
		CHECK(list.GetCount() == 10);
		CHECK(list.Get(0).Get() == 0);
		CHECK(list.Get(1).Get() == 2);
		CHECK(list.Get(2).Get() == 3);
		CHECK(list.Get(3).Get() == 4);
		CHECK(list.Get(4).Get() == 5);
		CHECK(list.Get(5).Get() == 100);
		CHECK(list.Get(6).Get() == 6);
		CHECK(list.Get(7).Get() == 7);
		CHECK(list.Get(8).Get() == 8);
		CHECK(list.Get(9).Get() == 9);

		int i = 0;
		for (const auto& val : list) {
			if (i >= 1 && i <= 4) {
				CHECK(val.Get() == i + 1);
			} else if (i == 5) {
				CHECK(val.Get() == 100);
			} else if (i >= 6) {
				CHECK(val.Get() == i);
			}
			i += 1;
		}

		List<MemoryObject> list2 = list;
		List<MemoryObject> list3{};
		list3 = list2;

		List<MemoryObject> list4 = Memory::Move(list);
	}
}