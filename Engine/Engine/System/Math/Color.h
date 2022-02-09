#pragma once
#include "Engine/System/Math/Mathf.h"

namespace Engine {
	struct Color {
		Color(float r = 0, float g = 0, float b = 0, float a = 1);
		float r;
		float g;
		float b;
		float a;

		static Color From8(byte r = 0, byte g = 0, byte b = 0, byte a = 255);
		//static Color FromHSV(float h = 0, float s = 0, float v = 0, float a = 1);
		static Color Lerp(const Color& from, const Color& to, float time);

		static const Color Clear;
		static const Color Black;
		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
	};
}