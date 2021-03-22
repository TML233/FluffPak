#pragma once

#include "Engine/Application/Node/Node2D.h"
#include "Engine/Math/Rect2.h"

namespace Engine {
	class Rect2D :public Node2D {
	public:
		Vector2 GetPivot() const;
		void SetPivot(Vector2 pivot);
		
		float GetZIndex() const;
		void SetZIndex(float zIndex);
		
		bool IsZIndexRelative() const;
		void SetZIndexRelative(bool relative);

		Rect2 GetRect() const;
	private:
		Vector2 size = Vector2(100, 100);
		Vector2 pivot = Vector2(0.5f, 0.5f);
		float zIndex = 0;
		bool zIndexRelative = true;
	};
}