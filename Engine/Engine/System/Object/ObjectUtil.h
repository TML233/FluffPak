#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Concept.h"

namespace Engine{
	class ObjectUtil final {
	public:
		STATIC_CLASS(ObjectUtil);

#pragma region ToStrings
		template<typename T>
		static String ToString(const T& obj) {
			return obj.ToString();
		}
		static String ToString(bool obj);
		static String ToString(byte obj);
		static String ToString(sbyte obj);
		static String ToString(int16 obj);
		static String ToString(uint16 obj);
		static String ToString(int32 obj);
		static String ToString(uint32 obj);
		static String ToString(int64 obj);
		static String ToString(uint64 obj);
		static String ToString(float obj);
		static String ToString(double obj);
#pragma endregion

#pragma region GetHashCodes
		template<typename T>
		static int32 GetHashCode(const T& obj) {
			return obj.GetHashCode();
		}
		template<Concept::IsEnum T>
		static int32 GetHashCode(T obj) {
			return (int32)obj;
		}
		static int32 GetHashCode(bool obj);
		static int32 GetHashCode(byte obj);
		static int32 GetHashCode(sbyte obj);
		static int32 GetHashCode(int16 obj);
		static int32 GetHashCode(uint16 obj);
		static int32 GetHashCode(int32 obj);
		static int32 GetHashCode(uint32 obj);
		static int32 GetHashCode(int64 obj);
		static int32 GetHashCode(uint64 obj);
		static int32 GetHashCode(float obj);
		static int32 GetHashCode(double obj);
		static int32 GetHashCode(const void* obj);
#pragma endregion
	};
}
