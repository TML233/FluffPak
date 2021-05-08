#include "doctest.h"
#include "Engine/Collection/Dictionary.h"
#include "Engine/System/String.h"

using namespace Engine;

TEST_SUITE("Collections") {
	TEST_CASE("Dictionary") {
		{
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

		{
			Dictionary<String, int> dic;
			bool sb = false;
			dic.Add(STRL("I AM SB"), 123);
			bool yjsp = false;
			dic.Add(STRL("YJSP"), 114514);
			bool mur = false;
			dic.Add(STRL("MUR"), 393);
			bool ark = false;
			dic.Add(STRL("Arknights"), 3000);
			bool sxc = false;
			dic.Add(STRL("孙笑川"), -8000);

			for (const auto& pair : dic) {
				if (pair.key == STRL("I AM SB") && pair.value == 123) {
					sb = true;
				} else if (pair.key == STRL("YJSP") && pair.value == 114514) {
					yjsp = true;
				} else if (pair.key == STRL("MUR") && pair.value == 393) {
					mur = true;
				} else if (pair.key == STRL("Arknights") && pair.value == 3000) {
					ark = true;
				} else if (pair.key == STRL("孙笑川") && pair.value == -8000) {
					sxc = true;
				}
			}
			bool result = sb && yjsp && mur && ark && sxc;
			CHECK(result);
		}
	}
}