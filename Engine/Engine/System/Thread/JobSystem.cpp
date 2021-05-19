#include "Engine/System/Thread/JobSystem.h"
#include "Engine/System/String.h"

namespace Engine {
#pragma region JobWorker
	template<>
	int32 ObjectUtil::GetHashCode<>(const Job::Preference& obj) {
		return (int32)obj;
	}

	JobWorker::JobWorker(JobSystem* manager, int32 id) :manager(manager), id(id) {}

	void JobWorker::Start() {
		shouldRun = true;

		thread = std::thread(ThreadFunction, this);
		thread.detach();
	}
	void JobWorker::RequireStop() {
		shouldRun = false;
	}

	void JobWorker::ThreadFunction(JobWorker* worker) {
		worker->running = true;
		//INFO_MSG(String::Format(STRL("Job worker {0} started."), worker->id).GetRawArray());

		SharedPtr<Job> job;
		while (worker->ShouldRun()) {
			if (job != nullptr) {
				RunJob(job);
				job = SharedPtr<Job>(nullptr);
			}

			{
				auto lock = AdvanceLock<Mutex>(worker->manager->jobsCondMutex);
				worker->manager->jobsCond.wait(lock, [worker, &job]() {
					if (!worker->ShouldRun()) {
						return true;
					}
					if (job == nullptr) {
						job = worker->GetJob();
					}
					return job != nullptr;
				});
			}
		}

		//INFO_MSG(String::Format(STRL("Job worker {0} stopped."), worker->id).GetRawArray());
		worker->running = false;
	}
	void JobWorker::RunJob(SharedPtr<Job>& job) {
		job->function(job.GetRaw());
		job->finished = true;
	}
	SharedPtr<Job> JobWorker::GetJob() {
		{
			auto lock = SimpleLock<Mutex>(exclusiveJobMutex);
			if (exclusiveJobs.GetCount() > 0) {
				auto job = exclusiveJobs.Get(exclusiveJobs.GetCount() - 1);
				exclusiveJobs.RemoveAt(exclusiveJobs.GetCount() - 1);
				return job;
			}
		}
		return manager->GetJob();
	}
	void JobWorker::AddExclusiveJob(SharedPtr<Job> job) {
		auto lock = SimpleLock<Mutex>(exclusiveJobMutex);
		exclusiveJobs.Add(job);
	}
	bool JobWorker::ShouldRun() const {
		return shouldRun;
	}
	bool JobWorker::IsRunning() const {
		return running;
	}

	int32 JobWorker::GetId() const {
		return id;
	}
#pragma endregion

#pragma region JobSystem
	JobSystem::JobSystem() {
		int32 hardware = ThreadUtils::GetHardwareThreadCount();
		INFO_MSG(String::Format(STRING_LITERAL("{0} hardware threads, creating {1} job workers."), hardware, hardware - 1).GetRawArray());
		//INFO_MSG(String::Format(STRING_LITERAL("L1 cache line size: {0} bytes."), CacheLineSize).GetRawArray());
		INFO_MSG(String::Format(STRING_LITERAL("Job struct size: {0} bytes."), sizeof(Job)).GetRawArray());

		preferenceToWorker.Add(Job::Preference::Window, 0);

		for (int32 i = 0; i < hardware - 1; i += 1) {
			lastId += 1;
			auto worker = SharedPtr<JobWorker>::Create(this,lastId);
			workers.Add(worker);
		}
	}

	void JobSystem::Start() {
		running = true;

		for (const auto& worker : workers) {
			worker->Start();
		}
	}

	void JobSystem::Stop() {
		for (const auto& worker : workers) {
			worker->RequireStop();
		}

		// Wait for all threads stop.
		bool stop = true;
		do {
			stop = true;
			jobsCond.notify_all();

			for (const auto& worker : workers) {
				if (worker->IsRunning()) {
					stop = false;
					break;
				}
			}
		} while (!stop);

		running = false;
	}

	SharedPtr<Job> JobSystem::AddJob(Job::WorkFunction function,void* data,sizeint dataLength,Job::Preference preference) {
		if (data != nullptr) {
			FATAL_ASSERT(dataLength <= Job::DataLength, u8"data is too large to put into a job! Consider putting a pointer to the actual data.");
		}

		// Prepare job
		auto job = SharedPtr<Job>::Create();
		job->function = function;
		if (data != nullptr) {
			for (sizeint i = 0; i < dataLength; i += 1) {
				job->data[i] = ((byte*)data)[i];
			}
		}

		// Exclusive job targeting
		int32 worker = -1;
		if (preference != Job::Preference::Null) {
			preferenceToWorker.TryGet(preference, worker);
		}
		
		// Add job
		if (worker < 0) {
			auto lock = SimpleLock<Mutex>(jobsMutex);
			jobs.Add(job);
		} else if (worker < workers.GetCount()) {
			workers.Get(worker)->AddExclusiveJob(job);
		} else {
			FATAL_CRASH(u8"PreferenceToWorker map error! Trying to add exclusive work to unexisting worker!");
		}

		{
			auto lock = SimpleLock<Mutex>(jobsCondMutex);
			jobsCond.notify_all();
		}
		return job;
	}
	SharedPtr<Job> JobSystem::GetJob() {
		auto lock = SimpleLock<Mutex>(jobsMutex);

		if (jobs.GetCount()>0) {
			auto job = jobs.Get(jobs.GetCount() - 1);
			jobs.RemoveAt(jobs.GetCount() - 1);
			return job;
		} else {
			return SharedPtr<Job>(nullptr);
		}
	}
	bool JobSystem::IsRunning() const {
		return running;
	}

	void JobSystem::WaitJob(SharedPtr<Job> job) {
		while (!job->finished) {
			// Help run jobs when waiting.
			//auto job = GetJob();
			//if (job!=nullptr) {
			//	JobWorker::RunJob(job);
			//}
		}
	}
#pragma endregion
}
