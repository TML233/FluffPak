#include "Engine/System/Object/Object.h"
#include "Engine/System/Object/ObjectUtil.h"
#include "Engine/System/String.h"
#include <string>
#include <typeinfo>

namespace Engine {
#pragma region Object

	// TODO: Change this capacity value to a reasonable one.
	Dictionary<InstanceId, bool> Object::objectLookup{ 100 };

	Object::~Object() {
		objectLookup.Remove(instanceId);
	}

	String Object::ToString() const {
		return String::Format(STRING_LITERAL(u8"{0} ({1})"), GetReflectionClassName(), GetInstanceId().Get());
	}
	int32 Object::GetHashCode() const {
		return ObjectUtil::GetHashCode(instanceId.Get());
	}

	InstanceId Object::GetInstanceId() const {
		return instanceId;
	}

	bool Object::IsInstanceValid(InstanceId id) {
		return objectLookup.ContainsKey(id);
	}

	// === MANUAL OBJECT ===

	ManualObject::ManualObject() {
		instanceId = InstanceId::Generate(false);
		objectLookup.Add(instanceId, true);
	}
	ManualObject::~ManualObject() {}
	bool ManualObject::IsReferenced() const {
		return false;
	}

	// === REFERENCED OBJECT ===

	ReferencedObject::ReferencedObject() {
		instanceId = InstanceId::Generate(true);
		objectLookup.Add(instanceId, true);
	}
	ReferencedObject::~ReferencedObject() {}
	bool ReferencedObject::IsReferenced() const {
		return true;
	}
	uint32 ReferencedObject::Reference() const {
		return referenceCount.Reference();
	}
	uint32 ReferencedObject::Dereference() const {
		return referenceCount.Dereference();
	}
	uint32 ReferencedObject::GetReferenceCount() const {
		return referenceCount.Get();
	}

#pragma endregion

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