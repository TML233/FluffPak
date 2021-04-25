#include "Engine/System/Debug.h"
#include "Engine/Application/Engine.h"
#include "Engine/Application/Node/NodeTree.h"
#include "Engine/System/Variant.h"
#include "Engine/Application/Node/TestNode.h"

using namespace Engine;
//using var = Variant;

int main() {
	using App = ::Engine::Engine;

	UniquePtr<App> engine = UniquePtr<App>::Create();
	UniquePtr<NodeTree> tree{ MEMNEW(NodeTree()) };

	tree->GetRoot()->AddChild(MEMNEW(TestNode()));

	engine->SetAppLoop(UniquePtr<AppLoop>(tree.Release()));
	engine->Start();

	return 0;
}