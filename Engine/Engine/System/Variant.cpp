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

	bool operator==(const Variant& a, const Variant& b) {
		if (!Variant::CanEvaluate(Variant::Operator::Equal, a.GetType(), b.GetType())) {
			return false;
		}
		return Variant::Evaluate(Variant::Operator::Equal, a, b);
	}
	bool operator!=(const Variant& a, const Variant& b) {
		if (!Variant::CanEvaluate(Variant::Operator::NotEqual, a.GetType(), b.GetType())) {
			return true;
		}
		return Variant::Evaluate(Variant::Operator::NotEqual, a, b);
	}
	bool operator<(const Variant& a, const Variant& b) {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::Less, a.GetType(), b.GetType()), "Cannot evaluate Less! No evaluator registered for this.", return false);
		return Variant::Evaluate(Variant::Operator::Less, a, b);
	}
	bool operator<=(const Variant& a, const Variant& b) {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::LessEqual, a.GetType(), b.GetType()), "Cannot evaluate LessEqual! No evaluator registered for this.", return false);
		return Variant::Evaluate(Variant::Operator::LessEqual, a, b);
	}
	bool operator>(const Variant& a, const Variant& b) {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::Greater, a.GetType(), b.GetType()), "Cannot evaluate Greater! No evaluator registered for this.", return false);
		return Variant::Evaluate(Variant::Operator::Greater, a, b);
	}
	bool operator>=(const Variant& a, const Variant& b) {
		ERR_ASSERT(Variant::CanEvaluate(Variant::Operator::GreaterEqual, a.GetType(), b.GetType()), "Cannot evaluate GreaterEqual! No evaluator registered for this.", return false);
		return Variant::Evaluate(Variant::Operator::GreaterEqual, a, b);
	}

	bool Variant::CanEvaluate(Operator op, Type a, Type b) {
		ERR_ASSERT((sizeint)op >= 0 && op < Operator::End, "op out of bounds.", return false);
		ERR_ASSERT((sizeint)a >= 0 && a < Type::End, "a out of bounds.", return false);
		ERR_ASSERT((sizeint)b >= 0 && b < Type::End, "b out of bounds.", return false);

		return evaluators[(sizeint)a][(sizeint)b][(sizeint)op] != nullptr;
	}
	Variant Variant::Evaluate(Operator op, const Variant& a,const Variant& b) {
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
		VARIANT_EVALUATOR(Null, Null, Equal) { return true; };
		VARIANT_EVALUATOR(Null, Null, NotEqual) { return false; };

		// Bool
		VARIANT_EVALUATOR(Bool, Bool, Equal) { return a.operator bool() == b.operator bool(); };
		VARIANT_EVALUATOR(Bool, Bool, NotEqual) { return a.operator bool() != b.operator bool(); };
		VARIANT_EVALUATOR(Bool, Null, Not) { return !a; };
		VARIANT_EVALUATOR(Bool, Bool, And) { return a.operator bool() && b.operator bool(); };
		VARIANT_EVALUATOR(Bool, Bool, Or) { return a.operator bool() || b.operator bool(); };
		VARIANT_EVALUATOR(Bool, Bool, XOr) { return a.operator bool() ^ b.operator bool(); };

		// Int64
		VARIANT_EVALUATOR(Int64, Int64, Add) { return a.operator int64() + b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, Subtract) { return a.operator int64() - b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, Multiply) { return a.operator int64() * b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, Divide) { return a.operator int64() / b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, Mod) { return a.operator int64() % b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, Positive) { return +(a.operator int64()); };
		VARIANT_EVALUATOR(Int64, Int64, Negative) { return -(a.operator int64()); };

		VARIANT_EVALUATOR(Int64, Int64, Equal) { return a.operator int64() == b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, NotEqual) { return a.operator int64() != b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, Less) { return a.operator int64() < b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, LessEqual) { return a.operator int64() <= b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, Greater) { return a.operator int64() > b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, GreaterEqual) { return a.operator int64() >= b.operator int64(); };
		
		VARIANT_EVALUATOR(Int64, Int64, BitAnd) { return a.operator int64() & b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, BitOr) { return a.operator int64() | b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, BitXOr) { return a.operator int64() ^ b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, BitFlip) { return ~(a.operator int64()); };
		VARIANT_EVALUATOR(Int64, Int64, BitShiftLeft) { return a.operator int64() << b.operator int64(); };
		VARIANT_EVALUATOR(Int64, Int64, BitShiftRight) { return a.operator int64() >> b.operator int64(); };

	}
}
