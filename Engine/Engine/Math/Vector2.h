#ifndef HEADER_CORE_MATH_VECTOR2
#define HEADER_CORE_MATH_VECTOR2

#include "Engine/System/Definition.h"
#include "Engine/System/Object.h"

namespace Engine {
	struct Vector2 final:public Object {
		Vector2(Float x, Float y);

		Float x;
		Float y;

		Float GetLength() const;
		Float GetLengthSquared() const;

		Vector2 GetNormalized() const;
		void Normalize();

		Vector2 operator+(const Vector2& value);
		Vector2 operator-(const Vector2& value);
		Vector2 operator*(Float value);
		Vector2 operator/(Float value);

		String ToString() const override;

		static const Vector2 up;
		static const Vector2 down;
		static const Vector2 left;
		static const Vector2 right;
		static const Vector2 one;
		static const Vector2 zero;

		static Vector2 Lerp(const Vector2& a, const Vector2& b, Float time);
		static Float Dot(const Vector2& a, const Vector2& b);
		static Float Cross(const Vector2& a, const Vector2& b);
	};
}

#endif