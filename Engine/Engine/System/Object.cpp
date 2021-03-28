#include "Engine/System/Object.h"
#include "Engine/System/ObjectUtil.h"
#include "Engine/System/String.h"
#include <string>
#include <typeinfo>

namespace Engine {
	// TODO: Change this capacity value to a reasonable one.
	Dictionary<InstanceId, bool> Object::objectLookup{ 100 };

	Object::~Object() {
		objectLookup.Remove(instanceId);
	}

	String Object::ToString() const {
		return typeid(*this).name();
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
	bool ManualObject::IsReferenced() const {
		return false;
	}

	// === REFERENCED OBJECT ===

	ReferencedObject::ReferencedObject() {
		instanceId = InstanceId::Generate(true);
		objectLookup.Add(instanceId, true);
	}
	bool ReferencedObject::IsReferenced() const {
		return true;
	}
	uint32 ReferencedObject::Reference() {
		return referenceCount.Reference();
	}
	uint32 ReferencedObject::Dereference() {
		return referenceCount.Dereference();
	}
	uint32 ReferencedObject::GetReferenceCount() const {
		return referenceCount.Get();
	}
}