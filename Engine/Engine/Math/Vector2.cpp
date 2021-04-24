#include "Engine/Math/Vector2.h"
#include "Engine/Math/Math.h"
#include "Engine/System/String.h"

namespace Engine {
	Vector2::Vector2(float x, float y) :x(x), y(y) {}

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

	Vector2 Vector2::operator+(const Vector2& value) const {
		return Vector2(x + value.x, y + value.y);
	}
	Vector2& Vector2::operator+=(const Vector2& value) {
		x += value.x;
		y += value.y;
		return *this;
	}
	Vector2 Vector2::operator-(const Vector2& value) const {
		return Vector2(x - value.x, y - value.y);
	}
	Vector2& Vector2::operator-=(const Vector2& value) {
		x -= value.x;
		y -= value.y;
		return *this;
	}
	Vector2 Vector2::operator*(float value) const {
		return Vector2(x * value, y * value);
	}
	Vector2 operator*(float a, const Vector2& b) {
		return Vector2(a * b.x, a * b.y);
	}
	Vector2& Vector2::operator*=(float value) {
		x *= value;
		y *= value;
		return *this;
	}
	Vector2 Vector2::operator/(float value) const {
		return Vector2(x / value, y / value);
	}
	Vector2 operator/(float a, const Vector2& b) {
		return Vector2(a / b.x, a / b.y);
	}
	Vector2& Vector2::operator/=(float value) {
		x /= value;
		y /= value;
		return *this;
	}
	bool Vector2::operator==(const Vector2& value) const {
		return (x == value.x && y == value.y);
	}
	bool Vector2::operator!=(const Vector2& value) const {
		return (x != value.x || y != value.y);
	}
	Vector2 Vector2::operator+() const {
		return *this;
	}
	Vector2 Vector2::operator-() const {
		return *this * -1;
	}

	String Vector2::ToString() const {
		return String::Format("({0}, {1})", x, y);
	}
	int32 Vector2::GetHashCode() const {
		return (*((int32*)(&x))) ^ (*((int32*)(&y)));
	}

	const Vector2 Vector2::Up(0, -1);
	const Vector2 Vector2::Down(0, 1);
	const Vector2 Vector2::Left(-1, 0);
	const Vector2 Vector2::Right(1, 0);
	const Vector2 Vector2::One(1, 1);
	const Vector2 Vector2::Zero(0, 0);

	float Vector2::Dot(const Vector2& a, const Vector2& b) {
		return (a.x * a.x + b.y * b.y);
	}
	float Vector2::Cross(const Vector2& a, const Vector2& b) {
		return (a.x * b.y - a.y * b.x);
	}
	Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float time) {
		return Vector2(Math::Lerp(a.x, b.x, time), Math::Lerp(a.y, b.y, time));
	}
	float Vector2::AngleBetween(const Vector2& a, const Vector2& b) {
		return Math::ArcCos(Vector2::Dot(a.GetNormalized(), b.GetNormalized()));
	}
}