#pragma once
#include "Engine/System/Definition.h"
#include <mutex>
#include <condition_variable>
#include <new>

namespace Engine{
	using Mutex = std::mutex;
	template<typename T>
	using SimpleLock = std::lock_guard<T>;
	template<typename T>
	using AdvanceLock = std::unique_lock<T>;

	using ConditionVariable = std::condition_variable;

	class ThreadUtils final{
		STATIC_CLASS(ThreadUtils);

	public:
		static int32 GetHardwareThreadCount();
		static inline constexpr sizeint CacheLineSize = 64;//std::hardware_destructive_interference_size;
	};
}
