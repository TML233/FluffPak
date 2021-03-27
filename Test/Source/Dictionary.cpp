#include "doctest.h"
#include "Engine/Collection/Dictionary.h"

using namespace Engine;

TEST_SUITE("Collections") {
	TEST_CASE("Dictionary") {
		Dictionary<int, char> dic{};
		CHECK(dic.Add(1, 'f'));
		CHECK(dic.Add(2, 'u'));
		CHECK(dic.Add(3, 'c'));
		CHECK(dic.Add(4, 'k'));
		CHECK(dic.Add(5, ' '));
		CHECK(dic.Add(6, 'y'));
		CHECK(dic.Add(7, 'o'));
		CHECK(dic.Add(8, 'u'));
		CHECK(dic.Add(9, '!'));

		CHECK(dic.GetCount() == 9);
		CHECK(dic.ContainsKey(3));
		CHECK(dic.ContainsKey(9));
		CHECK(dic.Get(1) == 'f');
		CHECK(dic.Get(2) == 'u');
		CHECK(dic.Get(9) == '!');

		CHECK(dic.Remove(2));

		CHECK(!dic.ContainsKey(2));
		CHECK(dic.ContainsKey(3));

		CHECK(dic.Add(2, 'a'));

		CHECK(dic.Get(2) == 'a');
	}
}