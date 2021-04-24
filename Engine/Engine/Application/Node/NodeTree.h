#pragma once

#include "Engine/Application/AppLoop.h"
#include "Engine/Application/Node/Node.h"

namespace Engine{
	class NodeTree final:public AppLoop {
	public:
		void OnStart() override;
		void OnUpdate(const Time& time) override;
		void OnPhysicsUpdate(const Time& time) override;

		void OnStop() override;
		bool IsRunning() const override;
	private:
		bool running = true;
		int32 currentFrame = 0;
	};
}