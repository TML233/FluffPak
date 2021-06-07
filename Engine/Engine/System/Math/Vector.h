#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"

namespace Engine {
	struct Vector2 final {
		Vector2(float x = 0, float y = 0);

		float x;
		float y;

		/// @brief Get the length / magnitude of the Vector2.
		/// @note If you need the length information for comparing distance, GetLengthSquared() is usually preferred because it is faster.
		float GetLength() const;
		/// @brief Get the squared length / magnitude of the Vector2.
		/// @note This is faster than GetLength() because it doesn't need to be rooted.\n
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
		/// 
		/// @note Dot production formular: ( A dot B = |A| * |B| * cos(angle) ).\n
		/// When A and B are all normalized vector, the formular can be simplified as ( A dot B = cos(angle) ), as the length of A and B are both simply 1.\n
		/// With a simple ArcCos operation, the angle between vector A and B can be calculated.
		/// 
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
	};
	Vector2 operator*(float a, const Vector2& b);
	Vector2 operator/(float a, const Vector2& b);

	struct Vector3 final {
		Vector3(float x = 0, float y = 0, float z = 0);
		Vector3(const Vector2& vec2, float z = 0);

		float x = 0;
		float y = 0;
		float z = 0;

		/// @brief Get the length / magnitude of the Vector2.
		/// @note If you need the length information for comparing distance, GetLengthSquared() is usually preferred because it is faster.
		float GetLength() const;
		/// @brief Get the squared length / magnitude of the Vector2.
		/// @note This is faster than GetLength() because it doesn't need to be rooted.\n
		/// If you need the length information for comparing distance, this method is usually preferred.
		float GetLengthSquared() const;

		/// @brief Get normalized Vector2. The length will be changed to 1 without changing its direction. 
		Vector3 GetNormalized() const;
		/// @brief Normalize the current Vector2. The length will be changed to 1 without changing its direction.
		void Normalize();

		Vector3 operator+(const Vector3& value) const;
		Vector3& operator+=(const Vector3& value);
		Vector3 operator-(const Vector3& value) const;
		Vector3& operator-=(const Vector3& value);
		Vector3 operator*(float value) const;
		Vector3& operator*=(float value);
		Vector3 operator/(float value) const;
		Vector3& operator/=(float value);
		bool operator==(const Vector3& value) const;
		bool operator!=(const Vector3& value) const;
		Vector3 operator+() const;
		Vector3 operator-() const;

		String ToString() const;
		int32 GetHashCode() const;

		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Left;
		static const Vector3 Right;
		static const Vector3 Forward;
		static const Vector3 Back;
		static const Vector3 One;
		static const Vector3 Zero;

		/// @brief Calculate the dot product.
		/// 
		/// @note Dot production formular: ( A dot B = |A| * |B| * cos(angle) ).\n
		/// When A and B are all normalized vector, the formular can be simplified as ( A dot B = cos(angle) ), as the length of A and B are both simply 1.\n
		/// With a simple ArcCos operation, the angle between vector A and B can be calculated.
		/// 
		/// @see https://docs.godotengine.org/en/stable/tutorials/math/vector_math.html#dot-product
		/// @see https://kidscancode.org/godot_recipes/math/dot_cross_product/
		static float Dot(const Vector3& a, const Vector3& b);

		/// @brief Calculate the cross product.
		/// @see https://kidscancode.org/godot_recipes/math/dot_cross_product/
		static Vector3 Cross(const Vector3& a, const Vector3& b);

		static Vector3 Lerp(const Vector3& a, const Vector3& b, float time);

		/// @brief Get the radian angle between Vector a and b.
		/// @return The radian angle.
		static float AngleBetween(const Vector3& a, const Vector3& b);
	};
	Vector3 operator*(float a, const Vector3& b);
	Vector3 operator/(float a, const Vector3& b);
}