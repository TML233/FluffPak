#pragma once

#include "Engine/Application/Node/Node.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Transform2.h"

namespace Engine {
	class Node2D :public Node {
		REFLECTION_CLASS(Node2D, Node) {}

	public:
		Vector2 GetPosition() const;
		void SetPosition(const Vector2& position);
		
		Vector2 GetScale() const;
		void SetScale(const Vector2& scale);

		float GetRotation() const;
		void SetRotation(float rotation);

		Vector2 GetGlobalPosition() const;
		Vector2 GetGlobalScale() const;
		float GetGlobalRotation() const;
		
		void UpdateLocalTransform() const;
		void UpdateGlobalTransform() const;

	private:
		Vector2 position = Vector2(0, 0);
		Vector2 scale = Vector2(1, 1);
		float rotation = 0;

		mutable Transform2 localTransform;
		mutable Transform2 globalTransform;
		mutable bool localTransformDirty = true;
		mutable bool globalTransformDirty = true;
	};
}