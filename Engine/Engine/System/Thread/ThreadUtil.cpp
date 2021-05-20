#include "Engine/System/Thread/ThreadUtil.h"
#include <thread>

namespace Engine {
	int32 ThreadUtil::GetHardwareThreadCount() {
		return (int32)std::thread::hardware_concurrency();
	}
}
