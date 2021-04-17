#pragma once
#include "Engine/System/Definition.h"
#include "Engine/Math/Math.h"
#include "Engine/Math/Vector2.h"

namespace Engine {
	struct Transform2 final {
		Transform2(Vector2 o = Vector2(0,0),Vector2 x = Vector2(1,0), Vector2 y = Vector2(0,1)) {}

		Vector2 o;
		Vector2 x;
		Vector2 y;

		Transform2 operator*(float scale) const;
		Transform2& operator*=(float scale);
		Transform2 operator+(const Vector2& translate) const;
		Transform2& operator+=(const Vector2& translate);
		Transform2 operator+(const Vector2& translate) const;
		Transform2& operator-=(const Vector2& translate);
		void Rotate(float radian);
		Transform2 GetRotated(float radian) const;

		Transform2 operator*(const Transform2& child) const;
		Transform2& operator*=(const Transform2& child);
	};
}
