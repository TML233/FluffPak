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
	ReflectionClass* Reflection::GetClass(const String& name) {
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

	bool ReflectionClass::IsMethodExists(const String& name) const {
		return methods.ContainsKey(name);
	}

	ReflectionMethod* ReflectionClass::GetMethod(const String& name) const {
		ERR_ASSERT(IsMethodExists(name), u8"Method doesn't exists!", return nullptr);
		return methods.Get(name).GetRaw();
	}

	ReflectionMethod* ReflectionClass::AddMethod(SharedPtr<ReflectionMethod> method) {
		bool succeeded = methods.Add(method->GetName(), method);
		FATAL_ASSERT(succeeded, String::Format(u8"{0}::{1} is already registered!", name, method->GetName()).GetRawArray());
		return method.GetRaw();
	}
#pragma endregion

#pragma region ReflectionMethod
	ReflectionMethod::ReflectionMethod(const String& name, SharedPtr<ReflectionMethodBind> bind) :name(name), bind(bind) {}
	ReflectionMethod::ReflectionMethod(
		const String& name, SharedPtr<ReflectionMethodBind> bind,
		std::initializer_list<String> argumentNames, std::initializer_list<Variant> defaultArguments
	) : name(name), bind(bind), argumentNames(argumentNames), defaultArguments(defaultArguments) {}

	String ReflectionMethod::GetName() const {
		return name;
	}
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

	void ReflectionMethod::SetBind(SharedPtr<ReflectionMethodBind> bind) {
		this->bind = bind;
	}
	SharedPtr<ReflectionMethodBind> ReflectionMethod::GetBind() const {
		return bind;
	}

	List<String>& ReflectionMethod::GetArgumentNameList() {
		return argumentNames;
	}
	List<Variant>& ReflectionMethod::GetDefaultArgumentList() {
		return defaultArguments;
	}

	ReflectionMethod::InvokeResult ReflectionMethod::Invoke(Object* target, const Variant** arguments, int32 argumentCount, Variant& returnValue) const {
		ERR_ASSERT(IsStatic() || target != nullptr, u8"target cannot be nullptr for a non-static method.", return InvokeResult::InvalidObject);
		
		int32 methodArgCount = GetArgumentCount();
		ERR_ASSERT(argumentCount <= methodArgCount, u8"Too many arguments.", return InvokeResult::TooManyArguments);

		int32 leastCount = GetArgumentCount() - defaultArguments.GetCount();
		ERR_ASSERT(argumentCount >= leastCount, u8"Not enough arguments.", return InvokeResult::TooFewArguments);

		return bind->Invoke(target, arguments, argumentCount, defaultArguments, returnValue);
	}
#pragma endregion

}