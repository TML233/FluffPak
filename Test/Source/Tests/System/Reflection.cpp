#include "doctest.h"
#include "Engine/System/Object/Object.h"

using namespace Engine;
TEST_CASE("Reflection") {
	CHECK(Reflection::IsClassExists(u8"::Engine::Object"));
	CHECK(Reflection::IsClassExists(u8"::Engine::ManualObject"));
	CHECK(Reflection::IsClassExists(u8"::Engine::ReferencedObject"));
	CHECK(!Reflection::IsClassExists(u8"::Engine::Fucked"));

	const ReflectionClass* cObj = Reflection::GetClass(u8"::Engine::Object");
	const ReflectionClass* cMan = Reflection::GetClass(u8"::Engine::ManualObject");
	const ReflectionClass* cRef = Reflection::GetClass(u8"::Engine::ReferencedObject");
	CHECK(!cObj->IsInstantiatable());
	CHECK(!cMan->IsInstantiatable());
	CHECK(!cRef->IsInstantiatable());

	CHECK(cObj->IsParentOf(cMan));
	CHECK(cObj->IsParentOf(cRef));
	CHECK(cMan->IsChildOf(cObj));
	CHECK(cRef->IsChildOf(cObj));
	CHECK(!cMan->IsChildOf(cRef));
}
