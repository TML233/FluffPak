#include "doctest.h"
#include "Engine/System/Object.h"

using namespace Engine;
TEST_CASE("Reflection") {
	CHECK(Reflection::IsClassExists("::Engine::Object"));
	CHECK(Reflection::IsClassExists("::Engine::ManualObject"));
	CHECK(Reflection::IsClassExists("::Engine::ReferencedObject"));
	CHECK(!Reflection::IsClassExists("::Engine::Fucked"));

	const ReflectionClass* cObj = Reflection::GetClass("::Engine::Object");
	const ReflectionClass* cMan = Reflection::GetClass("::Engine::ManualObject");
	const ReflectionClass* cRef = Reflection::GetClass("::Engine::ReferencedObject");
	CHECK(!cObj->IsInstantiatable());
	CHECK(!cMan->IsInstantiatable());
	CHECK(!cRef->IsInstantiatable());

	CHECK(cObj->IsParentOf(cMan));
	CHECK(cObj->IsParentOf(cRef));
	CHECK(cMan->IsChildOf(cObj));
	CHECK(cRef->IsChildOf(cObj));
	CHECK(!cMan->IsChildOf(cRef));
}
