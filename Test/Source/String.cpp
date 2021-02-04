#include "doctest.h"
#include "Engine/System/String.h"

TEST_CASE("String.Find"){
	String target = "This is a simple example.";
	CHECK(target.Find("fuck") == -1);
	CHECK(target.Find("this") == -1);
	CHECK(target.Find("This") == 0);
	CHECK(target.Find("simple") == 10);
	CHECK(target.Find("is") == 2);
	CHECK(target.Find(".") == 24);
	CHECK(target.Find("example") == 17);
}
