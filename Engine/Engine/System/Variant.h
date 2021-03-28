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
			Byte,
			Int16,
			Int32,
			Int64,
			Float,
			Double,

			// === Custom Data Types ===
			
			String,
			//Vector2,
			//Rect2,
			//NodePath,

			Object,
		};

		~Variant();
		Variant();
		Variant(bool value);
		Variant(byte value);
		Variant(int16 value);
		Variant(int32 value);
		Variant(int64 value);
		Variant(float value);
		Variant(double value);

		Variant(const String& value);
		
		Variant(Object* value);

	private:
		Type currentType;

		struct ObjectData {
			Object* ptr;
			ObjectInstanceId id;
		};
		union DataUnion {
			bool vBool;
			byte vByte;
			int16 vInt16;
			int32 vInt32;
			int64 vInt64;
			float vFloat;
			double vDouble;
			String vString;
			ObjectData vObject;
		};
		DataUnion data;
	};
}