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
		virtual Int32 GetHashCode() const;

		static String ToString(const Object& obj);
		static String ToString(Bool obj);
		static String ToString(Byte obj);
		static String ToString(SByte obj);
		static String ToString(Int16 obj);
		static String ToString(UInt16 obj);
		static String ToString(Int32 obj);
		static String ToString(UInt32 obj);
		static String ToString(Int64 obj);
		static String ToString(UInt64 obj);
		static String ToString(Float obj);
		static String ToString(Double obj);

		static Int32 GetHashCode(const Object& obj);
		static Int32 GetHashCode(Bool obj);
		static Int32 GetHashCode(Byte obj);
		static Int32 GetHashCode(SByte obj);
		static Int32 GetHashCode(Int16 obj);
		static Int32 GetHashCode(UInt16 obj);
		static Int32 GetHashCode(Int32 obj);
		static Int32 GetHashCode(UInt32 obj);
		static Int32 GetHashCode(Int64 obj);
		static Int32 GetHashCode(UInt64 obj);
		static Int32 GetHashCode(Float obj);
		static Int32 GetHashCode(Double obj);
	};
}

#endif