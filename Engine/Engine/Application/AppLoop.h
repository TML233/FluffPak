#pragma once
#include "Engine/System/Object.h"
#include "Engine/Application/Time.h"

namespace Engine {
	class AppLoop:public ManualObject {
	public:
		virtual ~AppLoop();

		// Called right before the first frame update.
		virtual void Start() = 0;

		// Called every frame update.
		virtual void Update(const Time& time) = 0;
		// Called every physics update.
		virtual void PhysicsUpdate(const Time& time) = 0;
		
		//virtual void Input(const InputEvent& event)=0;
		
		virtual bool IsRunning() const = 0;
	};
}
