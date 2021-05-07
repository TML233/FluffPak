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
		int32 FuckWhy(int32 a, int32 b) const {
			return a + b;
		}
	};
	TEST_CASE("ReflectionMethodBind Static") {
		auto foo = ReflectionMethodBindHelper::Create(Test::Fuck);
		CHECK(foo->IsStatic() == true);
		CHECK(foo->GetReturnType() == Variant::Type::Int64);
		CHECK(foo->GetArgumentCount() == 2);

		Variant a = 3;
		Variant b = 4;
		Variant* args[] = { &a,&b };
		List<Variant> defaultArgs{};
		Variant returnValue;
		ReflectionMethod::InvokeResult result = foo->Invoke(nullptr, (const Variant**)args, 2, defaultArgs, returnValue);
		CHECK(result == ReflectionMethod::InvokeResult::OK);
		CHECK(returnValue.AsInt64() == 7);
	}

	TEST_CASE("ReflectionMethodBind Object") {
		auto foo = ReflectionMethodBindHelper::Create(&Test::FuckWhy);
		CHECK(foo->IsStatic() == false);
		CHECK(foo->IsConst() == true);
	}

	class Bar :public ManualObject {
		REFLECTION_CLASS(::Bar, ::Engine::ManualObject) {
			REFLECTION_STATIC_METHOD(STRL("SetStatic"), SetStatic, { STRL("value") }, { 114514 });
			REFLECTION_STATIC_METHOD(STRL("GetStatic"), GetStatic, {}, {});

			REFLECTION_METHOD(STRL("Set"), Bar::Set, { STRL("value") }, { STRL("YJSP") });
			REFLECTION_METHOD(STRL("Get"), Bar::Get, {}, {});

			REFLECTION_PROPERTY(STRL("Value"), STRL("Get"), STRL("Set"));

			REFLECTION_SIGNAL(STRL("SignalTest"), { 
				SIGARGD(STRL("arg0"),Variant::Type::Object,STRL("::Bar")) 
			});
		}

	public:
		static int32 staticValue;
		static void SetStatic(int32 value) {
			Bar::staticValue = value;
		}
		static int32 GetStatic() {
			return staticValue;
		}

		String value;
		void Set(const String& value) {
			this->value = value;
		}
		String Get() const {
			return value;
		}

	};
	int32 Bar::staticValue = 999;

	TEST_CASE("ReflectionMethod") {
		auto cl = Reflection::GetClass(STRING_LITERAL("::Bar"));
		auto mSetStatic = cl->GetMethod(STRING_LITERAL("SetStatic"));

#pragma region SetStatic full argument
		{
			Variant argValue = 3;
			Variant* args[1] = { &argValue };

			Variant returnValue = 0;
			auto result = mSetStatic->Invoke(nullptr, (const Variant**)args, 1, returnValue);
			CHECK(result == ReflectionMethod::InvokeResult::OK);
			CHECK(returnValue.GetType() == Variant::Type::Null);

			CHECK(Bar::staticValue == 3);
		}
#pragma endregion

#pragma region SetStatic default argument
		{
			Variant* args[1] = { nullptr };

			Variant returnValue = 0;
			auto result = mSetStatic->Invoke(nullptr, (const Variant**)args, 0, returnValue);
			CHECK(result == ReflectionMethod::InvokeResult::OK);
			CHECK(returnValue.GetType() == Variant::Type::Null);

			CHECK(Bar::staticValue == 114514);
		}
#pragma endregion

		auto mGetStatic = cl->GetMethod(STRING_LITERAL("GetStatic"));

#pragma region GetStatic
		{
			Variant returnValue = 0;
			auto result = mGetStatic->Invoke(nullptr, nullptr, 0, returnValue);
			CHECK(result == ReflectionMethod::InvokeResult::OK);
			CHECK(returnValue.AsInt64() == 114514);
		}
#pragma endregion

		Bar obj;
		auto mSet = cl->GetMethod(STRING_LITERAL("Set"));

#pragma region Set
		{
			Variant value = STRING_LITERAL("MUR");
			Variant* args[1] = { &value };
			Variant returnValue = 0;

			auto result = mSet->Invoke(&obj, (const Variant**)args, 1, returnValue);

			CHECK(result == ReflectionMethod::InvokeResult::OK);
			CHECK(returnValue.GetType() == Variant::Type::Null);
		}
#pragma endregion
		
		auto mGet = cl->GetMethod(STRING_LITERAL("Get"));
#pragma region Get
		{
			Variant returnValue = 0;
			auto result = mGet->Invoke(&obj, nullptr, 0, returnValue);
			CHECK(result == ReflectionMethod::InvokeResult::OK);
			CHECK(returnValue.AsString() == STRING_LITERAL("MUR"));
		}
#pragma endregion
	}

	TEST_CASE("ReflectionProperty") {
		auto cl = Reflection::GetClass(STRL("::Bar"));
		auto prop = cl->GetProperty(STRL("Value"));

		CHECK(prop->GetType() == Variant::Type::String);

		Bar obj;

		String value = STRL("I AM SB");
		prop->Set(&obj, value);
		
		CHECK(obj.value == value);
		CHECK(prop->Get(&obj).AsString() == value);
	}
}