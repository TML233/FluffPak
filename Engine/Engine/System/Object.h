#ifndef HEADER_CORE_SYSTEM_OBJECT
#define HEADER_CORE_SYSTEM_OBJECT

#include "Engine/System/Definition.h"

namespace Engine {
	class Object {
	public:
		virtual ~Object();
		virtual String ToString() const;

		// Get hash code. Hash code is not used to identify objects!
		// Override this if the object indicates value.
		virtual int32 GetHashCode() const;

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
	};
}

#endif