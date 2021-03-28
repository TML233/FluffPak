#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/InstanceId.h"

namespace Engine {
	class Object;
	
	class Variant final {
	public:
		enum class Type:byte {
			Null,

			// === Plan Data Types ===

			Bool,
			Int64,
			Double,

			// === Custom Data Types ===
			
			String,
			//Vector2,
			//Rect2,
			//NodePath,

			Object,
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
		//Variant(Variant&& obj);
		//Variant& operator=(Variant&& obj);

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
	};
}