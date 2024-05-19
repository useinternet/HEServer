#pragma once
#include "hedefines.h"
#include "task.h"
#include <thread>

namespace he
{
	class thread
	{
		std::thread _thread;
		thread_id _id;

		std::queue<std::shared_ptr<task>> _task_queue;

		std::atomic<bool> _is_active;
		std::atomic<bool> _is_running;

		std::mutex _mutex;
		std::condition_variable _conditionVariable;
	public:
		thread();
		virtual ~thread();

	public:
		task_handle push_task(const task_delegate& in_task_delegate)
		{
			std::shared_ptr<task> task = task::create(in_task_delegate);

			std::lock_guard<std::mutex> lock(_mutex);
			_task_queue.push(task);

			_conditionVariable.notify_one();

			return task->create_handle();
		}

		uint64_t get_thread_id() const;
		bool is_waiting() const;
		bool is_running() const;

	private:
		void init();
		void run();
		void destroy();

	public:
	};


	inline thread_id current_thread_id()
	{
		return std::hash<std::thread::id>()(std::this_thread::get_id());
	}
}
