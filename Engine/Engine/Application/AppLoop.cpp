#include "Engine/Application/AppLoop.h"

namespace Engine {
	AppLoop::~AppLoop() {}

	bool AppLoop::ShouldRun() const {
		return shouldRun;
	}
	void AppLoop::SetShouldRun(bool shouldRun) {
		this->shouldRun = shouldRun;
	}
}
