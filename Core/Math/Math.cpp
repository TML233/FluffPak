#include <cmath>
#include "Core/Math/Math.h"

namespace Core {
	const Float Math::PI = 3.14159274;
	const Float Math::Deg2Rad = Math::PI / 180;
	const Float Math::Rad2Deg = 180 / Math::PI;

	Float Math::Clamp(Float value, Float min, Float max) {
		if (value < min) {
			return min;
		}
		if (value > max) {
			return max;
		}
		return value;
	}
	Float Math::Lerp(Float a, Float b, Float time) {
		return (b - a) * time + a;
	}

	Float Math::Sin(Float time) {
		return sinf(time);
	}
	Float Math::Cos(Float time) {
		return cosf(time);
	}

	Float Math::Sign(Float value) {
		if (value < 0) {
			return -1;
		}
		if (value > 0) {
			return 1;
		}
		return 0;
	}
	Float Math::Abs(Float value) {
		return (value < 0 ? -value : value);
	}

	Float Math::Round(Float value) {
		return roundf(value);
	}
	Float Math::Floor(Float value) {
		return floorf(value);
	}
	Float Math::Ceil(Float value) {
		return ceilf(value);
	}

	Float Math::Pow(Float base, Float power) {
		return powf(base, power);
	}
	Float Math::Sqrt(Float value) {
		return sqrtf(value);
	}
}