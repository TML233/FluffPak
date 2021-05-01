#pragma once
#include "Engine/System/Object.h"
#include "Engine/Application/Time.h"

namespace Engine {
	/// @brief Controls how the application runs its logic.
	class AppLoop:public ManualObject {
		REFLECTION_CLASS(::Engine::AppLoop, ::Engine::ManualObject) {}

	public:
		virtual ~AppLoop();

		/// @brief Called right before the first frame update.
		virtual void OnStart() = 0;

		/// @brief Called every frame update.
		virtual void OnUpdate(const Time& time) = 0;
		/// @brief Called every physics update.
		virtual void OnPhysicsUpdate(const Time& time) = 0;
		
		//virtual void Input(const InputEvent& event)=0;

		/// @brief Called right after the loop stopped.
		virtual void OnStop() = 0;
		
		virtual bool IsRunning() const = 0;
	};
}
