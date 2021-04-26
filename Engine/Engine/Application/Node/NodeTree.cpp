#include "Engine/Application/Node/NodeTree.h"

namespace Engine {
	NodeTree::NodeTree() {
		root->index = 0;
		root->SetNameUnchecked(STRING_LITERAL("Root"));
	}

	void NodeTree::OnStart() {
		GetRoot()->SystemAssignTree(this);
		running = true;

		INFO_MSG(root->GetTreeStructureFormated().GetRawArray());
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

	void NodeTree::RequestStop() {
		running = false;
	}
}