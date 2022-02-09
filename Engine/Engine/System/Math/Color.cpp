#include "Engine/System/Math/Color.h"

namespace Engine {
	Color::Color(float r, float g, float b, float a) :r(r), g(g), b(b), a(a) {}

	Color Color::From8(byte r, byte g, byte b, byte a) {
		return Color(
			r / 255.0f,
			g / 255.0f,
			b / 255.0f,
			a / 255.0f
		);
	}
	Color Color::Lerp(const Color& from, const Color& to, float time) {
		return Color(
			Mathf::Lerp(from.r, to.r, time),
			Mathf::Lerp(from.g, to.g, time),
			Mathf::Lerp(from.b, to.b, time),
			Mathf::Lerp(from.a, to.a, time)
		);
	}

	const Color Color::Clear(0, 0, 0, 0);
	const Color Color::Black(0, 0, 0, 1);
	const Color Color::White(1, 1, 1, 1);
	const Color Color::Red(1, 0, 0, 1);
	const Color Color::Green(0, 1, 0, 1);
	const Color Color::Blue(0, 0, 1, 1);
}