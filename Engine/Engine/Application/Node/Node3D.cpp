#include "Engine/Application/Node/Node3D.h"

namespace Engine {
	Vector3 Node3D::GetPosition() const {
		return position;
	}
	void Node3D::SetPosition(const Vector3& position) {
		this->position = position;
		localTransformDirty = true;
	}
	Vector3 Node3D::GetScale() const {
		return scale;
	}
	void Node3D::SetScale(const Vector3& scale) {
		this->scale = scale;
		localTransformDirty = true;
	}
	Quaternion Node3D::GetRotation() const {
		return rotation;
	}
	void Node3D::SetRotation(Quaternion rotation) {
		this->rotation = rotation;
		localTransformDirty = true;
	}

	void Node3D::UpdateLocalTransform() const {
		TransformMatrix t;
		t = rotation.ToTransformMatrix() * t;
		t = TransformMatrix::Scale(scale) * t;
		t = TransformMatrix::Translate(position) * t;
		localTransform = t;

		localTransformDirty = false;
	}
}
