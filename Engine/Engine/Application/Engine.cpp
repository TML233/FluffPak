#include "Engine/Application/Engine.h"
#include "Engine/System/Memory/Memory.h"
#include <chrono>
#include "Engine/Platform/Window.h"
#include "Engine/System/File.h"
#include "Engine/System/Thread/JobSystem.h"
#include "Engine/Application/AppLoop.h"

namespace Engine {
	Engine* Engine::instance = nullptr;
	Engine* Engine::GetInstance() {
		return instance;
	}

	Engine::Engine() {
		instance = this;

		windowManager.Reset(MEMNEW(PLATFORM_SPECIFIC_CLASS_WINDOWMANAGER));
		
		fileSystem.Reset(MEMNEW(FileSystem()));

		jobSystem.Reset(MEMNEW(JobSystem()));
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
	WindowManager* Engine::GetWindowManager() const {
		return windowManager.GetRaw();
	}
	FileSystem* Engine::GetFileSystem() const {
		return fileSystem.GetRaw();
	}
	JobSystem* Engine::GetJobSystem() const {
		return jobSystem.GetRaw();
	}

	void Engine::Run() {
#pragma region Info messages
		INFO_MSG(u8"Rabbik Engine Development");
		INFO_MSG(u8"Under MIT public license. TML 2020-2021");
#if defined(_MSC_VER)
		INFO_MSG(String::Format(STRING_LITERAL("Compiled by Microsoft Visual C++ {0}"), _MSC_VER).GetRawArray());
#elif defined(__GNUC__)
		INFO_MSG(String::Format(STRING_LITERAL("Compiled by GNU C++ {0}.{1}.{2}"), __GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__).GetRawArray());
#endif
#pragma endregion

#pragma region Start
		if (appLoop == nullptr) {
			FATAL_MSG(u8"No AppLoop has been assigned.");
			return;
		}

		jobSystem->Start();
		INFO_MSG(u8"Job system started.");
		appLoop->OnStart();
		INFO_MSG(u8"App loop started.");
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
			TimePoint now = Clock::now();

			if (now >= nextUpdate) {
				windowManager->Update();

				time.unscaledDelta = std::chrono::duration_cast<Duration>(now - lastUpdate).count();
				
				time.unscaledTotal += time.GetUnscaledDelta();
				time.total += time.GetDelta();

				time.totalFrames += 1;

				//INFO_MSG(ObjectUtil::ToString(time.GetTotal()).GetRawArray());

				appLoop->OnUpdate(time);
				appLoop->OnPhysicsUpdate(time);
				windowManager->RenderAll();

				lastUpdate = now;
				nextUpdate += std::chrono::microseconds(static_cast<int64>(1.0 / GetTargetFps() * 1000000));
				now = Clock::now();
				std::this_thread::sleep_for(nextUpdate - now - Duration(0.005));
			}

			lastTime = now;
		}
#pragma endregion

#pragma region Stop
		appLoop->OnStop();
		jobSystem->Stop();

		INFO_MSG(u8"AppLoop finished running.");
#pragma endregion
	}
}
