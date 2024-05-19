#include "PCH/PCH.h"
#include "task.h"

namespace he
{
	task::task()
	{
		_is_running   = false;
		_is_compelete = false;
	}

	bool task::is_running() const
	{
		return _is_running;
	}

	bool task::is_waiting() const
	{
		return is_running() == false && is_compelete() == false;
	}

	bool task::is_compelete() const
	{
		return _is_compelete;
	}

	void task::do_task()
	{
		_is_running = true;

		_task_delegate();

		_is_compelete = true;
		_is_running = false;
	}

	task_handle task::create_handle()
	{
		return task_handle(shared_from_this(), &_is_running, &_is_compelete);
	}

	std::shared_ptr<task> task::create(const task_delegate& task_delegate)
	{
		std::shared_ptr<task> result = std::make_shared<task>();
		result->bind(task_delegate);

		return result;
	}

	task_handle::task_handle(std::shared_ptr<task> task, const std::atomic<bool>* p_is_running, const std::atomic<bool>* p_is_compelete)
	{
		_task = task;
		_p_is_running = p_is_running;
		_p_is_compelete = p_is_compelete;
	}

	bool task_handle::is_valid() const
	{
		return _task.expired() == false && _p_is_running != nullptr;
	}

	bool task_handle::is_running() const
	{
		if (is_valid() == false)
		{
			return false;
		}

		return (*_p_is_running) == true;
	}

	bool task_handle::is_compelete() const
	{
		if (is_valid() == false)
		{
			return false;
		}

		return (*_p_is_compelete) == true;
	}

	bool task_handle::is_waiting() const
	{
		if (is_valid() == false)
		{
			return false;
		}

		return is_running() == false && is_compelete() == false;
	}

	void task_handle::wait()
	{
		while (is_running()) { }
	}
}
