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
		FATAL_ASSERT(succeeded, String::Format(STRING_LITERAL("Method {0}::{1} is already registered!"), name, method->GetName()).GetRawArray());
		return method.GetRaw();
	}

	bool ReflectionClass::RemoveMethod(const String& name) {
		return methods.Remove(name);
	}


	bool ReflectionClass::IsPropertyExists(const String& name) const {
		return properties.ContainsKey(name);
	}

	ReflectionProperty* ReflectionClass::GetProperty(const String& name) const {
		ERR_ASSERT(IsPropertyExists(name), u8"Method doesn't exists!", return nullptr);
		return properties.Get(name).GetRaw();
	}

	ReflectionProperty* ReflectionClass::AddProperty(SharedPtr<ReflectionProperty> prop) {
		bool succeeded = properties.Add(prop->GetName(), prop);
		FATAL_ASSERT(succeeded, String::Format(STRING_LITERAL("Property {0}::{1} is already registered!"), name, prop->GetName()).GetRawArray());
		return prop.GetRaw();
	}

	bool ReflectionClass::RemoveProperty(const String& name) {
		return properties.Remove(name);
	}


	bool ReflectionClass::IsSignalExists(const String& name) const {
		return signals.ContainsKey(name);
	}

	ReflectionSignal* ReflectionClass::GetSignal(const String& name) const {
		ERR_ASSERT(IsSignalExists(name), u8"Signal doesn't exists!", return nullptr);
		return signals.Get(name).GetRaw();
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

	ReflectionMethod::InvokeResult ReflectionMethod::Invoke(Object* target, const Variant** arguments, int32 argumentCount, Variant& returnValue) const {
		ERR_ASSERT(IsStatic() || target != nullptr, u8"target cannot be nullptr for a non-static method.", return InvokeResult::InvalidObject);
		
		int32 methodArgCount = GetArgumentCount();
		ERR_ASSERT(argumentCount <= methodArgCount, u8"Too many arguments.", return InvokeResult::TooManyArguments);

		int32 leastCount = GetArgumentCount() - defaultArguments.GetCount();
		ERR_ASSERT(argumentCount >= leastCount, u8"Not enough arguments.", return InvokeResult::TooFewArguments);

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
	Variant ReflectionProperty::Get(Object* obj) const {
		ERR_ASSERT(CanGet(), u8"The property cannot get.", return Variant());
		
		Variant returnValue;
		auto result = getter->Invoke(obj, nullptr, 0, returnValue);
		
		ERR_ASSERT(result == ReflectionMethod::InvokeResult::OK, u8"Failed to invoke getter.", return Variant());
		return returnValue;
	}
	void ReflectionProperty::Set(Object* obj,const Variant& value) {
		ERR_ASSERT(CanSet(), u8"The property cannot set.", return);

		const Variant* args[1] = { &value };
		Variant returnValue;
		auto result = setter->Invoke(obj, (const Variant**)args, 1, returnValue);

		ERR_ASSERT(result == ReflectionMethod::InvokeResult::OK, u8"Failed to invoke setter.", return);
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