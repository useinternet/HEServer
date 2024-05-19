#include "PCH/PCH.h"
#include "thread/thread.h"

namespace he
{
	thread::thread()
	{
		init();
	}

	thread::~thread()
	{
		destroy();
	}

	thread_id thread::get_thread_id() const
	{
		return _id;
	}

	bool thread::is_waiting() const
	{
		return _is_running == false;
	}

	bool thread::is_running() const
	{
		return _is_running == true;
	}

	void thread::init()
	{
		_thread = std::thread([&]() { run(); });
		_id     = std::hash<std::thread::id>()(_thread.get_id());

		_is_active  = true;
		_is_running = false;
	}

	void thread::run()
	{
		while (_is_active || _task_queue.empty() == false)
		{
			std::unique_lock<std::mutex> lock(_mutex);

			_is_active = _task_queue.empty() == false;

			_conditionVariable.wait(lock, [&]()->bool
			{
				return _task_queue.empty() == false || _is_active == false;
			});

			if (_task_queue.empty() == false)
			{
				std::shared_ptr<task> task = _task_queue.front(); _task_queue.pop();
			
				lock.unlock();

				if (task != nullptr)
				{
					task->do_task();
				}
			}
		}
	}

	void thread::destroy()
	{
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_is_active = false;
			_conditionVariable.notify_all();
		}

		_thread.join();
	}
}



