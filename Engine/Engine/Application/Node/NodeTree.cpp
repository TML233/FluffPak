#include "Engine/Application/Node/NodeTree.h"

namespace Engine {
	void NodeTree::OnUpdate(float deltaTime) {
		currentFrame += 1;
	}
	bool NodeTree::IsRunning() const {
		return running;
	}
	int32 NodeTree::GetCurrentFrame() const {
		return currentFrame;
	}
}