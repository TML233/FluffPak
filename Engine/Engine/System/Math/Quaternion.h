#pragma once
#include "Engine/System/Math/Vector.h"
#include "Engine/System/Math/Mathf.h"
#include "Engine/System/Math/TransformMatrix.h"

namespace Engine {
	struct Quaternion final {
		Quaternion(float x = 0, float y = 0, float z = 0, float w = 1);

		float x = 0;
		float y = 0;
		float z = 0;
		float w = 1;

		float GetSquaredMagnitude() const;
		float GetMagnitude() const;
		static inline constexpr float NormalizeTolerance = 0.00001f;
		void Normalize();
		Quaternion GetNormalized() const;
		void Conjugate();
		Quaternion GetConjugated() const;
		Quaternion& operator*=(const Quaternion& value);
		Quaternion operator*(const Quaternion& value) const;

		Vector3 operator*(const Vector3& value) const;

		static Quaternion FromAxisAngle(const Vector3& axis, float angle);
		static Quaternion FromEuler(const Vector3& angle);
		TransformMatrix ToTransformMatrix() const;
	};
}
