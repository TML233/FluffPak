#include "Engine/System/Math/Rect2.h"

namespace Engine {
	Rect2::Rect2(float left, float top, float right, float bottom) :left(left), top(top), right(right), bottom(bottom) {}

	bool Rect2::operator==(const Rect2& value) const {
		return (left == value.left && top == value.top && right == value.right && bottom == value.bottom);
	}
	bool Rect2::operator!=(const Rect2& value) const {
		return (left != value.left || top != value.top || right != value.right || bottom != value.bottom);
	}
}
