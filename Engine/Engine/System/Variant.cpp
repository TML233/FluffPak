#include "Engine/System/Variant.h"
#include "Engine/System/Object.h"

namespace Engine {
	Variant::~Variant() {
		Clear();
	}
#pragma region Ctors
	Variant::Variant() {}
	Variant::Variant(bool value) {
		ConstructBool(value);
	}
	Variant::Variant(byte value) {
		ConstructInt64(value);
	}
	Variant::Variant(sbyte value) {
		ConstructInt64(value);
	}
	Variant::Variant(int16 value) {
		ConstructInt64(value);
	}
	Variant::Variant(uint16 value) {
		ConstructInt64(value);
	}
	Variant::Variant(int32 value) {
		ConstructInt64(value);
	}
	Variant::Variant(uint32 value) {
		ConstructInt64(value);
	}
	Variant::Variant(int64 value) {
		ConstructInt64(value);
	}
	Variant::Variant(uint64 value) {
		if (value > 9223372036854775807) {
			WARN_MSG("Storing a UInt64 value that is too big for Int64! May lose some information.");
		}
		ConstructInt64(value);
	}
	Variant::Variant(float value) {
		ConstructDouble(value);
	}
	Variant::Variant(double value) {
		ConstructDouble(value);
	}
	Variant::Variant(const String& value) {
		ConstructString(value);
	}
	Variant::Variant(Object* value) {
		ConstructObject(ObjectData(value, value->GetInstanceId()));
	}
#pragma endregion

#pragma region Converts
	Variant::operator bool() const {
		switch (type) {
			case Type::Bool:
				return data.vBool;
			case Type::Int64:
				return data.vInt64 >= 1;
			case Type::Double:
				return data.vDouble >= 1;
		}
		return false;
	}
	Variant::operator byte() const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return 0;
	}
	Variant::operator sbyte() const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return 0;
	}
	Variant::operator int16() const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return 0;
	}
	Variant::operator uint16() const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return 0;
	}
	Variant::operator int32() const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return 0;
	}
	Variant::operator uint32() const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return 0;
	}
	Variant::operator int64() const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return 0;
	}
	Variant::operator uint64() const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return 0;
	}
	Variant::operator float() const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return 0;
	}
	Variant::operator double() const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return 0;
	}
	Variant::operator String() const {
		switch (type) {
			case Type::Null:
				return STRING_LITERAL("Null");
			case Type::Bool:
				return data.vBool ? STRING_LITERAL("True") : STRING_LITERAL("FALSE");
			case Type::Int64:
				return ObjectUtil::ToString(data.vInt64);
			case Type::Double:
				return ObjectUtil::ToString(data.vDouble);
			case Type::String:
				return data.vString;
		}
		return String();
	}
	Variant::operator Object* () const {
		switch (type) {
			case Type::Object:
				return data.vObject.ptr;
		}
		return nullptr;
	}
#pragma endregion

	String Variant::ToString() const {
		return operator String();
	}

	void Variant::Clear() {
		switch (type) {
			case Type::String:
				Memory::Destruct(&data.vString);
				break;

			case Type::Object:
				if (data.vObject.id.IsReferenced()) {
					// Containing object is reference-counted. Do the dereferencing.
					ReferencedObject* refObj = static_cast<ReferencedObject*>(data.vObject.ptr);
					if (refObj->Dereference() == 0) {
						MEMDEL(refObj);
					}
				}
				Memory::Destruct(&data.vObject);
				break;
		}

		type = Type::Null;
	}

	Variant::Type Variant::GetType() const {
		return type;
	}

	Variant& Variant::operator=(const Variant& obj) {
		if (&obj == this) {
			return *this;
		}

		Clear();

		type = obj.type;
		switch (type) {
			case Type::Bool:
				ConstructBool(obj.data.vBool);
				break;
			case Type::Int64:
				ConstructInt64(obj.data.vInt64);
				break;
			case Type::Double:
				ConstructDouble(obj.data.vDouble);
				break;
			case Type::String:
				ConstructString(obj.data.vString);
				break;
			case Type::Object:
				ConstructObject(obj.data.vObject);
				break;
		}

		return *this;
	}

	void Variant::ConstructBool(bool value) {
		type = Type::Bool;
		data.vBool = value;
	}
	void Variant::ConstructInt64(int64 value) {
		type = Type::Int64;
		data.vInt64 = value;
	}
	void Variant::ConstructDouble(double value) {
		type = Type::Double;
		data.vDouble = value;
	}
	void Variant::ConstructString(const String& value) {
		type = Type::String;
		Memory::Construct(&data.vString, value);
	}
	void Variant::ConstructObject(const ObjectData& value) {
		type = Type::Object;

		if (value.id.IsReferenced()) {
			// Containing object is reference-counted. Do the referencing.
			ReferencedObject* refObj = static_cast<ReferencedObject*>(value.ptr);
			refObj->Reference();
		}
		Memory::Construct(&data.vObject, value);
	}

	Variant::ObjectData::ObjectData(Object* ptr, const InstanceId& id) :ptr(ptr), id(id) {}
	Variant::DataUnion::DataUnion() {}
	Variant::DataUnion::~DataUnion() {}
}
