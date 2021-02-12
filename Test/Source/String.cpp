#include "doctest.h"
#include "Engine/System/String.h"

TEST_SUITE("String") {
	TEST_CASE("String - Comparing") {
		String strA = u8"鞋   履   甚   多";
		String strB = u8"滴滴，你个王八蛋！";
		CHECK(strA != strB);

		strA = u8"我是臭傻逼";
		strB = u8"我是臭傻逼";
		CHECK(strA == strB);
		CHECK(strA == u8"我是臭傻逼");
		CHECK(strA != u8"我是大名鼎鼎的V");
	}
	TEST_CASE("String - Simple search") {
		String target = "This is a simple example.";
		CHECK(target.IndexOf("fuck") == -1);
		CHECK(target.IndexOf("this") == -1);
		CHECK(target.IndexOf("simple") == 10);
		CHECK(target.IndexOf("example") == 17);
		CHECK(target.IndexOf("This") == 0);
		CHECK(target.IndexOf("is") == 2);
		CHECK(target.IndexOf(".") == 24);

		String cn = u8"我是傻逼";
		CHECK(cn.IndexOf(u8"傻逼") == sizeof(u8"我是") - 1);
		CHECK(cn.IndexOf(u8"我是傻") == 0);
		CHECK(cn.IndexOf(u8"🐴") == -1);

		String emoji = u8"💉💧🐮🍺";
		CHECK(emoji.IndexOf(u8"🐮") == sizeof(u8"💉💧") - 1);
		CHECK(emoji.IndexOf(u8"🐴") == -1);
	}

	TEST_CASE("String - Referencing") {
		String original = "Hello World!";
		CHECK(original.IsIndividual());

		String selfIndividual = original.ToIndividual();
		CHECK(selfIndividual.IsIndividual());
		CHECK(original.GetRawArray() == selfIndividual.GetRawArray());

		String substr = original.Substring(6, 5); //Will be "World"
		CHECK(!substr.IsIndividual());
		CHECK(original.GetRawArray() == substr.GetRawArray());
		CHECK(substr == "World");
		CHECK(substr.IndexOf("Hello") == -1);
		CHECK(substr.IndexOf("d") == 4);

		String individual = substr.ToIndividual();
		CHECK(individual.IsIndividual());
		CHECK(substr.GetRawArray() != individual.GetRawArray());
	}

	TEST_CASE("String - StartsWith and EndsWith") {
		String target = u8"伞兵一号卢本伟，准备就绪！";
		CHECK(target.StartsWith(u8"伞兵"));
		CHECK(!target.StartsWith(u8"Steam用户"));
		CHECK(target.EndsWith(u8"准备就绪！"));
		CHECK(!target.EndsWith(u8"跟我比划比划"));
	}
}