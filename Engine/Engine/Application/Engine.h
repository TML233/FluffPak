#pragma once

#include "Engine/System/Object.h"
#include "Engine/System/UniquePtr.h"
#include "Engine/Application/AppLoop.h"

namespace Engine {
	class Engine final{
	public:
		Engine();
		~Engine();
		void Start();
		void SetAppLoop(UniquePtr<AppLoop>&& appLoop);
		AppLoop* GetAppLoop() const;
	private:
		UniquePtr<AppLoop> currentAppLoop;
	};
}
