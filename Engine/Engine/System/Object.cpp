#include "Engine/System/Object.h"
#include "Engine/System/String.h"
#include <string>
#include <typeinfo>

namespace Engine {
	Object::~Object() {}
	String Object::ToString() const {
		return typeid(*this).name();
	}
	Int Object::GetHashCode() const {
		size_t addr = (size_t)this;
		if (sizeof(size_t) == sizeof(ULong)) {
			return Object::GetHashCode((ULong)addr);
		} else {
			return Object::GetHashCode((UInt)addr);
		}
	}

#pragma region ToStrings
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
#pragma endregion

#pragma region GetHashCodes
	Int Object::GetHashCode(const Object& obj) {
		return obj.GetHashCode();
	}
	Int Object::GetHashCode(Bool obj) {
		return obj;
	}
	Int Object::GetHashCode(Byte obj) {
		return obj;
	}
	Int Object::GetHashCode(SByte obj) {
		return obj;
	}
	Int Object::GetHashCode(Short obj) {
		return obj;
	}
	Int Object::GetHashCode(UShort obj) {
		return obj;
	}
	Int Object::GetHashCode(Int obj) {
		return obj;
	}
	Int Object::GetHashCode(UInt obj) {
		return obj;
	}
	Int Object::GetHashCode(Long obj) {
		return ((Int)obj) ^ ((Int)(obj >> 32));
	}
	Int Object::GetHashCode(ULong obj) {
		return ((Int)obj) ^ ((Int)(obj >> 32));
	}
	// Referenced .NET 5 standard library: https://source.dot.net
	Int Object::GetHashCode(Float obj) {
		Int bits = 0;
		std::memcpy(&bits, &obj, sizeof(bits));
		// NaN and zero will have the same hash code.
		if (((bits - 1) & 0x7FFFFFFF) >= 0x7F800000) {
			bits &= 0x7F800000;
		}
		return bits;
	}
	Int Object::GetHashCode(Double obj) {
		Long bits = 0;
		std::memcpy(&bits, &obj, sizeof(bits));
		// NaN and zero will have the same hash code.
		if (((bits - 1) & 0x7FFFFFFFFFFFFFFF) >= 0x7FF0000000000000) {
			bits &= 0x7FF0000000000000;
		}
		return Object::GetHashCode(bits);
	}
#pragma endregion
}