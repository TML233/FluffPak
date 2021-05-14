#include "doctest.h"
#include "Engine/Collection/Dictionary.h"
#include "Engine/System/String.h"
#include "../System/MemoryObject.h"

using namespace Engine;

TEST_SUITE("Collections") {
	TEST_CASE("Dictionary") {
		{
			Dictionary<MemoryObject, String> dic{};
			CHECK(dic.Add(MemoryObject(1), u8"f"));
			CHECK(dic.Add(MemoryObject(2), u8"u"));
			CHECK(dic.Add(MemoryObject(3), u8"c"));
			CHECK(dic.Add(MemoryObject(4), u8"k"));
			CHECK(dic.Add(MemoryObject(5), u8" "));
			CHECK(dic.Add(MemoryObject(6), u8"y"));
			CHECK(dic.Add(MemoryObject(7), u8"o"));
			CHECK(dic.Add(MemoryObject(8), u8"u"));
			CHECK(dic.Add(MemoryObject(9), u8"!"));

			CHECK(dic.GetCount() == 9);
			CHECK(dic.ContainsKey(MemoryObject(3)));
			CHECK(dic.ContainsKey(MemoryObject(9)));
			CHECK(dic.Get(MemoryObject(1)) == u8"f");
			CHECK(dic.Get(MemoryObject(2)) == u8"u");
			CHECK(dic.Get(MemoryObject(9)) == u8"!");

			CHECK(dic.Remove(MemoryObject(2)));

			CHECK(!dic.ContainsKey(MemoryObject(2)));
			CHECK(dic.ContainsKey(MemoryObject(3)));

			CHECK(dic.Add(MemoryObject(2), u8"a"));

			CHECK(dic.Get(MemoryObject(2)) == u8"a");

			Dictionary<MemoryObject, String> dic2 = dic;
			Dictionary<MemoryObject, String> dic3{};
			dic3 = dic2;

			Dictionary<MemoryObject, String> dic4 = Memory::Move(dic);
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