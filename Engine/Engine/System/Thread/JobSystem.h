#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/Collection/List.h"
#include "Engine/System/Collection/Deque.h"
#include "Engine/System/Memory/SharedPtr.h"
#include <thread>

namespace Engine {
	class JobSystem final {
	public:
		static int32 GetHardwareThreadCount() {
			return (int32)std::thread::hardware_concurrency();
		}

	private:
		class WorkerThread final {
			
		};

		List<SharedPtr<WorkerThread>> workers(8);
	};
}
