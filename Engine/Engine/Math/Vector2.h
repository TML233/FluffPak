#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Object.h"

namespace Engine {
	struct Vector2 final {
		Vector2(float x, float y);

		float x;
		float y;

		float GetLength() const;
		float GetLengthSquared() const;

		Vector2 GetNormalized() const;
		void Normalize();

		Vector2 operator+(const Vector2& value);
		Vector2 operator-(const Vector2& value);
		Vector2 operator*(float value);
		Vector2 operator/(float value);

		String ToString() const;
		int32 GetHashCode() const;

		static const Vector2 up;
		static const Vector2 down;
		static const Vector2 left;
		static const Vector2 right;
		static const Vector2 one;
		static const Vector2 zero;

		static Vector2 Lerp(const Vector2& a, const Vector2& b, float time);
		static float Dot(const Vector2& a, const Vector2& b);
		static float Cross(const Vector2& a, const Vector2& b);
	};
}