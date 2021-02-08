#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Object.h"

namespace Engine {
	struct Vector3 final:public Object {
		Vector3(float x, float y, float z);

		float x;
		float y;
		float z;

		float GetLength() const;
		float GetLengthSquared() const;

		Vector3 GetNormalized() const;
		void Normalize();

		Vector3 operator+(const Vector3& value);
		Vector3 operator-(const Vector3& value);
		Vector3 operator*(float value);
		Vector3 operator/(float value);

		String ToString() const override;

		static const Vector3 up;
		static const Vector3 down;
		static const Vector3 left;
		static const Vector3 right;
		static const Vector3 forward;
		static const Vector3 backward;
		static const Vector3 one;
		static const Vector3 zero;

		static Vector3 Lerp(const Vector3& a, const Vector3& b, float time);
		static float Dot(const Vector3& a, const Vector3& b);
		static Vector3 Cross(const Vector3& a, const Vector3& b);
	};
}