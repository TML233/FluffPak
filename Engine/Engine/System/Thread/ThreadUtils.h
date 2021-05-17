#pragma once
#include "Engine/System/Definition.h"
#include <thread>
#include <new>

namespace Engine{
	class ThreadUtils final{
		STATIC_CLASS(ThreadUtils);

	public:
		static int32 GetHardwareThreadCount();
		static inline constexpr sizeint CacheLineSize = 64;//std::hardware_destructive_interference_size;
	};
}
