#include "Core/System/Object.h"
#include "Core/System/String.h"
#include <string>
#include <typeinfo>

namespace Core {
	Object::~Object() {}
	String Object::ToString() const {
		return typeid(*this).name();
	}


	String Object::ToString(const Object& obj) {
		return obj.ToString();
	}
	String Object::ToString(Bool obj) {
		return (obj ? u8"True" : u8"False");
	}
	String Object::ToString(Byte obj) {
		return std::to_string(obj);
	}
	String Object::ToString(SByte obj) {
		return std::to_string(obj);
	}
	String Object::ToString(Short obj) {
		return std::to_string(obj);
	}
	String Object::ToString(UShort obj) {
		return std::to_string(obj);
	}
	String Object::ToString(Int obj) {
		return std::to_string(obj);
	}
	String Object::ToString(UInt obj) {
		return std::to_string(obj);
	}
	String Object::ToString(Long obj) {
		return std::to_string(obj);
	}
	String Object::ToString(ULong obj) {
		return std::to_string(obj);
	}
	String Object::ToString(Float obj) {
		return std::to_string(obj);
	}
	String Object::ToString(Double obj) {
		return std::to_string(obj);
	}
}