#include "Engine/Application/Engine.h"
#include "Engine/System/Memory.h"

namespace Engine {
	Engine::Engine() {}
	Engine::~Engine() {}
	void Engine::SetAppLoop(UniquePtr<AppLoop>&& appLoop) {
		currentAppLoop = Memory::Move(appLoop);
	}
	AppLoop* Engine::GetAppLoop() const {
		return currentAppLoop.GetRaw();
	}
	void Engine::Start() {
		while (currentAppLoop->IsRunning()) {
			// TODO: Timing
			currentAppLoop->Update(0);
		}
	}
}
