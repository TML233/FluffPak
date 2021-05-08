#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Atomic.h"
#include "Engine/System/Object/InstanceId.h"
#include "Engine/Collection/Dictionary.h"
#include "Engine/System/Object/Reflection.h"

namespace Engine{
	struct InstanceMethod final {
		InstanceMethod();
		InstanceMethod(const InstanceId& object, const String& methodName);
		InstanceMethod(Object* object, const String& methodName);

		InstanceId instanceId;
		String methodName;
		int32 GetHashCode() const;
		bool operator==(const InstanceMethod& obj) const;
	};

	// Inherit ManualManagement or AutoManagement instead.
	class Object {
		REFLECTION_ROOTCLASS(::Engine::Object) {
			REFLECTION_CLASS_INSTANTIABLE(false);

			REFLECTION_METHOD(STRL("ToString"), Object::ToString, {}, {});
			
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

		enum class SignalConnectResult {
			OK,
			InvalidSignal,
			InvalidObject
		};
		bool IsSignalConnected(const String& signal, const InstanceMethod& method) const;
		SignalConnectResult ConnectSignal(const String& signal, const InstanceMethod& method);
		bool DisconnectSignal(const String& signal, const InstanceMethod& method);
		bool EmitSignal(const String& signal);

	protected:
		static Dictionary<InstanceId, Object*> objectLookup;
		InstanceId instanceId;

	private:
		struct SignalGroup {
			Dictionary<InstanceMethod,bool> connections;
		};
		Dictionary<String, SharedPtr<SignalGroup>> signalGroups;
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