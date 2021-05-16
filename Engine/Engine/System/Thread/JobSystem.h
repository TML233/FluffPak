#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/Collection/List.h"
#include "Engine/System/Collection/Deque.h"
#include "Engine/System/Memory/SharedPtr.h"
#include <thread>
#include <mutex>

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
	class Job;

	class Job {
	public:
		virtual void Run() = 0;
	};

	class JobWorker final {
	public:
		JobWorker(JobSystem* manager);

		void Start();
		static void ThreadFunction(JobWorker* worker);
		void Stop();

	private:
		JobSystem* manager;
		std::thread thread;
	};

	class JobSystem final {
	public:
		JobSystem();

		static int32 GetHardwareThreadCount();

		void Start();
		void Stop();
		void AddJob(const SharedPtr<Job>& job);
		SharedPtr<Job> GetJob();
		bool HasJob() const;

	private:
		friend class JobWorker;

		List<SharedPtr<JobWorker>> workers{ 12 };

		List<SharedPtr<Job>> jobs;
		mutable Mutex jobsMutex;
		ConditionVariable jobsCond;
		mutable Mutex jobsCondMutex;
	};
}
