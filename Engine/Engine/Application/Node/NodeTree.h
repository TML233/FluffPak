#pragma once

#include "Engine/Application/AppLoop.h"
#include "Engine/Application/Node/Node.h"

namespace Engine{
	class NodeTree final:public AppLoop {
	public:
		void Start() override;
		void Update(const Time& time) override;
		void PhysicsUpdate(const Time& time) override;
		bool IsRunning() const override;
	private:
		bool running = true;
		int32 currentFrame = 0;
	};
}