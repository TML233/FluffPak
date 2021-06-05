#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/Math/Math.h"
#include "Engine/System/Math/Vector.h"

namespace Engine {
	struct TransformMatrix final {
		float matrix[4][4] = {
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,0,0,1},
		};

		void Translate(const Vector3& translate) {
			matrix[0][3] += translate.x;
			matrix[1][3] += translate.y;
			matrix[2][3] += translate.z;
		}
		TransformMatrix GetTranslated(const Vector3& translate) const {
			TransformMatrix m = *this;
			m.Translate(translate);
			return m;
		}
		void Scale(const Vector3& scale) {
			matrix[0][0] *= scale.x;
			matrix[0][1] *= scale.x;
			matrix[0][2] *= scale.x;

			matrix[1][0] *= scale.y;
			matrix[1][1] *= scale.y;
			matrix[1][2] *= scale.y;

			matrix[2][0] *= scale.z;
			matrix[2][1] *= scale.z;
			matrix[2][2] *= scale.z;
		}
		TransformMatrix GetScaled(const Vector3& scale) const {
			TransformMatrix m = *this;
			m.Scale(scale);
			return m;
		}
		void Rotate(Vector3 angle) {

		}
		TransformMatrix GetRotated(Vector3 angle) const {
			TransformMatrix m = *this;
			m.Rotate(angle);
			return m;
		}

		TransformMatrix& operator*=(const TransformMatrix& child) {
			TransformMatrix m = *this * child;
			*this = m;
			return *this;
		}
		TransformMatrix operator*(const TransformMatrix& child) const {
			TransformMatrix result = *this;

			for (byte i = 0; i < 4; i += 1) {
				for (byte j = 0; j < 4; j += 1) {
					float value = 0;
					for (byte k = 0; k < 4; k += 1) {
						value += matrix[i][k] * child.matrix[k][j];
					}
					result.matrix[i][j] = value;
				}
			}
			return result;
		}
		Vector3 operator*(const Vector3& child) const {
			return Vector3(
				matrix[0][0] * child.x + matrix[0][1] * child.y + matrix[0][2] * child.z,
				matrix[1][0] * child.x + matrix[1][1] * child.y + matrix[1][2] * child.z,
				matrix[2][0] * child.x + matrix[2][1] * child.y + matrix[2][2] * child.z
			);
		}
	};
}
