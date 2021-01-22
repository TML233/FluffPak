#ifndef HEADER_CORE_MATH_MATH
#define HEADER_CORE_MATH_MATH

#include "Core/Definition.h"

namespace Core {
	class Math final {
	public:
		static const Float PI;
		static const Float Deg2Rad;
		static const Float Rad2Deg;

		static Float Clamp(Float value, Float min = 0.0f, Float max = 1.0f);
		static Float Lerp(Float a, Float b, Float time);

		static Float Sin(Float time);
		static Float Cos(Float time);

		static Float Sign(Float value);
		static Float Abs(Float value);

		static Float Round(Float value);
		static Float Floor(Float value);
		static Float Ceil(Float value);

		static Float Pow(Float base, Float power);
		static Float Sqrt(Float value);
	};
}

#endif