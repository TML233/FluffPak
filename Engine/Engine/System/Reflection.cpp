#include "Engine/System/Reflection.h"

namespace Engine {
	Reflection::ClassData& Reflection::GetData() {
		static ClassData classes{ 30 };
		return classes;
	}
	bool Reflection::IsClassExists(const String& name) {
		return GetData().ContainsKey(name);
	}
	ReflectionClass* Reflection::GetClass(const String& name) {
		ERR_ASSERT(IsClassExists(name), "Class name doesn't exists.", return nullptr);
		return GetData().Get(name).GetRaw();
	}

	String ReflectionClass::GetName() const {
		return name;
	}
	String ReflectionClass::GetParentName() const {
		return parentName;
	}
	bool ReflectionClass::IsInstantiatable() const {
		return instantiable;
	}
	void ReflectionClass::SetInstantiable(bool instantiable) {
		this->instantiable = instantiable;
	}
	UniquePtr<Object> ReflectionClass::Instantiate() const {
		// Does this need to crash?
		ERR_ASSERT(IsInstantiatable(), "Trying to instantiate a class which is not instantiable!", return UniquePtr<Object>(nullptr));

		return (*instantiator)();
	}
}
