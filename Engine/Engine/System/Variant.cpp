#include "Engine/System/Variant.h"
#include "Engine/System/Object.h"
#include "Engine/System/Debug.h"

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
	bool Variant::AsBool(bool defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool;
			case Type::Int64:
				return data.vInt64 >= 1;
			case Type::Double:
				return data.vDouble >= 1;
		}
		return defaultValue;
	}
	byte Variant::AsByte(byte defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return defaultValue;
	}
	sbyte Variant::AsSByte(sbyte defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return defaultValue;
	}
	int16 Variant::AsInt16(int16 defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return defaultValue;
	}
	uint16 Variant::AsUInt16(uint16 defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return defaultValue;
	}
	int32 Variant::AsInt32(int32 defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return defaultValue;
	}
	uint32 Variant::AsUInt32(uint32 defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
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
				return data.vDouble;
		}
		return defaultValue;
	}
	uint64 Variant::AsUInt64(uint64 defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return defaultValue;
	}
	float Variant::AsFloat(float defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return defaultValue;
	}
	double Variant::AsDouble(double defaultValue) const {
		switch (type) {
			case Type::Bool:
				return data.vBool ? 1 : 0;
			case Type::Int64:
				return data.vInt64;
			case Type::Double:
				return data.vDouble;
		}
		return defaultValue;
	}
	String Variant::AsString() const {
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
			case Type::Object:
				return data.vObject.ptr->ToString();
		}
		return String();
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

	bool Variant::operator==(const Variant& obj) const {
		if (!CanEvaluate(Operator::Equal, type, obj.type)) {
			return false;
		}
		return Evaluate(Operator::Equal, *this, obj).AsBool();
	}
	bool Variant::operator!=(const Variant& obj) const {
		if (!CanEvaluate(Operator::NotEqual, type, obj.type)) {
			return true;
		}
		return Evaluate(Operator::NotEqual, *this, obj).AsBool();
	}
	bool Variant::operator>(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::Greater, GetType(), obj.GetType()), "Cannot evaluate Greater! No evaluator registered for this.", return false);
		return Evaluate(Operator::Greater, *this, obj).AsBool();
	}
	bool Variant::operator>=(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::GreaterEqual, GetType(), obj.GetType()), "Cannot evaluate GreaterEqual! No evaluator registered for this.", return false);
		return Evaluate(Operator::GreaterEqual, *this, obj).AsBool();
	}
	bool Variant::operator<(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::Less, GetType(), obj.GetType()), "Cannot evaluate Less! No evaluator registered for this.", return false);
		return Evaluate(Operator::Less, *this, obj).AsBool();
	}
	bool Variant::operator<=(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::LessEqual, GetType(), obj.GetType()), "Cannot evaluate LessEqual! No evaluator registered for this.", return false);
		return Evaluate(Operator::LessEqual, *this, obj).AsBool();
	}
	Variant Variant::operator+(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::Add, GetType(), obj.GetType()), "Cannot evaluate Add! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::Add, *this, obj);
	}
	Variant Variant::operator-(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::Subtract, GetType(), obj.GetType()), "Cannot evaluate Subtract! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::Subtract, *this, obj);
	}
	Variant Variant::operator*(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::Multiply, GetType(), obj.GetType()), "Cannot evaluate Multiply! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::Multiply, *this, obj);
	}
	Variant Variant::operator/(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::Divide, GetType(), obj.GetType()), "Cannot evaluate Divide! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::Divide, *this, obj);
	}
	Variant Variant::operator%(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::Mod, GetType(), obj.GetType()), "Cannot evaluate Mod! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::Mod, *this, obj);
	}
	Variant Variant::operator+() const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::Positive, GetType(), Type::Null), "Cannot evaluate Positive! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::Positive, *this, Variant());
	}
	Variant Variant::operator-() const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::Negative, GetType(), Type::Null), "Cannot evaluate Negetive! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::Negative, *this, Variant());
	}
	Variant Variant::operator&&(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::And, GetType(), obj.GetType()), "Cannot evaluate And! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::And, *this, obj);
	}
	Variant Variant::operator||(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::Or, GetType(), obj.GetType()), "Cannot evaluate Or! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::Or, *this, obj);
	}
	Variant Variant::operator!() const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::Not, GetType(), Type::Null), "Cannot evaluate Not! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::Not, *this, Variant());
	}
	Variant Variant::operator&(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::BitAnd, GetType(), obj.GetType()), "Cannot evaluate BitAnd! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::BitAnd, *this, obj);
	}
	Variant Variant::operator|(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::BitOr, GetType(), obj.GetType()), "Cannot evaluate BitOr! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::BitOr, *this, obj);
	}
	Variant Variant::operator^(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::BitXOr, GetType(), obj.GetType()), "Cannot evaluate BitXOr! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::BitXOr, *this, obj);
	}
	Variant Variant::operator~() const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::BitFlip, GetType(), Type::Null), "Cannot evaluate BitFlip! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::BitFlip, *this, Variant());
	}
	Variant Variant::operator<<(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::BitShiftLeft, GetType(), obj.GetType()), "Cannot evaluate BitShiftLeft! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::BitShiftLeft, *this, obj);
	}
	Variant Variant::operator>>(const Variant& obj) const {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::BitShiftRight, GetType(), obj.GetType()), "Cannot evaluate BitShiftRight! No evaluator registered for this.", return Variant());
		return Evaluate(Operator::BitShiftRight, *this, obj);
	}

	bool Variant::CanEvaluate(Operator op, Type a, Type b) {
		ERR_ASSERT((sizeint)op >= 0 && op < Operator::End, "op out of bounds.", return false);
		ERR_ASSERT((sizeint)a >= 0 && a < Type::End, "a out of bounds.", return false);
		ERR_ASSERT((sizeint)b >= 0 && b < Type::End, "b out of bounds.", return false);

		return evaluators[(sizeint)a][(sizeint)b][(sizeint)op] != nullptr;
	}
	Variant Variant::Evaluate(Operator op, const Variant& a, const Variant& b) {
		ERR_ASSERT(CanEvaluate(op, a.type, b.type), "No evaluator registered for target operator and types.", return Variant());

		Evaluator ev = evaluators[(sizeint)a.type][(sizeint)b.type][(sizeint)op];

		return ev(a, b);
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

	Variant::Evaluator Variant::evaluators[(sizeint)Type::End][(sizeint)Type::End][(sizeint)Operator::End]{};

#define VARIANT_EVALUATOR(typeA,typeB,op) evaluators[(sizeint)(Type::##typeA)][(sizeint)(Type::##typeB)][(sizeint)(Operator::##op)] = [](const Variant& a, const Variant& b) -> Variant

	Variant::EvaluatorInitializer::EvaluatorInitializer() {
#pragma region Null
		VARIANT_EVALUATOR(Null, Null, Equal) { return true; };
		VARIANT_EVALUATOR(Null, Null, NotEqual) { return false; };
#pragma endregion

#pragma region Bool
		VARIANT_EVALUATOR(Bool, Bool, Equal) { return a.AsBool() == b.AsBool(); };
		VARIANT_EVALUATOR(Bool, Bool, NotEqual) { return a.AsBool() != b.AsBool(); };
		VARIANT_EVALUATOR(Bool, Null, Not) { return !a.AsBool(); };
		VARIANT_EVALUATOR(Bool, Bool, And) { return a.AsBool() && b.AsBool(); };
		VARIANT_EVALUATOR(Bool, Bool, Or) { return a.AsBool() || b.AsBool(); };
		VARIANT_EVALUATOR(Bool, Bool, XOr) { return a.AsBool() ^ b.AsBool(); };
#pragma endregion

#pragma region Int64
		VARIANT_EVALUATOR(Int64, Int64, Add) { return a.AsInt64() + b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Int64, Subtract) { return a.AsInt64() - b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Int64, Multiply) { return a.AsInt64() * b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Int64, Divide) { return a.AsInt64() / b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Double, Add) { return a.AsInt64() + b.AsDouble(); };
		VARIANT_EVALUATOR(Int64, Double, Subtract) { return a.AsInt64() - b.AsDouble(); };
		VARIANT_EVALUATOR(Int64, Double, Multiply) { return a.AsInt64() * b.AsDouble(); };
		VARIANT_EVALUATOR(Int64, Double, Divide) { return a.AsInt64() / b.AsDouble(); };
		VARIANT_EVALUATOR(Int64, Int64, Mod) { return a.AsInt64() % b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Null, Positive) { return +(a.AsInt64()); };
		VARIANT_EVALUATOR(Int64, Null, Negative) { return -(a.AsInt64()); };

		VARIANT_EVALUATOR(Int64, Int64, Equal) { return a.AsInt64() == b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Int64, NotEqual) { return a.AsInt64() != b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Int64, Less) { return a.AsInt64() < b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Int64, LessEqual) { return a.AsInt64() <= b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Int64, Greater) { return a.AsInt64() > b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Int64, GreaterEqual) { return a.AsInt64() >= b.AsInt64(); };

		VARIANT_EVALUATOR(Int64, Int64, BitAnd) { return a.AsInt64() & b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Int64, BitOr) { return a.AsInt64() | b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Int64, BitXOr) { return a.AsInt64() ^ b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Null, BitFlip) { return ~(a.AsInt64()); };
		VARIANT_EVALUATOR(Int64, Int64, BitShiftLeft) { return a.AsInt64() << b.AsInt64(); };
		VARIANT_EVALUATOR(Int64, Int64, BitShiftRight) { return a.AsInt64() >> b.AsInt64(); };
#pragma endregion

#pragma region Double
		VARIANT_EVALUATOR(Double, Double, Add) { return a.AsDouble() + b.AsDouble(); };
		VARIANT_EVALUATOR(Double, Double, Subtract) { return a.AsDouble() - b.AsDouble(); };
		VARIANT_EVALUATOR(Double, Double, Multiply) { return a.AsDouble() * b.AsDouble(); };
		VARIANT_EVALUATOR(Double, Double, Divide) { return a.AsDouble() / b.AsDouble(); };
		VARIANT_EVALUATOR(Double, Int64, Add) { return a.AsDouble() + b.AsInt64(); };
		VARIANT_EVALUATOR(Double, Int64, Subtract) { return a.AsDouble() - b.AsInt64(); };
		VARIANT_EVALUATOR(Double, Int64, Multiply) { return a.AsDouble() * b.AsInt64(); };
		VARIANT_EVALUATOR(Double, Int64, Divide) { return a.AsDouble() / b.AsInt64(); };
		VARIANT_EVALUATOR(Double, Null, Positive) { return +(a.AsDouble()); };
		VARIANT_EVALUATOR(Double, Null, Negative) { return -(a.AsDouble()); };

		VARIANT_EVALUATOR(Double, Double, Equal) { return a.AsDouble() == b.AsDouble(); };
		VARIANT_EVALUATOR(Double, Double, NotEqual) { return a.AsDouble() != b.AsDouble(); };
		VARIANT_EVALUATOR(Double, Double, Less) { return a.AsDouble() < b.AsDouble(); };
		VARIANT_EVALUATOR(Double, Double, LessEqual) { return a.AsDouble() <= b.AsDouble(); };
		VARIANT_EVALUATOR(Double, Double, Greater) { return a.AsDouble() > b.AsDouble(); };
		VARIANT_EVALUATOR(Double, Double, GreaterEqual) { return a.AsDouble() >= b.AsDouble(); };
#pragma endregion

#pragma region String
		VARIANT_EVALUATOR(String, String, Equal) { return a.AsString() == b.AsString(); };
		VARIANT_EVALUATOR(String, String, NotEqual) { return a.AsString() != b.AsString(); };
		VARIANT_EVALUATOR(String, String, Add) { return a.AsString() + b.AsString(); };
		VARIANT_EVALUATOR(String, Bool, Add) { return a.AsString() + b.AsString(); };
		VARIANT_EVALUATOR(String, Int64, Add) { return a.AsString() + b.AsString(); };
		VARIANT_EVALUATOR(String, Double, Add) { return a.AsString() + b.AsString(); };
		VARIANT_EVALUATOR(Bool, String, Add) { return a.AsString() + b.AsString(); };
		VARIANT_EVALUATOR(Int64, String, Add) { return a.AsString() + b.AsString(); };
		VARIANT_EVALUATOR(Double, String, Add) { return a.AsString() + b.AsString(); };
#pragma endregion

#pragma region Object
		VARIANT_EVALUATOR(Object, Object, Equal) { return a.AsObject() == b.AsObject(); };
		VARIANT_EVALUATOR(Object, Object, NotEqual) { return a.AsObject() != b.AsObject(); };
#pragma endregion
	}
}
