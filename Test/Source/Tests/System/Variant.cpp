#include "doctest.h"
#include "Engine/System/Variant.h"

using namespace Engine;

TEST_SUITE("Variant") {
	TEST_CASE("Variant") {
		Variant null;
		CHECK(null.GetType() == Variant::Type::Null);
		CHECK(null == Variant());
		CHECK(null != 1);
		CHECK(null != "Null");

		Variant a = 1;
		CHECK(a.GetType() == Variant::Type::Int64);
		CHECK(a == 1);
		CHECK(a >= 0);
	}
	TEST_CASE("Vector2") {
		Variant v1 = Vector2(11, 23);
		CHECK(v1 * 3 == Vector2(33, 69));
		CHECK(v1.AsVector2() == Vector2(11, 23));
	}
}
