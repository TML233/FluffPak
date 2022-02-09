#pragma once

#include "Engine/System/Definition.h"
#include <cmath>

namespace Engine {
	class Mathf final {
	public:
		STATIC_CLASS(Mathf);

		static inline constexpr float Pi = 3.14159274f;
		static inline constexpr float Deg2Rad = Pi / 180;
		static inline constexpr float Rad2Deg = 180 / Pi;
		static inline constexpr float Infinity = INFINITY;
		static inline constexpr float NaN = NAN;

		static inline bool IsNaN(float value) {
			return std::isnan(value);
		}
		static inline bool IsInfinity(float value) {
			return std::isinf(value);
		}

		static inline float Clamp(float value, float min = 0.0f, float max = 1.0f) {
			if (value < min) {
				return min;
			}
			if (value > max) {
				return max;
			}
			return value;
		}
		static inline float Lerp(float a, float b, float time) {
			return (b - a) * time + a;
		}

		static inline float Tan(float value) {
			return std::tanf(value);
		}
		static inline float Sin(float value) {
			return std::sinf(value);
		}
		static inline float Cos(float value) {
			return std::cosf(value);
		}
		
		static inline float Atan(float value) {
			return std::atanf(value);
		}
		static inline float Atan2(float y, float x) {
			return std::atan2f(y, x);
		}
		static inline float Asin(float value) {
			return std::asinf(value);
		}
		static inline float Acos(float value) {
			return std::acosf(value);
		}

		static inline float Sign(float value) {
			if (value < 0) {
				return -1;
			}
			if (value > 0) {
				return 1;
			}
			return 0;
		}
		static inline float Abs(float value) {
			return (value < 0 ? -value : value);
		}
 
		static inline float Round(float value) {
			return std::roundf(value);
		}
		static inline float Floor(float value) {
			return std::floorf(value);
		}
		static inline float Ceil(float value) {
			return std::ceilf(value);
		}

		static inline float Pow(float baseValue, float power) {
			return std::powf(baseValue, power);
		}
		static inline float Sqrt(float value) {
			return std::sqrtf(value);
		}
	};
}