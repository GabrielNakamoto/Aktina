#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_h

#include <memory>
#include <thread>

class ThreadGuard
{
private:

		std::vector<std::thread> &threads;

public:

		explicit ThreadGuard(std::vector<std::thread> &threads)
			:	threads(threads)
		{
		}

		~ThreadGuard()
		{
			for (auto &thread : threads)
				if (thread.joinable())
					thread.join();
		}
};

template<typename T>
class threadsafe_queue
{
private:

		mutable std::mutex mut;
		std::queue<T> internal;
		std::condition_variable dat_cond;

public:

	threadsafe_queue()
	{
	};

	void push(T value)
	{
		std::lock_guard<std::mutex> lk(mut);
		internal.push(value);
	}

	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (internal.empty())
			return false;
		value = std::move(internal.front());
		internal.pop();

		return true;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return internal.empty();
	}

	size_t size() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return internal.size();
	}
};

#endif
