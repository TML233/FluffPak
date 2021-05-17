#include "Engine/System/Thread/ThreadUtils.h"

namespace Engine {
	int32 ThreadUtils::GetHardwareThreadCount() {
		return (int32)std::thread::hardware_concurrency();
	}
}
