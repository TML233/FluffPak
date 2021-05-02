#include "doctest.h"
#include "Engine/System/String.h"

using namespace Engine;

TEST_SUITE("String") {
	TEST_CASE("Replacing") {
		String str = STRING_LITERAL(u8"TestCase001 1234 asr");
		str = str.Replace(STRING_LITERAL(u8" "), String::GetEmpty());
		CHECK(str == STRING_LITERAL(u8"TestCase0011234asr"));
		str = str.Replace(STRING_LITERAL(u8"e"), STRING_LITERAL(u8"w"));
		CHECK(str == STRING_LITERAL(u8"TwstCasw0011234asr"));
		str = str.Replace(STRING_LITERAL(u8"s"), STRING_LITERAL(u8"123"));
		CHECK(str == STRING_LITERAL(u8"Tw123tCa123w0011234a123r"));
		str = str.Replace(STRING_LITERAL(u8"1"), String::GetEmpty());
		CHECK(str == STRING_LITERAL(u8"Tw23tCa23w00234a23r"));
		str = str.Replace(STRING_LITERAL(u8"0"), STRING_LITERAL(u8"SB"));
		CHECK(str == STRING_LITERAL(u8"Tw23tCa23wSBSB234a23r"));
	}
	TEST_CASE("Comparing") {
		String strA = STRING_LITERAL(u8"鞋   履   甚   多");
		String strB = STRING_LITERAL(u8"滴滴，你个王八蛋！");
		CHECK(strA != strB);

		strA = STRING_LITERAL(u8"我是臭傻逼");
		strB = STRING_LITERAL(u8"我是臭傻逼");
		CHECK(strA == strB);
		CHECK(strA == STRING_LITERAL(u8"我是臭傻逼"));
		CHECK(strA != STRING_LITERAL(u8"我是大名鼎鼎的V"));
	}
	TEST_CASE("Simple search") {
		String target = STRING_LITERAL(u8"This is a simple example.");
		CHECK(target.IndexOf(STRING_LITERAL(u8"fuck")) == -1);
		CHECK(target.IndexOf(STRING_LITERAL(u8"this")) == -1);
		CHECK(target.IndexOf(STRING_LITERAL(u8"simple")) == 10);
		CHECK(target.IndexOf(STRING_LITERAL(u8"example")) == 17);
		CHECK(target.IndexOf(STRING_LITERAL(u8"This")) == 0);
		CHECK(target.IndexOf(STRING_LITERAL(u8"is")) == 2);
		CHECK(target.IndexOf(STRING_LITERAL(u8".")) == 24);

		String cn = STRING_LITERAL(u8"我是傻逼");
		CHECK(cn.IndexOf(STRING_LITERAL(u8"傻逼")) == sizeof(u8"我是") - 1);
		CHECK(cn.IndexOf(STRING_LITERAL(u8"我是傻")) == 0);
		CHECK(cn.IndexOf(STRING_LITERAL(u8"🐴")) == -1);

		String emoji = STRING_LITERAL(u8"💉💧🐮🍺");
		CHECK(emoji.IndexOf(STRING_LITERAL(u8"🐮")) == sizeof(u8"💉💧") - 1);
		CHECK(emoji.IndexOf(STRING_LITERAL(u8"🐴")) == -1);
	}

	TEST_CASE("Referencing") {
		String original = STRING_LITERAL(u8"Hello World!");
		CHECK(original.IsIndividual());

		String selfIndividual = original.ToIndividual();
		CHECK(selfIndividual.IsIndividual());
		CHECK(original.GetRawArray() == selfIndividual.GetRawArray());

		String substr = original.Substring(6, 5); //Will be "World"
		CHECK(!substr.IsIndividual());
		CHECK(original.GetRawArray() == substr.GetRawArray());
		CHECK(substr == STRING_LITERAL(u8"World"));
		CHECK(substr.IndexOf(STRING_LITERAL(u8"Hello")) == -1);
		CHECK(substr.IndexOf(STRING_LITERAL(u8"d")) == 4);

		String individual = substr.ToIndividual();
		CHECK(individual.IsIndividual());
		CHECK(substr.GetRawArray() != individual.GetRawArray());
	}

	TEST_CASE("StartsWith & EndsWith") {
		String target = STRING_LITERAL(u8"伞兵一号卢本伟，准备就绪！");
		CHECK(target.StartsWith(STRING_LITERAL(u8"伞兵")));
		CHECK(!target.StartsWith(STRING_LITERAL(u8"Steam用户")));
		CHECK(target.EndsWith(STRING_LITERAL(u8"准备就绪！")));
		CHECK(!target.EndsWith(STRING_LITERAL(u8"跟我比划比划")));
	}
}