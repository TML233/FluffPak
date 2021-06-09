#include "Engine/System/Debug.h"
#include "Engine/Application/Engine.h"
#include "Engine/Application/Node/NodeTree.h"
#include "Engine/System/Object/Variant.h"
#include "Application/TestNode.h"

using namespace Engine;
//using var = Variant;

namespace Application {
	int Run() {
		using App = ::Engine::Engine;

		UniquePtr<App> engine = UniquePtr<App>::Create();
		UniquePtr<NodeTree> tree{ MEMNEW(NodeTree()) };

		tree->GetRoot()->AddChild(MEMNEW(TestNode()));
		tree->GetRoot()->AddChild(MEMNEW(Node()));
		tree->GetRoot()->GetChildByIndex(0)->AddChild(MEMNEW(Node()));
		tree->GetRoot()->GetChildByIndex(0)->AddChild(MEMNEW(Node()), 1);
		tree->GetRoot()->GetChildByIndex(1)->AddChild(MEMNEW(Node()));
		tree->GetRoot()->AddChild(MEMNEW(Node()));
		tree->GetRoot()->GetChildByIndex(2)->AddChild(MEMNEW(Node()));

		engine->SetAppLoop(UniquePtr<AppLoop>(tree.Release()));
		engine->Run();
		
		return 0;
	}
}

int main() {
	return Application::Run();
}