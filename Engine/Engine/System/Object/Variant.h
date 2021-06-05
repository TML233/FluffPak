#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Object/InstanceId.h"
#include "Engine/System/Math/Vector.h"
#include <type_traits>

namespace Engine {
	class Object;
	class ReferencedObject;

	class Variant final {
	public:
		// !! AddTypeHint 0.0: To add a new type, search for "AddTypeHint" for the guide.

#pragma region Types
		// !! AddTypeHint 1.0: Add an entry in Type.

		enum class Type:byte {
			Null,		// Null

			Bool,		// Boolean
			Int64,		// 64-bit Integer
			Double,		// 64-bit Float

			String,		// String
			Vector2,
			//Rect2,
			//NodePath,
			//Color,

			Object,		// ManualObject or ReferencedObject

			End,		// Marks the end
		};

		static String GetTypeName(Type type);

		template<typename T, typename = void>
		struct GetTypeFromNative;
		template<typename T, typename = void>
		struct CastToNative;
#pragma endregion

#pragma region Operators
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
			Positive,
			Negative,

			LogicAnd,
			LogicOr,
			LogicXOr,
			LogicNot,

			BitAnd,
			BitOr,
			BitXOr,
			BitFlip,
			BitShiftLeft,
			BitShiftRight,

			End,
		};
		static String GetOperatorName(Operator op);
#pragma endregion

		void Clear();
		Type GetType() const;

		~Variant();

#pragma region Ctors
		// !! AddTypeHint 4.0: Add a constructor to call ConstructT(T value).

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
		Variant(const Vector2& value);
		Variant(const u8char* value);
		Variant(Object* value);
#pragma endregion

#pragma region AsType
		// !! AddTypeHint 5.0: Add a AsType function for getting the original value.

		bool AsBool(bool defaultValue=false) const;
		int64 AsInt64(int64 defaultValue = 0) const;
		double AsDouble(double defaultValue = 0) const;
		String AsString(String defaultValue = u8"") const;
		Vector2 AsVector2(const Vector2& defaultValue = Vector2()) const;
		Object* AsObject(Object* defaultValue = nullptr) const;
#pragma endregion

		static bool CanConvertImplicitly(Type from, Type to);

		String ToString() const;

		Variant(const Variant& obj);
		Variant& operator=(const Variant& obj);

#pragma region Operators
		bool operator==(const Variant& obj) const;
		bool operator!=(const Variant& obj) const;
		bool operator>(const Variant& obj) const;
		bool operator>=(const Variant& obj) const;
		bool operator<(const Variant& obj) const;
		bool operator<=(const Variant& obj) const;

		Variant operator+(const Variant& obj) const;
		Variant operator-(const Variant& obj) const;
		Variant operator*(const Variant& obj) const;
		Variant operator/(const Variant& obj) const;
		Variant operator%(const Variant& obj) const;
		Variant operator+() const;
		Variant operator-() const;
		
		Variant operator&&(const Variant& obj) const;
		Variant operator||(const Variant& obj) const;
		Variant operator!() const;

		Variant operator&(const Variant& obj) const;
		Variant operator|(const Variant& obj) const;
		Variant operator^(const Variant& obj) const;
		Variant operator~() const;
		Variant operator<<(const Variant& obj) const;
		Variant operator>>(const Variant& obj) const;
#pragma endregion

		// Indicates that if a and b can evaluate with type formatting.
		static bool CanEvaluateDirectly(Operator op, Type a, Type b);
		// Indicates that if a and b can evaluate without any type formatting.
		static bool CanEvaluate(Operator op, Type a, Type b);
		// Evaluate operand a and b with operator. A type formatting will be done if needed.
		static Variant EvaluateDirectly(Operator op, const Variant& a, const Variant& b);
		// Evaluate operand a and b with operator directly, without type formatting.
		static Variant Evaluate(Operator op, const Variant& a, const Variant& b);
	private:
		struct ObjectData {
			ObjectData(Object* ptr, const InstanceId& id);
			Object* ptr;
			InstanceId id;
		};

#pragma region Union
		// !! AddTypeHint 2.0: Add an entry in DataUnion.

		union DataUnion {
			bool vBool;
			int64 vInt64;
			double vDouble;
			String vString;
			Vector2 vVector2;
			ObjectData vObject;
			DataUnion();
			~DataUnion();
		};
#pragma endregion

#pragma region Value constructor
		// !! AddTypeHint 3.0: Add a value constructor.

		void ConstructBool(bool value);
		void ConstructInt64(int64 value);
		void ConstructDouble(double value);
		void ConstructString(const String& value);
		void ConstructObject(const ObjectData& value);
		void ConstructVector2(const Vector2& value);
#pragma endregion

		DataUnion data;
		Type type = Type::Null;

		void AssignValue(const Variant& obj);


		typedef Variant(*Evaluator)(const Variant& a, const Variant& b);
		// [typeA][typeB][op]
		static Evaluator evaluatorTable[(sizeint)Type::End][(sizeint)Type::End][(sizeint)Operator::End];

		typedef Variant(*Convertor)(const Variant& value);
		static Convertor implicitConverterTable[(sizeint)Type::End][(sizeint)Type::End];
		
		class Initializer final {
		public:
			Initializer();
			static void InitEvaluatorTable();
			static void InitImplicitConversionTable();
		};
		inline static Initializer _initializer{};
	};


#pragma region GetTypeFromNative
	//! AddTypeHint 10.0: Add necessary GetTypeFromNative.

	template<>
	struct Variant::GetTypeFromNative<bool> {
		static const Type type = Type::Bool;
	};
	template<>
	struct Variant::GetTypeFromNative<byte> {
		static const Type type = Type::Int64;
	};
	template<>
	struct Variant::GetTypeFromNative<sbyte> {
		static const Type type = Type::Int64;
	};
	template<>
	struct Variant::GetTypeFromNative<int16> {
		static const Type type = Type::Int64;
	};
	template<>
	struct Variant::GetTypeFromNative<uint16> {
		static const Type type = Type::Int64;
	};
	template<>
	struct Variant::GetTypeFromNative<int32> {
		static const Type type = Type::Int64;
	};
	template<>
	struct Variant::GetTypeFromNative<uint32> {
		static const Type type = Type::Int64;
	};
	template<>
	struct Variant::GetTypeFromNative<int64> {
		static const Type type = Type::Int64;
	};
	template<>
	struct Variant::GetTypeFromNative<uint64> {
		static const Type type = Type::Int64;
	};
	template<>
	struct Variant::GetTypeFromNative<float> {
		static const Type type = Type::Double;
	};
	template<>
	struct Variant::GetTypeFromNative<double> {
		static const Type type = Type::Double;
	};
	template<>
	struct Variant::GetTypeFromNative<String> {
		static const Type type = Type::String;
	};
	template<>
	struct Variant::GetTypeFromNative<Vector2> {
		static const Type type = Type::Vector2;
	};
	template<typename T>
	struct Variant::GetTypeFromNative<T*, typename std::enable_if_t<std::is_base_of_v<Object, T>>> {
		static const Type type = Type::Object;
	};
	template<typename T>
	struct Variant::GetTypeFromNative<const T*, typename std::enable_if_t<std::is_base_of_v<Object, T>>> {
		static const Type type = Type::Object;
	};
	template<typename T>
	struct Variant::GetTypeFromNative<IntrusivePtr<T>, typename std::enable_if_t<std::is_base_of_v<ReferencedObject, T>>> {
		static const Type type = Type::Object;
	};
#pragma endregion

#pragma region CastToNative
	//! AddTypeHint 11.0: Add necessary CastToNative.

	template<>
	struct Variant::CastToNative<bool> {
		static bool Cast(const Variant& obj) {
			return obj.AsBool();
		}
	};
	template<>
	struct Variant::CastToNative<byte> {
		static byte Cast(const Variant& obj) {
			return (byte)obj.AsInt64();
		}
	};
	template<>
	struct Variant::CastToNative<sbyte> {
		static sbyte Cast(const Variant& obj) {
			return (sbyte)obj.AsInt64();
		}
	};
	template<>
	struct Variant::CastToNative<int16> {
		static int16 Cast(const Variant& obj) {
			return (int16)obj.AsInt64();
		}
	};
	template<>
	struct Variant::CastToNative<uint16> {
		static uint16 Cast(const Variant& obj) {
			return (uint16)obj.AsInt64();
		}
	};
	template<>
	struct Variant::CastToNative<int32> {
		static int32 Cast(const Variant& obj) {
			return (int32)obj.AsInt64();
		}
	};
	template<>
	struct Variant::CastToNative<uint32> {
		static uint32 Cast(const Variant& obj) {
			return (uint32)obj.AsInt64();
		}
	};
	template<>
	struct Variant::CastToNative<int64> {
		static int64 Cast(const Variant& obj) {
			return (int64)obj.AsInt64();
		}
	};
	template<>
	struct Variant::CastToNative<uint64> {
		static uint64 Cast(const Variant& obj) {
			return (uint64)obj.AsInt64();
		}
	};
	template<>
	struct Variant::CastToNative<String> {
		static String Cast(const Variant& obj) {
			return obj.AsString();
		}
	};
	template<>
	struct Variant::CastToNative<const String&> {
		static String Cast(const Variant& obj) {
			return obj.AsString();
		}
	};
	template<>
	struct Variant::CastToNative<Vector2> {
		static Vector2 Cast(const Variant& obj) {
			return obj.AsVector2();
		}
	};
	template<>
	struct Variant::CastToNative<const Vector2&> {
		static Vector2 Cast(const Variant& obj) {
			return obj.AsVector2();
		}
	};
	template<typename T>
	struct Variant::CastToNative<T*, typename std::enable_if_t<std::is_base_of_v<Object, T>>> {
		static T* Cast(const Variant& obj) {
			return (T*)obj.AsObject();
		}
	};
	template<typename T>
	struct Variant::CastToNative<const T*, typename std::enable_if_t<std::is_base_of_v<Object, T>>> {
		static const T* Cast(const Variant& obj) {
			return (const T*)obj.AsObject();
		}
	};
	template<typename T>
	struct Variant::CastToNative<IntrusivePtr<T>, typename std::enable_if_t<std::is_base_of_v<ReferencedObject, T>>> {
		static IntrusivePtr<T> Cast(const Variant& obj) {
			return IntrusivePtr<T>((T*)obj.AsObject());
		}
	};
#pragma endregion
}