#include "Engine/Application/Node/Node2D.h"

namespace Engine {
	Vector2 Node2D::GetPosition() const {
		return position;
	}
	void Node2D::SetPosition(const Vector2& position) {
		this->position = position;
		localTransformDirty = true;
	}
	Vector2 Node2D::GetScale() const {
		return scale;
	}
	void Node2D::SetScale(const Vector2& scale) {
		this->scale = scale;
		localTransformDirty = true;
	}
	float Node2D::GetRotation() const {
		return rotation;
	}
	void Node2D::SetRotation(float rotation) {
		this->rotation = rotation;
		localTransformDirty = true;
	}

	void Node2D::UpdateLocalTransform() const {
		TransformMatrix t;
		t = TransformMatrix::Rotate(Vector3(0, 0, 1), rotation) * t;
		t = TransformMatrix::Scale(Vector3(scale,1)) * t;
		t = TransformMatrix::Translate(position) * t;
		localTransform = t;

		localTransformDirty = false;
	}
}
