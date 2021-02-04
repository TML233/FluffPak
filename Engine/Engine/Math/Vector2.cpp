#include "Engine/Math/Vector2.h"
#include "Engine/Math/Math.h"
#include "Engine/System/String.h"

namespace Engine {
	Vector2::Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	float Vector2::GetLength() const {
		return Math::Sqrt(GetLengthSquared());
	}
	float Vector2::GetLengthSquared() const {
		return x * x + y * y;
	}

	Vector2 Vector2::GetNormalized() const {
		float len = GetLength();
		return Vector2(x / len, y / len);
	}
	void Vector2::Normalize() {
		float len = GetLength();
		x = x / len;
		y = y / len;
	}

	Vector2 Vector2::operator+(const Vector2& value) {
		return Vector2(x + value.x, y + value.y);
	}
	Vector2 Vector2::operator-(const Vector2& value) {
		return Vector2(x - value.x, y - value.y);
	}
	Vector2 Vector2::operator*(float value) {
		return Vector2(x * value, y * value);
	}
	Vector2 Vector2::operator/(float value) {
		return Vector2(x / value, y / value);
	}

	String Vector2::ToString() const {
		return "(" + Object::ToString(x) + ", " + Object::ToString(y) + ")";
	}

	const Vector2 Vector2::up(0, -1);
	const Vector2 Vector2::down(0, 1);
	const Vector2 Vector2::left(-1, 0);
	const Vector2 Vector2::right(1, 0);
	const Vector2 Vector2::one(1, 1);
	const Vector2 Vector2::zero(0, 0);

	Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float time) {
		return Vector2(Math::Lerp(a.x, b.x, time), Math::Lerp(a.y, b.y, time));
	}
	float Vector2::Dot(const Vector2& a, const Vector2& b) {
		return (a.x * b.x + a.y * b.y);
	}
	float Vector2::Cross(const Vector2& a, const Vector2& b) {
		return (a.x * b.y - a.y * b.x);
	}
}