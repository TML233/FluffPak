#ifndef HEADER_CORE_MATH_MATH
#define HEADER_CORE_MATH_MATH

#include "Engine/System/Definition.h"

namespace Engine {
	class Math final {
	public:
		static const float PI;
		static const float Deg2Rad;
		static const float Rad2Deg;

		static float Clamp(float value, float min = 0.0f, float max = 1.0f);
		static float Lerp(float a, float b, float time);

		static float Sin(float time);
		static float Cos(float time);

		static float Sign(float value);
		static float Abs(float value);

		static float Round(float value);
		static float Floor(float value);
		static float Ceil(float value);

		static float Pow(float base, float power);
		static float Sqrt(float value);
	};
}

#endif