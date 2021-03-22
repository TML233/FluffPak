#pragma once

#include "Engine/Application/Node/Node.h"
#include "Engine/Math/Vector2.h"

namespace Engine {
	class Node2D :public Node {
	public:
		float GetRotation() const;

		Vector2 GetPosition() const;
		void SetPosition(Vector2 position);
		
		void SetScale(Vector2 scale);
		void SetRotation(float rotation);
		
	private:
		Vector2 position = Vector2(0, 0);
		Vector2 scale = Vector2(1, 1);
		float rotation = 0;
	};
}