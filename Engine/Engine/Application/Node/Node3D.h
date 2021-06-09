#pragma once

#include "Engine/Application/Node/Node.h"
#include "Engine/System/Math/Vector.h"
#include "Engine/System/Math/TransformMatrix.h"
#include "Engine/System/Math/Quaternion.h"

namespace Engine {
	class Node3D :public Node {
		REFLECTION_CLASS(::Engine::Node3D, ::Engine::Node) {}

	public:
		Vector3 GetPosition() const;
		void SetPosition(const Vector3& position);

		Vector3 GetScale() const;
		void SetScale(const Vector3& scale);

		Quaternion GetRotation() const;
		void SetRotation(Quaternion rotation);

		

		void UpdateLocalTransform() const;
		void UpdateGlobalTransform() const;

	private:
		Vector3 position = Vector3(0, 0, 0);
		Vector3 scale = Vector3(1, 1, 1);
		Quaternion rotation = Quaternion();

		mutable TransformMatrix localTransform;
		mutable TransformMatrix globalTransform;
		mutable bool localTransformDirty = true;
		mutable bool globalTransformDirty = true;
	};
}