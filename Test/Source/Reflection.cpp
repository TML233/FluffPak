#include "doctest.h"
#include "Engine/System/Reflection.h"

namespace Test {
	class Object {
	public:
		REFLECTION_ROOTCLASS(Test::Object) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}
	};

	class Derived :public Object {
		REFLECTION_CLASS(Test::Derived, Test::Object) {
			REFLECTION_CLASS_INSTANTIABLE(true);
		}
	};
}

using namespace Engine;
TEST_CASE("Reflection"){
	CHECK(Reflection::IsClassExists("Test::Object"));
	CHECK(Reflection::IsClassExists("Test::Derived"));
	CHECK(!Reflection::IsClassExists("Test::Fucked"));
	
	ReflectionClass* cObj = Reflection::GetClass("Test::Object");
	ReflectionClass* cDer = Reflection::GetClass("Test::Derived");
	CHECK(!cObj->IsInstantiatable());
	CHECK(cDer->IsInstantiatable());
}
