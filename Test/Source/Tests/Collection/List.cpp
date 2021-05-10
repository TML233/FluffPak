#include "doctest.h"
#include "Engine/Collection/List.h"

using namespace Engine;

TEST_SUITE("Collections") {
	TEST_CASE("List") {
		List<int32> list{};

		for (int32 i = 0; i < 10; i += 1) {
			list.Add(i);
		}

		// 0 1 2 3 4 5 6 7 8 9
		CHECK(list.GetCount() == 10);
		CHECK(list.Get(1) == 1);
		CHECK(list.Get(8) == 8);
		
		list.RemoveAt(1);
		// 0 2 3 4 5 6 7 8 9
		CHECK(list.GetCount() == 9);
		CHECK(list.Get(1) == 2);
		CHECK(list.Get(8) == 9);

		// 0 2 3 4 5 100 6 7 8 9
		list.Insert(5, 100);
		CHECK(list.GetCount() == 10);
		CHECK(list.Get(5) == 100);
		CHECK(list.Get(6) == 6);
		CHECK(list.Get(9) == 9);

		int i = 0;
		for (auto val : list) {
			if (i >= 1 && i <= 4) {
				CHECK(val == i + 1);
			} else if (i == 5) {
				CHECK(val == 100);
			} else if (i >= 6) {
				CHECK(val == i);
			}
			i += 1;
		}

		List<int32> list2 = list;
		List<int32> list3{};
		list3 = list2;

		List<int32> list4 = Memory::Move(list);
	}
}