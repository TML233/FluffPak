#pragma once

#include "Engine/System/Object/Object.h"
#include "Engine/System/Memory/UniquePtr.h"
#include "Engine/Application/AppLoop.h"
#include "Engine/Application/Time.h"
#include "Engine/Application/NativeWindow.h"
#include "Engine/System/File.h"

namespace Engine {
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
		void SetTargetFps(int32 targetFps);
		/// @brief Get the FPS limit.
		int32 GetTargetFps() const;

		NativeWindowManager* GetNativeWindowManager() const;
		FileSystem* GetFileSystem() const;

	private:
		static Engine* instance;

		UniquePtr<AppLoop> appLoop{};
		Time time{};
		UniquePtr<NativeWindowManager> nativeWindowManager;
		UniquePtr<FileSystem> fileSystem;

		int32 targetFps = 60;
	};
}
