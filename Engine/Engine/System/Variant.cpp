#include "Engine/System/Variant.h"
#include "Engine/System/Object.h"
#include "Engine/System/Debug.h"

namespace Engine {
	String Variant::GetTypeName(Type type) {
		switch (type) {
			case Type::Null:
				return STRING_LITERAL(u8"Null");

			case Type::Bool:
				return STRING_LITERAL(u8"Bool");
			case Type::Int64:
				return STRING_LITERAL(u8"Int64");
			case Type::Double:
				return STRING_LITERAL(u8"Double");

			case Type::String:
				return STRING_LITERAL(u8"String");

			case Type::Object:
				return STRING_LITERAL(u8"Object");

			default:
				return STRING_LITERAL(u8"Undefined Type");
		}
	}
	String Variant::GetOperatorName(Operator op) {
		switch (op) {
			case Operator::Equal:
				return STRING_LITERAL(u8"Equal");
			case Operator::NotEqual:
				return STRING_LITERAL(u8"NotEqual");
			case Operator::Less:
				return STRING_LITERAL(u8"Less");
			case Operator::LessEqual:
				return STRING_LITERAL(u8"LessEqual");
			case Operator::Greater:
				return STRING_LITERAL(u8"Greater");
			case Operator::GreaterEqual:
				return STRING_LITERAL(u8"GreaterEqual");

			case Operator::Add:
				return STRING_LITERAL(u8"Add");
			case Operator::Subtract:
				return STRING_LITERAL(u8"Subtract");
			case Operator::Multiply:
				return STRING_LITERAL(u8"Multiply");
			case Operator::Divide:
				return STRING_LITERAL(u8"Divide");
			case Operator::Mod:
				return STRING_LITERAL(u8"Mod");
			case Operator::Positive:
				return STRING_LITERAL(u8"Positive");
			case Operator::Negative:
				return STRING_LITERAL(u8"Negative");

			case Operator::LogicAnd:
				return STRING_LITERAL(u8"LogicAnd");
			case Operator::LogicOr:
				return STRING_LITERAL(u8"LogicOr");
			case Operator::LogicXOr:
				return STRING_LITERAL(u8"LogicXOr");
			case Operator::LogicNot:
				return STRING_LITERAL(u8"LogicNot");

			case Operator::BitAnd:
				return STRING_LITERAL(u8"BitAnd");
			case Operator::BitOr:
				return STRING_LITERAL(u8"BitOr");
			case Operator::BitXOr:
				return STRING_LITERAL(u8"BitXOr");
			case Operator::BitFlip:
				return STRING_LITERAL(u8"BitFlip");
			case Operator::BitShiftLeft:
				return STRING_LITERAL(u8"BitShiftLeft");
			case Operator::BitShiftRight:
				return STRING_LITERAL(u8"BitShiftRight");

			default:
				return STRING_LITERAL(u8"Undefined Operator");
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
	Variant::Variant(const strchar* value) {
		ConstructString(String(value));
	}
	Variant::Variant(const Vector2& value) {
		ConstructVector2(value);
	}
	Variant::Variant(Object* value) {
		ConstructObject(ObjectData(value, (value != nullptr ? value->GetInstanceId() : InstanceId())));
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
				return STRING_LITERAL(u8"[Null]");
			case Type::Bool:
				return data.vBool ? STRING_LITERAL(u8"True") : STRING_LITERAL(u8"False");
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
					return STRING_LITERAL(u8"[Nullptr]");
				}
				if(!Object::IsInstanceValid(data.vObject.id)){
					return STRING_LITERAL(u8"[Released Object]");
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

	Variant& Variant::operator=(const Variant& obj) {
		if (&obj == this) {
			return *this;
		}

		Clear();

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

#pragma region Evaluating
#pragma region Operators
	bool Variant::operator==(const Variant& obj) const {
		if (!CanEvaluateDirectly(Operator::Equal, type, obj.type)) {
			return false;
		}
		return EvaluateDirectly(Operator::Equal, *this, obj).AsBool();
	}
	bool Variant::operator!=(const Variant& obj) const {
		if (!CanEvaluateDirectly(Operator::NotEqual, type, obj.type)) {
			return true;
		}
		return EvaluateDirectly(Operator::NotEqual, *this, obj).AsBool();
	}
	bool Variant::operator<(const Variant& obj) const {
		return EvaluateDirectly(Operator::Less, *this, obj).AsBool();
	}
	bool Variant::operator<=(const Variant& obj) const {
		return EvaluateDirectly(Operator::LessEqual, *this, obj).AsBool();
	}
	bool Variant::operator>(const Variant& obj) const {
		return EvaluateDirectly(Operator::Greater, *this, obj).AsBool();
	}
	bool Variant::operator>=(const Variant& obj) const {
		return EvaluateDirectly(Operator::GreaterEqual, *this, obj).AsBool();
	}
	Variant Variant::operator+(const Variant& obj) const {
		return EvaluateDirectly(Operator::Add, *this, obj);
	}
	Variant Variant::operator-(const Variant& obj) const {
		return EvaluateDirectly(Operator::Subtract, *this, obj);
	}
	Variant Variant::operator*(const Variant& obj) const {
		return EvaluateDirectly(Operator::Multiply, *this, obj);
	}
	Variant Variant::operator/(const Variant& obj) const {
		return EvaluateDirectly(Operator::Divide, *this, obj);
	}
	Variant Variant::operator%(const Variant& obj) const {
		return EvaluateDirectly(Operator::Mod, *this, obj);
	}
	Variant Variant::operator+() const {
		return EvaluateDirectly(Operator::Positive, *this, Variant());
	}
	Variant Variant::operator-() const {
		return EvaluateDirectly(Operator::Negative, *this, Variant());
	}
	Variant Variant::operator&&(const Variant& obj) const {
		return EvaluateDirectly(Operator::LogicAnd, *this, obj);
	}
	Variant Variant::operator||(const Variant& obj) const {
		return EvaluateDirectly(Operator::LogicOr, *this, obj);
	}
	Variant Variant::operator!() const {
		return EvaluateDirectly(Operator::LogicNot, *this, Variant());
	}
	Variant Variant::operator&(const Variant& obj) const {
		return EvaluateDirectly(Operator::BitAnd, *this, obj);
	}
	Variant Variant::operator|(const Variant& obj) const {
		return EvaluateDirectly(Operator::BitOr, *this, obj);
	}
	Variant Variant::operator^(const Variant& obj) const {
		return EvaluateDirectly(Operator::BitXOr, *this, obj);
	}
	Variant Variant::operator~() const {
		return EvaluateDirectly(Operator::BitFlip, *this, Variant());
	}
	Variant Variant::operator<<(const Variant& obj) const {
		return EvaluateDirectly(Operator::BitShiftLeft, *this, obj);
	}
	Variant Variant::operator>>(const Variant& obj) const {
		return EvaluateDirectly(Operator::BitShiftRight, *this, obj);
	}
#pragma endregion

	bool Variant::CanEvaluateDirectly(Operator op, Type a, Type b) {
		ERR_ASSERT((sizeint)op >= 0 && op < Operator::End, u8"op out of bounds.", return false);
		ERR_ASSERT((sizeint)a >= 0 && a < Type::End, u8"a out of bounds.", return false);
		ERR_ASSERT((sizeint)b >= 0 && b < Type::End, u8"b out of bounds.", return false);

		return evaluatorTable[(sizeint)a][(sizeint)b][(sizeint)op] != nullptr;
	}
	Variant Variant::EvaluateDirectly(Operator op, const Variant& a, const Variant& b) {
		ERR_ASSERT(CanEvaluateDirectly(op, a.type, b.type), String::Format(u8"No evaluator registered for [{0}] with [{1}] and [{2}].", GetOperatorName(op), GetTypeName(a.type), GetTypeName(b.type)).GetRawArray(), return Variant());

		Evaluator ev = evaluatorTable[(sizeint)a.type][(sizeint)b.type][(sizeint)op];

		return ev(a, b);
	}

	Variant::Evaluator Variant::evaluatorTable[(sizeint)Type::End][(sizeint)Type::End][(sizeint)Operator::End]{};

#define VARIANT_EVALUATOR(typeA,typeB,op) evaluatorTable[(sizeint)(Type::typeA)][(sizeint)(Type::typeB)][(sizeint)(Operator::op)] = [](const Variant& a, const Variant& b) -> Variant
	void Variant::Initializer::InitEvaluatorTable() {
		// !! AddTypeHint 9.0: Add necessary evaluator.

#pragma region Null
		VARIANT_EVALUATOR(Null, Null, Equal) { return true; };
		VARIANT_EVALUATOR(Null, Null, NotEqual) { return false; };
#pragma endregion

#pragma region Bool
		VARIANT_EVALUATOR(Bool, Bool, Equal) { return a.AsBool() == b.AsBool(); };
		VARIANT_EVALUATOR(Bool, Bool, NotEqual) { return a.AsBool() != b.AsBool(); };
		VARIANT_EVALUATOR(Bool, Null, LogicNot) { return !a.AsBool(); };
		VARIANT_EVALUATOR(Bool, Bool, LogicAnd) { return a.AsBool() && b.AsBool(); };
		VARIANT_EVALUATOR(Bool, Bool, LogicOr) { return a.AsBool() || b.AsBool(); };
		VARIANT_EVALUATOR(Bool, Bool, LogicXOr) { return a.AsBool() ^ b.AsBool(); };
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

#pragma region Vector2
		VARIANT_EVALUATOR(Vector2, Vector2, Equal) { return a.AsVector2() == b.AsVector2(); };
		VARIANT_EVALUATOR(Vector2, Vector2, NotEqual) { return a.AsVector2() != b.AsVector2(); };

		VARIANT_EVALUATOR(Vector2, Vector2, Add) { return a.AsVector2() + b.AsVector2(); };
		VARIANT_EVALUATOR(Vector2, Vector2, Subtract) { return a.AsVector2() - b.AsVector2(); };
		VARIANT_EVALUATOR(Vector2, Null, Positive) { return +a.AsVector2(); };
		VARIANT_EVALUATOR(Vector2, Null, Negative) { return -a.AsVector2(); };

		VARIANT_EVALUATOR(Vector2, Int64, Multiply) { return a.AsVector2() * static_cast<float>(b.AsInt64()); };
		VARIANT_EVALUATOR(Vector2, Int64, Divide) { return a.AsVector2() / static_cast<float>(b.AsInt64()); };
		VARIANT_EVALUATOR(Int64, Vector2, Multiply) { return static_cast<float>(a.AsInt64()) * b.AsVector2(); };
		VARIANT_EVALUATOR(Int64, Vector2, Divide) { return static_cast<float>(a.AsInt64()) / b.AsVector2(); };
		VARIANT_EVALUATOR(Vector2, Double, Multiply) { return a.AsVector2() * static_cast<float>(b.AsDouble()); };
		VARIANT_EVALUATOR(Vector2, Double, Divide) { return a.AsVector2() / static_cast<float>(b.AsDouble()); };
		VARIANT_EVALUATOR(Double, Vector2, Multiply) { return static_cast<float>(a.AsDouble()) * b.AsVector2(); };
		VARIANT_EVALUATOR(Double, Vector2, Divide) { return static_cast<float>(a.AsDouble()) / b.AsVector2(); };
#pragma endregion

	}
#undef VARIANT_EVALUATOR

#pragma endregion

	Variant::Initializer::Initializer() {
		InitImplicitConversionTable();
		InitEvaluatorTable();
	}
}