#pragma once

#include "Engine/Application/AppLoop.h"
#include "Engine/Application/Node/Node.h"

namespace Engine{
	class NodeTree final:public AppLoop {
	public:
		void Update(float deltaTime) override;
		bool IsRunning() const override;
		int32 GetCurrentFrame() const;
	private:
		bool running = true;
		int32 currentFrame = 0;
	};
}