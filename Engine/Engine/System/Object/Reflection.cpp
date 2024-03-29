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
	bool Reflection::TryGetClass(const String& name, ReflectionClass*& result) {
		SharedPtr<ReflectionClass> intermediate;
		if (GetData().TryGet(name, intermediate)) {
			result = intermediate.GetRaw();
			return true;
		} else {
			return false;
		}
	}
	bool Reflection::TryGetClass(const String& name, const ReflectionClass*& result) {
		SharedPtr<ReflectionClass> intermediate;
		if (GetData().TryGet(name, intermediate)) {
			result = intermediate.GetRaw();
			return true;
		} else {
			return false;
		}
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
			if (!Reflection::TryGetClass(current->parentName, current)) {
				current = nullptr;
			}
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


	bool ReflectionClass::HasMethod(const String& name) const {
		return methods.ContainsKey(name);
	}
	bool ReflectionClass::HasMethodInTree(const String& name) const {
		const ReflectionClass* current = this;
		do {
			if (current->HasMethod(name)) {
				return true;
			}
			if (!Reflection::TryGetClass(current->parentName, current)) {
				current = nullptr;
			}
		} while (current != nullptr);
		return false;
	}
	bool ReflectionClass::TryGetMethod(const String& name, ReflectionMethod*& result) const {
		SharedPtr<ReflectionMethod> intermediate;
		if (methods.TryGet(name, intermediate)) {
			result = intermediate.GetRaw();
			return true;
		} else {
			return false;
		}
	}
	ReflectionMethod* ReflectionClass::GetMethodOrNull(const String& name) const {
		ReflectionMethod* ptr = nullptr;
		TryGetMethod(name, ptr);
		return ptr;
	}
	bool ReflectionClass::TryGetMethodInTree(const String& name, ReflectionMethod*& result) const {
		const ReflectionClass* current = this;
		do {
			if (current->TryGetMethod(name, result)) {
				return true;
			}
			if (!Reflection::TryGetClass(current->parentName, current)) {
				current = nullptr;
			}
		} while (current != nullptr);
		return false;
	}
	ReflectionMethod* ReflectionClass::AddMethod(SharedPtr<ReflectionMethod> method) {
		bool succeeded = methods.Add(method->GetName(), method);
		FATAL_ASSERT(succeeded, String::Format(STRING_LITERAL("Method {0}::{1} is already registered!"), name, method->GetName()).GetRawArray());
		return method.GetRaw();
	}
	bool ReflectionClass::RemoveMethod(const String& name) {
		return methods.Remove(name);
	}


	bool ReflectionClass::HasProperty(const String& name) const {
		return properties.ContainsKey(name);
	}
	bool ReflectionClass::HasPropertyInTree(const String& name) const {
		const ReflectionClass* current = this;
		do {
			if (current->HasProperty(name)) {
				return true;
			}
			if (!Reflection::TryGetClass(current->parentName, current)) {
				current = nullptr;
			}
		} while (current != nullptr);
		return false;
	}
	bool ReflectionClass::TryGetProperty(const String& name, ReflectionProperty*& result) const {
		SharedPtr<ReflectionProperty> intermediate;
		if (properties.TryGet(name, intermediate)) {
			result = intermediate.GetRaw();
			return true;
		} else {
			return false;
		}
	}
	bool ReflectionClass::TryGetPropertyInTree(const String& name, ReflectionProperty*& result) const {
		const ReflectionClass* current = this;
		do {
			if (current->TryGetProperty(name, result)) {
				return true;
			}
			if (!Reflection::TryGetClass(current->parentName, current)) {
				current = nullptr;
			}
		} while (current != nullptr);
		return false;
	}
	ReflectionProperty* ReflectionClass::AddProperty(SharedPtr<ReflectionProperty> prop) {
		bool succeeded = properties.Add(prop->GetName(), prop);
		FATAL_ASSERT(succeeded, String::Format(STRING_LITERAL("Property {0}::{1} is already registered!"), name, prop->GetName()).GetRawArray());
		return prop.GetRaw();
	}
	bool ReflectionClass::RemoveProperty(const String& name) {
		return properties.Remove(name);
	}


	bool ReflectionClass::HasSignal(const String& name) const {
		return signals.ContainsKey(name);
	}
	bool ReflectionClass::HasSignalInTree(const String& name) const {
		const ReflectionClass* current = this;
		do {
			if (current->HasSignal(name)) {
				return true;
			}
			if (!Reflection::TryGetClass(current->parentName, current)) {
				current = nullptr;
			}
		} while (current != nullptr);
		return false;
	}
	bool ReflectionClass::TryGetSignal(const String& name, ReflectionSignal*& result) const {
		SharedPtr<ReflectionSignal> intermediate;
		if (signals.TryGet(name, intermediate)) {
			result = intermediate.GetRaw();
			return true;
		} else {
			return false;
		}
	}
	bool ReflectionClass::TryGetSignalInTree(const String& name, ReflectionSignal*& result) const {
		const ReflectionClass* current = this;
		do {
			if (current->TryGetSignal(name, result)) {
				return true;
			}
			if (!Reflection::TryGetClass(current->parentName, current)) {
				current = nullptr;
			}
		} while (current != nullptr);
		return false;
	}
	ReflectionSignal* ReflectionClass::AddSignal(SharedPtr<ReflectionSignal> signal) {
		bool succeeded = signals.Add(signal->GetName(), signal);
		FATAL_ASSERT(succeeded, String::Format(STRING_LITERAL("Signal {0}::{1} is already registered!"), name, signal->GetName()).GetRawArray());
		return signal.GetRaw();
	}
	bool ReflectionClass::RemoveSignal(const String& name) {
		return signals.Remove(name);
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

	ResultCode ReflectionMethod::Invoke(Object* target, const Variant** arguments, int32 argumentCount, Variant& returnValue) const {
		ERR_ASSERT(IsStatic() || target != nullptr, u8"target cannot be nullptr for a non-static method.", return ResultCode::InvalidObject);
		
		int32 methodArgCount = GetArgumentCount();
		ERR_ASSERT(argumentCount <= methodArgCount, u8"Too many arguments.", return ResultCode::TooManyArguments);

		int32 leastCount = GetArgumentCount() - defaultArguments.GetCount();
		ERR_ASSERT(argumentCount >= leastCount, u8"Not enough arguments.", return ResultCode::TooFewArguments);

		return bind->Invoke(target, arguments, argumentCount, defaultArguments, returnValue);
	}
#pragma endregion

#pragma region ReflectionProperty
	ReflectionProperty::ReflectionProperty(
		const String& name,
		ReflectionMethod* getter, ReflectionMethod* setter,
		Hint hint, const String& hintText
	) :name(name), getter(getter), setter(setter), hint(hint), hintText(hintText) {}

	String ReflectionProperty::GetName() const {
		return name;
	}
	Variant::Type ReflectionProperty::GetType() const {
		if (!CanGet()) {
			return Variant::Type::Null;
		}
		return getter->GetReturnType();
	}
	bool ReflectionProperty::CanGet() const {
		return getter != nullptr;
	}
	bool ReflectionProperty::CanSet() const {
		return setter != nullptr;
	}
	ResultCode ReflectionProperty::Get(const Object* obj,Variant& result) const {
		ERR_ASSERT(CanGet(), u8"The property cannot get.", return ResultCode::NotSupported);
		
		auto invokeResult = getter->Invoke(const_cast<Object*>(obj), nullptr, 0, result);
		
		ERR_ASSERT(invokeResult == ResultCode::OK, u8"Failed to invoke getter.", return invokeResult);
		return ResultCode::OK;
	}
	ResultCode ReflectionProperty::Set(Object* obj,const Variant& value) {
		ERR_ASSERT(CanSet(), u8"The property cannot set.", return ResultCode::NotSupported);

		const Variant* args[1] = { &value };
		Variant returnValue;
		auto result = setter->Invoke(obj, (const Variant**)args, 1, returnValue);

		ERR_ASSERT(result == ResultCode::OK, u8"Failed to invoke setter.", return result);
		return ResultCode::OK;
	}

	ReflectionMethod* ReflectionProperty::GetGetter() const {
		return getter;
	}
	void ReflectionProperty::SetGetter(ReflectionMethod* method) {
		getter = method;
	}
	ReflectionMethod* ReflectionProperty::GetSetter() const {
		return setter;
	}
	void ReflectionProperty::SetSetter(ReflectionMethod* method) {
		setter = method;
	}

	ReflectionProperty::Hint ReflectionProperty::GetHint() const {
		return hint;
	}
	void ReflectionProperty::SetHint(Hint hint) {
		this->hint = hint;
	}
	String ReflectionProperty::GetHintText() const {
		return hintText;
	}
	void ReflectionProperty::SetHintText(const String& hintText) {
		this->hintText = hintText;
	}
#pragma endregion

#pragma region ReflectionSignal
	ReflectionSignal::ArgumentInfo::ArgumentInfo() :name(STRING_LITERAL("error")), type(Variant::Type::Null), detailedClass(String::GetEmpty()) {}
	ReflectionSignal::ArgumentInfo::ArgumentInfo(const String& name, Variant::Type type, const String& detailedClass) :name(name), type(type), detailedClass(detailedClass) {}

	String ReflectionSignal::GetName() const {
		return name;
	}
	int32 ReflectionSignal::GetArgumentCount() const {
		return arguments.GetCount();
	}
	ReflectionSignal::ArgumentInfo ReflectionSignal::GetArgument(int32 index) const {
		ERR_ASSERT(index >= 0 && index < GetArgumentCount(), u8"index out of bounds.", return ArgumentInfo(STRING_LITERAL("error"), Variant::Type::Null));
		return arguments.Get(index);
	}

	ReflectionSignal::ReflectionSignal(String name, std::initializer_list<ArgumentInfo> arguments) :name(name), arguments(arguments) {}
#pragma endregion

}