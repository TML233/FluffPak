#include "Engine/System/Thread/JobSystem.h"
#include "Engine/System/String.h"

namespace Engine {
	JobWorker::JobWorker(JobSystem* manager,int32 id) :manager(manager),id(id) {}

	void JobWorker::Start() {
		thread = std::thread(ThreadFunction, this);
		thread.detach();
	}

	void JobWorker::ThreadFunction(JobWorker* worker) {
		worker->running = true;
		INFO_MSG(String::Format(STRL("Job worker {0} started."), worker->id).GetRawArray());

		while (worker->manager->IsRunning()) {
			auto lock = AdvanceLock<Mutex>(worker->manager->jobsCondMutex);
			while (worker->manager->IsRunning()&&!worker->manager->HasJob()) {
				worker->manager->jobsCond.wait(lock);
			}

			auto job = worker->manager->GetJob();
			if (job != nullptr) {
				job->function(job.GetRaw(), nullptr);
				job->finished = true;
			}
		}

		INFO_MSG(String::Format(STRL("Job worker {0} stopped."), worker->id).GetRawArray());
		worker->running = false;
	}

	bool JobWorker::IsRunning() const {
		return running;
	}




	JobSystem::JobSystem() {
		int32 hardware = GetHardwareThreadCount();
		INFO_MSG(String::Format(STRING_LITERAL("{0} hardware threads, creating {1} job workers."), hardware, hardware - 1).GetRawArray());
		//INFO_MSG(String::Format(STRING_LITERAL("L1 cache line size: {0} bytes."), CacheLineSize).GetRawArray());
		INFO_MSG(String::Format(STRING_LITERAL("Job struct size: {0} bytes."), sizeof(Job)).GetRawArray());

		for (int32 i = 0; i < hardware - 1; i += 1) {
			lastId += 1;
			auto worker = SharedPtr<JobWorker>::Create(this,lastId);
			workers.Add(worker);
		}
	}
	
	int32 JobSystem::GetHardwareThreadCount() {
		return (int32)std::thread::hardware_concurrency();
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

	void JobSystem::AddJob(Job::WorkFunction function) {
		auto job = SharedPtr<Job>::Create();
		job->function = function;

		{
			auto lock = SimpleLock<Mutex>(jobsMutex);
			jobs.Add(job);
		}

		//INFO_MSG(u8"JobSystem fed a job, notifing one.");
		jobsCond.notify_one();
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
}
