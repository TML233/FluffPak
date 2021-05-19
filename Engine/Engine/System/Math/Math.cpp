#include <cmath>
#include "Engine/System/Math/Math.h"

namespace Engine {
	const float Math::PI = 3.14159265358f;
	const float Math::Deg2Rad = Math::PI / 180;
	const float Math::Rad2Deg = 180 / Math::PI;

	float Math::Clamp(float value, float min, float max) {
		if (value < min) {
			return min;
		}
		if (value > max) {
			return max;
		}
		return value;
	}
	float Math::Lerp(float a, float b, float time) {
		return (b - a) * time + a;
	}

	float Math::Tan(float value) {
		return tanf(value);
	}
	float Math::Cot(float value) {
		return 1 / tanf(value);
	}
	float Math::Sin(float value) {
		return sinf(value);
	}
	float Math::Cos(float value) {
		return cosf(value);
	}

	float Math::ArcTan(float value) {
		return atanf(value);
	}
	float Math::ArcCot(float value) {
		return 1 / atanf(value);
	}
	float Math::ArcSin(float value) {
		return asinf(value);
	}
	float Math::ArcCos(float value) {
		return acosf(value);
	}

	float Math::Sign(float value) {
		if (value < 0) {
			return -1;
		}
		if (value > 0) {
			return 1;
		}
		return 0;
	}
	float Math::Abs(float value) {
		return (value < 0 ? -value : value);
	}

	float Math::Round(float value) {
		return roundf(value);
	}
	float Math::Floor(float value) {
		return floorf(value);
	}
	float Math::Ceil(float value) {
		return ceilf(value);
	}

	float Math::Pow(float base, float power) {
		return powf(base, power);
	}
	float Math::Sqrt(float value) {
		return sqrtf(value);
	}
}