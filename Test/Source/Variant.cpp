#include "doctest.h"
#include "Engine/System/Variant.h"

using namespace Engine;

TEST_SUITE("Variant") {
	TEST_CASE("Variant") {
		Variant null;
		CHECK(null.GetType() == Variant::Type::Null);
		CHECK(null.operator int32() != 1);
		CHECK(null.operator String() == "Null");

		Variant a = 1;
		CHECK(a.GetType() == Variant::Type::Int64);
		CHECK(a.operator int64() == 1);
		CHECK(a.operator int16() == 1);
	}
}
