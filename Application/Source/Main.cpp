#include "Engine/System/Debug.h"
#include "Engine/Application/Engine.h"
#include "Engine/Application/Node/NodeTree.h"

using namespace Engine;
int main() {
	UniquePtr<::Engine::Engine> app(MEMNEW(::Engine::Engine));
	INFO_MSG("Using \"NodeTree\" as AppLoop.");
	app->SetAppLoop(UniquePtr<AppLoop>(MEMNEW(NodeTree)));
	app->Start();
	return 0;
}