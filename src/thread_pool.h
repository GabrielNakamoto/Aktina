#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <iostream>
#include <functional>
#include <thread>
#include <queue>
#include <condition_variable>

#include "thread_worker.h"

// TODO: Create thread safe queue? right now just use mutexes


class ThreadPool
{
friend class ThreadWorker;

private:
	std::atomic<bool> should_finish{false};
	std::atomic<int> active_jobs{};

	std::condition_variable new_job;
	std::condition_variable job_end;

	std::mutex queue_mutex;
	std::vector<std::thread> threads;
	std::queue<std::function<void()> > jobs;

	int n_workers;

public:

	ThreadPool()
	{
		const unsigned int maxThreads = std::thread::hardware_concurrency();

		// assume at least 2 cores?
		n_workers = maxThreads ? maxThreads : 2;

		// initialize threads
		for (int i = 0; i < n_workers; ++i)
		{
			threads.emplace_back(ThreadWorker(this));
		}

		for (auto &thread : threads)
			thread.detach();
	}

	void appendJob(std::function<void()> job)
	{
		{
			std::lock_guard<std::mutex> guard(queue_mutex);
			jobs.push(job);
		}
		new_job.notify_all();
	}

	void waitForCompletion()
	{
		// wait until queue is empty and then stop execution
	
		std::unique_lock<std::mutex> guard(queue_mutex);

		job_end.wait(guard, [this] {
				return this->jobs.empty() && ! this->active_jobs;
		});

		should_finish = true;
		new_job.notify_all();
	}

	[[nodiscard]] size_t workers() const
	{
		return n_workers;
	}
};

#endif
