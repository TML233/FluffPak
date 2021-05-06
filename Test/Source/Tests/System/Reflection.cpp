#include "doctest.h"
#include "Engine/System/Object/Object.h"

using namespace Engine;
TEST_SUITE("Reflection") {
	TEST_CASE("Auto registeration") {
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
		static int32 Fuck(int32 a, int32 b) {
			return a + b;
		}
	};
	TEST_CASE("ReflectionMethodBind") {
		auto foo = ReflectionMethodBindHelper::Create(Test::Fuck);
		CHECK(foo->IsStatic() == true);
		CHECK(foo->GetReturnType() == Variant::Type::Int64);
		CHECK(foo->GetArgumentCount() == 2);

		Variant a = 3;
		Variant b = 4;
		Variant* args[] = { &a,&b };
		List<Variant> defaultArgs{};
		Variant result = foo->Invoke(nullptr, (const Variant**)args, 2, defaultArgs);
		CHECK(result.AsInt64() == 7);
	}


	class Bar :public ManualObject {
		REFLECTION_CLASS(::Bar, ::Engine::ManualObject) {
			REFLECTION_CLASS_STATIC_METHOD(STRL(u8"SetStatic"), SetStatic, { STRL(u8"value") }, { 114514 });
			REFLECTION_CLASS_STATIC_METHOD(STRL(u8"GetStatic"), GetStatic, {}, {});
		}
	public:
		static void SetStatic(int32 value) {
			Bar::value = value;
		}
		static int32 GetStatic() {
			return value;
		}

		static int32 value;
	};
	int32 Bar::value = 999;

	TEST_CASE("ReflectionMethod Static") {
		auto cl = Reflection::GetClass(STRING_LITERAL(u8"::Bar"));
		auto mSet = cl->GetMethod(STRING_LITERAL(u8"SetStatic"));
		auto mGet = cl->GetMethod(STRING_LITERAL(u8"GetStatic"));

#pragma region SetStatic full argument
		{
			Variant argValue = 3;
			Variant* args[1] = { &argValue };

			Variant returnValue = 0;
			auto result = mSet->Invoke(nullptr, (const Variant**)args, 1, returnValue);
			CHECK(result == ReflectionMethod::InvokeResult::OK);
			CHECK(returnValue.GetType() == Variant::Type::Null);

			CHECK(Bar::value == 3);
		}
#pragma endregion

#pragma region SetStatic default argument
		{
			Variant* args[1] = { nullptr };

			Variant returnValue = 0;
			auto result = mSet->Invoke(nullptr, (const Variant**)args, 0, returnValue);
			CHECK(result == ReflectionMethod::InvokeResult::OK);
			CHECK(returnValue.GetType() == Variant::Type::Null);

			CHECK(Bar::value == 114514);
		}
#pragma endregion
	}
}