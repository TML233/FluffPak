#include "Engine/Application/Engine.h"
#include "Engine/System/Memory.h"
#include <chrono>

namespace Engine {
	Engine* Engine::instance = nullptr;
	Engine* Engine::GetInstance() {
		return instance;
	}

	Engine::Engine() {
		instance = this;
	}
	Engine::~Engine() {
		if (instance == this) {
			instance = nullptr;
		}
	}
	void Engine::SetAppLoop(UniquePtr<AppLoop>&& appLoop) {
		this->appLoop = Memory::Move(appLoop);
	}
	AppLoop* Engine::GetAppLoop() const {
		return appLoop.GetRaw();
	}
	void Engine::Start() {
		if (appLoop == nullptr) {
			FATAL_MSG("No AppLoop has been assigned.");
			return;
		}

		appLoop->Start();

		while (appLoop->IsRunning()) {
			// TODO: Timing.
			time.unscaledDelta = 0;
			time.totalFrames += 1;
			time.unscaledTotal += time.GetUnscaledDelta();
			time.total += time.GetDelta();

			appLoop->Update(time);
			appLoop->PhysicsUpdate(time);
		}



		INFO_MSG("AppLoop finished running. Stopping...");
	}
}
