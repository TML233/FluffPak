#include "Engine/System/Thread/JobSystem.h"
#include "Engine/System/String.h"

namespace Engine {
#pragma region JobWorker
	template<>
	int32 ObjectUtil::GetHashCode<>(const JobWorker::Preference& obj) {
		return (int32)obj;
	}

	JobWorker::JobWorker(JobSystem* manager, int32 id) :manager(manager), id(id) {}

	void JobWorker::Start() {
		thread = std::thread(ThreadFunction, this);
		thread.detach();
	}

	void JobWorker::ThreadFunction(JobWorker* worker) {
		worker->running = true;
		INFO_MSG(String::Format(STRL("Job worker {0} started."), worker->id).GetRawArray());

		while (worker->ShouldRun()) {
			while (worker->ShouldRun() && !worker->HasJob()) {
				//INFO_MSG(String::Format(STRL("Worker {0} waiting for job..."),worker->GetId()).GetRawArray());
				auto lock = AdvanceLock<Mutex>(worker->manager->jobsCondMutex);
				worker->manager->jobsCond.wait(lock);
			}
			//INFO_MSG(String::Format(STRL("Worker {0} exited wait loop."), worker->GetId()).GetRawArray());
			auto job = worker->GetJob();
			if (job != nullptr) {
				//INFO_MSG(String::Format(STRL("Worker {0} start working..."), worker->GetId()).GetRawArray());
				job->function(job.GetRaw(), job->data, worker);
				job->finished = true;
			}
		}

		INFO_MSG(String::Format(STRL("Job worker {0} stopped."), worker->id).GetRawArray());
		worker->running = false;
	}
	bool JobWorker::HasJob() const {
		return exclusiveJobs.GetCount() > 0 || manager->HasJob();
	}
	SharedPtr<Job> JobWorker::GetJob() {
		if (exclusiveJobs.GetCount() > 0) {
			auto lock = SimpleLock<Mutex>(exclusiveJobMutex);
			auto job = exclusiveJobs.Get(exclusiveJobs.GetCount() - 1);
			exclusiveJobs.RemoveAt(exclusiveJobs.GetCount() - 1);
			return job;
		}
		return manager->GetJob();
	}
	void JobWorker::AddExclusiveJob(SharedPtr<Job> job) {
		auto lock = SimpleLock<Mutex>(exclusiveJobMutex);
		exclusiveJobs.Add(job);
	}
	bool JobWorker::ShouldRun() const {
		return manager->IsRunning();
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

		preferenceToWorker.Add(JobWorker::Preference::Window, 0);

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
		running = false;

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
	}

	void JobSystem::AddJob(Job::WorkFunction function,void* data,sizeint dataLength,JobWorker::Preference preference) {
		if (data != nullptr) {
			FATAL_ASSERT(dataLength <= Job::DataLength, u8"data is too large to put into a job! Consider putting a pointer to the actual data.");
		}

		// Prepare job
		auto job = SharedPtr<Job>::Create();
		job->function = function;
		if (data != nullptr) {
			std::memcpy(job->data, data, dataLength);
		}

		// Exclusive job targeting
		int32 worker = -1;
		if (preference != JobWorker::Preference::Null) {
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

		//INFO_MSG(u8"JobSystem fed a job, notifing one.");
		jobsCond.notify_all();
	}
	SharedPtr<Job> JobSystem::GetJob() {
		auto lock = SimpleLock<Mutex>(jobsMutex);

		if (HasJob()) {
			auto job = jobs.Get(jobs.GetCount() - 1);
			jobs.RemoveAt(jobs.GetCount() - 1);
			return job;
		} else {
			return SharedPtr<Job>(nullptr);
		}
	}
	bool JobSystem::HasJob() const {
		return jobs.GetCount() > 0;
	}
	bool JobSystem::IsRunning() const {
		return running;
	}
#pragma endregion
}
