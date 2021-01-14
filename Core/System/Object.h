#ifndef HEADER_CORE_SYSTEM_OBJECT
#define HEADER_CORE_SYSTEM_OBJECT

#include "Core/Definition.h"

namespace Core {
	class Object {
	public:
		virtual ~Object();
		virtual String ToString() const;

		// Get hash code. Hash code is not used to identify objects!
		// Override this if the object indicates value.
		virtual Int GetHashCode() const;

		static String ToString(const Object& obj);
		static String ToString(Bool obj);
		static String ToString(Byte obj);
		static String ToString(SByte obj);
		static String ToString(Short obj);
		static String ToString(UShort obj);
		static String ToString(Int obj);
		static String ToString(UInt obj);
		static String ToString(Long obj);
		static String ToString(ULong obj);
		static String ToString(Float obj);
		static String ToString(Double obj);

		static Int GetHashCode(const Object& obj);
		static Int GetHashCode(Bool obj);
		static Int GetHashCode(Byte obj);
		static Int GetHashCode(SByte obj);
		static Int GetHashCode(Short obj);
		static Int GetHashCode(UShort obj);
		static Int GetHashCode(Int obj);
		static Int GetHashCode(UInt obj);
		static Int GetHashCode(Long obj);
		static Int GetHashCode(ULong obj);
		static Int GetHashCode(Float obj);
		static Int GetHashCode(Double obj);
	};
}

#endif