#pragma once

#include "thread.h"

#define SCHEDULE_FN_BIND(func) std::bind(&##func, this, std::placeholders::_1)



namespace he
{
	struct scheduler_arguments
	{
		
	};
	
	class scheduler
	{
		std::vector<std::unique_ptr<thread>> _threads;
		int32_t _recommanded_thread_index;
		bool _bIsTaskRunning;
	public:
		scheduler();
		virtual ~scheduler() = default;
		
		static void initialize(const scheduler_arguments& in_args = scheduler_arguments());
		static void shutdown();
		static scheduler& get_instance();
	protected:

		task_handle ScheduleAsync(const task_delegate& in_task_delegate)
		{
			uint8_t recommanded_thread_index = get_recommand_thread_index();

			task_handle result = _threads[recommanded_thread_index]->push_task(in_task_delegate);
			return result;
		}
	
	private:
		uint8_t get_recommand_thread_index();
	};
}
