#include "Engine/Math/Vector3.h"
#include "Engine/Math/Math.h"
#include "Engine/System/String.h"

namespace Engine {
	Vector3::Vector3(Float x, Float y, Float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Float Vector3::GetLength() const {
		return Math::Sqrt(GetLengthSquared());
	}
	Float Vector3::GetLengthSquared() const {
		return x * x + y * y + z * z;
	}

	Vector3 Vector3::GetNormalized() const {
		Float len = GetLength();
		return Vector3(x / len, y / len, z / len);
	}
	void Vector3::Normalize() {
		Float len = GetLength();
		x = x / len;
		y = y / len;
		z = z / len;
	}

	Vector3 Vector3::operator+(const Vector3& value) {
		return Vector3(x + value.x, y + value.y, z + value.z);
	}
	Vector3 Vector3::operator-(const Vector3& value) {
		return Vector3(x - value.x, y - value.y, z - value.z);
	}
	Vector3 Vector3::operator*(Float value) {
		return Vector3(x * value, y * value, z * value);
	}
	Vector3 Vector3::operator/(Float value) {
		return Vector3(x / value, y / value, z / value);
	}

	String Vector3::ToString() const {
		return "(" + Object::ToString(x) + ", " + Object::ToString(y) + ", " + Object::ToString(z) + ")";
	}

	const Vector3 Vector3::up(0, -1, 0);
	const Vector3 Vector3::down(0, 1, 0);
	const Vector3 Vector3::left(-1, 0, 0);
	const Vector3 Vector3::right(1, 0, 0);
	const Vector3 Vector3::forward(0, 0, -1);
	const Vector3 Vector3::backward(0, 0, 1);
	const Vector3 Vector3::one(1, 1, 1);
	const Vector3 Vector3::zero(0, 0, 0);

	Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, Float time) {
		return Vector3(Math::Lerp(a.x, b.x, time), Math::Lerp(a.y, b.y, time), Math::Lerp(a.z, b.z, time));
	}
	Float Vector3::Dot(const Vector3& a, const Vector3& b) {
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}
	Vector3 Vector3::Cross(const Vector3& a, const Vector3& b) {
		return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}
}