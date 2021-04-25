#include "Engine/Application/Node/NodeTree.h"

namespace Engine {
	void NodeTree::OnStart() {
		GetRoot()->SystemAssignTree(this);
		running = true;
	}

	void NodeTree::OnUpdate(const Time& time) {
		GetRoot()->SystemUpdate(time.GetDelta());
	}
	void NodeTree::OnPhysicsUpdate(const Time& time) {
		GetRoot()->SystemPhysicsUpdate(time.GetDelta());
	}
	void NodeTree::OnStop() {
		running = false;
		root.Reset();
	}
	bool NodeTree::IsRunning() const {
		return running;
	}

	typename NodeTree::RootType* NodeTree::GetRoot() const {
		return root.GetRaw();
	}
}