#ifndef HEADER_CORE_ENGINE_ENIGNE
#define HEADER_CORE_ENGINE_ENGINE

#include "Core/Definition.h"
#include "Core/Template/UniquePtr.h"
#include "Core/Engine/AppLoop.h"

namespace Core {
	class Engine final{
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

#endif