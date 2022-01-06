#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Thread/Atomic.h"
#include "Engine/System/Object/InstanceId.h"
#include "Engine/System/Collection/Dictionary.h"
#include "Engine/System/Object/Reflection.h"
#include "Engine/System/Memory/CopyOnWrite.h"

namespace Engine{
	struct Invokable final {
		Invokable();
		Invokable(const InstanceId& object, const String& methodName);
		Invokable(Object* object, const String& methodName);

		InstanceId instanceId;
		String methodName;
		int32 GetHashCode() const;
		bool operator==(const Invokable& obj) const;
	};

	// Inherit ManualManagement or AutoManagement instead.
	class Object {
		REFLECTION_ROOTCLASS(::Engine::Object) {
			REFLECTION_CLASS_INSTANTIABLE(false);

			REFLECTION_METHOD(STRL("ToString"), Object::ToString, ARGLIST(), ARGLIST());
			
		}

	public:
		static bool IsInstanceValid(const InstanceId& id);
		static Object* GetInstance(const InstanceId& id);

		virtual ~Object() = 0;

		// Indicates if current object is a ReferencedObject
		virtual bool IsReferenced() const = 0;

		virtual String ToString() const;

		// Get hash code. Hash code is not used to identify objects!
		// Override this if the object indicates value.
		virtual int32 GetHashCode() const;

		InstanceId GetInstanceId() const;

#pragma region Reflection
		bool HasProperty(const String& name) const;
		bool CanPropertySet(const String& name) const;
		bool CanPropertyGet(const String& name) const;
		ResultCode GetPropertyValue(const String& name, Variant& result) const;
		ResultCode SetPropertyValue(const String& name, const Variant& value);

		bool HasMethod(const String& name) const;
		ResultCode InvokeMethod(const String& name, const Variant** arguments, int32 argumentCount, Variant& result);
		 
		bool HasSignal(const String& name) const;
		bool IsSignalConnected(const String& signal, const Invokable& invokable) const;
		ResultCode ConnectSignal(const String& signal, const Invokable& invokable, ReflectionSignal::ConnectFlag flag = ReflectionSignal::ConnectFlag::Null);
		bool DisconnectSignal(const String& signal, const Invokable& invokable);
		bool EmitSignal(const String& signal,const Variant** arguments,int32 argumentCount);
#pragma endregion

	protected:
		static Dictionary<InstanceId, Object*> objectLookup;
		InstanceId instanceId;

	private:
		struct SignalConnectionGroup {
			using ConnectionsType = CopyOnWrite<Dictionary<Invokable, ReflectionSignal::ConnectFlag>>;
			ConnectionsType connections = ConnectionsType::Create();
		};
		Dictionary<String, SharedPtr<SignalConnectionGroup>> signalConnections;
	};

	// Represents a Object which its memory management is done by the user.
	class ManualObject :public Object {
		REFLECTION_CLASS(::Engine::ManualObject,::Engine::Object) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		ManualObject();
		virtual ~ManualObject() = 0;
		bool IsReferenced() const override;
	};

	// Represents a Object which its memory management is done automatically via smart pointers.
	class ReferencedObject :public Object {
		REFLECTION_CLASS(::Engine::ReferencedObject, ::Engine::Object) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		ReferencedObject();
		virtual ~ReferencedObject() = 0;

		bool IsReferenced() const override;
		uint32 Reference() const;
		uint32 Dereference() const;
		uint32 GetReferenceCount() const;
	private:
		mutable ReferenceCount referenceCount;
	};
}