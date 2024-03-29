#pragma once

#include "Engine/Application/Node/Node.h"
#include "Engine/System/Math/Vector.h"
#include "Engine/System/Math/TransformMatrix.h"

namespace Engine {
	class Node2D :public Node {
		REFLECTION_CLASS(::Engine::Node2D, ::Engine::Node) {}

	public:
		Vector2 GetPosition() const;
		void SetPosition(const Vector2& position);
		
		Vector2 GetScale() const;
		void SetScale(const Vector2& scale);

		float GetRotation() const;
		void SetRotation(float rotation);


		
		void UpdateLocalTransform() const;
		void UpdateGlobalTransform() const;

	private:
		Vector2 position = Vector2(0, 0);
		Vector2 scale = Vector2(1, 1);
		float rotation = 0;

		mutable TransformMatrix localTransform;
		mutable TransformMatrix globalTransform;
		mutable bool localTransformDirty = true;
		mutable bool globalTransformDirty = true;
	};
}