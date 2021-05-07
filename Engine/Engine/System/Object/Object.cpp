#include "Engine/System/Object/Object.h"
#include "Engine/System/Object/ObjectUtil.h"
#include "Engine/System/String.h"

namespace Engine {
#pragma region Object
	// Change this capacity value to a reasonable one.
	Dictionary<InstanceId, bool> Object::objectLookup{ 100 };

	Object::~Object() {
		objectLookup.Remove(instanceId);
	}

	String Object::ToString() const {
		return String::Format(STRING_LITERAL("{0} ({1})"), GetReflectionClassName(), GetInstanceId().Get());
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
#pragma endregion

#pragma region ManualObject
	ManualObject::ManualObject() {
		instanceId = InstanceId::Generate(false);
		objectLookup.Add(instanceId, true);
	}
	ManualObject::~ManualObject() {}
	bool ManualObject::IsReferenced() const {
		return false;
	}
#pragma endregion

#pragma region ReferencedObject
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
}