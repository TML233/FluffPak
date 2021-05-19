#pragma once
#include "Engine/System/Definition.h"

namespace Engine {
	struct Rect2 final{
		Rect2(float left = 0, float top = 0, float right = 0, float bottom = 0);

		float left;
		float top;
		float right;
		float bottom;

		bool operator==(const Rect2& value) const;
		bool operator!=(const Rect2& value) const;
	};
}
