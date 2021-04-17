#include "Engine/Math/Transform2.h"

namespace Engine {
	Transform2::Transform2(Vector2 o, Vector2 x, Vector2 y) :o(o), x(x), y(y) {}

	Transform2 Transform2::operator*(float scale) const {
		return Transform2(o, x * scale, y * scale);
	}
	Transform2& Transform2::operator*=(float scale) {
		x *= scale;
		y *= scale;
		return *this;
	}
	Transform2 Transform2::operator+(const Vector2& translate) const {
		return Transform2(o + translate, x, y);
	}
	Transform2& Transform2::operator+=(const Vector2& translate) {
		o += translate;
		return *this;
	}
	Transform2 Transform2::operator+(const Vector2& translate) const {
		return Transform2(o - translate, x, y);
	}
	Transform2& Transform2::operator-=(const Vector2& translate) {
		o -= translate;
		return *this;
	}
	void Transform2::Rotate(float radian) {
		x.x *= Math::Cos(radian);
		x.y *= Math::Sin(radian);
		y.x *= -Math::Sin(radian);
		y.y *= Math::Cos(radian);
	}
	Transform2 Transform2::GetRotated(float radian) const {
		return Transform2(
			o,
			Vector2(x.x * Math::Cos(radian), x.y * Math::Sin(radian)),
			Vector2(y.x * -Math::Sin(radian), y.y * Math::Cos(radian))
		);
	}

	Transform2 Transform2::operator*(const Transform2& child) const {
		return Transform2(
			x * child.o.x + y * child.o.y + o,
			x * child.x.x + y * child.x.y,
			x * child.y.x + y * child.y.y
		);
	}

	Transform2& Transform2::operator*=(const Transform2& child) {
		o = x * child.o.x + y * child.o.y + o;
		x = x * child.x.x + y * child.x.y;
		y = x * child.y.x + y * child.y.y;
		return *this;
	}
}
