#include "doctest.h"
#include "Engine/System/Object/Variant.h"
#include "Engine/System/Object/Object.h"

using namespace Engine;

TEST_SUITE("Variant") {
	TEST_CASE("Variant") {
		Variant null;
		CHECK(null.GetType() == Variant::Type::Null);

		Variant a = 1;
		CHECK(a.GetType() == Variant::Type::Int32);
		CHECK(a.AsInt64() == 1);
	}
	TEST_CASE("Vector2") {
		Variant v1 = Vector2(11, 23);
		CHECK(v1.AsVector2() == Vector2(11, 23));
	}
	TEST_CASE("Native") {
		//CHECK(Variant::GetTypeFromNative<bool>::type == Variant::Type::Bool);
		//CHECK(Variant::GetTypeFromNative<ReferencePtr<ReferencedObject>>::type == Variant::Type::Object);

		Variant a = 114514;
		int32 b=Variant::CastToNative<int32>::Cast(a);
		CHECK(b == 114514);
	}
}
