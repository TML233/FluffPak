#include <cmath>
#include "Engine/Math/Math.h"

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

	float Math::Sin(float time) {
		return sinf(time);
	}
	float Math::Cos(float time) {
		return cosf(time);
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