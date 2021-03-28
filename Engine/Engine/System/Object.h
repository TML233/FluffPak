#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Atomic.h"
#include "Engine/System/InstanceId.h"
#include "Engine/Collection/Dictionary.h"

namespace Engine {
	// Inherit ManualManagement or AutoManagement instead.
	class Object {
	public:
		static bool IsInstanceValid(InstanceId id);
		virtual ~Object();

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
	// Raw pointers of ManualObject can be passed around.
	class ManualObject :public Object {
	public:
		ManualObject();
		bool IsReferenced() const override;
	};

	// Represents a Object which its memory management is done automatically via smart pointers.
	// No raw pointer of ReferenceObject should be passed around!
	class ReferencedObject :public Object {
	public:
		ReferencedObject();
		bool IsReferenced() const override;
		uint32 Reference();
		uint32 Dereference();
		uint32 GetReferenceCount() const;
	private:
		ReferenceCount referenceCount;
	};
}