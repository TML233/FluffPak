#include "Engine/Application/Engine.h"
#include "Engine/System/Memory/Memory.h"
#include <chrono>
#include "Engine/Platform/Window.h"
#include "Engine/System/File/FileSystem.h"
#include "Engine/System/Thread/JobSystem.h"
#include "Engine/Application/AppLoop.h"
#include "Engine/Application/Rendering/Renderer.h"

namespace Engine {
	Engine* Engine::instance = nullptr;
	Engine* Engine::GetInstance() {
		return instance;
	}

	Engine::Engine() {
		instance = this;

#pragma region Info messages
		INFO_MSG(u8"Rabbik Engine Development");
		INFO_MSG(u8"Under MIT public license. TML 2020-2021");
#if defined(_MSC_VER)
		INFO_MSG(String::Format(STRING_LITERAL("Compiled by Microsoft Visual C++ {0}"), _MSC_VER).GetRawArray());
#elif defined(__GNUC__)
		INFO_MSG(String::Format(STRING_LITERAL("Compiled by GNU C++ {0}.{1}.{2}"), __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__).GetRawArray());
#endif
#pragma endregion
		INFO_MSG(u8"===== Initializing =====");
		INFO_MSG(u8"==> File System");
		fileSystem.Reset(MEMNEW(FileSystem()));

		INFO_MSG(u8"==> Job System");
		jobSystem.Reset(MEMNEW(JobSystem()));

		INFO_MSG(u8"==> Window System");
		windowSystem.Reset(MEMNEW(PLATFORM_SPECIFIC_CLASS_WINDOWMANAGER));

		INFO_MSG(u8"==> Renderer");
		renderer.Reset(MEMNEW(Renderer()));

		INFO_MSG(u8"===== Ready =====");
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
	float Engine::GetTargetFps() const {
		return targetFps;
	}
	void Engine::SetTargetFps(float targetFps) {
		this->targetFps = targetFps;
	}
	float Engine::GetFps() const {
		return fps;
	}
	float Engine::GetFpsUpdateFrequency() const {
		return fpsUpdateFrequency;
	}
	void Engine::SetFpsUpdateFrequency(float frequency) {
		fpsUpdateFrequency = frequency;
	}

	Time& Engine::GetTime() {
		return time;
	}
	WindowSystem* Engine::GetWindowSystem() const {
		return windowSystem.GetRaw();
	}
	FileSystem* Engine::GetFileSystem() const {
		return fileSystem.GetRaw();
	}
	JobSystem* Engine::GetJobSystem() const {
		return jobSystem.GetRaw();
	}
	Renderer* Engine::GetRenderer() const {
		return renderer.GetRaw();
	}

	void Engine::Run() {
		INFO_MSG(u8"===== Starting =====");
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

		TimePoint lastUpdate = Clock::now() - std::chrono::duration_cast<Clock::duration>(Duration(1.0 / GetTargetFps()));;;
		TimePoint nextUpdate = Clock::now();

		TimePoint lastFpsCheck = Clock::now();
		int32 updateTimes = 0;

		while (appLoop->IsRunning()) {
			TimePoint now = Clock::now();

			if (now >= nextUpdate) {
				windowSystem->Update();

				time.unscaledDelta = std::chrono::duration_cast<Duration>(now - lastUpdate).count();
				time.unscaledTotal += time.GetUnscaledDelta();
				time.total += time.GetDelta();
				time.totalFrames += 1;
				appLoop->OnUpdate(time);
				appLoop->OnRender();

#pragma region FPS Count
				updateTimes += 1;
				Duration fpsCheckDuration = now-lastFpsCheck;
				if (fpsCheckDuration >= Duration(fpsUpdateFrequency)) {
					fps = updateTimes / fpsCheckDuration.count();
					updateTimes = 0;
					lastFpsCheck = now;
				}
#pragma endregion

				lastUpdate = now;
				do {
					nextUpdate += std::chrono::duration_cast<Clock::duration>(Duration(1.0 / GetTargetFps()));
				} while (nextUpdate < lastUpdate);

				now = Clock::now();
				std::this_thread::sleep_for(nextUpdate - now - Duration(0.005));
			}
		}
#pragma endregion

#pragma region Stop
		appLoop->OnStop();
		jobSystem->Stop();

		INFO_MSG(u8"AppLoop finished running.");
#pragma endregion
	}
}
