#include "thread_pool.h"
#include "thread_worker.h"

ThreadWorker::ThreadWorker(ThreadPool *parent)
	:	scheduler(parent)
{

}

void ThreadWorker::operator()()
{
	std::unique_lock<std::mutex> guard(scheduler->queue_mutex);

	while (! this->scheduler->should_finish)
	{
		scheduler->new_job.wait(guard, [this] {
				return (! this->scheduler->jobs.empty()) || (this->scheduler->should_finish);
				});

		if (scheduler->should_finish)
			return;

		auto job = scheduler->jobs.front();
		scheduler->jobs.pop();

		guard.unlock();
		job();
		guard.lock();

		scheduler->job_end.notify_one();
	}
}
