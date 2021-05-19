#include "Engine/System/Math/Transform2.h"

namespace Engine {
	Transform2::Transform2(const Vector2& o, const Vector2& x, const Vector2& y) :o(o), x(x), y(y) {}

	void Transform2::Translate(const Vector2& translate) {
		o += translate;
	}
	Transform2 Transform2::GetTranslated(const Vector2& translate) const {
		Transform2 r = *this;
		r.Translate(translate);
		return r;
	}
	void Transform2::Scale(const Vector2& scale) {
		x *= scale.x;
		y *= scale.y;
	}
	Transform2 Transform2::GetScaled(const Vector2& scale) const {
		Transform2 r = *this;
		r.Scale(scale);
		return r;
	}
	void Transform2::Rotate(float radian) {
		x.x *= Math::Cos(radian);
		x.y *= Math::Sin(radian);
		y.x *= -Math::Sin(radian);
		y.y *= Math::Cos(radian);
	}
	Transform2 Transform2::GetRotated(float radian) const {
		Transform2 r = *this;
		r.Rotate(radian);
		return r;
	}

	Transform2& Transform2::operator*=(const Transform2& child) {
		o = x * child.o.x + y * child.o.y + o;
		x = x * child.x.x + y * child.x.y;
		y = x * child.y.x + y * child.y.y;
		return *this;
	}
	Transform2 Transform2::operator*(const Transform2& child) const {
		Transform2 r = *this;
		r *= child;
		return r;
	}

	float Transform2::GetBasisDeterminant() const {
		return x.x * y.y - y.x * x.y;
	}
	Vector2 Transform2::TransformBasis(const Vector2& vec) const {
		return Vector2(
			x.x * vec.x + y.x * vec.y,
			x.y * vec.x + y.y * vec.y
		);
	}
	Vector2 Transform2::InverseTransformBasis(const Vector2& vec) const {
		return Vector2(Vector2::Dot(x, vec), Vector2::Dot(y, vec));
	}
	Vector2 Transform2::Transform(const Vector2& vec) const {
		return TransformBasis(vec) + o;
	}
	Vector2 Transform2::InverseTransform(const Vector2& vec) const {
		Vector2 v = vec - o;
		return Vector2(Vector2::Dot(x, v), Vector2::Dot(y, v));
	}
}
