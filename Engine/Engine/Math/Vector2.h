#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"

namespace Engine {
	struct Vector2 final {
		Vector2(float x = 0, float y = 0);

		float x;
		float y;

		/// @brief Get the length / magnitude of the Vector2.
		/// If you need the length information for comparing distance, GetLengthSquared() is usually preferred because it is faster.
		float GetLength() const;

		/// @brief Get the squared length / magnitude of the Vector2.
		/// This is faster than GetLength() because it doesn't need to be rooted.
		/// If you need the length information for comparing distance, this method is usually preferred.
		float GetLengthSquared() const;

		/// @brief Get normalized Vector2. The length will be changed to 1 without changing its direction. 
		Vector2 GetNormalized() const;

		/// @brief Normalize the current Vector2. The length will be changed to 1 without changing its direction.
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

		String ToString() const;
		int32 GetHashCode() const;

		static const Vector2 Up;
		static const Vector2 Down;
		static const Vector2 Left;
		static const Vector2 Right;
		static const Vector2 One;
		static const Vector2 Zero;

		/// @brief Calculate the dot product.
		/// Dot production formular: ( A dot B = |A| * |B| * cos(angle) ).
		/// When A and B are all normalized vector, the formular can be simplified as ( A dot B = cos(angle) ), as the length of A and B are both simply 1.
		/// With a simple ArcCos operation, the angle between vector A and B can be calculated.
		/// @see https://docs.godotengine.org/en/stable/tutorials/math/vector_math.html#dot-product
		/// @see https://kidscancode.org/godot_recipes/math/dot_cross_product/
		static float Dot(const Vector2& a, const Vector2& b);

		/// @brief Calculate the cross product.
		/// @see https://kidscancode.org/godot_recipes/math/dot_cross_product/
		static float Cross(const Vector2& a, const Vector2& b);

		static Vector2 Lerp(const Vector2& a, const Vector2& b, float time);

		/// @brief Get the radian angle between Vector a and b.
		/// @return The radian angle.
		static float AngleBetween(const Vector2& a, const Vector2& b);

		friend Vector2 operator*(float a, const Vector2& b);
		friend Vector2 operator/(float a, const Vector2& b);
	};
	Vector2 operator*(float a, const Vector2& b);
	Vector2 operator/(float a, const Vector2& b);
}