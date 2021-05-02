#include "Engine/Application/Engine.h"
#include "Engine/System/Memory.h"
#include <chrono>
#include "Engine/Platform/NativeWindow.h"

namespace Engine {
	Engine* Engine::instance = nullptr;
	Engine* Engine::GetInstance() {
		return instance;
	}

	Engine::Engine() {
		instance = this;

		nativeWindowManager.Reset(MEMNEW(PLATFORM_SPECIFIC_CLASS_NATIVEWINDOWMANAGER));
		fileSystem.Reset(MEMNEW(FileSystem()));
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
	int32 Engine::GetTargetFps() const {
		return targetFps;
	}
	void Engine::SetTargetFps(int32 targetFps) {
		this->targetFps = targetFps;
	}
	Time& Engine::GetTime() {
		return time;
	}
	NativeWindowManager* Engine::GetNativeWindowManager() const {
		return nativeWindowManager.GetRaw();
	}
	FileSystem* Engine::GetFileSystem() const {
		return fileSystem.GetRaw();
	}

	void Engine::Run() {
#pragma region Start
		if (appLoop == nullptr) {
			FATAL_MSG(u8"No AppLoop has been assigned.");
			return;
		}

		appLoop->OnStart();
#pragma endregion

#pragma region Loop
		// Process other things...

		using Clock = std::chrono::steady_clock;
		using TimePoint = Clock::time_point;
		using Duration = std::chrono::duration<double>;

		TimePoint lastTime = Clock::now();
		TimePoint lastUpdate = Clock::now() - std::chrono::microseconds(static_cast<int64>(1.0/GetTargetFps()*1000000));
		TimePoint nextUpdate = Clock::now();

		while (appLoop->IsRunning()) {
			GetNativeWindowManager()->Update();

			TimePoint now = Clock::now();

			if (now >= nextUpdate) {
				time.unscaledDelta = std::chrono::duration_cast<Duration>(now - lastUpdate).count();
				
				time.unscaledTotal += time.GetUnscaledDelta();
				time.total += time.GetDelta();

				time.totalFrames += 1;

				//INFO_MSG(ObjectUtil::ToString(time.GetTotal()).GetRawArray());

				appLoop->OnUpdate(time);
				appLoop->OnPhysicsUpdate(time);

				lastUpdate = now;
				nextUpdate += std::chrono::microseconds(static_cast<int64>(1.0 / GetTargetFps() * 1000000));
			}

			lastTime = now;
		}
#pragma endregion

#pragma region Stop
		appLoop->OnStop();

		INFO_MSG(u8"AppLoop finished running.");
#pragma endregion
	}
}
