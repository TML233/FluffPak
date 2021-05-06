#include "Engine/System/Object/Reflection.h"
#include "Engine/System/Object/Object.h"

namespace Engine{
#pragma region Reflection
	Reflection::ClassData& Reflection::GetData() {
		static ClassData classes{ 30 };
		return classes;
	}
	bool Reflection::IsClassExists(const String& name) {
		return GetData().ContainsKey(name);
	}
	ReflectionClass* Reflection::AddClass(const String& name, const String& parent) {
		SharedPtr<ReflectionClass> data = SharedPtr<ReflectionClass>::Create();

		data->name = name;
		data->parentName = parent;

		return (GetData().Add(name, data) ? data.GetRaw() : nullptr);
	}
	const ReflectionClass* Reflection::GetClass(const String& name) {
		ERR_ASSERT(IsClassExists(name), u8"Class name doesn't exists.", return nullptr);
		return GetData().Get(name).GetRaw();
	}
#pragma endregion

#pragma region ReflectionClass
	String ReflectionClass::GetName() const {
		return name;
	}
	String ReflectionClass::GetParentName() const {
		return parentName;
	}
	bool ReflectionClass::IsChildOf(const ReflectionClass* target) const {
		if (target == nullptr) {
			return false;
		}
		const ReflectionClass* current = this;
		do {
			if (current == target) {
				return true;
			}
			current = (Reflection::IsClassExists(current->parentName) ? Reflection::GetClass(current->parentName) : nullptr);
		} while (current != nullptr);
		return false;
	}
	bool ReflectionClass::IsParentOf(const ReflectionClass* target) const {
		if (target == nullptr) {
			return false;
		}
		return target->IsChildOf(this);
	}
	bool ReflectionClass::IsInstantiatable() const {
		return instantiable;
	}
	void ReflectionClass::SetInstantiable(bool instantiable) {
		this->instantiable = instantiable;
	}
	UniquePtr<Object> ReflectionClass::Instantiate() const {
		ERR_ASSERT(IsInstantiatable(), u8"Trying to instantiate a class which is not instantiable!", return UniquePtr<Object>(nullptr));

		return (*instantiator)();
	}
#pragma endregion

#pragma region ReflectionMethod
	bool ReflectionMethod::IsConst() const {
		return bind->IsConst();
	}
	bool ReflectionMethod::IsStatic() const {
		return bind->IsStatic();
	}
	int32 ReflectionMethod::GetArgumentCount() const {
		return bind->GetArgumentCount();
	}
	Variant::Type ReflectionMethod::GetReturnType() const {
		return bind->GetReturnType();
	}
	ReflectionMethod::InvokeResult ReflectionMethod::Invoke(Object* target, const Variant** arguments, int32 argumentCount, Variant& returnValue) const {
		ERR_ASSERT(IsStatic() || target != nullptr, u8"target cannot be nullptr for a non-static method.", return InvokeResult::InvalidObject);
		
		int32 methodArgCount = GetArgumentCount();
		ERR_ASSERT(argumentCount <= methodArgCount, u8"Too many arguments.", return InvokeResult::TooManyArguments);

		int32 leastCount = GetArgumentCount() - defaultArguments.GetCount();
		ERR_ASSERT(argumentCount >= leastCount, u8"Not enough arguments.", return InvokeResult::TooFewArguments);

		returnValue = bind->Invoke(target, arguments, argumentCount, defaultArguments);

		return InvokeResult::OK;
	}
#pragma endregion

}