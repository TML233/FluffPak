#include "Engine/Application/Node/NodeTree.h"
#include "Engine/Application/Engine.h"
#include "Engine/Application/Window.h"

namespace Engine {
	NodeTree::NodeTree() {
		root->index = 0;
		root->SetNameUnchecked(STRING_LITERAL("Root"));
	}

	void NodeTree::OnStart() {
		// Create the first window.
		WindowManager* nwm = ::Engine::Engine::GetInstance()->GetWindowManager();
		Window* nw = nwm->Create();
		nw->SetTitle(STRING_LITERAL("Rabbik Engine"));
		nw->SetSize(Vector2(640, 480));
		nw->SetVisible(true);

		GetRoot()->SystemAssignTree(this);
		running = true;

		INFO_MSG(root->GetTreeStructureFormated().GetRawArray());
	}

	void NodeTree::OnUpdate(const Time& time) {
		GetRoot()->SystemUpdate(time.GetDelta());

		if (stopWhenNoWindow && ::Engine::Engine::GetInstance()->GetWindowManager()->GetCount() <= 0) {
			running = false;
		}
	}
	void NodeTree::OnPhysicsUpdate(const Time& time) {
		GetRoot()->SystemPhysicsUpdate(time.GetDelta());

		if (stopWhenNoWindow && ::Engine::Engine::GetInstance()->GetWindowManager()->GetCount() <= 0) {
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