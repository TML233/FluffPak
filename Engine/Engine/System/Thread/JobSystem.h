#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/Collection/List.h"
#include "Engine/System/Collection/Dictionary.h"
#include "Engine/System/Memory/SharedPtr.h"
#include "Engine/System/Thread/ThreadUtils.h"
#include <mutex>
#include <condition_variable>

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
		using WorkFunction = void (*)(Job* job, void* data, JobWorker* worker);
		static inline constexpr sizeint DataLength = 64 - 8 - 1;

		WorkFunction function;
		bool finished = false;
		// Data zone, also prevents false sharing.
		byte data[DataLength];
	};

	class JobWorker final {
	public:
		enum class Preference :byte {
			Null,
			Window
		};

		JobWorker(JobSystem* manager,int32 id);

		void Start();
		static void ThreadFunction(JobWorker* worker);
		bool ShouldRun() const;
		bool IsRunning() const;

		bool HasJob() const;
		SharedPtr<Job> GetJob();
		void AddExclusiveJob(SharedPtr<Job> job);

		int32 GetId() const;

	private:
		JobSystem* manager;
		std::thread thread;
		volatile bool running = false;

		List<SharedPtr<Job>> exclusiveJobs{ 30 };
		mutable Mutex exclusiveJobMutex;

		int32 id;
	};

	class JobSystem final {
	public:
		JobSystem();

		void Start();
		void Stop();
		void AddJob(Job::WorkFunction function, void* data = nullptr, sizeint dataLength = 0, JobWorker::Preference preference = JobWorker::Preference::Null);
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

		Dictionary<JobWorker::Preference, int32> preferenceToWorker;

		int32 lastId = -1;
	};
}
