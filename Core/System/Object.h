#ifndef HEADER_CORE_SYSTEM_OBJECT
#define HEADER_CORE_SYSTEM_OBJECT

#include "Core/Definition.h"

namespace Core {
	class Object {
	public:
		virtual ~Object();
		virtual String ToString() const;
		//virtual Int GetHashCode() const;

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

	};
}

#endif