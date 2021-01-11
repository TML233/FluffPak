#ifndef HEADER_CORE_MATH_VECTOR3
#define HEADER_CORE_MATH_VECTOR3

#include "Core/Definition.h"
#include "Core/System/Object.h"

namespace Core {
	struct Vector3 final:public Object {
		Vector3(Float x, Float y, Float z);

		Float x;
		Float y;
		Float z;

		Float GetLength() const;
		Float GetLengthSquared() const;

		Vector3 GetNormalized() const;
		void Normalize();

		Vector3 operator+(const Vector3& value);
		Vector3 operator-(const Vector3& value);
		Vector3 operator*(Float value);
		Vector3 operator/(Float value);

		String ToString() const override;

		static const Vector3 up;
		static const Vector3 down;
		static const Vector3 left;
		static const Vector3 right;
		static const Vector3 forward;
		static const Vector3 backward;
		static const Vector3 one;
		static const Vector3 zero;

		static Vector3 Lerp(const Vector3& a, const Vector3& b, Float time);
		static Float Dot(const Vector3& a, const Vector3& b);
		static Vector3 Cross(const Vector3& a, const Vector3& b);
	};
}

#endif