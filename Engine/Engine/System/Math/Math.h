#pragma once

#include "Engine/System/Definition.h"
#include <cmath>

namespace Engine {
	class Math final {
	public:
		STATIC_CLASS(Math);

		static inline constexpr float PI = (float)3.14159265358f;
		static inline constexpr float Deg2Rad = PI / 180;
		static inline constexpr float Rad2Deg = 180 / PI;
		static const float Infinity;

		static inline bool IsNaN(float value) {
			return std::isnan(value);
		}
		static inline bool IsInfinity(float value) {
			return std::isinf(value);
		}

		static float Clamp(float value, float min = 0.0f, float max = 1.0f);
		static float Lerp(float a, float b, float time);

		static inline float Tan(float value) {
			return std::tanf(value);
		}
		static inline float Cot(float value) {
			return 1 / std::tanf(value);
		}
		static inline float Sin(float value) {
			return std::sinf(value);
		}
		static inline float Cos(float value) {
			return std::cosf(value);
		}
		
		static inline float ArcTan(float value) {
			return std::atanf(value);
		}
		static inline float ArcCot(float value) {
			return 1 / std::atanf(value);
		}
		static inline float ArcSin(float value) {
			return std::asinf(value);
		}
		static inline float ArcCos(float value) {
			return std::acosf(value);
		}

		static float Sign(float value);
		static float Abs(float value);
 
		static inline float Round(float value) {
			return std::roundf(value);
		}
		static inline float Floor(float value) {
			return std::floorf(value);
		}
		static inline float Ceil(float value) {
			return std::ceilf(value);
		}

		static inline float Pow(float base, float power) {
			return std::powf(base, power);
		}
		static inline float Sqrt(float value) {
			return std::sqrtf(value);
		}
	};
}