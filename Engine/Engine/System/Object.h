#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Atomic.h"
#include "Engine/System/ObjectId.h"

namespace Engine {
	// Inherit ManualManagement or AutoManagement instead.
	class Object {
	public:
		virtual ~Object();

		// Indicates if current object is a ReferencedObject
		virtual bool IsReferenced() const = 0;

		virtual String ToString() const;

		// Get hash code. Hash code is not used to identify objects!
		// Override this if the object indicates value.
		virtual int32 GetHashCode() const;

		ObjectId GetInstanceId() const;

#pragma region ToStrings
		template<typename T>
		static String ToString(const T& obj) {
			return obj.ToString();
		}
		static String ToString(bool obj);
		static String ToString(byte obj);
		static String ToString(sbyte obj);
		static String ToString(int16 obj);
		static String ToString(uint16 obj);
		static String ToString(int32 obj);
		static String ToString(uint32 obj);
		static String ToString(int64 obj);
		static String ToString(uint64 obj);
		static String ToString(float obj);
		static String ToString(double obj);
#pragma endregion

#pragma region GetHashCodes
		template<typename T>
		static int32 GetHashCode(const T& obj) {
			return obj.GetHashCode();
		}
		static int32 GetHashCode(bool obj);
		static int32 GetHashCode(byte obj);
		static int32 GetHashCode(sbyte obj);
		static int32 GetHashCode(int16 obj);
		static int32 GetHashCode(uint16 obj);
		static int32 GetHashCode(int32 obj);
		static int32 GetHashCode(uint32 obj);
		static int32 GetHashCode(int64 obj);
		static int32 GetHashCode(uint64 obj);
		static int32 GetHashCode(float obj);
		static int32 GetHashCode(double obj);
#pragma endregion

	protected:
		ObjectId instanceId{};
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