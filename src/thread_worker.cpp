#include "thread_pool.h"
#include "thread_worker.h"

ThreadWorker::ThreadWorker(ThreadPool *parent)
	:	scheduler(parent)
{

}

void ThreadWorker::operator()()
{
	std::unique_lock<std::mutex> guard(scheduler->queue_mutex);

	scheduler->new_job.wait(guard, [this] {
			return this->scheduler->jobs.empty();
	});

	auto job = scheduler->jobs.front();
	scheduler->jobs.pop();

	guard.unlock();
	job();
}
