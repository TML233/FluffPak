#include "Engine/System/Object/Object.h"
#include "Engine/System/Object/ObjectUtil.h"
#include "Engine/System/String.h"

namespace Engine {
	int32 InstanceMethod::GetHashCode() const {
		return ObjectUtil::GetHashCode(object) ^ ObjectUtil::GetHashCode(methodName);
	}
	bool InstanceMethod::operator==(const InstanceMethod& obj) const {
		return (object == obj.object && methodName == obj.methodName);
	}

#pragma region Object
	// Change this capacity value to a reasonable one.
	Dictionary<InstanceId, Object*> Object::objectLookup{ 100 };

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
	Object* Object::GetInstance(InstanceId id) {
		Object* obj = nullptr;
		objectLookup.TryGet(id, obj);
		return obj;
	}

	bool Object::IsSignalConnected(const String& signal, const InstanceMethod& method) const {
		SharedPtr<SignalGroup> group;
		bool found = signalGroups.TryGet(signal, group);
		if (!found) {
			return false;
		}
		return group->connections.ContainsKey(method);
	}

	Object::SignalConnectResult Object::ConnectSignal(const String& signal, const InstanceMethod& method) {
		SharedPtr<SignalGroup> group;
		
		bool found = signalGroups.TryGet(signal, group);
		// Not found, try to add.
		if (!found) {
			auto cl = Reflection::GetClass(GetReflectionClassName());
			FATAL_ASSERT(cl != nullptr, u8"ReflectionClass not found! Be sure to register the class when declaring!");
			
			if (cl->IsSignalExists(signal)) {
				group = SharedPtr<SignalGroup>::Create();
				signalGroups.Add(signal, group);
			} else {
				ERR_ASSERT(group != nullptr, u8"Signal doesn't exist.", return SignalConnectResult::InvalidSignal);
			}
		}

		ERR_ASSERT(IsInstanceValid(method.object), u8"Cannot connect to a non-existing object!", return SignalConnectResult::InvalidObject);

		group->connections.Set(method, true);

		return SignalConnectResult::OK;
	}

	bool Object::DisconnectSignal(const String& signal, const InstanceMethod& method) {
		SharedPtr<SignalGroup> group;

		bool found = signalGroups.TryGet(signal, group);
		if (!found) {
			return false;
		}

		return group->connections.Remove(method);
	}

	bool Object::EmitSignal(const String& signal) {
		SharedPtr<SignalGroup> group;

		bool found = signalGroups.TryGet(signal, group);
		if (!found) {
			return false;
		}

		// TODO: Iterate the dictionary.

		return true;
	}


#pragma endregion

#pragma region ManualObject
	ManualObject::ManualObject() {
		instanceId = InstanceId::Generate(false);
		objectLookup.Add(instanceId, this);
	}
	ManualObject::~ManualObject() {}
	bool ManualObject::IsReferenced() const {
		return false;
	}
#pragma endregion

#pragma region ReferencedObject
	ReferencedObject::ReferencedObject() {
		instanceId = InstanceId::Generate(true);
		objectLookup.Add(instanceId, this);
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