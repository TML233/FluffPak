#include "Engine/System/Thread/JobSystem.h"
#include "Engine/System/String.h"

namespace Engine {
	JobWorker::JobWorker(JobSystem* manager) :manager(manager) {}

	void JobWorker::Start() {
		thread = std::thread(ThreadFunction, this);
		thread.detach();
	}

	void JobWorker::ThreadFunction(JobWorker* worker) {
		while (true) {
			auto lock = AdvanceLock<Mutex>(worker->manager->jobsCondMutex);
			while (!worker->manager->HasJob()) {
			//	INFO_MSG(u8"Worker is waiting...");
				worker->manager->jobsCond.wait(lock);
			//	INFO_MSG(u8"Worker awake!");
			}
			//INFO_MSG(u8"Worker got out of the wait loop!");

			auto job = worker->manager->GetJob();
			if (job != nullptr) {
				//INFO_MSG(u8"Worker has a job, running.");
				job->Run();
			//} else {
			//	INFO_MSG(u8"Worker has no job!");
			}
		}
	}

	void JobWorker::Stop() {

	}





	JobSystem::JobSystem() {
		int32 hardware = GetHardwareThreadCount();
		INFO_MSG(String::Format(STRING_LITERAL("{0} hardware threads, creating {1} job workers."), hardware, hardware - 1).GetRawArray());

		for (int32 i = 0; i < hardware - 1; i += 1) {
			workers.Add(SharedPtr<JobWorker>::Create(this));
		}
	}
	
	int32 JobSystem::GetHardwareThreadCount() {
		return (int32)std::thread::hardware_concurrency();
	}

	void JobSystem::Start() {
		for (const auto& worker : workers) {
			worker->Start();
		}
	}

	void JobSystem::Stop() {

	}

	void JobSystem::AddJob(const SharedPtr<Job>& job) {
		auto lock = AdvanceLock<Mutex>(jobsCondMutex);
		jobs.Add(job);
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
}
