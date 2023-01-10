#pragma once
#include <functional>
#include <vector>
#include <thread>
#include <condition_variable>
#include <queue>

using Task = std::function<void()>;

class ThreadPool
{
public:
	explicit ThreadPool(const size_t& numThreads)
	{
		start(numThreads);
	}
	~ThreadPool()
	{
		stop();
	}
	void submit(Task task)
	{
		// Scope for locks
		{
			std::unique_lock<std::mutex> lock{ m_EventMutex };
			m_TasksQueue.emplace(/*std::move*/(task));
		}

		m_EventCV.notify_one();
	}

private:
	std::queue<Task> m_TasksQueue;
	std::vector<std::thread> m_Threads;
	std::condition_variable m_EventCV;
	std::mutex m_EventMutex;
	bool m_Finished = false;
	void start(const size_t& numThreads)
	{
		for (auto ii = 0u; ii < numThreads; ii++)
		{
			m_Threads.emplace_back([=]() {

				while (true)
				{
					Task task;
					{
						std::unique_lock<std::mutex> lock{ m_EventMutex };

						m_EventCV.wait(lock, [=] {return m_Finished || !m_TasksQueue.empty(); });

						if (m_Finished || m_TasksQueue.empty())
							break;

						task = std::move(m_TasksQueue.front());
						m_TasksQueue.pop();
					}

					// To keep the critical section as least as possible
					// we dont need the critical section for the task to be executed, 
					// we provided the scope above.
					task();
				}
				});
		}
	}
	void stop() noexcept
	{
		{
			std::unique_lock<std::mutex> lock{ m_EventMutex };
			m_Finished = true;
		}
		m_EventCV.notify_all();

		for (auto& thread : m_Threads)
		{
			thread.join();
		}
	}
};




//#include <vector>
//#include <thread>
//
//#include <memory>
//#include <mutex>
//#include <queue>
//#include <condition_variable>
//#include <functional>
//
//#include <queue>
//#include <memory>
//#include <mutex>
//#include <condition_variable>
//
//template<typename T>
//class threadsafe_queue_using_cv
//{
//private:
//	mutable std::mutex mut;
//	std::queue<T> data_queue;
//	std::condition_variable data_cond;
//public:
//	threadsafe_queue_using_cv()
//	{}
//	threadsafe_queue_using_cv(threadsafe_queue_using_cv const& other)
//	{
//		std::lock_guard<std::mutex> lk(other.mut);
//		data_queue = other.data_queue;
//	}
//	void push(T new_value)
//	{
//		std::lock_guard<std::mutex> lk(mut);
//		data_queue.push(new_value);
//		data_cond.notify_all();
//	}
//	void wait_and_pop(T& value)
//	{
//		std::unique_lock<std::mutex> lk(mut);
//		data_cond.wait(lk, [this] {return !data_queue.empty(); });
//		value = data_queue.front();
//		data_queue.pop();
//	}
//	std::shared_ptr<T> wait_and_pop()
//	{
//		std::unique_lock<std::mutex> lk(mut);
//		data_cond.wait(lk, [this] {return !data_queue.empty(); });
//		std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
//		data_queue.pop();
//		return res;
//	}
//	bool try_pop(T& value)
//	{
//		std::lock_guard<std::mutex> lk(mut);
//		if (data_queue.empty())
//			return false;
//		value = data_queue.front();
//		data_queue.pop();
//		return true;
//	}
//	std::shared_ptr<T> try_pop()
//	{
//		std::lock_guard<std::mutex> lk(mut);
//		if (data_queue.empty())
//			return std::shared_ptr<T>();
//		std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
//		data_queue.pop();
//		return res;
//	}
//	bool empty() const
//	{
//		std::lock_guard<std::mutex> lk(mut);
//		return data_queue.empty();
//	}
//};
//
//
//
//
//class join_threads
//{
//	std::vector<std::thread>& threads;
//public:
//	explicit join_threads(std::vector<std::thread>& threads_) :
//		threads(threads_)
//	{}
//	~join_threads()
//	{
//		for (unsigned long i = 0; i < threads.size(); ++i)
//		{
//			if (threads[i].joinable())
//				threads[i].join();
//		}
//	}
//};
//
//
//class thread_pool
//{
//	std::atomic_bool done;
//	threadsafe_queue_using_cv<std::function<void()> > work_queue;
//	std::vector<std::thread> threads;
//	join_threads joiner;
//	void worker_thread()
//	{
//		while (!done)
//		{
//			std::function<void()> task;
//			if (work_queue.try_pop(task))
//			{
//				task();
//			}
//			else
//			{
//				std::this_thread::yield();
//			}
//		}
//	}
//public:
//	thread_pool() :
//		done(false), joiner(threads)
//	{
//		unsigned const thread_count = std::thread::hardware_concurrency();
//		try
//		{
//			for (unsigned i = 0; i < thread_count; ++i)
//			{
//				threads.push_back(
//					std::thread(&thread_pool::worker_thread, this));
//			}
//		}
//		catch (...)
//		{
//			done = true;
//			throw;
//		}
//	}
//	~thread_pool()
//	{
//		done = true;
//	}
//	template<typename FunctionType>
//	void submit(FunctionType f)
//	{
//		work_queue.push(std::function<void()>(f));
//	}
//};
