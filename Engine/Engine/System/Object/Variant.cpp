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
			case Type::Int64:
				return STRING_LITERAL("Int64");
			case Type::Double:
				return STRING_LITERAL("Double");

			case Type::String:
				return STRING_LITERAL("String");

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
			WARN_MSG(u8"Storing a UInt64 value that is too big for Int64! May lose some information.");
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
		ConstructInt64((int64)value);
	}
#pragma endregion

#pragma region AsType
	// !! AddTypeHint 5.2: Implement your AsType function.

	bool Variant::AsBool(bool defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool;
			case Type::Int64:
				return data.vInt64 != 0;
			case Type::Double:
				return data.vDouble != 0;
		}
		return defaultValue;
	}
	int64 Variant::AsInt64(int64 defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return static_cast<int64>(data.vDouble);
		}
		return defaultValue;
	}
	double Variant::AsDouble(double defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return static_cast<double>(data.vInt64);
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
			case Type::Int64:
				return ObjectUtil::ToString(data.vInt64);
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
			case Type::Int64:
				ConstructInt64(obj.data.vInt64);
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