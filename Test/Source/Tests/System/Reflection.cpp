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
class Test {
public:
	static int32 Fuck(int32 a,int32 b) {
		return a + b;
	}
};
TEST_CASE("ReflectionMethodBind"){
	auto foo = MEMNEW(ReflectionMethodBindStaticReturn(Test::Fuck));
	CHECK(foo->IsStatic() == true);
	CHECK(foo->GetReturnType() == Variant::Type::Int64);
	CHECK(foo->GetArgumentCount() == 2);

	Variant a = 3;
	Variant b = 4;
	Variant* args[] = { &a,&b };
	List<Variant> defaultArgs{};
	Variant result=foo->Invoke(nullptr, (const Variant**)args, 2, defaultArgs);
	CHECK(result.AsInt64() == 7);
}
