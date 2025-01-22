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

	std::condition_variable new_job;

	std::mutex queue_mutex;
	std::vector<std::thread> threads;
	std::queue<std::function<void()> > jobs;

	int workerThreads;

public:

	ThreadPool()
	{
		const unsigned int maxThreads = std::thread::hardware_concurrency();

		// assume at least 2 cores?
		workerThreads = maxThreads ? maxThreads : 2;

		// initialize threads
		for (int i = 0; i < workerThreads; ++i)
		{
			threads.push_back(std::thread(ThreadWorker(this)));
		}
	}

	void appendJob(std::function<void()> &job)
	{
		std::unique_lock<std::mutex> guard(queue_mutex);

		jobs.push(job);
		new_job.notify_all();
	}
};

#endif
