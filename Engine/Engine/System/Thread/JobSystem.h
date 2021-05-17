#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/Collection/List.h"
#include "Engine/System/Collection/Deque.h"
#include "Engine/System/Memory/SharedPtr.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <new>

#undef GetJob

namespace Engine {
	using Mutex = std::mutex;
	template<typename T>
	using SimpleLock = std::lock_guard<T>;
	template<typename T>
	using AdvanceLock = std::unique_lock<T>;

	using ConditionVariable = std::condition_variable;

	class JobSystem;
	class JobWorker;
	struct Job;

	struct Job {
		using WorkFunction = void (*)(Job* job, void* data);

		WorkFunction function;
		bool finished = false;
		// avoid false sharing
		byte padding[64-8-1];
	};

	class JobWorker final {
	public:
		JobWorker(JobSystem* manager,int32 id);

		void Start();
		static void ThreadFunction(JobWorker* worker);
		bool IsRunning() const;

	private:
		JobSystem* manager;
		std::thread thread;
		volatile bool running = false;

		int32 id;
	};

	class JobSystem final {
	public:
		JobSystem();

		static int32 GetHardwareThreadCount();
		//static inline constexpr sizeint CacheLineSize = std::hardware_destructive_interference_size;

		void Start();
		void Stop();
		void AddJob(Job::WorkFunction function);
		SharedPtr<Job> GetJob();
		bool HasJob() const;
		bool IsRunning() const;

	private:
		friend class JobWorker;

		volatile bool running = false;

		List<SharedPtr<JobWorker>> workers{ 12 };

		List<SharedPtr<Job>> jobs{ 100 };
		mutable Mutex jobsMutex;
		ConditionVariable jobsCond;
		mutable Mutex jobsCondMutex;

		int32 lastId = -1;
	};
}
