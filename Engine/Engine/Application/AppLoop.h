#pragma once
#include "Engine/System/Object.h"

namespace Engine {
	class AppLoop:public ManualObject {
	public:
		virtual ~AppLoop();
		virtual void Update(float deltaTime) = 0;
		//virtual void OnInput(InputEvent event)=0;
		virtual bool IsRunning() const = 0;
	};
}
