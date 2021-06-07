#include "Engine/System/Math/Vector.h"
#include "Engine/System/Math/Math.h"
#include "Engine/System/String.h"
#include "Engine/System/Object/ObjectUtil.h"

namespace Engine {
#pragma region Vector2
	Vector2::Vector2(float x, float y) :x(x), y(y) {}

	float Vector2::GetLength() const {
		return Math::Sqrt(GetLengthSquared());
	}
	float Vector2::GetLengthSquared() const {
		return x * x + y * y;
	}

	Vector2 Vector2::GetNormalized() const {
		Vector2 vec = *this;
		vec.Normalize();
		return vec;
	}
	void Vector2::Normalize() {
		float len = GetLength();
		x /= len;
		y /= len;
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
		return String::Format(u8"({0}, {1})", x, y);
	}
	int32 Vector2::GetHashCode() const {
		return ObjectUtil::GetHashCode(x) ^ ObjectUtil::GetHashCode(x);
	}

	const Vector2 Vector2::Up(0, -1);
	const Vector2 Vector2::Down(0, 1);
	const Vector2 Vector2::Left(-1, 0);
	const Vector2 Vector2::Right(1, 0);
	const Vector2 Vector2::One(1, 1);
	const Vector2 Vector2::Zero(0, 0);

	float Vector2::Dot(const Vector2& a, const Vector2& b) {
		return (a.x * b.x + a.y * b.y);
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
#pragma endregion

#pragma region Vector3
	Vector3::Vector3(float x, float y, float z) :x(x), y(y), z(z) {}
	Vector3::Vector3(const Vector2& vec2, float z) : x(vec2.x), y(vec2.y), z(z) {}

	float Vector3::GetLengthSquared() const {
		return x * x + y * y + z * z;
	}
	float Vector3::GetLength() const {
		return Math::Sqrt(GetLengthSquared());
	}

	void Vector3::Normalize() {
		float len = GetLength();
		x /= len;
		y /= len;
		z /= len;
	}
	Vector3 Vector3::GetNormalized() const {
		Vector3 vec = *this;
		vec.Normalize();
		return vec;
	}

	Vector3 Vector3::operator+(const Vector3& value) const {
		return Vector3(x + value.x, y + value.y, z + value.z);
	}
	Vector3& Vector3::operator+=(const Vector3& value) {
		x += value.x;
		y += value.y;
		z += value.z;
		return *this;
	}
	Vector3 Vector3::operator-(const Vector3& value) const {
		return Vector3(x - value.x, y - value.y, z - value.z);
	}
	Vector3& Vector3::operator-=(const Vector3& value) {
		x -= value.x;
		y -= value.y;
		z -= value.z;
		return *this;
	}
	Vector3 Vector3::operator*(float value) const {
		return Vector3(x * value, y * value, z * value);
	}
	Vector3 operator*(float a, const Vector3& b) {
		return Vector3(a * b.x, a * b.y, a * b.z);
	}
	Vector3& Vector3::operator*=(float value) {
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}
	Vector3 Vector3::operator/(float value) const {
		return Vector3(x / value, y / value, z / value);
	}
	Vector3 operator/(float a, const Vector3& b) {
		return Vector3(a / b.x, a / b.y, a / b.z);
	}
	Vector3& Vector3::operator/=(float value) {
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}
	bool Vector3::operator==(const Vector3& value) const {
		return (x == value.x && y == value.y && z == value.z);
	}
	bool Vector3::operator!=(const Vector3& value) const {
		return (x != value.x || y != value.y || z != value.z);
	}
	Vector3 Vector3::operator+() const {
		return *this;
	}
	Vector3 Vector3::operator-() const {
		return *this * -1;
	}

	String Vector3::ToString() const {
		return String::Format(u8"({0}, {1}, {2})", x, y, z);
	}
	int32 Vector3::GetHashCode() const {
		return ObjectUtil::GetHashCode(x) ^ ObjectUtil::GetHashCode(x) ^ ObjectUtil::GetHashCode(z);
	}

	const Vector3 Vector3::Up(0, 1, 0);
	const Vector3 Vector3::Down(0, -1, 0);
	const Vector3 Vector3::Left(-1, 0, 0);
	const Vector3 Vector3::Right(1, 0, 0);
	const Vector3 Vector3::Forward(0, 0, 1);
	const Vector3 Vector3::Back(0, 0, -1);
	const Vector3 Vector3::One(1, 1, 1);
	const Vector3 Vector3::Zero(0, 0, 0);

	float Vector3::Dot(const Vector3& a, const Vector3& b) {
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}
	Vector3 Vector3::Cross(const Vector3& a, const Vector3& b) {
		return Vector3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}
	Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float time) {
		return Vector3(Math::Lerp(a.x, b.x, time), Math::Lerp(a.y, b.y, time), Math::Lerp(a.z, b.z, time));
	}
	float Vector3::AngleBetween(const Vector3& a, const Vector3& b) {
		return Math::ArcCos(Vector3::Dot(a.GetNormalized(), b.GetNormalized()));
	}
#pragma endregion
}