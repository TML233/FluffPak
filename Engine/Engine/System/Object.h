#pragma once

#include "Engine/System/Definition.h"

namespace Engine {
	class Object {
	public:
		virtual ~Object();
		virtual String ToString() const;

		// Get hash code. Hash code is not used to identify objects!
		// Override this if the object indicates value.
		virtual int32 GetHashCode() const;

#pragma region ToStrings
		static String ToString(const Object& obj);
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
		static int32 GetHashCode(const Object& obj);
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

#pragma region Move & Forward support
	private:
		template<typename T>
		class ReferenceRemover {
		public:
			using Type = T;
		};

		template<typename T>
		class ReferenceRemover<T&> {
		public:
			using Type = T;
		};

		template<typename T>
		class ReferenceRemover<T&&> {
		public:
			using Type = T;
		};

	public:
		// Turn everything to Right Value Reference.
		// Convenient tool for moving objects.
		template<typename T>
		static typename ReferenceRemover<T>::Type Move(T&& obj) {
			return static_cast<typename ReferenceRemover<T>::Type&&>(obj);
		}

		// Keep the Right Value Reference still as a Right Value.
		template<typename T>
		static T&& Forward(typename ReferenceRemover<T>::Type& obj) {
			return static_cast<T&&>(obj);
		}
		// Keep the Right Value Reference still as a Right Value.
		template<typename T>
		static T&& Forward(typename ReferenceRemover<T>::Type&& obj) {
			return static_cast<T&&>(obj);
		}
	};
#pragma endregion
}