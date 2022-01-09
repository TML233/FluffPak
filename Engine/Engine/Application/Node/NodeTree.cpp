#include "Engine/Application/Node/NodeTree.h"
#include "Engine/Application/Engine.h"
#include "Engine/Application/Window.h"
#include "Engine/Application/Rendering/Renderer.h"

namespace Engine {
	NodeTree::NodeTree() {
		root->index = 0;
		root->SetNameUnchecked(STRL("Root"));
	}

	void NodeTree::OnStart() {
		// Create the first window.
		WindowSystem* nwm = ENGINEINST->GetWindowSystem();
		Window* nw = nwm->CreateWindow();
		nw->SetTitle(STRL("Rabbik Engine"));
		nw->SetSize(Vector2(1280, 720));
		nw->SetVisible(true);
		nw->SetResizable(true);
		nw->SetMaximizeButton(true);

		GetRoot()->SystemAssignTree(this);

		INFO_MSG(root->GetTreeStructureFormated().GetRawArray());
	}

	void NodeTree::OnUpdate(const Time& time) {
		if (stopWhenNoWindow && ENGINEINST->GetWindowSystem()->GetWindowCount() <= 0) {
			SetShouldRun(false);
		} else {
			GetRoot()->SystemUpdate(time.GetDelta());
		}
	}
	void NodeTree::OnPhysicsUpdate(const Time& time) {
		if (stopWhenNoWindow && ENGINEINST->GetWindowSystem()->GetWindowCount() <= 0) {
			SetShouldRun(false);
		} else {
			GetRoot()->SystemPhysicsUpdate(time.GetDelta());
		}
	}
	void NodeTree::OnRender() {
		auto renderer = ENGINEINST->GetRenderer();
		renderer->Render();
	}
	void NodeTree::OnStop() {
		root.Reset();
	}
	typename NodeTree::RootType* NodeTree::GetRoot() const {
		return root.GetRaw();
	}
}