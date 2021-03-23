#pragma once

#include "Engine/System/Object.h"
#include "Engine/System/UniquePtr.h"
#include "Engine/Application/AppLoop.h"
#include "Engine/Application/Time.h"

namespace Engine {
	class Engine final{
#pragma region Static
	public:
		static Engine* GetInstance();
	private:
		static Engine* instance;
#pragma endregion

	public:
		Engine();
		~Engine();
		// Start the engine, this will block the program until the engine stops.
		void Start();

		// Assgin an AppLoop. An AppLoop UniquePtr will be moved in to the engine.
		void SetAppLoop(UniquePtr<AppLoop>&& appLoop);
		// Get the current AppLoop.
		AppLoop* GetAppLoop() const;

		// Get the time controller.
		Time& GetTime() const;

		// Controls the update frequency. -1 means no limit.
		// Note: Physics update frequency is controlled by Time::PhysicsDelta.
		void SetTargetFps(int fps);
		// Get the FPS limit.
		float GetTargetFps() const;


	private:
		UniquePtr<AppLoop> appLoop{};
		Time time{};
		//VirtualFileSystem vfs{};

		int targetFps = 60;
	};
}
