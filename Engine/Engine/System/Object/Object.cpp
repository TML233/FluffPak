#include "Engine/System/Object/Object.h"
#include "Engine/System/Object/ObjectUtil.h"
#include "Engine/System/String.h"

namespace Engine {
	Invokable::Invokable() :instanceId(InstanceId()), methodName(String::GetEmpty()) {}
	Invokable::Invokable(const InstanceId& object, const String& methodName) :instanceId(object), methodName(methodName) {}
	Invokable::Invokable(Object* object, const String& methodName) : instanceId(object == nullptr ? InstanceId() : object->GetInstanceId()), methodName(methodName) {}
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

	bool Object::HasProperty(const String& name) const {
		ReflectionClass* c = nullptr;
		if (!Reflection::TryGetClass(GetReflectionClassName(), c)) {
			return false;
		}
		return c->HasPropertyInTree(name);
	}
	bool Object::CanPropertyGet(const String& name) const {
		ReflectionClass* c = nullptr;
		if (!Reflection::TryGetClass(GetReflectionClassName(), c)) {
			return false;
		}
		ReflectionProperty* prop = nullptr;
		if (!c->TryGetProperty(name, prop)) {
			return false;
		}
		return prop->CanGet();
	}
	bool Object::CanPropertySet(const String& name) const {
		ReflectionClass* c = nullptr;
		if (!Reflection::TryGetClass(GetReflectionClassName(), c)) {
			return false;
		}
		ReflectionProperty* prop = nullptr;
		if (!c->TryGetProperty(name, prop)) {
			return false;
		}
		return prop->CanSet();
	}
	ResultCode Object::GetPropertyValue(const String& name, Variant& result) const {
		ReflectionClass* c = nullptr;
		if (!Reflection::TryGetClass(GetReflectionClassName(), c)) {
			ERR_MSG(String::Format(STRL("Class \"{0}\" not found!"), name).GetRawArray());
			return ResultCode::NotFound;
		}
		ReflectionProperty* prop = nullptr;
		if (!c->TryGetPropertyInTree(name, prop)) {
			ERR_MSG(String::Format(STRL("Property \"{0}\" not found!"), name).GetRawArray());
			return ResultCode::NotFound;
		}
		return prop->Get(this, result);
	}
	ResultCode Object::SetPropertyValue(const String& name,const Variant& value) {
		ReflectionClass* c = nullptr;
		if (!Reflection::TryGetClass(GetReflectionClassName(), c)) {
			ERR_MSG(String::Format(STRL("Class \"{0}\" not found!"), name).GetRawArray());
			return ResultCode::NotFound;
		}
		ReflectionProperty* prop = nullptr;
		if (!c->TryGetPropertyInTree(name, prop)) {
			ERR_MSG(String::Format(STRL("Property \"{0}\" not found!"), name).GetRawArray());
			return ResultCode::NotFound;
		}
		return prop->Set(this, value);
	}

	bool Object::HasMethod(const String& name) const {
		ReflectionClass* c = nullptr;
		if (!Reflection::TryGetClass(GetReflectionClassName(), c)) {
			return false;
		}
		return c->HasMethodInTree(name);
	}
	ResultCode Object::InvokeMethod(const String& name, const Variant** arguments, int32 argumentCount, Variant& result) {
		ReflectionClass* c = nullptr;
		if (!Reflection::TryGetClass(GetReflectionClassName(), c)) {
			ERR_MSG(String::Format(STRL("Class \"{0}\" not found!"), name).GetRawArray());
			return ResultCode::NotFound;
		}
		ReflectionMethod* method = nullptr;
		if (!c->TryGetMethodInTree(name, method)) {
			ERR_MSG(String::Format(STRING_LITERAL("Method {0}::{1} not found!"), GetReflectionClassName(), name).GetRawArray());
			return ResultCode::NotFound;
		}
		return method->Invoke(this, arguments, argumentCount, result);
	}

	bool Object::HasSignal(const String& name) const {
		ReflectionClass* c = nullptr;
		if (!Reflection::TryGetClass(GetReflectionClassName(), c)) {
			return false;
		}
		return c->HasSignalInTree(name);
	}
	bool Object::IsSignalConnected(const String& signal, const Invokable& invokable) const {
		if (extraData == nullptr) {
			return false;
		}
		SharedPtr<ExtraData::SignalConnectionGroup> group;
		bool found = extraData->signalConnections.TryGet(signal, group);
		if (!found) {
			return false;
		}
		return group->connections.DoRead()->ContainsKey(invokable);
	}
	ResultCode Object::ConnectSignal(const String& signal, const Invokable& invokable, const Variant** extraArguments,int32 extraArgumentCount, ReflectionSignal::ConnectFlag flag) {
		if (extraData == nullptr) {
			extraData = UniquePtr<ExtraData>::Create();
		}
		SharedPtr<ExtraData::SignalConnectionGroup> group;
		bool found = extraData->signalConnections.TryGet(signal, group);
		// Not found, try to add.
		if (!found) {
			if (HasSignal(signal)) {
				group = SharedPtr<ExtraData::SignalConnectionGroup>::Create();
				extraData->signalConnections.Add(signal, group);
			} else {
				ERR_ASSERT(group != nullptr, String::Format(STRING_LITERAL("Signal {0}::{1} doesn't exist."), GetReflectionClassName(), signal).GetRawArray(), return ResultCode::NotFound);
			}
		}

		ERR_ASSERT(IsInstanceValid(invokable.instanceId), u8"Cannot connect to a non-existing object!", return ResultCode::InvalidObject);

		auto data = SharedPtr<ExtraData::SignalConnection>::Create();
		data->flag = flag;
		for (int32 i = 0; i < extraArgumentCount; i += 1) {
			data->extraArguments.Add(*(extraArguments[i]));
		}
		group->connections.DoWrite()->Add(invokable, data);

		return ResultCode::OK;
	}
	bool Object::DisconnectSignal(const String& signal, const Invokable& invokable) {
		if (extraData == nullptr) {
			return false;
		}
		SharedPtr<ExtraData::SignalConnectionGroup> group;
		bool found = extraData->signalConnections.TryGet(signal, group);
		if (!found) {
			return false;
		}
		return group->connections.DoWrite()->Remove(invokable);
	}
	bool Object::EmitSignal(const String& signal,const Variant** arguments,int32 argumentCount) {
		if (extraData == nullptr) {
			return false;
		}
		SharedPtr<ExtraData::SignalConnectionGroup> group;
		bool found = extraData->signalConnections.TryGet(signal, group);
		if (!found) {
			return false;
		}

		// Get the COW Dictionary.
		// When other connection functions are operating the dictionary, COW will make a new copy.
		ExtraData::SignalConnectionGroup::ConnectionsType cow = group->connections;
		
		// Iterate the dictionary.
		for (const auto& entry : *(cow.DoRead())) {
			const Invokable& invokable = entry.key;
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

			//ReflectionSignal::ConnectFlag flag = entry.value->flag;
			const auto& extraArgs = entry.value->extraArguments;
			Variant tempReturn;
			int32 extraArgCount = extraArgs.GetCount();
			if(extraArgCount == 0){
				// Directly invoke when no extra arguments.
				target->InvokeMethod(invokable.methodName, arguments, argumentCount, tempReturn);
			} else {
				int32 newArgCount = argumentCount + extraArgCount;
				auto newArgs = UniquePtr<const Variant* []>::Create(newArgCount);
				for (int i = 0; i < argumentCount; i += 1) {
					newArgs.GetRaw()[i] = arguments[i];
				}
				for (int i = 0; i < extraArgCount; i += 1) {
					newArgs.GetRaw()[argumentCount + i] = extraArgs.GetRawElementPtr() + i;
				}
				target->InvokeMethod(invokable.methodName, newArgs.GetRaw(), newArgCount, tempReturn);
			}
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