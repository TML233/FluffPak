#include "Engine/System/Object/Variant.h"
#include "Engine/System/Object/Object.h"
#include "Engine/System/Debug.h"

namespace Engine {
	String Variant::GetTypeName(Type type) {
		switch (type) {
			case Type::Null:
				return STRING_LITERAL("Null");

			case Type::Bool:
				return STRING_LITERAL("Bool");
			case Type::Byte:
				return STRING_LITERAL("Byte");
			case Type::SByte:
				return STRING_LITERAL("SByte");
			case Type::Int16:
				return STRING_LITERAL("Int16");
			case Type::UInt16:
				return STRING_LITERAL("UInt16");
			case Type::Int32:
				return STRING_LITERAL("Int32");
			case Type::UInt32:
				return STRING_LITERAL("UInt32");
			case Type::Int64:
				return STRING_LITERAL("Int64");
			case Type::UInt64:
				return STRING_LITERAL("UInt64");
			case Type::Float:
				return STRING_LITERAL("Float");
			case Type::Double:
				return STRING_LITERAL("Double");

			case Type::String:
				return STRING_LITERAL("String");
			case Type::Vector2:
				return STRING_LITERAL("Vector2");

			case Type::Object:
				return STRING_LITERAL("Object");

			default:
				return STRING_LITERAL("Undefined Type");
		}
	}
	
	Variant::~Variant() {
		Clear();
	}
#pragma region Ctors
	// !! AddTypeHint 4.1: Implement your constructor to call the value constructor(the ConstructT function).

	Variant::Variant() {}
	Variant::Variant(bool value) {
		ConstructBool(value);
	}
	Variant::Variant(byte value) {
		ConstructByte(value);
	}
	Variant::Variant(sbyte value) {
		ConstructSByte(value);
	}
	Variant::Variant(int16 value) {
		ConstructInt16(value);
	}
	Variant::Variant(uint16 value) {
		ConstructUInt16(value);
	}
	Variant::Variant(int32 value) {
		ConstructInt32(value);
	}
	Variant::Variant(uint32 value) {
		ConstructUInt32(value);
	}
	Variant::Variant(int64 value) {
		ConstructInt64(value);
	}
	Variant::Variant(uint64 value) {
		ConstructUInt64(value);
	}
	Variant::Variant(float value) {
		ConstructFloat(value);
	}
	Variant::Variant(double value) {
		ConstructDouble(value);
	}
	Variant::Variant(const String& value) {
		ConstructString(value);
	}
	Variant::Variant(const u8char* value) {
		ConstructString(String(value));
	}
	Variant::Variant(const Vector2& value) {
		ConstructVector2(value);
	}
	Variant::Variant(Object* value) {
		ConstructObject(ObjectData(value, (value != nullptr ? value->GetInstanceId() : InstanceId())));
	}

	template<Concept::IsEnum T>
	Variant::Variant(T value) {
		ConstructInt32((int32)value);
	}
#pragma endregion

#pragma region AsType
	// !! AddTypeHint 5.2: Implement your AsType function.

	bool Variant::AsBool(bool defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool;
			case Type::Byte:
				return data.vByte != 0;
			case Type::SByte:
				return data.vSByte != 0;
			case Type::Int16:
				return data.vInt16 != 0;
			case Type::UInt16:
				return data.vUInt16 != 0;
			case Type::Int32:
				return data.vInt32 != 0;
			case Type::UInt32:
				return data.vUInt32 != 0;
			case Type::Int64:
				return data.vInt64 != 0;
			case Type::UInt64:
				return data.vUInt64 != 0;
			case Type::Float:
				return data.vFloat != 0;
			case Type::Double:
				return data.vDouble != 0;
		}
		return defaultValue;
	}
	byte Variant::AsByte(byte defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Byte:
				return data.vByte;
			case Type::SByte:
				return static_cast<byte>(data.vSByte);
			case Type::Int16:
				return static_cast<byte>(data.vInt16);
			case Type::UInt16:
				return static_cast<byte>(data.vUInt16);
			case Type::Int32:
				return static_cast<byte>(data.vInt32);
			case Type::UInt32:
				return static_cast<byte>(data.vUInt32);
			case Type::Int64:
				return static_cast<byte>(data.vInt64);
			case Type::UInt64:
				return static_cast<byte>(data.vUInt64);
			case Type::Float:
				return static_cast<byte>(data.vFloat);
			case Type::Double:
				return static_cast<byte>(data.vDouble);
		}
		return defaultValue;
	}
	sbyte Variant::AsSByte(sbyte defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Byte:
				return static_cast<sbyte>(data.vByte);
			case Type::SByte:
				return data.vSByte;
			case Type::Int16:
				return static_cast<sbyte>(data.vInt16);
			case Type::UInt16:
				return static_cast<sbyte>(data.vUInt16);
			case Type::Int32:
				return static_cast<sbyte>(data.vInt32);
			case Type::UInt32:
				return static_cast<sbyte>(data.vUInt32);
			case Type::Int64:
				return static_cast<sbyte>(data.vInt64);
			case Type::UInt64:
				return static_cast<sbyte>(data.vUInt64);
			case Type::Float:
				return static_cast<sbyte>(data.vFloat);
			case Type::Double:
				return static_cast<sbyte>(data.vDouble);
		}
		return defaultValue;
	}
	int16 Variant::AsInt16(int16 defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Byte:
				return data.vByte;
			case Type::SByte:
				return data.vSByte;
			case Type::Int16:
				return data.vInt16;
			case Type::UInt16:
				return static_cast<int16>(data.vUInt16);
			case Type::Int32:
				return static_cast<int16>(data.vInt32);
			case Type::UInt32:
				return static_cast<int16>(data.vUInt32);
			case Type::Int64:
				return static_cast<int16>(data.vInt64);
			case Type::UInt64:
				return static_cast<int16>(data.vUInt64);
			case Type::Float:
				return static_cast<int16>(data.vFloat);
			case Type::Double:
				return static_cast<int16>(data.vDouble);
		}
		return defaultValue;
	}
	uint16 Variant::AsUInt16(uint16 defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Byte:
				return data.vByte;
			case Type::SByte:
				return data.vSByte;
			case Type::Int16:
				return static_cast<uint16>(data.vInt16);
			case Type::UInt16:
				return data.vUInt16;
			case Type::Int32:
				return static_cast<uint16>(data.vInt32);
			case Type::UInt32:
				return static_cast<uint16>(data.vUInt32);
			case Type::Int64:
				return static_cast<uint16>(data.vInt64);
			case Type::UInt64:
				return static_cast<uint16>(data.vUInt64);
			case Type::Float:
				return static_cast<uint16>(data.vFloat);
			case Type::Double:
				return static_cast<uint16>(data.vDouble);
		}
		return defaultValue;
	}
	int32 Variant::AsInt32(int32 defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Byte:
				return data.vByte;
			case Type::SByte:
				return data.vSByte;
			case Type::Int16:
				return data.vInt16;
			case Type::UInt16:
				return data.vUInt16;
			case Type::Int32:
				return data.vInt32;
			case Type::UInt32:
				return static_cast<int32>(data.vUInt32);
			case Type::Int64:
				return static_cast<int32>(data.vInt64);
			case Type::UInt64:
				return static_cast<int32>(data.vUInt64);
			case Type::Float:
				return static_cast<int32>(data.vFloat);
			case Type::Double:
				return static_cast<int32>(data.vDouble);
		}
		return defaultValue;
	}
	uint32 Variant::AsUInt32(uint32 defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Byte:
				return data.vByte;
			case Type::SByte:
				return data.vSByte;
			case Type::Int16:
				return data.vInt16;
			case Type::UInt16:
				return data.vUInt16;
			case Type::Int32:
				return static_cast<uint32>(data.vInt32);
			case Type::UInt32:
				return data.vUInt32;
			case Type::Int64:
				return static_cast<uint32>(data.vInt64);
			case Type::UInt64:
				return static_cast<uint32>(data.vUInt64);
			case Type::Float:
				return static_cast<uint32>(data.vFloat);
			case Type::Double:
				return static_cast<uint32>(data.vDouble);
		}
		return defaultValue;
	}
	int64 Variant::AsInt64(int64 defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Byte:
				return data.vByte;
			case Type::SByte:
				return data.vSByte;
			case Type::Int16:
				return data.vInt16;
			case Type::UInt16:
				return data.vUInt16;
			case Type::Int32:
				return data.vInt32;
			case Type::UInt32:
				return data.vUInt32;
			case Type::Int64:
				return data.vInt64;
			case Type::UInt64:
				return static_cast<int64>(data.vUInt64);
			case Type::Float:
				return static_cast<int64>(data.vFloat);
			case Type::Double:
				return static_cast<int64>(data.vDouble);
		}
		return defaultValue;
	}
	uint64 Variant::AsUInt64(uint64 defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Byte:
				return data.vByte;
			case Type::SByte:
				return data.vSByte;
			case Type::Int16:
				return data.vInt16;
			case Type::UInt16:
				return data.vUInt16;
			case Type::Int32:
				return data.vInt32;
			case Type::UInt32:
				return data.vUInt32;
			case Type::Int64:
				return static_cast<uint64>(data.vInt64);
			case Type::UInt64:
				return data.vUInt64;
			case Type::Float:
				return static_cast<uint64>(data.vFloat);
			case Type::Double:
				return static_cast<uint64>(data.vDouble);
		}
		return defaultValue;
	}
	float Variant::AsFloat(float defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Byte:
				return data.vByte;
			case Type::SByte:
				return data.vSByte;
			case Type::Int16:
				return data.vInt16;
			case Type::UInt16:
				return data.vUInt16;
			case Type::Int32:
				return data.vInt32;
			case Type::UInt32:
				return data.vUInt32;
			case Type::Int64:
				return data.vInt64;
			case Type::UInt64:
				return data.vUInt64;
			case Type::Float:
				return data.vFloat;
			case Type::Double:
				return static_cast<float>(data.vDouble);
		}
		return defaultValue;
	}
	double Variant::AsDouble(double defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Byte:
				return data.vByte;
			case Type::SByte:
				return data.vSByte;
			case Type::Int16:
				return data.vInt16;
			case Type::UInt16:
				return data.vUInt16;
			case Type::Int32:
				return data.vInt32;
			case Type::UInt32:
				return data.vUInt32;
			case Type::Int64:
				return data.vInt64;
			case Type::UInt64:
				return data.vUInt64;
			case Type::Float:
				return data.vFloat;
			case Type::Double:
				return data.vDouble;
		}
		return defaultValue;
	}
	String Variant::AsString(String defaultValue) const {
		// !! AddTypeHint 5.3: Add a AsString entry.
		switch (type) {
			case Type::Null:
				return STRING_LITERAL("[Null]");
			case Type::Bool:
				return data.vBool ? STRING_LITERAL("True") : STRING_LITERAL("False");
			case Type::Byte:
				return ObjectUtil::ToString(data.vByte);
			case Type::SByte:
				return ObjectUtil::ToString(data.vSByte);
			case Type::Int16:
				return ObjectUtil::ToString(data.vInt16);
			case Type::UInt16:
				return ObjectUtil::ToString(data.vUInt16);
			case Type::Int32:
				return ObjectUtil::ToString(data.vInt32);
			case Type::UInt32:
				return ObjectUtil::ToString(data.vUInt32);
			case Type::Int64:
				return ObjectUtil::ToString(data.vInt64);
			case Type::UInt64:
				return ObjectUtil::ToString(data.vUInt64);
			case Type::Float:
				return ObjectUtil::ToString(data.vFloat);
			case Type::Double:
				return ObjectUtil::ToString(data.vDouble);
			case Type::String:
				return data.vString;
			case Type::Vector2:
				return data.vVector2.ToString();
			case Type::Object:
				if (data.vObject.ptr == nullptr) {
					return STRING_LITERAL("[Nullptr]");
				}
				if(!Object::IsInstanceValid(data.vObject.id)){
					return STRING_LITERAL("[Released Object]");
				}
				return data.vObject.ptr->ToString();
		}
		return defaultValue;
	}
	Vector2 Variant::AsVector2(const Vector2& defaultValue) const {
		switch (type) {
			case Type::Vector2:
				return data.vVector2;
		}
		return defaultValue;
	}
	Object* Variant::AsObject(Object* defaultValue) const {
		switch (type) {
			case Type::Object:
				return data.vObject.ptr;
		}
		return defaultValue;
	}
#pragma endregion

	String Variant::ToString() const {
		return AsString();
	}

	bool Variant::CanConvertImplicitly(Type from, Type to) {
		if (from == to) {
			return true;
		}

		return (implicitConverterTable[(sizeint)from, (sizeint)to] != nullptr);
	}

	Variant::Convertor Variant::implicitConverterTable[(sizeint)Type::End][(sizeint)Type::End] = {};

#define VARIANT_CONVERTIBLE(from,to) implicitConverterTable[(sizeint)Type::from][(sizeint)Type::to]=[](const Variant& value)->Variant
	void Variant::Initializer::InitImplicitConversionTable() {
		// !! AddTypeHint 8.0: Add necessary implicit conversion.
		VARIANT_CONVERTIBLE(Bool, Int64) { return value.AsInt64(); };
		VARIANT_CONVERTIBLE(Bool, Double) { return value.AsDouble(); };
		
		VARIANT_CONVERTIBLE(Int64, Double) { return value.AsDouble(); };
		
		VARIANT_CONVERTIBLE(Null, Object) { return Variant((Object*)nullptr); };
	}
#undef VARIANT_CONVERTIBLE

	void Variant::Clear() {
		// !! AddTypeHint 7.0: Add a entry to destruct the value.
		switch (type) {
			case Type::String:
				Memory::Destruct(&data.vString);
				break;

			case Type::Vector2:
				Memory::Destruct(&data.vVector2);
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

	void Variant::AssignValue(const Variant& obj) {
		type = obj.type;

		// !! AddTypeHint 6.0: Add an entry to call the value constructor.
		switch (type) {
			case Type::Bool:
				ConstructBool(obj.data.vBool);
				break;
			case Type::Byte:
				ConstructByte(obj.data.vByte);
				break;
			case Type::SByte:
				ConstructSByte(obj.data.vSByte);
				break;
			case Type::Int16:
				ConstructInt16(obj.data.vInt16);
				break;
			case Type::UInt16:
				ConstructUInt16(obj.data.vUInt16);
				break;
			case Type::Int32:
				ConstructInt32(obj.data.vInt32);
				break;
			case Type::UInt32:
				ConstructUInt32(obj.data.vUInt32);
				break;
			case Type::Int64:
				ConstructInt64(obj.data.vInt64);
				break;
			case Type::UInt64:
				ConstructUInt64(obj.data.vUInt64);
				break;
			case Type::Float:
				ConstructFloat(obj.data.vFloat);
				break;
			case Type::Double:
				ConstructDouble(obj.data.vDouble);
				break;
			case Type::String:
				ConstructString(obj.data.vString);
				break;
			case Type::Vector2:
				ConstructVector2(obj.data.vVector2);
				break;
			case Type::Object:
				ConstructObject(obj.data.vObject);
				break;
		}
	}
	Variant::Variant(const Variant& obj) {
		AssignValue(obj);
	}
	Variant& Variant::operator=(const Variant& obj) {
		if (&obj == this) {
			return *this;
		}

		Clear();

		AssignValue(obj);

		return *this;
	}

#pragma region Value constructors
	// !! AddTypeHint 3.1: Implement your value constructor.

	void Variant::ConstructBool(bool value) {
		type = Type::Bool;
		data.vBool = value;
	}
	void Variant::ConstructByte(byte value) {
		type = Type::Byte;
		data.vByte = value;
	}
	void Variant::ConstructSByte(sbyte value) {
		type = Type::SByte;
		data.vSByte = value;
	}
	void Variant::ConstructInt16(int16 value) {
		type = Type::Int16;
		data.vInt16 = value;
	}
	void Variant::ConstructUInt16(uint16 value) {
		type = Type::UInt16;
		data.vUInt16 = value;
	}
	void Variant::ConstructInt32(int32 value) {
		type = Type::Int32;
		data.vInt32 = value;
	}
	void Variant::ConstructUInt32(uint32 value) {
		type = Type::UInt32;
		data.vUInt32 = value;
	}
	void Variant::ConstructInt64(int64 value) {
		type = Type::Int64;
		data.vInt64 = value;
	}
	void Variant::ConstructUInt64(uint64 value) {
		type = Type::UInt64;
		data.vUInt64 = value;
	}
	void Variant::ConstructFloat(float value) {
		type = Type::Float;
		data.vFloat = value;
	}
	void Variant::ConstructDouble(double value) {
		type = Type::Double;
		data.vDouble = value;
	}
	void Variant::ConstructString(const String& value) {
		type = Type::String;
		Memory::Construct(&data.vString, value);
	}
	void Variant::ConstructVector2(const Vector2& value) {
		type = Type::Vector2;
		Memory::Construct(&data.vVector2, value);
	}
	void Variant::ConstructObject(const ObjectData& value) {
		type = Type::Object;
		if (value.ptr != nullptr) {
			if (value.id.IsReferenced()) {
				// Containing object is reference-counted. Do the referencing.
				ReferencedObject* refObj = static_cast<ReferencedObject*>(value.ptr);
				refObj->Reference();
			}
		}
		Memory::Construct(&data.vObject, value);
	}
#pragma endregion

	Variant::ObjectData::ObjectData(Object* ptr, const InstanceId& id) :ptr(ptr), id(id) {}
	Variant::DataUnion::DataUnion() {}
	Variant::DataUnion::~DataUnion() {}

	Variant::Initializer::Initializer() {
		InitImplicitConversionTable();
	}
}