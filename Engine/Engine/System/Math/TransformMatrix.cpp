#include "Engine/System/Math/TransformMatrix.h"
#include "Engine/System/Math/Quaternion.h"

namespace Engine {
	TransformMatrix TransformMatrix::operator*(const TransformMatrix& child) const {
		TransformMatrix result = *this;

		for (int32 i = 0; i < 4; i += 1) {
			for (int32 j = 0; j < 4; j += 1) {
				float value = 0;
				for (int32 k = 0; k < 4; k += 1) {
					value += child.matrix[i][k] * matrix[k][j];
				}
				result.matrix[i][j] = value;
			}
		}
		return result;
	}
	Vector3 TransformMatrix::operator*(const Vector3& child) const {
		return Vector3(
			matrix[0][0] * child.x + matrix[0][1] * child.y + matrix[0][2] * child.z,
			matrix[1][0] * child.x + matrix[1][1] * child.y + matrix[1][2] * child.z,
			matrix[2][0] * child.x + matrix[2][1] * child.y + matrix[2][2] * child.z
		);
	}

	const float* TransformMatrix::GetRaw() const {
		return (float*)matrix;
	}

	TransformMatrix TransformMatrix::Translate(const Vector3& value) {
		TransformMatrix m;
		m.matrix[3][0] = value.x;
		m.matrix[3][1] = value.y;
		m.matrix[3][2] = value.z;
		return m;
	}
	TransformMatrix TransformMatrix::Scale(const Vector3& value) {
		TransformMatrix m;
		m.matrix[0][0] = value.x;
		m.matrix[1][1] = value.y;
		m.matrix[2][2] = value.z;
		return m;
	}
	TransformMatrix TransformMatrix::Rotate(const Vector3& axis, float angle) {
		return Quaternion::FromAxisAngle(axis, angle).ToTransformMatrix();
	}
	TransformMatrix TransformMatrix::Ortho(float left, float right, float bottom, float top, float near, float far) {
		TransformMatrix m;
		m.matrix[0][0] = 2 / (right - left);
		m.matrix[1][1] = 2 / (top - bottom);
		m.matrix[2][2] = -2 / (far - near);
		m.matrix[3][0] = -(right + left) / (right - left);
		m.matrix[3][1] = -(top + bottom) / (top - bottom);
		m.matrix[3][2] = -(far + near) / (far - near);

		return m;
	}
	TransformMatrix TransformMatrix::Perspective(float fov, float aspect, float near, float far) {
		const float tanfov = MathF::Tan(fov / 2);
		TransformMatrix m;
		m.matrix[0][0] = 1 / (aspect * tanfov);
		m.matrix[1][1] = 1 / tanfov;
		m.matrix[2][2] = -(far + near) / (far - near);
		m.matrix[2][3] = -1;
		m.matrix[3][2] = -(2 * far * near) / (far - near);
		m.matrix[3][3] = 0;

		return m;
	}
	TransformMatrix TransformMatrix::LookAt(const Vector3& position, const Vector3& target) {
		static const Vector3 up = Vector3(0, 1, 0);

		const Vector3 f = (target - position).GetNormalized();
		const Vector3 s = Vector3::Cross(f, up).GetNormalized();
		const Vector3 u = Vector3::Cross(s, f);

		TransformMatrix result;
		result.matrix[0][0] = s.x;
		result.matrix[1][0] = s.y;
		result.matrix[2][0] = s.z;
		result.matrix[0][1] = u.x;
		result.matrix[1][1] = u.y;
		result.matrix[2][1] = u.z;
		result.matrix[0][2] = -f.x;
		result.matrix[1][2] = -f.y;
		result.matrix[2][2] = -f.z;
		result.matrix[3][0] = -Vector3::Dot(s, position);
		result.matrix[3][1] = -Vector3::Dot(u, position);
		result.matrix[3][2] = Vector3::Dot(f, position);
		return result;
	}
}
