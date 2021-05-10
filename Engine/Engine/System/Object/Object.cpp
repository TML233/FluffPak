#include "Engine/System/Object/Object.h"
#include "Engine/System/Object/ObjectUtil.h"
#include "Engine/System/String.h"

namespace Engine {
	Invokable::Invokable() :instanceId(InstanceId()), methodName(String::GetEmpty()) {}
	Invokable::Invokable(const InstanceId& object, const String& methodName) :instanceId(object), methodName(methodName) {}
	Invokable::Invokable(Object* object, const String& methodName) : instanceId(object->GetInstanceId()), methodName(methodName) {}
	int32 Invokable::GetHashCode() const {
		return ObjectUtil::GetHashCode(instanceId) ^ ObjectUtil::GetHashCode(methodName);
	}
	bool Invokable::operator==(const Invokable& obj) const {
		return (instanceId == obj.instanceId && methodName == obj.methodName);
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

	bool Object::IsInstanceValid(const InstanceId& id) {
		return objectLookup.ContainsKey(id);
	}
	Object* Object::GetInstance(const InstanceId& id) {
		Object* obj = nullptr;
		objectLookup.TryGet(id, obj);
		return obj;
	}

	bool Object::HasSignal(const String& name) const {
		return Reflection::GetClass(GetReflectionClassName())->HasSignal(name);
	}
	bool Object::HasMethod(const String& name) const {
		return Reflection::GetClass(GetReflectionClassName())->HasMethod(name);
	}
	ReflectionMethod::InvokeResult Object::InvokeMethod(const String& name, const Variant** arguments, int32 argumentCount, Variant& result) {
		ReflectionMethod* method = Reflection::GetClass(GetReflectionClassName())->GetMethod(name);
		ERR_ASSERT(method != nullptr, String::Format(STRING_LITERAL("Method {0}::{1} not found!"), GetReflectionClassName(), name).GetRawArray(), return ReflectionMethod::InvokeResult::InvalidMethod);
		return method->Invoke(this, arguments, argumentCount, result);
	}

	bool Object::IsSignalConnected(const String& signal, const Invokable& invokable) const {
		SharedPtr<SignalConnectionGroup> group;
		bool found = signalConnections.TryGet(signal, group);
		if (!found) {
			return false;
		}
		return group->connections.DoRead()->ContainsKey(invokable);
	}

	ReflectionSignal::ConnectResult Object::ConnectSignal(const String& signal, const Invokable& invokable, ReflectionSignal::ConnectFlag flag) {
		SharedPtr<SignalConnectionGroup> group;
		bool found = signalConnections.TryGet(signal, group);
		// Not found, try to add.
		if (!found) {
			if (HasSignal(signal)) {
				group = SharedPtr<SignalConnectionGroup>::Create();
				signalConnections.Add(signal, group);
			} else {
				ERR_ASSERT(group != nullptr, String::Format(STRING_LITERAL("Signal {0}::{1} doesn't exist."), GetReflectionClassName(), signal).GetRawArray(), return ReflectionSignal::ConnectResult::InvalidSignal);
			}
		}

		ERR_ASSERT(IsInstanceValid(invokable.instanceId), u8"Cannot connect to a non-existing object!", return ReflectionSignal::ConnectResult::InvalidObject);

		group->connections.DoWrite()->Add(invokable, flag);

		return ReflectionSignal::ConnectResult::OK;
	}

	bool Object::DisconnectSignal(const String& signal, const Invokable& invokable) {
		SharedPtr<SignalConnectionGroup> group;
		bool found = signalConnections.TryGet(signal, group);
		if (!found) {
			return false;
		}

		return group->connections.DoWrite()->Remove(invokable);
	}

	bool Object::EmitSignal(const String& signal,const Variant** arguments,int32 argumentCount) {
		SharedPtr<SignalConnectionGroup> group;
		bool found = signalConnections.TryGet(signal, group);
		if (!found) {
			return false;
		}

		// Get the COW Dictionary.
		// When other connection functions are operating the dictionary, COW will make a new copy.
		SignalConnectionGroup::ConnectionsType cow = group->connections;
		
		// Iterate the dictionary.
		for (const auto& entry : *(cow.DoRead())) {
			const Invokable& invokable = entry.key;
			ReflectionSignal::ConnectFlag flag = entry.value;
			

			Object* target = Object::GetInstance(invokable.instanceId);

			// Disconnect the connection if the object doesn't exists anymore.
			if (target == nullptr) {
				ERR_MSG(String::Format(
					STRING_LITERAL("Signal {0}::{1}, Connection {2}::{3}. Object doesn't exist anymore."),
					GetReflectionClassName(), signal, invokable.instanceId.Get(), invokable.methodName
				).GetRawArray());

				DisconnectSignal(signal, invokable);
				continue;
			}

			Variant temp;
			target->InvokeMethod(invokable.methodName, arguments, argumentCount, temp);
		}

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