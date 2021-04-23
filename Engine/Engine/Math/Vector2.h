#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"

namespace Engine {
	struct Vector2 final {
		Vector2(float x = 0, float y = 0);

		float x;
		float y;

		float GetLength() const;
		float GetLengthSquared() const;

		Vector2 GetNormalized() const;
		void Normalize();

		Vector2 operator+(const Vector2& value) const;
		Vector2& operator+=(const Vector2& value);
		Vector2 operator-(const Vector2& value) const;
		Vector2& operator-=(const Vector2& value);
		Vector2 operator*(float value) const;
		Vector2& operator*=(float value);
		Vector2 operator/(float value) const;
		Vector2& operator/=(float value);
		bool operator==(const Vector2& value) const;
		bool operator!=(const Vector2& value) const;
		Vector2 operator+() const;
		Vector2 operator-() const;
		float Dot(const Vector2& vec) const;
		float Cross(const Vector2& vec) const;

		String ToString() const;
		int32 GetHashCode() const;

		static const Vector2 up;
		static const Vector2 down;
		static const Vector2 left;
		static const Vector2 right;
		static const Vector2 one;
		static const Vector2 zero;

		static Vector2 Lerp(const Vector2& a, const Vector2& b, float time);

		friend Vector2 operator*(float a, const Vector2& b);
		friend Vector2 operator/(float a, const Vector2& b);
	};
	Vector2 operator*(float a, const Vector2& b);
	Vector2 operator/(float a, const Vector2& b);
}