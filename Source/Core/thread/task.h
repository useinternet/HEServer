#pragma once
#include <memory>
#include <functional>

namespace he
{
	using task_delegate = std::function<void()>; 
		
	class task_handle;
	class task : public std::enable_shared_from_this<task>
	{
		friend class thread;
	private:
		task_delegate  _task_delegate;

		std::atomic<bool> _is_running;
		std::atomic<bool> _is_compelete;
	public:
		task();
		virtual ~task() = default;

		bool is_running() const;
		bool is_waiting() const;
		bool is_compelete() const;

		void bind(const task_delegate& task_delegate)
		{
			_task_delegate = task_delegate;
		}

		void do_task();

		task_handle create_handle();
	public:
		static std::shared_ptr<task> create(const task_delegate& task_delegate);
	};

	class task_handle 
	{
		friend task;

	private:
		std::weak_ptr<task> _task;
		const std::atomic<bool>* _p_is_running;
		const std::atomic<bool>* _p_is_compelete;

	private:
		task_handle(std::shared_ptr<task> task, const std::atomic<bool>* p_is_running, const std::atomic<bool>* p_is_compelete);

	public:
		task_handle() = default;

		bool is_valid() const;
		bool is_running() const;
		bool is_compelete() const;
		bool is_waiting() const;
		void wait();
	};
}



