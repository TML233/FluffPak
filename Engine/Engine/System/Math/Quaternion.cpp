#include "Engine/System/Math/Quaternion.h"

namespace Engine{
	Quaternion::Quaternion(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {}

	float Quaternion::GetSquaredMagnitude() const {
		return x * x + y * y + z * z + w * w;
	}
	float Quaternion::GetMagnitude() const {
		return Math::Sqrt(GetSquaredMagnitude());
	}
	void Quaternion::Normalize() {
		float mag2 = GetSquaredMagnitude();
		if (mag2 != 0 && Math::Abs(mag2 - 1) > NormalizeTolerance) {
			float mag = Math::Sqrt(mag2);
			w /= mag;
			x /= mag;
			y /= mag;
			z /= mag;
		}
	}
	Quaternion Quaternion::GetNormalized() const {
		Quaternion q = *this;
		q.Normalize();
		return q;
	}
	void Quaternion::Conjugate() {
		x *= -1;
		y *= -1;
		z *= -1;
	}
	Quaternion Quaternion::GetConjugated() const {
		Quaternion q = *this;
		q.Conjugate();
		return q;
	}
	Quaternion& Quaternion::operator*=(const Quaternion& value) {
		float xx = w * value.x + x * value.w + y * value.z - z * value.y;
		float yy = w * value.y + y * value.w + z * value.x - x * value.z;
		float zz = w * value.z + z * value.w + x * value.y - y * value.x;
		float ww = w * value.w - x * value.x - y * value.y - z * value.z;
		x = xx;
		y = yy;
		z = zz;
		w = ww;
		return *this;
	}
	Quaternion Quaternion::operator*(const Quaternion& value) const {
		Quaternion q = *this;
		q *= value;
		return q;
	}

	Vector3 Quaternion::operator*(const Vector3& value) const {
		Vector3 n = value.GetNormalized();

		Quaternion qv;
		qv.x = n.x;
		qv.y = n.y;
		qv.z = n.z;
		qv.w = 0;

		Quaternion qr = qv * GetConjugated();
		qr = *this * qr;

		return Vector3(qr.x, qr.y, qr.z);
	}

	Quaternion Quaternion::FromAxisAngle(const Vector3& axis, float angle) {
		angle *= 0.5f;

		Vector3 n = axis.GetNormalized();
		float sina = Math::Sin(angle);

		return Quaternion(n.x * sina, n.y * sina, n.z * sina, Math::Cos(angle));
	}
	Quaternion Quaternion::FromEuler(const Vector3& angle) {
		float p = angle.x / 2.0f;
		float y = angle.y / 2.0f;
		float r = angle.z / 2.0f;

		float sinp = Math::Sin(p);
		float siny = Math::Sin(y);
		float sinr = Math::Sin(r);
		float cosp = Math::Cos(p);
		float cosy = Math::Cos(y);
		float cosr = Math::Cos(r);

		Quaternion q(
			sinr * cosp * cosy - cosr * sinp * siny,
			cosr * sinp * cosy + sinr * cosp * siny,
			cosr * cosp * siny - sinr * sinp * cosy,
			cosr * cosp * cosy + sinr * sinp * siny
		);
		q.Normalize();

		return q;
	}
	TransformMatrix Quaternion::ToTransformMatrix() const {
		TransformMatrix m;

		float x2 = x * x;
		float y2 = y * y;
		float z2 = z * z;
		float xy = x * y;
		float xz = x * z;
		float yz = y * z;
		float wx = w * x;
		float wy = w * y;
		float wz = w * z;

		m.matrix[0][0] = 1 - 2 * (y2 + z2);
		m.matrix[0][1] = 2 * (xy - wz);
		m.matrix[0][2] = 2 * (xz + wy);
		m.matrix[0][3] = 0;

		m.matrix[1][0] = 2 * (xy + wz);
		m.matrix[1][1] = 1 - 2 * (x2 + z2);
		m.matrix[1][2] = 2 * (yz - wx);
		m.matrix[1][3] = 0;

		m.matrix[2][0] = 2 * (xz - wy);
		m.matrix[2][1] = 2 * (yz + wx);
		m.matrix[2][2] = 1 - 2 * (x2 + y2);
		m.matrix[2][3] = 0;

		m.matrix[3][0] = 0;
		m.matrix[3][1] = 0;
		m.matrix[3][2] = 0;
		m.matrix[3][3] = 1;

		return m;
	}
}
