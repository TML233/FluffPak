#include "doctest.h"
#include "Engine/System/String.h"

TEST_CASE("String::Find"){
	String target = "This is a simple example.";
	CHECK(target.IndexOf("fuck") == -1);
	CHECK(target.IndexOf("this") == -1);
	CHECK(target.IndexOf("This") == 0);
	CHECK(target.IndexOf("simple") == 10);
	CHECK(target.IndexOf("is") == 2);
	CHECK(target.IndexOf(".") == 24);
	CHECK(target.IndexOf("example") == 17);

	String cn = u8"我是傻逼";
	CHECK(cn.IndexOf(u8"傻逼") == sizeof(u8"我是") - 1);
	CHECK(cn.IndexOf(u8"我是傻") == 0);
	CHECK(cn.IndexOf(u8"🐴") == -1);

	String emoji = u8"💉💧🐮🍺";
	CHECK(emoji.IndexOf(u8"🐮") == sizeof(u8"💉💧") - 1);
	CHECK(emoji.IndexOf(u8"🐴") == -1);
}
