#include "Engine/Application/Node/NodeTree.h"
#include "Engine/Application/Engine.h"

namespace Engine {
	NodeTree::NodeTree() {
		root->index = 0;
		root->SetNameUnchecked(STRING_LITERAL("Root"));
	}

	void NodeTree::OnStart() {
		// Create the first window.
		NativeWindowManager* nwm = ::Engine::Engine::GetInstance()->GetNativeWindowManager();
		NativeWindow* nw = nwm->Create();
		nw->SetTitle(STRING_LITERAL("Rabbik Engine"));
		nw->SetVisible(true);
		nw->SetSize(Vector2(640, 480));
		nw->SetResizable(true);

		GetRoot()->SystemAssignTree(this);
		running = true;

		INFO_MSG(root->GetTreeStructureFormated().GetRawArray());
	}

	void NodeTree::OnUpdate(const Time& time) {
		GetRoot()->SystemUpdate(time.GetDelta());

		if (stopWhenNoWindow && ::Engine::Engine::GetInstance()->GetNativeWindowManager()->GetCount() <= 0) {
			running = false;
		}
	}
	void NodeTree::OnPhysicsUpdate(const Time& time) {
		GetRoot()->SystemPhysicsUpdate(time.GetDelta());

		if (stopWhenNoWindow && ::Engine::Engine::GetInstance()->GetNativeWindowManager()->GetCount() <= 0) {
			running = false;
		}
	}
	void NodeTree::OnStop() {
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