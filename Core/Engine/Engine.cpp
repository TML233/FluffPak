#include "Core/Engine/Engine.h"
#include "Core/System/String.h"
#include "Core/System/Console.h"

namespace Core {
	Engine& Engine::GetInstance() {
		return *instance;
	}
	Engine* Engine::instance = nullptr;

	Engine::Engine() {
		if (instance) {
			throw Exception("Singleton Error: Engine is already instantiated.");
		}
		instance = this;
	}
	Engine::~Engine() {

	}

	AppLoop& Engine::GetAppLoop() const {
		return *appLoop;
	}
}