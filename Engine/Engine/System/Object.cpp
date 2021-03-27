#include "Engine/System/Object.h"
#include "Engine/System/String.h"
#include "Engine/System/Memory.h"
#include <string>
#include <typeinfo>

namespace Engine {
	Object::~Object() {}

	String Object::ToString() const {
		return typeid(*this).name();
	}
	int32 Object::GetHashCode() const {
		return Object::GetHashCode(instanceId.Get());
	}

	ObjectId Object::GetInstanceId() const {
		return instanceId;
	}

#pragma region ToStrings
	String Object::ToString(bool obj) {
		return (obj ? u8"True" : u8"False");
	}
	String Object::ToString(byte obj) {
		return std::to_string(obj);
	}
	String Object::ToString(sbyte obj) {
		return std::to_string(obj);
	}
	String Object::ToString(int16 obj) {
		return std::to_string(obj);
	}
	String Object::ToString(uint16 obj) {
		return std::to_string(obj);
	}
	String Object::ToString(int32 obj) {
		return std::to_string(obj);
	}
	String Object::ToString(uint32 obj) {
		return std::to_string(obj);
	}
	String Object::ToString(int64 obj) {
		return std::to_string(obj);
	}
	String Object::ToString(uint64 obj) {
		return std::to_string(obj);
	}
	String Object::ToString(float obj) {
		return std::to_string(obj);
	}
	String Object::ToString(double obj) {
		return std::to_string(obj);
	}
#pragma endregion

#pragma region GetHashCodes
	int32 Object::GetHashCode(bool obj) {
		return obj;
	}
	int32 Object::GetHashCode(byte obj) {
		return obj;
	}
	int32 Object::GetHashCode(sbyte obj) {
		return obj;
	}
	int32 Object::GetHashCode(int16 obj) {
		return obj;
	}
	int32 Object::GetHashCode(uint16 obj) {
		return obj;
	}
	int32 Object::GetHashCode(int32 obj) {
		return obj;
	}
	int32 Object::GetHashCode(uint32 obj) {
		return obj;
	}
	int32 Object::GetHashCode(int64 obj) {
		return ((int32)obj) ^ ((int32)(obj >> 32));
	}
	int32 Object::GetHashCode(uint64 obj) {
		return ((int32)obj) ^ ((int32)(obj >> 32));
	}
	// Referenced .NET 5 standard library: https://source.dot.net
	int32 Object::GetHashCode(float obj) {
		int32 bits = 0;
		std::memcpy(&bits, &obj, sizeof(bits));
		// NaN and zero will have the same hash code.
		if (((bits - 1) & 0x7FFFFFFF) >= 0x7F800000) {
			bits &= 0x7F800000;
		}
		return bits;
	}
	int32 Object::GetHashCode(double obj) {
		int64 bits = 0;
		std::memcpy(&bits, &obj, sizeof(bits));
		// NaN and zero will have the same hash code.
		if (((bits - 1) & 0x7FFFFFFFFFFFFFFF) >= 0x7FF0000000000000) {
			bits &= 0x7FF0000000000000;
		}
		return Object::GetHashCode(bits);
	}
#pragma endregion

	ManualObject::ManualObject() {
		instanceId = ObjectId::Generate(false);
	}
	bool ManualObject::IsReferenced() const {
		return false;
	}

	ReferencedObject::ReferencedObject() {
		instanceId = ObjectId::Generate(true);
	}
	bool ReferencedObject::IsReferenced() const {
		return true;
	}
	uint32 ReferencedObject::Reference() {
		return referenceCount.Reference();
	}
	uint32 ReferencedObject::Dereference() {
		return referenceCount.Dereference();
	}
	uint32 ReferencedObject::GetReferenceCount() const {
		return referenceCount.Get();
	}
}