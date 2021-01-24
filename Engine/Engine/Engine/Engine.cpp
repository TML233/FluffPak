#include "Engine/Engine/Engine.h"
#include "Engine/System/String.h"
#include "Engine/System/Console.h"

namespace Engine {
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