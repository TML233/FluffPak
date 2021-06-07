#include <cmath>
#include "Engine/System/Math/Math.h"

namespace Engine {
	const float Math::Infinity = INFINITY;

	bool Math::IsNaN(float value) {
		return std::isnan(value);
	}
	bool Math::IsInfinity(float value) {
		return std::isinf(value);
	}

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
		return std::tanf(value);
	}
	float Math::Cot(float value) {
		return 1 / std::tanf(value);
	}
	float Math::Sin(float value) {
		return std::sinf(value);
	}
	float Math::Cos(float value) {
		return std::cosf(value);
	}

	float Math::ArcTan(float value) {
		return std::atanf(value);
	}
	float Math::ArcCot(float value) {
		return 1 / std::atanf(value);
	}
	float Math::ArcSin(float value) {
		return std::asinf(value);
	}
	float Math::ArcCos(float value) {
		return std::acosf(value);
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
		return std::roundf(value);
	}
	float Math::Floor(float value) {
		return std::floorf(value);
	}
	float Math::Ceil(float value) {
		return std::ceilf(value);
	}

	float Math::Pow(float base, float power) {
		return std::powf(base, power);
	}
	float Math::Sqrt(float value) {
		return std::sqrtf(value);
	}
}