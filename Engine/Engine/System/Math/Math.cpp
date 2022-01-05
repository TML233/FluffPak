#include "Engine/System/Math/Math.h"

namespace Engine {
	const float Math::Infinity = INFINITY;

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
}