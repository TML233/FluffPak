#include "doctest.h"
#include "Engine/System/Variant.h"

using namespace Engine;

TEST_SUITE("Variant") {
	TEST_CASE("Variant") {
		Variant null;
		CHECK(null.GetType() == Variant::Type::Null);
		CHECK(null == Variant());
		CHECK(null != Variant(1));
		CHECK(null != Variant("Null"));

		Variant a = 1;
		CHECK(a.GetType() == Variant::Type::Int64);
		CHECK(a == Variant(1));
		CHECK(a >= Variant(0));
	}
}
