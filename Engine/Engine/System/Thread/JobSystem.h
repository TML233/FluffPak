#pragma once
#include "Engine/System/Definition.h"

namespace Engine {
	class JobSystem final {
	public:
		static int32 GetHardwareThreadCount() {
			return (int32)std::thread::hardware_concurrency();
		}
	};
}
