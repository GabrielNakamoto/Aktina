#include "thread_pool.h"
#include "thread_worker.h"

ThreadWorker::ThreadWorker(ThreadPool *parent)
	:	scheduler(parent)
{

}

void ThreadWorker::operator()()
{

	while (true)
	{
		std::function<void()> job;

		{
			std::unique_lock<std::mutex> guard(scheduler->queue_mutex);
			scheduler->new_job.wait(guard, [this] {
					return (! this->scheduler->jobs.empty()) || (this->scheduler->should_finish);
			});

			if (scheduler->should_finish)
				return;

			job = std::move(scheduler->jobs.front());
			scheduler->jobs.pop();
		}

		scheduler->active_jobs++;

		job();

		scheduler->active_jobs--;
		scheduler->job_end.notify_one();

		if (scheduler->should_finish)
			return;
	}
}
