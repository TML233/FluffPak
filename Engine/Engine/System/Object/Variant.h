#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Object/InstanceId.h"
#include "Engine/System/Math/Vector.h"
#include "Engine/System/Concept.h"
#include <type_traits>

namespace Engine {
	class Object;

	class Variant final {
	public:
		// !! AddTypeHint 0.0: To add a new type, search for "AddTypeHint" for the guide.

#pragma region Types
		// !! AddTypeHint 1.0: Add an entry in Type.

		enum class Type:byte {
			Null,		// Null

			Bool,		// Boolean
			Byte,
			SByte,
			Int16,
			UInt16,
			Int32,
			UInt32,
			Int64,		// 64-bit Integer
			UInt64,
			Float,
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

		template<typename T>
		struct GetTypeFromNative;
		template<typename T>
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

		template<Concept::IsEnum T>
		Variant(T value);

#pragma endregion

#pragma region AsType
		// !! AddTypeHint 5.0: Add a AsType function for getting the original value.

		bool AsBool(bool defaultValue=false) const;
		byte AsByte(byte defaultValue = 0) const;
		sbyte AsSByte(sbyte defaultValue = 0) const;
		int16 AsInt16(int16 defaultValue = 0) const;
		uint16 AsUInt16(uint16 defaultValue = 0) const;
		int32 AsInt32(int32 defaultValue = 0) const;
		uint32 AsUInt32(uint32 defaultValue = 0) const;
		int64 AsInt64(int64 defaultValue = 0) const;
		uint64 AsUInt64(uint64 defaultValue = 0) const;
		float AsFloat(float defaultValue = 0) const;
		double AsDouble(double defaultValue = 0) const;
		String AsString(String defaultValue = u8"") const;
		Vector2 AsVector2(const Vector2& defaultValue = Vector2()) const;
		Object* AsObject(Object* defaultValue = nullptr) const;
#pragma endregion

		static bool CanConvertImplicitly(Type from, Type to);

		String ToString() const;

		Variant(const Variant& obj);
		Variant& operator=(const Variant& obj);

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
			byte vByte;
			sbyte vSByte;
			int16 vInt16;
			uint16 vUInt16;
			int32 vInt32;
			uint32 vUInt32;
			int64 vInt64;
			uint64 vUInt64;
			float vFloat;
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
		void ConstructByte(byte value);
		void ConstructSByte(sbyte value);
		void ConstructInt16(int16 value);
		void ConstructUInt16(uint16 value);
		void ConstructInt32(int32 value);
		void ConstructUInt32(uint32 value);
		void ConstructInt64(int64 value);
		void ConstructUInt64(uint64 value);
		void ConstructFloat(float value);
		void ConstructDouble(double value);
		void ConstructString(const String& value);
		void ConstructObject(const ObjectData& value);
		void ConstructVector2(const Vector2& value);
#pragma endregion

		DataUnion data;
		Type type = Type::Null;

		void AssignValue(const Variant& obj);


		typedef Variant(*Convertor)(const Variant& value);
		static Convertor implicitConverterTable[(sizeint)Type::End][(sizeint)Type::End];
		
		class Initializer final {
		public:
			Initializer();
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
		static const Type type = Type::Byte;
	};
	template<>
	struct Variant::GetTypeFromNative<sbyte> {
		static const Type type = Type::SByte;
	};
	template<>
	struct Variant::GetTypeFromNative<int16> {
		static const Type type = Type::Int16;
	};
	template<>
	struct Variant::GetTypeFromNative<uint16> {
		static const Type type = Type::UInt16;
	};
	template<>
	struct Variant::GetTypeFromNative<int32> {
		static const Type type = Type::Int32;
	};
	template<>
	struct Variant::GetTypeFromNative<uint32> {
		static const Type type = Type::UInt32;
	};
	template<>
	struct Variant::GetTypeFromNative<int64> {
		static const Type type = Type::Int64;
	};
	template<>
	struct Variant::GetTypeFromNative<uint64> {
		static const Type type = Type::UInt64;
	};
	template<>
	struct Variant::GetTypeFromNative<float> {
		static const Type type = Type::Float;
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

	template<Concept::IsObject T>
	struct Variant::GetTypeFromNative<T*> {
		static const Type type = Type::Object;
	};
	template<Concept::IsObject T>
	struct Variant::GetTypeFromNative<const T*> {
		static const Type type = Type::Object;
	};

	template<Concept::IsReferencedObject T>
	struct Variant::GetTypeFromNative<IntrusivePtr<T>> {
		static const Type type = Type::Object;
	};
	template<Concept::IsReferencedObject T>
	struct Variant::GetTypeFromNative<const IntrusivePtr<T>> {
		static const Type type = Type::Object;
	};

	template<Concept::IsEnum T>
	struct Variant::GetTypeFromNative<T> {
		static const Type type = Type::Int32;
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
			return obj.AsByte();
		}
	};
	template<>
	struct Variant::CastToNative<sbyte> {
		static sbyte Cast(const Variant& obj) {
			return obj.AsSByte();
		}
	};
	template<>
	struct Variant::CastToNative<int16> {
		static int16 Cast(const Variant& obj) {
			return obj.AsInt16();
		}
	};
	template<>
	struct Variant::CastToNative<uint16> {
		static uint16 Cast(const Variant& obj) {
			return obj.AsUInt16();
		}
	};
	template<>
	struct Variant::CastToNative<int32> {
		static int32 Cast(const Variant& obj) {
			return obj.AsInt32();
		}
	};
	template<>
	struct Variant::CastToNative<uint32> {
		static uint32 Cast(const Variant& obj) {
			return obj.AsUInt32();
		}
	};
	template<>
	struct Variant::CastToNative<int64> {
		static int64 Cast(const Variant& obj) {
			return obj.AsInt64();
		}
	};
	template<>
	struct Variant::CastToNative<uint64> {
		static uint64 Cast(const Variant& obj) {
			return obj.AsUInt64();
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
	template<Concept::IsObject T>
	struct Variant::CastToNative<T*> {
		static T* Cast(const Variant& obj) {
			return (T*)obj.AsObject();
		}
	};
	template<Concept::IsObject T>
	struct Variant::CastToNative<const T*> {
		static const T* Cast(const Variant& obj) {
			return (const T*)obj.AsObject();
		}
	};
	template<Concept::IsReferencedObject T>
	struct Variant::CastToNative<IntrusivePtr<T>> {
		static IntrusivePtr<T> Cast(const Variant& obj) {
			return IntrusivePtr<T>((T*)obj.AsObject());
		}
	};
	template<Concept::IsReferencedObject T>
	struct Variant::CastToNative<const IntrusivePtr<T>> {
		static const IntrusivePtr<T> Cast(const Variant& obj) {
			return IntrusivePtr<T>((T*)obj.AsObject());
		}
	};
	template<Concept::IsReferencedObject T>
	struct Variant::CastToNative<const IntrusivePtr<T>&> {
		static const IntrusivePtr<T> Cast(const Variant& obj) {
			return IntrusivePtr<T>((T*)obj.AsObject());
		}
	};

	template<Concept::IsEnum T>
	struct Variant::CastToNative<T> {
		static T Cast(const Variant& obj) {
			return (T)obj.AsInt64();
		}
	};
#pragma endregion
}