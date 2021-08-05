#pragma once
#include "Engine/System/Object/Object.h"
#include "Engine/Application/Time.h"

namespace Engine {
	/// @brief Controls how the application runs its logic.
	class AppLoop:public ManualObject {
		REFLECTION_CLASS(::Engine::AppLoop, ::Engine::ManualObject) {}

	public:
		virtual ~AppLoop();

		/// @brief Called when the AppLoop is being started.\n
		/// Do the initialization work here.
		virtual void OnStart() = 0;

		/// @brief Called every frame update.
		virtual void OnUpdate(const Time& time) = 0;
		/// @brief Called every physics update.
		virtual void OnPhysicsUpdate(const Time& time) = 0;

		virtual void OnRender() = 0;
		
		//virtual void Input(const InputEvent& event)=0;

		/// @brief Called when the AppLoop is being shutdown.\n
		/// It is called after IsRunning() returns false.\n
		/// Do deinitialization work here.
		virtual void OnStop() = 0;
		
		/// @brief Used to indicate the engine whether to keep running.\n
		/// If return false, the engine will enter the shutdown state and then call OnStop().
		virtual bool IsRunning() const = 0;
	};
}
