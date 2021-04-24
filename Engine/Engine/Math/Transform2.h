#pragma once
#include "Engine/System/Definition.h"
#include "Engine/Math/Math.h"
#include "Engine/Math/Vector2.h"

namespace Engine {
	struct Transform2 final {
		Transform2(const Vector2& o = Vector2(0, 0), const Vector2& x = Vector2(1, 0), const Vector2& y = Vector2(0, 1));

		Vector2 o;
		Vector2 x;
		Vector2 y;

#pragma region Affine
		/// @brief Translate the origin.
		void Translate(const Vector2& translate);
		/// @brief Returns a new Transform2 with translated origin.
		Transform2 GetTranslated(const Vector2& translate) const;
		/// @brief Scale the basis.
		void Scale(const Vector2& scale);
		/// @brief Returns a new Transform2 with scaled basis.
		Transform2 GetScaled(const Vector2& scale) const;
		/// @brief Rotate the basis.
		void Rotate(float radian);
		/// @brief Returns a new Transform2 with rotated basis.
		Transform2 GetRotated(float radian) const;
#pragma endregion

		/// @brief Combine two Transform2
		Transform2 operator*(const Transform2& child) const;
		/// @brief Combine two Transform2
		Transform2& operator*=(const Transform2& child);

		float GetBasisDeterminant() const;
		Vector2 TransformBasis(const Vector2& vec) const;
		Vector2 InverseTransformBasis(const Vector2& vec) const;
		Vector2 Transform(const Vector2& vec) const;
		Vector2 InverseTransform(const Vector2& vec) const;

		void Invert();
		Transform2 GetInverted() const;
	};
}
