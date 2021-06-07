#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/Math/Math.h"
#include "Engine/System/Math/Vector.h"

namespace Engine {
	struct TransformMatrix final {
		float matrix[4][4] = {
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,0,0,1},
		};
		TransformMatrix operator*(const TransformMatrix& child) const;
		Vector3 operator*(const Vector3& child) const;
		const float* GetRaw() const;

		static TransformMatrix Translate(const Vector3& value);
		static TransformMatrix Scale(const Vector3& value);
		static TransformMatrix Rotate(const Vector3& axis, float angle);
		static TransformMatrix Ortho(float left, float right, float bottom, float top, float near, float far);
		static TransformMatrix Perspective(float fov, float aspect, float near, float far);
	};
}
