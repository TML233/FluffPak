#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Atomic.h"
#include "Engine/System/Object/InstanceId.h"
#include "Engine/Collection/Dictionary.h"
#include "Engine/System/Object/Reflection.h"

namespace Engine{
	// Inherit ManualManagement or AutoManagement instead.
	class Object {
		REFLECTION_ROOTCLASS(::Engine::Object) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		static bool IsInstanceValid(InstanceId id);
		virtual ~Object() = 0;

		// Indicates if current object is a ReferencedObject
		virtual bool IsReferenced() const = 0;

		virtual String ToString() const;

		// Get hash code. Hash code is not used to identify objects!
		// Override this if the object indicates value.
		virtual int32 GetHashCode() const;

		InstanceId GetInstanceId() const;

	protected:
		static Dictionary<InstanceId, bool> objectLookup;
		InstanceId instanceId;
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