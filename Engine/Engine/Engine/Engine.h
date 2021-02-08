#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/UniquePtr.h"
#include "Engine/Engine/AppLoop.h"

namespace Engine {
	class Engine final :public Object {
	public:
		static Engine& GetInstance();

		Engine();
		~Engine();
		Engine(const Engine& obj) = delete;
		Engine& operator=(const Engine& obj) = delete;
		void Start();

		//Console& GetConsole() const;
		AppLoop& GetAppLoop() const;
	private:
		static Engine* instance;
		//UniquePtr<Console> console;
		UniquePtr<AppLoop> appLoop;
	};
}