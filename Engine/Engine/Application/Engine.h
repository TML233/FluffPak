#pragma once
#include "Engine/System/Memory/UniquePtr.h"
#include "Engine/Application/Time.h"

#define ENGINEINST ::Engine::Engine::GetInstance()

namespace Engine {
	class WindowSystem;
	class FileSystem;
	class JobSystem;
	class AppLoop;

	/// @brief The engine application manager. Contains every information necessary for a application to run.
	class Engine final{
	public:
		/// @brief Get the currently active Engine instance. 
		static Engine* GetInstance();

		Engine();
		~Engine();
		/// @brief Start the engine, this will block the program until the engine stops.
		void Run();

		/// @brief Assgin an AppLoop. An AppLoop UniquePtr will be moved into the engine.
		void SetAppLoop(UniquePtr<AppLoop>&& appLoop);
		/// @brief Get the current AppLoop.
		AppLoop* GetAppLoop() const;

		/// @brief Get the time controller.
		Time& GetTime();

		/// @brief Controls the update frequency. -1 means no limit.
		/// @note Physics update frequency is controlled by Time::PhysicsDelta.
		void SetTargetFps(float targetFps);
		/// @brief Get the FPS limit.
		float GetTargetFps() const;

		float GetFps() const;
		float GetFpsUpdateFrequency() const;
		void SetFpsUpdateFrequency(float frequency);

		WindowSystem* GetWindowSystem() const;
		FileSystem* GetFileSystem() const;
		JobSystem* GetJobSystem() const;

	private:
		static Engine* instance;

		UniquePtr<AppLoop> appLoop{};
		Time time{};
		UniquePtr<WindowSystem> windowSystem;
		UniquePtr<FileSystem> fileSystem;
		UniquePtr<JobSystem> jobSystem;

		float targetFps = 60;
		float fps = 0;
		float fpsUpdateFrequency = 1;
	};
}
