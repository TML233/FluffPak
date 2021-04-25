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

		appLoop->OnStart();


		// Aliases for std::chrono
		using Clock = std::chrono::steady_clock;
		using TimePoint = std::chrono::time_point<Clock>;
		using Duration = std::chrono::duration<double>;

		TimePoint lastTime = Clock::now();

		while (appLoop->IsRunning()) {
			TimePoint now = Clock::now();

			float delta = std::chrono::duration_cast<Duration>(lastTime - now).count();

			// TODO: Timing.
			time.unscaledDelta = delta;
			time.totalFrames += 1;
			time.unscaledTotal += time.GetUnscaledDelta();
			time.total += time.GetDelta();

			appLoop->OnUpdate(time);
			appLoop->OnPhysicsUpdate(time);

			lastTime = now;
		}

		appLoop->OnStop();

		INFO_MSG("AppLoop finished running.");
	}
}
