#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <iostream>
#include <functional>
#include <thread>
#include <queue>
#include <condition_variable>

#include "thread_safe_queue.h"

// TODO: Create thread safe queue? right now just use mutexes


class ThreadPool
{
private:
	std::atomic<bool> should_finish{false};
	std::atomic<int> active_jobs{};

	std::vector<std::thread> threads;
	threadsafe_queue<std::function<void()> > jobs;

	ThreadGuard joiner;

	std::mutex finish_lock;
	std::condition_variable job_finished;

	int n_workers;

void threadWorker()
{
	while (! should_finish)
	{
		std::function<void()> job;
		if (jobs.try_pop(job))
		{
			active_jobs++;

			job();

			active_jobs--;

			job_finished.notify_one();
		}
		else
		{
			std::this_thread::yield();
		}
	}
}

public:

	ThreadPool()
		:	joiner(threads)
	{
		const unsigned int maxThreads = std::thread::hardware_concurrency();

		// assume at least 2 cores?
		n_workers = maxThreads ? maxThreads : 2;

		// initialize threads
		for (int i = 0; i < n_workers; ++i)
		{
			threads.emplace_back(&ThreadPool::threadWorker, this);
		}
	}

	void appendJob(std::function<void()> job)
	{
		jobs.push(job);
	}

	void waitForCompletion()
	{
		while (! jobs.empty())
		{
			std::cout << "\rRemaining jobs: " << jobs.size() << std::flush;
			std::this_thread::yield();
		}

		should_finish = true;
	}

	[[nodiscard]] size_t workers() const
	{
		return n_workers;
	}
};

#endif
