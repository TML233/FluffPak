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
		using WorkFunction = void (*)(Job* job);
		static inline constexpr sizeint DataLength = 64 - 8 - 1;
		enum class Preference :byte {
			Null,
			Window
		};
		template<typename T>
		volatile T* GetDataAs() {
			return (volatile T*)(&data);
		}

		WorkFunction function;
		volatile bool finished = false;
		// Data zone, also prevents false sharing.
		volatile byte data[DataLength];
	};

	class JobWorker final {
	public:
		JobWorker(JobSystem* manager,int32 id);

		/// @brief Start the worker.
		void Start();
		/// @brief Require the worker to stop.\n
		/// Use JobWorker::IsRunning() to check if the worker has stopped.
		void RequireStop();

		/// @brief Indicates that should this worker continue running.
		bool ShouldRun() const;
		/// @brief Indicates that if the thread of this worker is still running.
		bool IsRunning() const;

		/// @brief Job worker thread function. 
		static void ThreadFunction(JobWorker* worker);
		/// @brief Job running sequence.
		static void RunJob(SharedPtr<Job>& job);

		int32 GetId() const;

	private:
		friend class JobSystem;

		bool HasJob() const;
		SharedPtr<Job> GetJob();
		void AddExclusiveJob(SharedPtr<Job> job);

		JobSystem* manager;
		std::thread thread;
		volatile bool running = false;
		volatile bool shouldRun = false;

		List<SharedPtr<Job>> exclusiveJobs{ 30 };
		mutable Mutex exclusiveJobMutex;

		int32 id;
	};

	class JobSystem final {
	public:
		JobSystem();

		/// @brief Start the job system.
		void Start();
		/// @brief Stop the job system.\n
		/// Will block until all the worker threads stop.
		void Stop();
		/// @brief Add a job.
		SharedPtr<Job> AddJob(Job::WorkFunction function, void* data = nullptr, sizeint dataLength = 0, Job::Preference preference = Job::Preference::Null);
		/// @brief Indicates if the job system has job waiting for running.
		bool HasJob() const;
		/// @brief Indicates if the job system is still running.
		bool IsRunning() const;
		/// @brief Wait for a job stop. Will help run other jobs while waiting.
		void WaitJob(SharedPtr<Job> job);

	private:
		friend class JobWorker;

		/// @brief Get a job from the public job queue.
		SharedPtr<Job> GetJob();

		volatile bool running = false;

		List<SharedPtr<JobWorker>> workers{ 12 };

		List<SharedPtr<Job>> jobs{ 100 };
		mutable Mutex jobsMutex;
		ConditionVariable jobsCond;
		mutable Mutex jobsCondMutex;

		Dictionary<Job::Preference, int32> preferenceToWorker;

		int32 lastId = -1;
	};
}
