#pragma once

#include "Engine/System/Definition.h"

namespace Engine {
	class Math final {
	public:
		STATIC_CLASS(Math);

		static inline constexpr float PI = (float)3.14159265358f;
		static inline constexpr float Deg2Rad = PI / 180;
		static inline constexpr float Rad2Deg = 180 / PI;
		static const float Infinity;

		static bool IsNaN(float value);
		static bool IsInfinity(float value);

		static float Clamp(float value, float min = 0.0f, float max = 1.0f);
		static float Lerp(float a, float b, float time);

		static float Tan(float value);
		static float Cot(float value);
		static float Sin(float value);
		static float Cos(float value);
		
		static float ArcTan(float value);
		static float ArcCot(float value);
		static float ArcSin(float value);
		static float ArcCos(float value);

		static float Sign(float value);
		static float Abs(float value);

		static float Round(float value);
		static float Floor(float value);
		static float Ceil(float value);

		static float Pow(float base, float power);
		static float Sqrt(float value);
	};
}