#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/InstanceId.h"

namespace Engine {
	class Object;
	
	class Variant final {
	public:
		enum class Type:byte {
			Null,		// Null

			Bool,		// Boolean
			Int64,		// 64-bit Integer
			Double,		// 64-bit Float

			String,		// String
			//Vector2,
			//Rect2,
			//NodePath,
			//Color,

			Object,		// ManualObject or ReferencedObject

			End,		// Marks the end
		};
		enum class Operator :byte {
			Equal,
			NotEqual,
			Less,
			LessEqual,
			Greater,
			GreaterEqual,

			Add,
			Subtract,
			Multiply,
			Divide,
			Mod,
			Negative,
			Positive,

			And,
			Or,
			XOr,
			Not,

			BitAnd,
			BitOr,
			BitXOr,
			BitFlip,
			BitShiftLeft,
			BitShiftRight,

			End,
		};
		void Clear();
		Type GetType() const;

		~Variant();

#pragma region Ctors
		Variant();
		Variant(bool value);
		Variant(byte value);
		Variant(sbyte value);
		Variant(int16 value);
		Variant(uint16 value);
		Variant(int32 value);
		Variant(uint32 value);
		Variant(int64 value);
		Variant(uint64 value);
		Variant(float value);
		Variant(double value);
		Variant(const String& value);
		Variant(Object* value);
#pragma endregion

#pragma region Converts
		operator bool() const;
		operator byte() const;
		operator sbyte() const;
		operator int16() const;
		operator uint16() const;
		operator int32() const;
		operator uint32() const;
		operator int64() const;
		operator uint64() const;
		operator float() const;
		operator double() const;
		operator String() const;
		operator Object*() const;
#pragma endregion

		String ToString() const;

		Variant& operator=(const Variant& obj);
		/*bool operator==(const Variant& obj) const;
		bool operator!=(const Variant& obj) const;
		bool operator>(const Variant& obj) const;
		bool operator>=(const Variant& obj) const;
		bool operator<(const Variant& obj) const;
		bool operator<=(const Variant& obj) const;*/

		static bool CanEvaluate(Operator op, Type a, Type b);
		static Variant Evaluate(Operator op, const Variant& a, const Variant& b);

	private:
		struct ObjectData {
			ObjectData(Object* ptr, const InstanceId& id);
			Object* ptr;
			InstanceId id;
		};
		union DataUnion {
			bool vBool;
			int64 vInt64;
			double vDouble;
			String vString;
			ObjectData vObject;
			DataUnion();
			~DataUnion();
		};

		void ConstructBool(bool value);
		void ConstructInt64(int64 value);
		void ConstructDouble(double value);
		void ConstructString(const String& value);
		void ConstructObject(const ObjectData& value);

		DataUnion data;
		Type type = Type::Null;

#pragma region Evaluation internal
		typedef Variant(*Evaluator)(const Variant& a, const Variant& b);

		// Evaluator table [typeA][typeB][op]
		static Evaluator evaluators[(sizeint)Type::End][(sizeint)Type::End][(sizeint)Operator::End];

		class EvaluatorInitializer final{
		public:
			EvaluatorInitializer();
		};
		inline static EvaluatorInitializer _evaluatorInitializer{};
#pragma endregion
	};
	bool operator==(const Variant& a, const Variant& b);
	bool operator!=(const Variant& a, const Variant& b);
	bool operator>(const Variant& a, const Variant& b);
	bool operator>=(const Variant& a, const Variant& b);
	bool operator<(const Variant& a, const Variant& b);
	bool operator<=(const Variant& a, const Variant& b);
}