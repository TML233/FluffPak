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
}