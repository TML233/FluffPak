#include "doctest.h"
#include "Engine/System/Object.h"

using namespace Engine;
TEST_CASE("Reflection") {
	CHECK(Reflection::IsClassExists("::Engine::Object"));
	CHECK(Reflection::IsClassExists("::Engine::ManualObject"));
	CHECK(Reflection::IsClassExists("::Engine::ReferencedObject"));
	CHECK(!Reflection::IsClassExists("::Engine::Fucked"));

	ReflectionClass* cObj = Reflection::GetClass("::Engine::Object");
	ReflectionClass* cMan = Reflection::GetClass("::Engine::ManualObject");
	ReflectionClass* cRef = Reflection::GetClass("::Engine::ReferencedObject");
	CHECK(!cObj->IsInstantiatable());
	CHECK(!cMan->IsInstantiatable());
	CHECK(!cRef->IsInstantiatable());
}
