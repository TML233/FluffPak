#include "Engine/Application/Node/Rect2D.h"

namespace Engine {
	Vector2 Rect2D::GetPivot() const {
		return pivot;
	}
	void Rect2D::SetPivot(Vector2 pivot) {
		this->pivot = pivot;
	}
}
