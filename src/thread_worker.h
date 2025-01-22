#ifndef THREAD_WORKER_H
#define THREAD_WORKER_H

#include <iostream>
#include <thread>
#include <condition_variable>

class ThreadPool;

class ThreadWorker
{
private:

	ThreadPool *scheduler;

public:

	ThreadWorker(ThreadPool *parent);

	void operator ()();
};

#endif
