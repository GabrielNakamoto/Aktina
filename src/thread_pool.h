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
	std::atomic_bool should_finish;

	std::condition_variable new_job;
	std::condition_variable job_end;

	std::mutex queue_mutex;
	std::vector<std::thread> threads;
	std::queue<std::function<void()> > jobs;

	int workerThreads;

public:

	ThreadPool()
	{
		std::cout << "Initialized thread pool\n";
		const unsigned int maxThreads = std::thread::hardware_concurrency();

		// assume at least 2 cores?
		workerThreads = maxThreads ? maxThreads : 2;

		// initialize threads
		for (int i = 0; i < workerThreads; ++i)
		{
			threads.emplace_back(ThreadWorker(this));
		}
		std::cout << "Created threads\n";

		for (auto &thread : threads)
			thread.detach();
	}

	void appendJob(std::function<void()> job)
	{
		std::unique_lock<std::mutex> guard(queue_mutex);

		jobs.push(job);
		new_job.notify_all();
	}

	void waitForCompletion()
	{
		// wait until queue is empty and then stop execution
	
		std::unique_lock<std::mutex> guard(queue_mutex);

		job_end.wait(guard, [this] { return ! this->queue.empty(); });

		this->shutdown();
	}

private:

	void shutdown()
	{
		std::unique_lock<std::mutex> guard(queue_mutex);

		should_finish = true;
		new_job.notify_all();
	}
};

#endif
