#include "Engine/System/Object/ObjectUtil.h"

namespace Engine {
#pragma region ToStrings
	String ObjectUtil::ToString(bool obj) {
		return (obj ? u8"True" : u8"False");
	}
	String ObjectUtil::ToString(byte obj) {
		return std::to_string(obj);
	}
	String ObjectUtil::ToString(sbyte obj) {
		return std::to_string(obj);
	}
	String ObjectUtil::ToString(int16 obj) {
		return std::to_string(obj);
	}
	String ObjectUtil::ToString(uint16 obj) {
		return std::to_string(obj);
	}
	String ObjectUtil::ToString(int32 obj) {
		return std::to_string(obj);
	}
	String ObjectUtil::ToString(uint32 obj) {
		return std::to_string(obj);
	}
	String ObjectUtil::ToString(int64 obj) {
		return std::to_string(obj);
	}
	String ObjectUtil::ToString(uint64 obj) {
		return std::to_string(obj);
	}
	String ObjectUtil::ToString(float obj) {
		return std::to_string(obj);
	}
	String ObjectUtil::ToString(double obj) {
		return std::to_string(obj);
	}
#pragma endregion

#pragma region GetHashCodes
	int32 ObjectUtil::GetHashCode(bool obj) {
		return obj;
	}
	int32 ObjectUtil::GetHashCode(byte obj) {
		return obj;
	}
	int32 ObjectUtil::GetHashCode(sbyte obj) {
		return obj;
	}
	int32 ObjectUtil::GetHashCode(int16 obj) {
		return obj;
	}
	int32 ObjectUtil::GetHashCode(uint16 obj) {
		return obj;
	}
	int32 ObjectUtil::GetHashCode(int32 obj) {
		return obj;
	}
	int32 ObjectUtil::GetHashCode(uint32 obj) {
		return *((int32*)(&obj));
	}
	int32 ObjectUtil::GetHashCode(int64 obj) {
		return ((int32)obj) ^ ((int32)(obj >> 32));
	}
	int32 ObjectUtil::GetHashCode(uint64 obj) {
		return ObjectUtil::GetHashCode(*((int64*)(&obj)));
	}
	// Referenced .NET 5 standard library: https://source.dot.net
	int32 ObjectUtil::GetHashCode(float obj) {
		int32 bits = *((int32*)(&obj));
		// NaN and zero will have the same hash code.
		if (((bits - 1) & 0x7FFFFFFF) >= 0x7F800000) {
			bits &= 0x7F800000;
		}
		return bits;
	}
	int32 ObjectUtil::GetHashCode(double obj) {
		int64 bits = *((int64*)(&obj));
		// NaN and zero will have the same hash code.
		if (((bits - 1) & 0x7FFFFFFFFFFFFFFF) >= 0x7FF0000000000000) {
			bits &= 0x7FF0000000000000;
		}
		return ObjectUtil::GetHashCode(bits);
	}
	int32 ObjectUtil::GetHashCode(const void* obj) {
		sizeint v = *((sizeint*)(&obj));
		return GetHashCode(v);
	}
#pragma endregion
}
