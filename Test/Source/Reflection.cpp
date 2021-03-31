#include "doctest.h"
#include "Engine/System/Reflection.h"

namespace Test {
	class Object {
	public:
		REFLECTION_CLASS_ROOT(Test::Object);
	};

	class Derived :public Object {
		REFLECTION_CLASS(Test::Derived, Test::Object);
	};
}

using namespace Engine;
TEST_CASE("Reflection"){
	CHECK(Reflection::IsClassExists("Test::Object"));
	CHECK(Reflection::IsClassExists("Test::Derived"));
	CHECK(!Reflection::IsClassExists("Test::Fucked"));
}
