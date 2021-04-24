#pragma once

#include "Engine/System/Object.h"
#include "Engine/System/UniquePtr.h"
#include "Engine/Application/AppLoop.h"
#include "Engine/Application/Time.h"

namespace Engine {
	/// @brief The engine application manager. Contains every information necessary for a application to run.
	class Engine final{
	public:
		/// @brief Get the currently active Engine instance. 
		static Engine* GetInstance();

		Engine();
		~Engine();
		/// @brief Start the engine, this will block the program until the engine stops.
		void Start();

		/// @brief Assgin an AppLoop. An AppLoop UniquePtr will be moved into the engine.
		void SetAppLoop(UniquePtr<AppLoop>&& appLoop);
		/// @brief Get the current AppLoop.
		AppLoop* GetAppLoop() const;

		/// @brief Get the time controller.
		Time& GetTime() const;

		/// @brief Controls the update frequency. -1 means no limit.
		/// @note Physics update frequency is controlled by Time::PhysicsDelta.
		void SetTargetFps(int fps);
		/// @brief Get the FPS limit.
		float GetTargetFps() const;


	private:
		static Engine* instance;

		UniquePtr<AppLoop> appLoop{};
		Time time{};

		int targetFps = 60;
	};
}
